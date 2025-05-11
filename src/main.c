#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>      // basename()
#include <limits.h>      // PATH_MAX
#include <getopt.h>

#include "camera/capture.h"
#include "image-processing/raw2gray.h"
#include "image-processing/analysis.h"
#include "common/common.h"

// DATADIR will be injected by CMake as the absolute path to <repo>/data
#ifndef DATADIR
#define DATADIR "data"
#endif

static const char *exts[] = { ".raw", ".tiff", ".dng", NULL };

static struct option longopts[] = {
    { "mode",    required_argument,  NULL, 'm' },
    { "input",   required_argument,  NULL, 'i' },
    { "outdir",  required_argument,  NULL, 'o' },
    { "help",    no_argument,        NULL, 'h' },
    { NULL,      0,                  NULL,  0  }
};

static void usage(const char *prog) {
    fprintf(stderr,
      "Usage: %s --mode (static|dynamic) [--input <file>] --outdir <dir>\n",
      prog);
    exit(1);
}

static void remove_ext(char *s) {
    char *dot = strrchr(s, '.');
    if (dot) *dot = '\0';
}

/**
 * Fill raw_path[] and base[] according to mode.
 * Uses resolved_outdir (absolute) for dynamic mode.
 */
static int make_paths(const char *mode,
                      const char *infile,
                      const char *resolved_outdir,
                      char raw_path[PATH_MAX],
                      char base[PATH_MAX])
{
    if (strcmp(mode, "static") == 0) {
        if (infile) {
            strncpy(raw_path, infile, PATH_MAX);
        } else {
            char *picked = common_pick_random_file(DATADIR "/captures", exts);
            if (!picked) return -1;
            strncpy(raw_path, picked, PATH_MAX);
            free(picked);
        }
        char tmp[PATH_MAX];
        strncpy(tmp, basename(raw_path), PATH_MAX);
        remove_ext(tmp);
        strncpy(base, tmp, PATH_MAX);
    }
    else if (strcmp(mode, "dynamic") == 0) {
        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        char timestamp[32];
        strftime(timestamp, sizeof timestamp, "%Y%m%d_%H%M%S", &tm);

        // name and path under data/captures
        snprintf(base, PATH_MAX, "capture_%s", timestamp);
        snprintf(raw_path, PATH_MAX,
                 DATADIR "/captures/%s.dng", base);
    }
    else {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    char *mode   = NULL;
    char *infile = NULL;
    char *outdir = NULL;
    int   c;

    while ((c = getopt_long(argc, argv, "m:i:o:h", longopts, NULL)) != -1) {
        switch (c) {
            case 'm': mode   = optarg; break;
            case 'i': infile = optarg; break;
            case 'o': outdir = optarg; break;
            default:  usage(argv[0]);
        }
    }
    if (!mode || !outdir) usage(argv[0]);

    // Resolve outdir to absolute under PROJECT_ROOT
    char resolved_outdir[PATH_MAX];
    if (outdir[0] != '/') {
        snprintf(resolved_outdir, PATH_MAX, "%s/%s", PROJECT_ROOT, outdir);
    } else {
        strncpy(resolved_outdir, outdir, PATH_MAX);
    }
    resolved_outdir[PATH_MAX-1] = '\0';

    if (common_mkdir_p(resolved_outdir) < 0) {
        fprintf(stderr, "Error: cannot create directory %s\n", resolved_outdir);
        return 1;
    }

    char raw_path[PATH_MAX], base[PATH_MAX];
    if (make_paths(mode, infile, resolved_outdir, raw_path, base) < 0) {
        fprintf(stderr, "Error: failed to determine input for mode \"%s\"\n", mode);
        usage(argv[0]);
    }

    // Load or capture the RawImage
    RawImage raw = {0};
    if (strcmp(mode, "static") == 0) {
        raw = raw_open(raw_path);
    } else {
        raw = capture_from_camera(raw_path);
    }
    if (!raw.data) {
        fprintf(stderr, "Error: failed to load RAW data from %s\n", raw_path);
        return 1;
    }

    // Convert, analyze
    GrayImage gray = raw2gray(&raw);
    AnalysisResult res = find_brightest(&gray, 50);

    // Build outputs in resolved_outdir
    char gray_png[PATH_MAX], report_txt[PATH_MAX];
    snprintf(gray_png,  PATH_MAX, "%s/%s_grayscale.png", resolved_outdir, base);
    snprintf(report_txt, PATH_MAX, "%s/%s_report.txt",    resolved_outdir, base);

    if (raw2gray_write_png(&gray, gray_png) < 0)
        fprintf(stderr, "Warning: could not write %s\n", gray_png);
    if (analysis_write_report(&res, report_txt) < 0)
        fprintf(stderr, "Warning: could not write %s\n", report_txt);

    printf("Wrote grayscale → %s\n", gray_png);
    printf("Wrote report    → %s\n", report_txt);

    // Cleanup
    raw_free(&raw);
    gray_free(&gray);
    analysis_free(&res);
    return 0;
}
