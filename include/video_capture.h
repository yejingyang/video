#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include "sdf.h"

#define BUFFER_COUNT    4

struct buffer
{
    void        *start;
    size_t      length;
};

typedef struct
{
    int32_t         width;
    int32_t         height;
    int32_t         fps;
    int32_t         camera_fd;
    int32_t         frame_count;
    int8_t          *device_name;
    struct buffer   *buffers;
}video_capture_config;


//define a global video capture varile
video_capture_config g_vc_config;


int32_t video_capture_init(int32_t width, int32_t height, int32_t fps);

int32_t video_capture_get_frame(uint8_t *pic_out, int32_t *length);

int32_t video_capture_deinit();

#endif // VIDEO_CAPTURE_H
