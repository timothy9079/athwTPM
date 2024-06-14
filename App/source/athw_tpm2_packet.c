#include "athw_tpm2.h"

#ifdef _LITTLE_ENDIAN_ORDER_
    #define cpu_to_be16(d) _byte_swap_16(d)
    #define cpu_to_be32(d) _byte_swap_32(d)
    #define be16_to_cpu(d) _byte_swap_16(d)
    #define be32_to_cpu(d) _byte_swap_32(d)
#else
	#define cpu_to_be16(d) (d)
	#define cpu_to_be32(d) (d)
	#define cpu_to_be64(d) (d)
	#define be16_to_cpu(d) (d)
	#define be32_to_cpu(d) (d)
	#define be64_to_cpu(d) (d)
#endif

#if 0
/* convert 16 bit integer to opaque */
static inline void _c16toa(uint16_t wc_u16, uint8_t *c) {
    c[0] = (wc_u16 >> 8) & 0xff;
    c[1] =  wc_u16 & 0xff;
}
/* convert 32 bit integer to opaque */
static inline void _c32toa(uint32_t wc_u32, uint8_t *c) {
    c[0] = (wc_u32 >> 24) & 0xff;
    c[1] = (wc_u32 >> 16) & 0xff;
    c[2] = (wc_u32 >>  8) & 0xff;
    c[3] =  wc_u32 & 0xff;
}
#endif



void athw_tpm2_packet_u16tou8_array(uint16_t val, uint8_t *b)
{
    if (b) {
        ATHW_UINT16_TO_BE_BUF(b,val);
    }

}

void athw_tpm2_packet_u32tou8_array(uint32_t val, uint8_t *b) 
{
    if (b) {
        ATHW_UINT32_TO_BE_BUF(b, val);
    }
}

uint16_t ahtw_tpm2_packet_swapu16(uint16_t val) 
{
    return cpu_to_be16(val);
}

uint32_t ahtw_tpm2_packet_swapu32(uint32_t val) 
{
    return cpu_to_be32(val);
}


int athw_tpm2_pktinit(void *ctxhndl, athw_tpm2pkt_t *pkt) 
{

    //athw_tpm2pkt_t *pkt = NULL;
    athw_tpm2ctx_t *ctx = NULL;

    X_ASSERT_PARAM((ctxhndl != NULL), ERRNGATE(ESNULLP));
    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));

    pkt = (athw_tpm2pkt_t *)pkt;
    ctx = (athw_tpm2ctx_t *)ctxhndl;

    pkt->buf = ctx->cmdrsp_buf;
    pkt->pos = ATHW_TPM2_HEADER_SZ;
    pkt->size = sizeof ctx->cmdrsp_buf;

    return EOK;
}

int athw_tpm2_pktappend_u8(athw_tpm2pkt_t *pkt, uint8_t val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint8_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint8_t) <= pkt->size)) {
        pkt->buf[pkt->pos] = val;
        pkt->pos += sizeof(uint8_t);
    }

    return EOK;

}

int athw_tpm2_pktparse_u8(athw_tpm2pkt_t *pkt, uint8_t *val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint8_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((val != NULL), ERRNGATE(ESNULLP));

    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint8_t) <= pkt->size)) {
        if (val) value = pkt->buf[pkt->pos];

        pkt->pos += sizeof(uint8_t);
    }


    if(value) *val = value;

    return EOK;

}

int athw_tpm2_pktappend_u16(athw_tpm2pkt_t *pkt, uint16_t val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint16_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint16_t) <= pkt->size)) {
        value = be16_to_cpu(value); 
        x_memcpy(&pkt->buf[pkt->pos], sizeof(uint16_t), &val, sizeof(uint16_t));
        pkt->pos += sizeof(uint16_t);
    }

    return EOK;
}

int athw_tpm2_pktparse_u16(athw_tpm2pkt_t *pkt, uint16_t *val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint16_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((val != NULL), ERRNGATE(ESNULLP)); 

    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint16_t) <= pkt->size)) {
        if (val) {
            x_memcpy(&value, sizeof(uint16_t), &pkt->buf[pkt->pos], sizeof(uint16_t));
            value = be16_to_cpu(value);
        }
        pkt->pos += sizeof(uint16_t);
    }

    if(value) *val = value;

    return EOK;
}

int athw_tpm2_pktappend_u32(athw_tpm2pkt_t *pkt, uint32_t val) 
{
    //athw_tpm2pkt_t *pkt = NULL;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint32_t) <= pkt->size)) {
        val = cpu_to_be32(val);
        x_memcpy(&pkt->buf[pkt->pos], sizeof(uint32_t), &val, sizeof(uint32_t));
        pkt->pos += sizeof(UINT32);
    }

    return EOK;
}


