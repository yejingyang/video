#ifndef RTP_H
#define RTP_H

#include "sdf.h"
#include "ortp/ortp.h"

typedef struct
{
    int8_t          *ip;
    int16_t         base_port;
    int32_t         payload_type;
    uint32_t        timestamp_inc;
    uint32_t        current_timestamp;
}rtp_base_config;


typedef struct
{
    int8_t          *ip;
    int16_t         base_port;
    int32_t         payload_type;
    uint32_t        timestamp_inc;
    uint32_t        current_timestamp;
    RtpSession      *rtp_session;
}xx_ortp_config;


int32_t rtp_init(int8_t *ip, int16_t base_port, int32_t payload_type, int32_t timestamp_inc);

int32_t rtp_send(uint8_t *src, int32_t length);

void rtp_update_timestamp();

void rtp_deinit();

rtp_base_config     g_rtp_base_config;


#if 1

int32_t xx_ortp_init(rtp_base_config *base_config);

int32_t xx_ortp_send(uint8_t *src, int32_t length);

void xx_ortp_update_timestamp();

void xx_ortp_deinit();

xx_ortp_config      g_xx_ortp_config;

#endif

#if 0

int32_t jrtp_init(rtp_base_config *base_config);

int32_t jrtp_send(uint8_t *src, int32_t length);

void jrtp_update_timestamp();

void jrtp_deinit();

#endif


#endif // RTP_H
