#ifndef SDF_H
#define SDF_H

typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef char                int8_t;
typedef short int           int16_t;
typedef int                 int32_t;
typedef long long           int64_t;


//define return error number
#define RET_SUCCESS             0
#define RET_PARAM_COMMON_ERROR  0x00000001
#define RET_PARAM_INVALID_VALUE (RET_PARAM_COMMON_ERROR + 1)
#define RET_PARAM_TOO_BIG       (RET_PARAM_COMMON_ERROR + 2)
#define RET_PARAM_TOO_SMALL     (RET_PARAM_COMMON_ERROR + 3)

#define RET_CODEC_COMMON_ERROR  0x00010001
#define RET_CODEC_PARAM_ERROR   (RET_CODEC_COMMON_ERROR + 1)
#define RET_CODEC_OTHER_ERROR   (RET_CODEC_COMMON_ERROR + 2)
#define RET_CODEC_ENCODE_ERROR  (RET_CODEC_COMMON_ERROR + 3)

#define RET_RTP_COMMON_ERROR    0x00020001
#define RET_RTP_CONNECT_ERROR   (RET_RTP_COMMON_ERROR + 1)
#define RET_RTP_SEND_ERROR      (RET_RTP_COMMON_ERROR + 2)
#define RET_RTP_RECV_ERROR      (RET_RTP_COMMON_ERROR + 3)
#define RET_RTP_PARAM_ERROR     (RET_RTP_COMMON_ERROR + 4)

#define RET_MEM_COMMON_ERROR    0x00030001
#define RET_MEM_NOT_ENOUGH_ERR  (RET_MEM_COMMON_ERROR + 1)
#define RET_MEM_INVALID_POINT   (RET_MEM_COMMON_ERROR + 2)


#define RET_MFC_COMMON_ERROR    0x00040001
#define RET_MFC_INIT_ERROR      (RET_MFC_COMMON_ERROR + 1)
#define RET_MFC_EXE_ERROR       (RET_MFC_COMMON_ERROR + 2)
#define RET_MFC_GET_IN_BUF_ERR  (RET_MFC_COMMON_ERROR + 3)
#define RET_MFC_GET_OUT_BUF_ERR (RET_MFC_COMMON_ERROR + 4)




typedef struct
{
    int32_t         width;
    int32_t         height;
    int32_t         fps;
    uint16_t        base_port;
}global_config;

#endif // SDF_H