int athw_tpm2_pktparse_u32(athw_tpm2pkt_t *pkt, uint32_t *val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint16_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((val != NULL), ERRNGATE(ESNULLP));

    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(uint32_t) <= pkt->size)) {
        if (val) {
            x_memcpy(&value, sizeof(uint32_t), &pkt->buf[pkt->pos], sizeof(uint32_t));
            value = be32_to_cpu(value);
        }
        pkt->pos += sizeof(uint32_t);
    }


    if(value) *val = value;

    return EOK;
}

int athw_tpm2_pktappend_s32(athw_tpm2pkt_t *pkt, int32_t val) 
{
    //athw_tpm2pkt_t *pkt = NULL;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(int32_t) <= pkt->size)) {
        val = cpu_to_be32(val);
        x_memcpy(&pkt->buf[pkt->pos], sizeof(int32_t), &val, sizeof(int32_t));
        pkt->pos += sizeof(int32_t);
    }

    return EOK;
}


int athw_tpm2_pktparse_s32(athw_tpm2pkt_t *pkt, int32_t *val) 
{
    //athw_tpm2pkt_t *pkt = NULL;
    uint16_t value = 0;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((val != NULL), ERRNGATE(ESNULLP));

    pkt = (athw_tpm2pkt_t *)pkt;

    if (pkt && (pkt->pos + (int)sizeof(int32_t) <= pkt->size)) {
        if (val) {
            x_memcpy(&value, sizeof(int32_t), &pkt->buf[pkt->pos], sizeof(int32_t));
            value = be32_to_cpu(value);
        }
        pkt->pos += sizeof(int32_t);
    }


    //(val != NULL)? *val = value : (void)0U;
    if(value) *val = value;

    return EOK;
}

int athw_tpm2_pktappend_bytes(athw_tpm2pkt_t *pkt, uint8_t *buf, int len) 
{
    //athw_tpm2pkt_t *pkt = NULL;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((buf != NULL), ERRNGATE(ESNULLP));
    X_ASSERT_PARAM((len > 0), ERRNGATE(ESZEROL));

    pkt = (athw_tpm2pkt_t *)pkt; 

    if (pkt && (pkt->pos + len <= pkt->size)) {
        if (buf) x_memcpy(&pkt->buf[pkt->pos], len, buf, len);
        pkt->pos += len;
    }

    return EOK;
}


int athw_tpm2_pktparse_bytes(athw_tpm2pkt_t *pkt, uint8_t *buf, int len) 
{
    //athw_tpm2pkt_t *pkt = NULL;

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));
    //X_ASSERT_PARAM((buf != NULL), ERRNGATE(ESNULLP));
    X_ASSERT_PARAM((len > 0), ERRNGATE(ESZEROL));

    pkt = (athw_tpm2pkt_t *)pkt;

    if (buf) {
        int copylen = len;

        if (pkt->pos + copylen > pkt->size) {
            copylen = pkt->size - pkt->pos;
        }

        x_memcpy(buf, copylen, &pkt->buf[pkt->pos], copylen);
    }

    pkt->pos += len;

    return EOK;
}


int athw_tpm2_pktmark_u32(athw_tpm2pkt_t *pkt, int *size) 
{

    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));

    //(size != NULL)? *size = pkt->pos : (void)0;
    if (size) {
        *size = pkt->pos;
    }

    return athw_tpm2_pktappend_u32(pkt, 0);
}


int athw_tpm2_pktplace_u32(athw_tpm2pkt_t *pkt, int size) 
{
    uint32_t data = 0UL;
    uint8_t *ptrsz = NULL;
    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));

    ptrsz = &pkt->buf[size];
    size += sizeof(uint32_t);

    if (size <= pkt->pos) {
        size = pkt->pos - size;

        data = cpu_to_be32(size);
        x_memcpy(ptrsz, sizeof data,&data, sizeof data);
    }


    return EOK;
}

int athw_tpm2_pktparse(int rc, athw_tpm2pkt_t *pkt) 
{
    uint32_t temp;
    uint32_t sz_resp;
    X_ASSERT_PARAM((pkt != NULL), ERRNGATE(ESNULLP));

    pkt->pos = 0;

    athw_tpm2_pktparse_u16(pkt, NULL);
    athw_tpm2_pktparse_u32(pkt, &sz_resp);
    athw_tpm2_pktparse_u32(pkt, &temp);

    pkt->size = sz_resp;
    rc = temp;


    return rc;

}


/**
 * @fn athw_tpm2_pktfinialize - 
 * 
 * @author hyowon.cho (2024-03-27)
 * 
 * @param pkt    TPM packet structure
 * @param tag    Tag
 * @param cc     Command Code
 * 
 * @return int 
 */
int athw_tpm2_pktfinalize(athw_tpm2pkt_t *pkt, uint16_t tag, uint32_t cc) 
{
    uint32_t sz_cmd = pkt->pos;
    pkt->pos = 0;

    athw_tpm2_pktappend_u16(pkt, tag);
    athw_tpm2_pktappend_u32(pkt, sz_cmd);
    athw_tpm2_pktappend_u32(pkt, cc);

    pkt->pos = sz_cmd;
    return sz_cmd;

}


