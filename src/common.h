
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>  
// libcamera capture
int capture_init(void);
int capture_frame_raw(const char *out_path);

// raw -> 16-bit grayscale
int raw2gray_init(void);
int raw2gray(const char *raw_path, const char *gray_path);

// image analysis
int analysis_init(void);
int analysis_threshold(const char *gray_path, const char *bin_path, uint16_t threshold);
int analysis_edge(const char *gray_path, const char *edge_path, int low, int high);

// bluetooth
int bt_init(void);
int bt_connect(const char *mac);
int bt_send_cmd(const char *cmd);
int bt_receive_status(char *buf, size_t buflen);
int bt_read_battery(int *percent);

#endif // COMMON_H