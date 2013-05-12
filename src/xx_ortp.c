/*
 *
 *
 *
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "include/ortp/ortp.h"
#include "include/ortp/rtp.h"
#include "include/sdf.h"
#include "include/rtp.h"


int32_t xx_ortp_init(rtp_base_config *base_config)
{
    int32_t     ret = RET_SUCCESS;
    xx_ortp_config *ortp_config = &g_xx_ortp_config;
    int8_t      *m_SSRC = NULL;

    do
    {
        //check input parameters
        if (NULL == base_config)
        {
            printf("input param wrong on func:%s\n", __func__);
            ret = RET_RTP_PARAM_ERROR;
            break;
        }

        ortp_config->ip = base_config->ip;
        ortp_config->base_port = base_config->base_port;
        ortp_config->payload_type = base_config->payload_type;
        ortp_config->timestamp_inc = base_config->timestamp_inc;
        ortp_config->current_timestamp = base_config->current_timestamp;

        ortp_init();
        ortp_scheduler_init();

        ortp_config->rtp_session = rtp_session_new(RTP_SESSION_SENDONLY);
        if (NULL == ortp_config->rtp_session)
        {
            printf("out of memory when RTP session initial\n");
            ret = RET_RTP_PARAM_ERROR;
            break;
        }

        rtp_session_set_scheduling_mode(rtp_session->rtp_session, 1);
        rtp_session_set_blocking_mode(rtp_session->rtp_session, 1);
        rtp_session_set_remote_addr(rtp_session->rtp_session, ortp_config->ip, ortp_config->base_port);
        rtp_session_set_payload_type(rtp_session->rtp_session, ortp_config->payload_type);

        m_SSRC = getenv("SSRC");
        if (NULL != m_SSRC)
        {
            rtp_session_set_ssrc(rtp_session->rtp_session, atoi(m_SSRC));
        }

        rtp_session->cur_timestamp = 0;
        printf("RTP initialize successfully\n");
    }while(0);

    return ret;
}



int32_t xx_ortp_send(uint8_t *src, int32_t length)
{
    int32_t     sended_bytes = 0;
    xx_ortp_config *ortp_config = &g_xx_ortp_config;

    //parameter check
    if (NULL == src || 0 >= length)
    {
        printf("wrong input params on func:%s\n", __func__);
        return -1;
    }

    sended_bytes = rtp_session_send_with_ts(ortp_config->rtp_session,(uint8 *)src,length,ortp_config->current_timestamp);

    return sended_bytes;
}


void xx_ortp_update_timestamp()
{
    xx_ortp_config *ortp_config = &g_xx_ortp_config;

    ortp_config->current_timestamp += ortp_config->timestamp_inc;
}

void xx_ortp_deinit()
{
    xx_ortp_config *ortp_config = &g_xx_ortp_config;

    rtp_session_destroy(ortp_config->rtp_session);
    ortp_config->rtp_session = NULL;
}
