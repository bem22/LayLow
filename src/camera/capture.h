#pragma once
#include "../image-processing/raw2gray.h"   // for RawImage

/**
 * capture_to_raw()
 *
 * Runs a system call to libcamera-still (or libcamera-raw) to dump
 * a raw sensor file to `path`, then returns 0 on success or -1 on error.
 */
int capture_to_raw(const char *path);

/**
 * capture_from_camera()
 *
 * Convenience wrapper: shell out, then load the RawImage via raw_open().
 * Returns a RawImage with .data==NULL on failure.
 */
RawImage capture_from_camera(const char *path);
