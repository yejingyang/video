#ifndef FORMAT_CONVERT_H
#define FORMAT_CONVERT_H

#include "sdf.h"
#include "x264.h"


typedef struct
{
    int32_t     width;
    int32_t     height;
    int32_t     frame_in_type;
    int32_t     frame_out_type;
}format_convert_config;


format_convert_config   g_format_convert_config;


void fc_init(int32_t width, int32_t height);

void fc_yuyv_to_i420_x264(uint8_t *pic_in, x264_picture_t *x264_pic_out);

void fc_yuyv_to_i420_mfc6410(uint8_t *pic_in, uint8_t *pic_out);

void fc_deinit();

#endif // FORMAT_CONVERT_H
