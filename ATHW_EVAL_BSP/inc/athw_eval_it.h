#ifndef _ATHW_EVAL_IT_H
#define _ATHW_EVAL_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct  _io_buf_t {
	void *tx;           /*! Transfer buffer pointer*/
	int txlen;          /*! Length of Transfer */
	void *rx;           /*! Receive buffer pointer */
	int rxlen;         /*! Length of Receive */
}io_buf_t;

typedef  struct  _spi_ioctx_t {
	void *h_ctx;        /*! Device handle*/
	void *h_io;         /*! IO device handle (like UART, I2C and SPI) */
	io_buf_t iobuf;     /*! Data exchange structure*/
	int state; 			/*! State of interface interrupt call*/
}spi_ioctx_t;
#pragma pack()

#pragma pack(1)
/**
 * ATHW Interface handle structure
 * 
 * @author User (1/9/2024)
 */
typedef struct _athw_it_handle_t {
	UART_HandleTypeDef	*h_debuguart;		/**< Serial & Debugf UART interface*/
	SPI_HandleTypeDef 	*h_hostspi;		 	/**< TPM SPI Master interface */
	SPI_HandleTypeDef 	*h_tpmspi;			/**< Host SPI Slave interface*/
}athw_if_handle_t;

#pragma pack()

int athw_eval_it_init(void *priv);
#ifdef __cplusplus
}
#endif
#endif
