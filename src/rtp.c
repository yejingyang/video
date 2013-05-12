/*
 *
 *
 *
 */

#include <stdio.h>

#include "include/rtp.h"

int32_t rtp_init(int8_t *ip, int16_t base_port, int32_t payload_type, int32_t timestamp_inc)
{
    int32_t         ret = RET_SUCCESS;
    rtp_base_config *base_config = &g_rtp_base_config;

    do
    {
        //check parameter
        if (NULL == ip || base_port <= 0)
        {
            printf("params wrong on func:%s\n", __func__);
            ret = RET_RTP_PARAM_ERROR;
            break;
        }

        base_config->ip = ip;
        base_config->base_port = base_port;
        base_config->payload_type = payload_type;
        base_config->timestamp_inc = timestamp_inc;
        base_config->current_timestamp = 0;

#ifdef RTP_ORTP
        ret = xx_ortp_init(base_config);
#elif

#endif
    }while(0);

    return ret;
}


int32_t rtp_send(uint8_t *src, int32_t length)
{
    int32_t     ret = RET_SUCCESS;

#ifdef RTP_ORTP
    ret = xx_ortp_send(src, length);
#elif

#endif
}


void rtp_update_timestamp()
{
#ifdef RTP_ORTP
    xx_ortp_update_timestamp();
#elif

#endif
}



void rtp_deinit()
{
#ifdef RTP_ORTP
    xx_ortp_deinit();
#elif

#endif
}
