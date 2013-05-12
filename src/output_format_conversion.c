/*
 *
 *
 *
 */

#include <stdio.h>
#include <string.h>

#include "include/output_format_conversion.h"

void ofc_linemode_to_xnalmode(uint8_t *in, int32_t length, x_nal_t *xnal_out)
{
    uint8_t     *search_in = in;
    int32_t     *search_dept = length;
    int32_t     index = 0;

    //check params
    if (NULL == in || length <= 0 || NULL == xnal_out)
    {
        printf("params error on func:%s\n", __func__);
        return;
    }

    memset(xnal_out, 0, sizeof(x_nal_t));
    if (search_in[0] == 0x00 && search_in[1] == 0x00 && search_in[2] == 0x00 && search_in[3] == 0x01)
    {
        while ((index = ofc_search_next_nalu_header(search_in, search_dept)) >= 0)
        {
            xnal_out[xnal_out->i_nal].payload = search_in;
            xnal_out[xnal_out->i_nal].i_payload = index;
            xnal_out->i_nal++;

            search_in += index;
            search_dept -= index;
        }

        xnal_out[xnal_out->i_nal].payload = search_in;
        xnal_out[xnal_out->i_nal].i_payload = search_dept;
        xnal_out->i_nal++;
    }
    else
    {
        return;
    }
}


//must check out buffer whether overload
void ofc_xnalmode_to_linemode(x_nal_t *xnal_in, uint8_t *out)
{
    int32_t     i = 0;

    //check params
    if (NULL == xnal_in || NULL == out)
    {
        printf("params error on func:%s\n", __func__);
        return;
    }

    for (i = 0; i < xnal_in->i_nal; i++)
    {
        memcpy(out, xnal_in[i].payload, xnal_in[i].i_payload);

        out += xnal_in[i].i_payload;
    }
}


void ofc_x264nalmode_to_xnalmode(x264_nal_t *nal_in,int32_t nNal, x_nal_t *xnal_out)
{
    int32_t         i = 0;

    //check params
    if (NULL == nal_in || NULL == xnal_out || nNal > 10)
    {
        printf("params error on func:%s\n", __func__);
        return;
    }

    for (i = 0; i < nNal; i++)
    {
        xnal_out[i].payload = nal_in[i].payload;
        xnal_out[i].i_payload = nal_in[i].i_payload;
        xnal_out->i_nal++;
    }
}


int32_t ofc_search_nalu_header(uint8_t *in, int32_t length)
{
    uint8_t         *search_in = in;
    int32_t         i = 0;
    int32_t         search_dept = length - 4;

    //check params
    if (NULL == in || length <= 0)
    {
        printf("params error on func:%s\n", __func__);
        return -2;
    }

    for (i = 0; i < search_dept; i++)
    {
        if (search_in[0] == 0x00)
        {
            if (search_in[1] == 0x00)
            {
                if (search_in[2] == 0x00 && search_in[3] == 0x01)
                {
                    break;
                }
            }
        }
        search_in++;
    }

    if (i >= search_dept)
    {
        return -1;
    }
    else
    {
        return i;
    }
}



/*
 *  not search from header, here we think the head(offset 0x00) is
 *  the nalu header
 */
int32_t ofc_search_next_nalu_header(uint8_t *in, int32_t length)
{
    uint8_t         *search_in = in;
    int32_t         i = 0;
    int32_t         search_dept = length - 4;

    //check params
    if (NULL == in || length <= 0)
    {
        printf("params error on func:%s\n", __func__);
        return -2;
    }

    //
    for (i = 4; i < search_dept; i++)
    {
        if (search_in[0] == 0x00)
        {
            if (search_in[1] == 0x00)
            {
                if (search_in[2] == 0x00 && search_in[3] == 0x01)
                {
                    break;
                }
            }
        }
        search_in++;
    }

    if (i >= search_dept)
    {
        return -1;
    }
    else
    {
        return i;
    }
}
