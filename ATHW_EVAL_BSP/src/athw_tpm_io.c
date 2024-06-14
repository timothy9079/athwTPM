#include "x_common.h"
#include "configs.h"

#include "athw_tpm_io.h"
#include "athw_tpm_tis.h"
//#include "athw_errno.h"

extern athw_if_handle_t *if_hnd; 

static int _athw_tpm2io_ctx_param_check(tpmio_ctx_t *h) 
{
	int ret = EOK;
	X_ASSERT_PARAM((h != NULL), ERRNGATE(ESNULLP));
	X_ASSERT_PARAM((h->handle != NULL), ERRNGATE(ESNULLP));
	X_ASSERT_PARAM((h->txlv != NULL), ERRNGATE(ESNULLP));
	X_ASSERT_PARAM((h->rxlv != NULL), ERRNGATE(ESNULLP));

	X_ASSERT_PARAM((h->txlv->value != NULL), ERRNGATE(ESNULLP));
	X_ASSERT_PARAM((h->txlv->len > 0), ERRNGATE(ESZEROL));

	if(h->rxlv != NULL) {
		X_ASSERT_PARAM((h->rxlv->value != NULL), ERRNGATE(ESNULLP));
		X_ASSERT_PARAM((h->rxlv->len > 0), ERRNGATE(ESZEROL));
	}

	return ret;

}

/**
 * @fn athw_tpm2io_spi - TPM command send and receive
 * 
 * @author hyowon.cho (2024-03-14)
 * 
 * @param [in,out]priv   TPM Input/Output context handle 
 * 
 * @return On success, return EOK, otherwise return ne 
 */
int athw_tpm2io_spi(void *priv) 
{
	int ret = EOK;
	tpmio_ctx_t *ctx = NULL;
#ifdef _ATHW_STM32_TPM_
	SPI_HandleTypeDef *handle; // = (SPI_HandleTypeDef *)ctx->handle;
	HAL_StatusTypeDef status = HAL_OK;
#else
#error "Unsupport Device, Please check the Device Define"
	//void *handle; // = ctx->handle;
	//uint32_t status = 0UL;
#endif

	int timeout = ATHW_TPM_SPI_WAIT_RETRY;
	
	ctx = (tpmio_ctx_t *)priv;

	ret = _athw_tpm2io_ctx_param_check(ctx);
	if(ret != EOK) {
		tr_log("SPI I/O param check fail");
		goto errdone;
	}

	handle = (SPI_HandleTypeDef *)ctx->handle;

#ifdef _ATHW_STM32_TPM_
	__HAL_SPI_ENABLE(handle);
	HAL_GPIO_WritePin(ATHW_TPM_SPI_IO_PORT, ATHW_TPM_SPI_CS, GPIO_PIN_RESET);

	status = HAL_SPI_TransmitReceive(handle, (uint8_t *)ctx->txlv->value,
									 (uint8_t *)ctx->rxlv->value,
									 ATHW_TPM_TIS_HEADER_SZ,
									 ATHW_TPM_SPI_TMO);
	if(status != HAL_OK) {
		tr_log("TPM SPI TIS HEADER exchange fail");
		ret = ERRNGATE(EIOFAIL);
		goto errdone;
	}

	// Check for TPM wait states
	if((((uint8_t *)ctx->rxlv->value)[ATHW_TPM_TIS_HEADER_SZ - 1] & ATHW_TPM_TIS_READY_MASK) == 0) {
		do {
			// check for TPM ready
			status = HAL_SPI_TransmitReceive(handle, (uint8_t *)ctx->txlv->value,
											 (uint8_t *)ctx->rxlv->value, 1,
											 ATHW_TPM_SPI_TMO);
			if(((uint8_t *)ctx->rxlv->value)[0] & ATHW_TPM_TIS_READY_MASK) 
				break;


		} while(status == HAL_OK && --timeout > 0);
#ifdef ATHW_TPM_DEBUG
		//tr_log("TPM SPI Ready wait %d \r\n", ATHW_TPM_SPI_WAIT_RETRY - timeout);
#endif			

		if(timeout <= 0) {
			tr_log("TPM ready check fail");
			ret = ERRNGATE(EIOFAIL);
			goto errdone;
		}


	}

	// Send remainder data
	status = HAL_SPI_TransmitReceive(handle,
									 &((uint8_t *)ctx->txlv->value)[ATHW_TPM_TIS_HEADER_SZ],
									 &((uint8_t *)ctx->rxlv->value)[ATHW_TPM_TIS_HEADER_SZ],
									 ctx->txlv->len - ATHW_TPM_TIS_HEADER_SZ,
									 ATHW_TPM_SPI_TMO);
	




	//HAL_GPIO_WritePin(TPM_SPI_CS_PORT, TPM_SPI_CS_PIN, GPIO_PIN_SET); 
#else
#error "Unsupport Device, Please check the Device Define"
#endif

	 
		 
errdone:

#ifdef _STM32_TPM_
	HAL_GPIO_WritePin(ATHW_TPM_SPI_IO_PORT, ATHW_TPM_SPI_CS, GPIO_PIN_SET);
	__HAL_SPI_DISABLE(handle);
#endif

	if(status != HAL_OK) {
		ret = ERRNGATE(status);
	}


	return ret;

}

