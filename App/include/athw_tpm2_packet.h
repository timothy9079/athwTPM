#ifndef _athw_tpm2_packet_h
#define _athw_tpm2_packet_h

#ifdef __cplusplus
extern "C"
{
#endif

//#include "athw_tpm2_types.h"

#define ATHW_TPM2_HEADER_SZ 		10

typedef struct _tlv_t {
	uint16_t tag;
	uint32_t length;
	uint8_t  value[1];
}tlv_t;

/**
 * @brief TAG and Length structure
 * @val tag
 * @val length
 * 
 * @author hyowon.cho (2024-03-25)
 */
typedef struct _tl_t {
	uint16_t tag;		/*! TAG*/
	uint32_t length;	/*! Length */
}tl_t; 


typedef struct _athw_tpm2_header_t {
	tl_t 	tl;
	union {
		uint32_t code;
		uint32_t cc;
		int32_t  rc;
	};
}athw_tpm2_header_t;

typedef struct _athw_tpm2pkt_t {
	uint8_t *buf;
	int32_t  pos;
	int32_t  size;
}athw_tpm2pkt_t;

int athw_tpm2_pktparse(int rc, athw_tpm2pkt_t *pkt);
int athw_tpm2_pktplace_u32(athw_tpm2pkt_t *pkt, int size);
int athw_tpm2_pktmark_u32(athw_tpm2pkt_t *pkt, int *size);
int athw_tpm2_pktparse_bytes(athw_tpm2pkt_t *pkt, uint8_t *buf, int len);
int athw_tpm2_pktappend_bytes(athw_tpm2pkt_t *pkt, uint8_t *buf, int len);
int athw_tpm2_pktparse_s32(athw_tpm2pkt_t *pkt, int32_t *val);
int athw_tpm2_pktappend_s32(athw_tpm2pkt_t *pkt, int32_t val);
int athw_tpm2_pktparse_u32(athw_tpm2pkt_t *pkt, uint32_t *val);
int athw_tpm2_pktappend_u32(athw_tpm2pkt_t *pkt, uint32_t val);
int athw_tpm2_pktparse_u16(athw_tpm2pkt_t *pkt, uint16_t *val);
int athw_tpm2_pktappend_u16(athw_tpm2pkt_t *pkt, uint16_t val);
int athw_tpm2_pktparse_u8(athw_tpm2pkt_t *pkt, uint8_t *val);
int athw_tpm2_pktappend_u8(athw_tpm2pkt_t *pkt, uint8_t val);
int athw_tpm2_pktinit(void *ctxhndl, athw_tpm2pkt_t *pkt);
uint32_t ahtw_tpm2_packet_swapu32(uint32_t val);
uint16_t ahtw_tpm2_packet_swapu16(uint16_t val);
void athw_tpm2_packet_u32tou8_array(uint32_t val, uint8_t *b);
void athw_tpm2_packet_u16tou8_array(uint16_t val, uint8_t *b);
int athw_tpm2_pktfinalize(athw_tpm2pkt_t *pkt, uint16_t tag, uint32_t cc);

#ifdef __cplusplus
}
#endif
#endif
