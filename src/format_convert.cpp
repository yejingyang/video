/*
 *
 *
 *
 */

#include <stdio.h>

#include "include/format_convert.h"


void fc_init(int32_t width, int32_t height)
{
    format_convert_config   *fc_config = &g_format_convert_config;

    fc_config->width = width;
    fc_config->height = height;

    printf("format convert init successfully\n");
}


void fc_yuyv_to_i420_x264(uint8_t *pic_in, x264_picture_t *x264_pic_out)
{
    format_convert_config   *fc_config = &g_format_convert_config;

    int32_t         width = fc_config->width;
    int32_t         height = fc_config->height;
    int32_t         half_width = 0;
    int32_t         bytes_per_line = 0;

    uint8_t         *pic_y = NULL;
    uint8_t         *pic_u = NULL;
    uint8_t         *pic_v = NULL;

    uint8_t         *in = pic_in;

    int32_t         i = 0;
    int32_t         j = 0;

    //check input parameters
    if (NULL == pic_in || NULL == x264_pic_out)
    {
        printf("input parameter error on func:%s\n", __func__);
        return;
    }

    x264_pic_out->img.i_plane = 3;
    x264_pic_out->img.i_stride[0] = width;
    x264_pic_out->img.i_stride[1] = (width >> 1);
    x264_pic_out->img.i_stride[2] = (width >> 1);

    pic_y = x264_pic_out->img.plane[0];
    pic_u = x264_pic_out->img.plane[1];
    pic_v = x264_pic_out->img.plane[2];

    bytes_per_line = width * 2;
    half_width = width / 2;

    for (i = 0; i < height; i += 2)
    {
        for (j = 0; j < half_width; j += 4)
        {
            y[0] = in[0];
            u[0] = in[1];
            y[1] = in[2];
            v[0] = in[3];

            y += 2;
            u++;
            v++;
            in += 4;
        }

        for (j = 0; j < half_width; j += 4)
        {
            y[0] = in[0];
            y[1] = in[2];

            y += 2;
            in += 4;
        }
    }
}


void fc_yuyv_to_i420_mfc6410(uint8_t *pic_in, uint8_t *pic_out)
{
    format_convert_config   *fc_config = &g_format_convert_config;
    int32_t         width = fc_config->width;
    int32_t         height = fc_config->height;
    int32_t         half_width = 0;
    int32_t         bytes_per_line = 0;

    uint8_t         *pic_y = NULL;
    uint8_t         *pic_u = NULL;
    uint8_t         *pic_v = NULL;

    uint8_t         *in = pic_in;

    int32_t         i = 0;
    int32_t         j = 0;

    //check input parameters
    if (NULL == pic_in || NULL == pic_out)
    {
        printf("input parameter error on func:%s\n", __func__);
        return;
    }

    pic_y = pic_out;
    pic_u = pic_out + (width * height);
    pic_v = pic_u + ((width * height) >> 2);

    bytes_per_line = width << 1;
    half_width = width >> 1 ;

    for (i = 0; i < height; i += 2)
    {
        for (j = 0; j < half_width; j += 4)
        {
            y[0] = in[0];
            u[0] = in[1];
            y[1] = in[2];
            v[0] = in[3];

            y += 2;
            u++;
            v++;
            in += 4;
        }

        for (j = 0; j < half_width; j += 4)
        {
            y[0] = in[0];
            y[1] = in[2];

            y += 2;
            in += 4;
        }
    }
}


void fc_deinit()
{
    printf("format convert deinit successfully!\n");
}
