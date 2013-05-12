/*
 *
 *
 */

#include "include/h264_codec.h"


int32_t h264_codec_init(int32_t width, int32_t height, int32_t fps)
{
    int32_t     ret = RET_SUCCESS;

    g_h264_config.width = width;
    g_h264_config.height = height;
    g_h264_config.fps = fps;

#ifdef VIDEO_CODEC_X264
    ret = xcodec_init(width, height, fps);
#else
    ret = mfc6410_init(width, height, fps);
#endif

    return ret;
}

int32_t h264_codec_exe(uint8_t *pic_in)
{
    int32_t     ret = RET_SUCCESS;

    g_h264_config.pic_in = pic_in;

#ifdef VIDEO_CODEC_X264
    ret = xcodec_exe(pic_in);
#else
    ret = mfc6410_exe(pic_in);
#endif

    return ret;
}

int32_t h264_codec_get_output(uint8_t *pic_out, int32_t *out_length)
{
    int32_t     ret = RET_SUCCESS;

#ifdef VIDEO_CODEC_X264
    ret = xcodec_get_output(g_h264_config.pic_out, &g_h264_config.out_length);
#else
    ret = mfc6410_get_output(g_h264_config.pic_out, &g_h264_config.out_length);
#endif

    pic_out = g_h264_config.pic_out;
    *out_length = g_h264_config.out_length;

    return ret;
}

void h264_codec_deinit()
{
#ifdef VIDEO_CODEC_X264
    xcodec_deinit();
#else
    mfc6410_deinit();
#endif
}