int athw_tpm2io_spi_bypass(void *priv)
{
	int ret = EOK;
	tpmio_ctx_t *ctx = NULL;
#ifdef _ATHW_STM32_TPM_
	SPI_HandleTypeDef *handle; // = (SPI_HandleTypeDef *)ctx->handle;
	HAL_StatusTypeDef status = HAL_OK;
#else
#error "Unsupport Device, Please check the Device Define"
	//void *handle; // = ctx->handle;
	//uint32_t status = 0UL;
#endif

	int timeout = ATHW_TPM_SPI_WAIT_RETRY;

	ctx = (tpmio_ctx_t *)priv;

	ret = _athw_tpm2io_ctx_param_check(ctx);
	if(ret != EOK){
		tr_log("SPI I/O param check fail");
		goto errdone;
	}

	handle = (SPI_HandleTypeDef *)ctx->handle;

#ifdef _ATHW_STM32_TPM_
	//__HAL_SPI_ENABLE(handle);
	HAL_GPIO_WritePin(ATHW_TPM_SPI_IO_PORT, ATHW_TPM_SPI_CS, GPIO_PIN_RESET);

	status = HAL_SPI_TransmitReceive(handle,(uint8_t *)ctx->txlv->value,
									 (uint8_t *)ctx->rxlv->value,
									 ctx->txlv->len,
									 ATHW_TPM_SPI_TMO);
#endif    
errdone:

#ifdef _STM32_TPM_
	HAL_GPIO_WritePin(ATHW_TPM_SPI_IO_PORT, ATHW_TPM_SPI_CS, GPIO_PIN_SET);
	//__HAL_SPI_DISABLE(handle);
#endif

	if(status != HAL_OK){
		ret = ERRNGATE(status);
	}


	return ret;

}
/**
 * 
 * 
 * @author hyowon.cho (2024-03-14)
 * 
 * @param priv   
 * 
 * @return int 
 */
int athw_tpm2io_i2c(void *priv) 
{
	(void)priv;
	return 0;

}

/**
 * @gn athw_tpm_io - TPM command transfer and receive
 * 
 * @author hyowon.cho (2024-03-15)
 * 
 * @param addr   
 * @param isread 
 * @param ptr    
 * @param len    
 * 
 * @return int 
 */
