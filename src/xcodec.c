/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>


#include "include/h264_codec.h"


static int32_t xcodec_params_set(xcodec_config *x_config, int32_t width, int32_t, int32_t fps)
{
    //set the encoder parameter by default
    x264_param_default(x_config->param);

    //set the frame width and height
    x_config->param->i_width = width;
    x_config->param->i_height = height;

    x_config->param->rc.i_lookahead = 0;

    x_config->param->i_fps_num = fps;
    x_config->param->i_fps_den = 1;

    //set the maxium quantization step length to 26
    x_config->param->rc.i_qp_max = 26;
    //disable the CABAC encode method
    x_config->param->b_cabac = 0;
    //enable diamod analysis search method
    x_config->param->i_me_method = X264_ME_DIA;
    //set reference frame with 1 frame
    x_config->param->i_frame_reference = 1;

    //set idr jiangge
    x_config->param->i_keyint_max = 15;
    x_config->param->i_keyint_min = 5;

    //set the net mtu(1400)
//    x_config->param->i_slice_max_size = 1400;

    //use base line profile
    x264_param_apply_profile(x_config->param, x264_profile_names[0]);

    if (NULL == (x_config->handle = x264_encoder_open(x_config->param)))
    {
        return RET_CODEC_PARAM_ERROR;
    }

    //create a new picture
    x264_picture_alloc(x_config->picture, X264_CSP_I420, width, height);

    //set pts
    x_config->picture->i_pts = 0;

    return RET_SUCCESS;
}


int32_t xcodec_init(int32_t width, int32_t height, int32_t fps)
{
    int32_t     ret = RET_SUCCESS;

    do
    {
        //parameters check
        if ((width % 16 != 0) || (height % 16 != 0))
        {
            ret = RET_PARAM_INVALID_VALUE;
            break;
        }

        g_xcodec_config.width = width;
        g_xcodec_config.height = height;
        g_xcodec_config.fps = fps;

        //get memory for x264 encoder parameter structure
        if (NULL == (g_xcodec_config.param = (x264_param_t *)malloc(sizeof(x264_param_t))))
        {
            printf("out of memory on %s\n", __func__);
            ret = RET_MEM_NOT_ENOUGH_ERR;
            break;
        }

        //get memory for encoder picture structure
        if (NULL == (g_xcodec_config.picture = (x264_picture_t *)malloc(sizeof(x264_picture_t))))
        {
            printf("out of memory on %s\n", __func__);
            ret = RET_MEM_NOT_ENOUGH_ERR;
            break;
        }

        //get memory for picture buffer field
        if (NULL == (g_xcodec_config.pic_buf = (uint8_t*)malloc(sizeof(uint8_t) * width * height *3)))
        {
            printf("out of memory on %s\n", __func__);
            ret = RET_MEM_NOT_ENOUGH_ERR;
            break;
        }

        //get memory for output buffer field
        if (NULL == (g_xcodec_config.pic_out = (uint8_t*)malloc((sizeof(uint8_t) * width * height *3) >> 1)))
        {
            printf("out of memory on %s\n", __func__);
            ret = RET_MEM_NOT_ENOUGH_ERR;
            break;
        }

        ret = xcodec_params_set(&g_xcodec_config, width, height, fps);

    } while(0);

    return ret;
}


int32_t xcodec_exe(uint8_t *pic_in)
{
    xcodec_config   *x_config = &g_xcodec_config;
    x264_picture_t	pic_out;
    x264_nal_t      *nal;
    int32_t         nNal = -1;
    int32_t         i = 0;
    int32_t         copy_count = 0;
    int32_t         ret = RET_SUCCESS;

    do
    {
        //picture format conversion and save it in the picture planar field
        fc_yuyv_to_i420_x264(pic_in, x_config->picture);

        x_config->picture->i_type = X264_TYPE_AUTO;


        //encode a picture
        if (x264_encoder_encode(x_config->handle, &nal,
                &nNal, x_config->picture, &pic_out) < 0)
        {
            printf("x264_encoder_encode failure\n");
            ret = RET_CODEC_ENCODE_ERROR;
            break;
        }

        //update pts
        x_config->picture->i_pts++;

        x_config->out_length = 0;

        //copy the result to the output buffer
        for (i = 0; i < nNal; i++)
        {
            memcpy(x_config->pic_out + copy_count, nal[i].p_payload, nal[i].i_payload);
            copy_count += nal[i].i_payload;
            x_config->out_length += nal[i].i_payload;
        }

    }while(0);


    return ret;
}


int32_t xcodec_get_output(uint8_t *pic_out, int32_t *out_length)
{
    pic_out = (uint8_t *)g_xcodec_config.pic_out;
    *out_length = g_xcodec_config.out_length;
}


void xcodec_deinit()
{
    xcodec_config   *x_config = &g_xcodec_config;

    if (x_config->picture)
    {
        x264_picture_clean(x_config->picture);
        x_config->picture = NULL;
    }

    if (x_config->param)
    {
        free(x_config->param);
        x_config->param = NULL;
    }

    if (x_config->handle)
    {
        x264_encoder_close(x_config->handle);
    }

    if (x_config->pic_buf)
    {
        free(x_config->pic_buf);
        x_config->pic_buf = NULL;
    }

    if (x_config->pic_out)
    {
        free(x_config->pic_buf);
        x_config->pic_out = NULL;
    }
}
