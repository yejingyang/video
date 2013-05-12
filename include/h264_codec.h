#ifndef H264_CODEC_H
#define H264_CODEC_H

#include "sdf.h"
#include "x264.h"
#include "format_convert.h"
#include "output_format_conversion.h"


typedef struct
{
    int32_t         width;
    int32_t         height;
    int32_t         fps;
    uint8_t         *pic_in;
    uint8_t         *pic_out;
    int32_t         out_length;
    x_nal_t         xnal_out;
}h264_config;


typedef struct
{
    void            *handle;
    int32_t         width;
    int32_t         height;
    int32_t         fps;
    uint8_t         *pic_in;
    uint8_t         *pic_out;
    int32_t         out_length;
    x_nal_t         xnal_out;
}mfc6410_config;


typedef struct
{
    x264_param_t    *param;
    x264_t          *handle;
    x264_picture_t  *picture;
    x264_nal_t      *nal;
    int32_t         nNal;
    uint8_t         *pic_in;
    uint8_t         *pic_buf;
    uint8_t         *pic_out;
    int32_t         out_length;
    x_nal_t         xnal_out;
}xcodec_config;


int32_t h264_codec_init(int32_t width, int32_t height, int32_t fps);

int32_t h264_codec_exe(uint8_t *pic_in);

int32_t h264_codec_get_output(uint8_t *pic_out, int32_t *out_length);

x_nal_t* h264_codec_get_xnal();

void h264_codec_deinit();

h264_config g_h264_config;


#if 1

int32_t xcodec_init(int32_t width, int32_t height, int32_t fps);

int32_t xcodec_exe(uint8_t *pic_in);

int32_t xcodec_get_output(uint8_t *pic_out, int32_t *out_length);

x_nal_t* xcodec_get_xnal();

void xcodec_deinit();

xcodec_config g_xcodec_config;

#endif


#if 1

int32_t mfc6410_init(int32_t width, int32_t height, int32_t fps);

int32_t mfc6410_exe(uint8_t *pic_in);

int32_t mfc6410_get_output(uint8_t *pic_out, int32_t *out_length);

x_nal_t* mfc6410_get_xnal();

void mfc6410_deinit();

mfc6410_config g_mfc6410_config;

#endif




#endif // H264_CODEC_H
