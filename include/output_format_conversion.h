#ifndef OUTPUT_FORMAT_CONVERSION_H
#define OUTPUT_FORMAT_CONVERSION_H


#include "x264.h"
#include "sdf.h"



typedef struct
{
    int32_t         i_nal;
    struct
    {
        uint8_t     *payload;
        int32_t     i_payload;
    }nal[10];
}x_nal_t;


void ofc_linemode_to_xnalmode(uint8_t *in, int32_t length, x_nal_t *xnal_out);


void ofc_xnalmode_to_linemode(x_nal_t *xnal_in, uint8_t *out);


void ofc_x264nalmode_to_xnalmode(x264_nal_t *nal_in, int32_t nNal, x_nal_t *xnal_out);


int32_t ofc_search_nalu_header(uint8_t *in, int32_t length);


int32_t ofc_search_next_nalu_header(uint8_t *in, int32_t length);


#endif // OUTPUT_FORMAT_CONVERSION_H