int athw_tpm_io(uint32_t addr, int isread, void *ptr, int len)
{
	int ret = EOK;
	//tpmio_ctx_t *ctx;
	tpmio_ctx_t ctx;
	tpmio_lv_t txlv;
	tpmio_lv_t rxlv;

	uint8_t txbuf[ATHW_TPM_SPI_FRAME_SZ + ATHW_TPM_TIS_HEADER_SZ] = { 0, };
	uint8_t rxbuf[ATHW_TPM_SPI_FRAME_SZ + ATHW_TPM_TIS_HEADER_SZ] = { 0, }; 

	X_ASSERT_PARAM((ptr != NULL), ERRNGATE(ESNULLP));
#if 0
	ctx = (tpmio_ctx_t *)priv;
	ret = _athw_tpm2io_ctx_param_check(ctx);
	if(ret != EOK) {
		tr_log("Context struct parch check fail");
		goto errdone;
	}
#endif

	txlv.value = (void *)txbuf;  //x_memcreate(ATHW_TPM_TIS_HEADER_SZ + len);
	rxlv.value = (void *)rxbuf;  //x_memcreate(ATHW_TPM_TIS_HEADER_SZ + len);

	txlv.len = rxlv.len = len + ATHW_TPM_TIS_HEADER_SZ;

	ctx.handle = if_hnd->h_tpmspi;//ATHW_TPM_SPI_PORT;
	ctx.txlv = &txlv;
	ctx.rxlv = &rxlv;

	// Generate TPM Header
	((uint8_t *)ctx.txlv->value)[1] = (addr >> 16) 	& 0xFF;
	((uint8_t *)ctx.txlv->value)[2] = (addr >> 8) & 0xFF;
	((uint8_t *)ctx.txlv->value)[3] = (addr)&0xFF;

	if(isread) {
		((uint8_t *)ctx.txlv->value)[0] = ATHW_TPM_TIS_READ | ((len & 0xFF) - 1);
	}
	else {
		((uint8_t *)ctx.txlv->value)[0] = ATHW_TPM_TIS_WRITE | ((len & 0xFF) - 1);
		x_memcpy(&((uint8_t *)ctx.txlv->value)[ATHW_TPM_TIS_HEADER_SZ], len,
				 (const void *)ptr, len);
		
	}

	_x_print_bin("Send Buf", (const uint8_t *)ctx.txlv->value,len + ATHW_TPM_TIS_HEADER_SZ);
	ret = athw_tpm2io_spi((void *)&ctx);
	if(ret != EOK) {
		goto errdone;
	}

	_x_print_bin("RX Buf", (const uint8_t *)ctx.rxlv->value, len + ATHW_TPM_TIS_HEADER_SZ);


	if(isread) {
		x_memcpy(ptr, len, &((uint8_t *)ctx.rxlv->value)[ATHW_TPM_TIS_HEADER_SZ], len);
	}

#ifdef ATHW_TPM_DEBUG
	//printf("TPM IO exchange : (ret : %d, sz : %d) \r\n", ret, len);
	//_x_print_bin("TX", (const uint8_t *)ctx.txlv->value, len + ATHW_TPM_TIS_HEADER_SZ);
	//_x_print_bin("RX", (const uint8_t *)ptr, len);
#endif

	//ctx.txlv = ()x_memcreate(sizeof ctx.txlv);
	//x_memcreate(ctx.txlv->value, len + ATHW_TPM_TIS_HEADER_SZ);

	//x_memcreate(ctx.rxlv, )

errdone:

//	x_memdestory(txlv.value);
//	x_memdestory(rxlv.value);
	return ret;
}

int athw_tpm_io_bypass(void *ptr, int len, void *out)
{
	int ret = EOK;
	//tpmio_ctx_t *ctx;
	tpmio_ctx_t ctx;
	tpmio_lv_t txlv;
	tpmio_lv_t rxlv;

	uint8_t txbuf[ATHW_TPM_SPI_FRAME_SZ + ATHW_TPM_TIS_HEADER_SZ] = { 0, };
	uint8_t rxbuf[ATHW_TPM_SPI_FRAME_SZ + ATHW_TPM_TIS_HEADER_SZ] = { 0, };

	X_ASSERT_PARAM((ptr != NULL), ERRNGATE(ESNULLP));
#if 0
	ctx = (tpmio_ctx_t *)priv;
	ret = _athw_tpm2io_ctx_param_check(ctx);
	if(ret != EOK) {
		tr_log("Context struct parch check fail");
		goto errdone;
	}
#endif

	txlv.value = (void *)ptr;  //x_memcreate(ATHW_TPM_TIS_HEADER_SZ + len);
	rxlv.value = (void *)out;  //x_memcreate(ATHW_TPM_TIS_HEADER_SZ + len);

	txlv.len = rxlv.len = len + ATHW_TPM_TIS_HEADER_SZ;

	ctx.handle = if_hnd->h_tpmspi; //ATHW_TPM_SPI_PORT;
	ctx.txlv = &txlv;
	ctx.rxlv = &rxlv;


	//_x_print_bin("Bypass Send Buf", (const uint8_t *)ctx.txlv->value, len + ATHW_TPM_TIS_HEADER_SZ);
	//ret = athw_tpm2io_spi_bypass((void *)&ctx);
	ret = athw_tpm2io_spi((void *)&ctx); 
	if(ret != EOK){
		goto errdone;
	}

	//_x_print_bin("Bypass RX Buf", (const uint8_t *)ctx.rxlv->value, len + ATHW_TPM_TIS_HEADER_SZ); 


#ifdef ATHW_TPM_DEBUG
	//printf("TPM IO exchange : (ret : %d, sz : %d) \r\n", ret, len);
	//_x_print_bin("TX", (const uint8_t *)ctx.txlv->value, len + ATHW_TPM_TIS_HEADER_SZ);
	//_x_print_bin("RX", (const uint8_t *)ptr, len);
#endif

	//ctx.txlv = ()x_memcreate(sizeof ctx.txlv);
	//x_memcreate(ctx.txlv->value, len + ATHW_TPM_TIS_HEADER_SZ);

	//x_memcreate(ctx.rxlv, )

	errdone:

//	x_memdestory(txlv.value);
//	x_memdestory(rxlv.value);
	return ret;
}



