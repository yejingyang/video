/*
 *
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "include/mfc6410/SsbSipH264Encode.h"
#include "include/mfc6410/SsbSipLogMsg.h"

#include "include/h264_codec.h"

int32_t mfc6410_init(int32_t width, int32_t height, int32_t fps)
{
    mfc6410_config  *mfc_config = &g_mfc6410_config;
    int32_t         ret = RET_SUCCESS;
    int32_t         frame_rate = 21000;
    int32_t         bit_rate = 2100;
    int32_t         gop_num = 28;

    do
    {
        g_mfc6410_config.width = width;
        g_mfc6410_config.height = height;
        g_mfc6410_config.fps = fps;

        mfc_config->handle = SsbSipH264EncodeInit(width, height, frame_rate, bit_rate, gop_num);
        if (mfc_config->handle == NULL)
        {
            printf("SsbSipH264EncodeInit failed\n");
            ret = RET_MFC_INIT_ERROR;
            break;
        }

        mfc_config->pic_in = SsbSipH264EncodeGetInBuf(mfc_config->handle, 0);
        if (mfc_config->pic_in == NULL)
        {
            printf("SsbSipH264GetInBuf error\n");
            ret = RET_MFC_GET_IN_BUF_ERR;
            break;
        }
    }while(0);

    return ret;
}

int32_t mfc6410_exe(uint8_t *pic_in)
{
    int32_t     ret = RET_SUCCESS;
    mfc6410_config  *mfc_config = &g_mfc6410_config;

    do
    {
        //convert picture from yuyv to yuv420 format
        fc_yuyv_to_i420_mfc6410(pic_in, mfc_config->pic_in);

        SsbSipH264EncodeExe(mfc_config->handle);

        mfc_config->pic_out = (uint8_t *)SsbSipH264EncodeGetOutBuf(mfc_config->handle, &mfc_config->out_length);
    }while(0);

    return ret;
}

int32_t mfc6410_get_output(uint8_t *pic_out, int32_t *out_length)
{
    pic_out = (uint8_t *)g_mfc6410_config.pic_out;
    *out_length = g_mfc6410_config.out_length;
}

void mfc6410_deinit()
{
    if (g_mfc6410_config.handle)
    {
        SsbSipH264EncodeDeInit(g_mfc6410_config.handle);
        g_mfc6410_config.handle = NULL;
    }
}
