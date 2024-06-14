#ifndef _athw_tpm2_tis_h
#define _athw_tpm2_tis_h

#ifdef __cplusplus
extern "C"
{
#endif

#include "athw_tpm2.h"

#define ATHW_TPM_TIS_READ       			0x80
#define ATHW_TPM_TIS_WRITE      			0x00

#define ATHW_TPM_TIS_HEADER_SZ  			4

#define ATHW_TPM_TIS_READY_MASK 			0x01

/* Typically only 0-2 wait states are required */
#ifndef ATHW_TPM_TIS_MAX_WAIT
#define ATHW_TPM_TIS_MAX_WAIT   			3
#endif

// TPM Context structure
typedef struct _athw_tpm2ctx_t {
	// TPM2 TIS information
	int locality;
	uint32_t caps;
	uint32_t didvid;

	uint8_t cmdrsp_buf[4096];
	uint8_t rid;

    tpmio_ctx_t *ioctx;

}athw_tpm2ctx_t; 


int athw_tpm2_tis_request_locality(void *handle, int tmo);

int athw_tpm2_tis_checklocality(int locality, uint8_t *access);
int athw_tpm2_tis_getinfo(void *handle);
int athw_tpm2_tis_startup_wait(int tmo);
int athw_tpm2_check_locality(int locality, uint8_t *access);
int athw_tpm2_tis_sendcmd(athw_tpm2ctx_t *ctx, athw_tpm2pkt_t *pkt);

#ifdef __cplusplus
}
#endif

#endif
