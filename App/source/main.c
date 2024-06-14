#include "main.h"
#include "athw_tpm2.h"
#include "athw_it_types.h"

#include <stdio.h>
#include <string.h>

extern void error_handler(void *priv);
UART_HandleTypeDef dbguart;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small msg_info (option LD Linker->Libraries->Small msg_info
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    //(void)f;
    // place your implementation of futpc here
    while( HAL_UART_Transmit(&dbguart, (uint8_t *)&ch, 1, 30000) != HAL_OK) {
        ;
    }

    return ch;
}

GETCHAR_PROTOTYPE
{
    //(void)f;
    uint8_t ch = 0;
    while( HAL_UART_Receive(&dbguart, (uint8_t *)&ch, 1, 30000) != HAL_OK) {
        ;
    }

    return ch;
}

#if 0
int __io_putchar(int ch) 
{
    uint8_t c[1];
    c[0] = ch & 0x00FF;


    HAL_UART_Transmit(&dbguart, &*c, 1, 10);
    return ch;
}

int _write(int32_t file, uint8_t *ptr, int32_t len) 
{
    (void)file;

    int idx;

    for( idx = 0; idx < len; idx++ ) {
        __io_putchar(*ptr++);
    }

    return len;

    //return HAL_UART_Transmit(&huart1, ptr, len, 0xFFFF);
}


int _read (int fd, char *ptr, int len)
{
    /* Read "len" of char to "ptr" from file id "fd"
    * Return number of char read.
    * Need implementing with UART here. */
    (void)fd;

    return HAL_UART_Receive(&dbguart, (uint8_t*)ptr, len, 0xFFFF);
}

void _ttywrch(int ch) {
    /* Write one char "ch" to the default console
    * Need implementing with UART here. */
    _write(0, (uint8_t *)&ch, 1);
}
#endif

extern int athw_tpm2_init(void *handle, int tmotries);
extern void _pkt_proc_suspend_tick(void);
extern void _pkt_proc_resume_tick(void);
extern uint8_t spi_rx_end;
extern int rxcnt; 
extern uint8_t rxbuf[64];
extern uint8_t is_timbegin;
static uint32_t gn_starttick; 

extern int is_processing;
extern uint8_t respbuf [ ATHW_TPM_SPI_FRAME_SZ + ATHW_TPM_TIS_HEADER_SZ ];

const struct athw_fs_config cfg = {
    // block device operations
    .read  = athw_fs_rambd_read,
    .prog  = athw_fs_rambd_prog,
    .erase = athw_fs_rambd_erase,
    .sync  = athw_fs_rambd_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 128,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};


static void consume_spi_rxfifo(void *handle) 
{
    SPI_HandleTypeDef *hnd = (SPI_HandleTypeDef *)handle;
    uint8_t consumedata = 0;
    int consumecnt = 0L;

    // RX buffer not empty
    //printf("consume data : ");
    while( __HAL_SPI_GET_FLAG(hnd, SPI_FLAG_RXNE) ) {
        consumedata = (uint8_t)hnd->Instance->DR;
        consumecnt++;
        //printf("0x%x ", consumedata);
    }
    
    printf("\r\n");


    //x_mem_print_bin("RX not empty", &consumedata, consumecnt);

    return;
}

uint8_t rxtemp[64] = {0, };

static int athw_tpm_receive_transmit(SPI_HandleTypeDef *hnd, uint8_t *txbuf, int len) 
{
    int ret =  0L;
    uint8_t temp = 1;
    uint8_t validsync = 1;
    int txcnt = 0L;
    int real_tx_cnt = 0;
    // 3 sec timeout 
    
    while( 1 ) {
        ret = HAL_SPI_Receive(hnd, &temp, 1, 1000);
        switch( ret ) {
        case HAL_OK:

            rxtemp[txcnt] = temp;
            //printf("info [%d] rx 0x%x  tx 0x%x \r\n", txcnt, temp, txbuf[txcnt]);
            if( txcnt++ > len ) {
                txcnt = 0;
            }
            
            if( temp == 0 ) {
              validsync = 1;
            }
            else {
              
              validsync = txbuf[real_tx_cnt+4];
              real_tx_cnt++;
            }
            

             //if(rxtemp[txcnt] 
            //ret = HAL_SPI_Transmit(hnd, &txbuf[txcnt], 1, 500);
            ret = HAL_SPI_Transmit(hnd, &validsync, 1, 1000);
            //tr_log("info [%d] rx 0x%x  tx 0x%x ret = %d \r\n", 
            //       txcnt, temp, txbuf[txcnt], ret);
            break;
        case HAL_ERROR:
            tr_log("SPI RX HAL ERROR \r\n");
            goto errordone;
            //x_mem_print_bin("error", rxbuf, 12);;
            break;
        case HAL_BUSY:
            tr_log("SPI BUSY \r\n");
            goto errordone;
            break;
        case HAL_TIMEOUT:\
            tr_log("SPI Timeout \r\n");
            goto errordone;
            break;
        default:
            tr_log("Unknown Error : %d \r\n", ret);
            goto errordone;
            break;
        }

    }

errordone:

    return txcnt;
}


int main(void)
{
    extern int athw_pkt_proc_init(void *priv);
    extern uint32_t SystemCoreClock;
    int ret = 0;
    //UART_HandleTypeDef	debguart;		/**< Serial & Debugf UART interface*/
    SPI_HandleTypeDef 	hostspi;		 	/**< TPM SPI Master interface */
    SPI_HandleTypeDef 	tpmspi;	
    //athw_if_handle_t    ifhnd;
    
    int remaing_len = 0;
    uint8_t checker_bit = 0;

    uint32_t test_resp = 0x00000001;
    
    athw_tpm2ctx_t tpmctx;
    uint8_t rxdata = 0;

    athw_if_handle_t ifhnd = {
      .h_debuguart = &dbguart,
      .h_hostspi = &hostspi,
      .h_tpmspi = &tpmspi
    };



    x_memzero(ifhnd.h_debuguart, sizeof *ifhnd.h_debuguart);
    x_memzero(ifhnd.h_hostspi, sizeof *ifhnd.h_hostspi);
    x_memzero(ifhnd.h_tpmspi, sizeof *ifhnd.h_tpmspi);


//    ifhnd.h_debuguart = (UART_HandleTypeDef *)&dbguart;
//    ifhnd.h_hostspi = (SPI_HandleTypeDef *)&hostspi;
//    ifhnd.h_tpmspi = (SPI_HandleTypeDef *)&tpmspi;


    // ATHW Device peripheral and low device setting
    ret = athw_eval_it_init((void *)&ifhnd);
    if( ret != EOK ) {
        error_handler(NULL);
    }

    printf("System Core Clock : %d Hz \r\n", SystemCoreClock);

    ret = athw_pkt_proc_init((void *)&ifhnd);
    if( ret != EOK ) {
        printf("Packet proc initialization fail !!! \r\n");
        error_handler(NULL);
    }

    ret = athw_tpm2_init((void *)&tpmctx, 5);
    if( ret != EOK ) {
        printf("TPM2 device init fail \r\n");
        error_handler(NULL);
    }

    printf("tpm init done !!! \r\n");

    do {

        //consume_spi_rxfifo(ifhnd.h_hostspi); 
#if 1
        if( is_processing == false ) {
            //ret = HAL_SPI_TransmitReceive(ifhnd.h_hostspi, &rxdata,  &rxdata, 1, 0xFFFFFFFF);
            ret = HAL_SPI_Receive(ifhnd.h_hostspi, &rxdata, 1, 0xFFFFFFFF); 
            switch( ret ) {
            case HAL_OK:
                rxbuf[rxcnt++] = rxdata;
                
                //printf("rx[%d] = 0x%x \r\n" , rxcnt - 1, rxbuf[rxcnt - 1]);

                if( rxcnt == 4  && (rxbuf[1] != 0xD4)) {
                    // Packet-sync not valid
                    //x_mem_print_bin("header", rxbuf, rxcnt);
                    consume_spi_rxfifo(ifhnd.h_hostspi);

                    ret = HAL_SPI_Transmit(ifhnd.h_hostspi,          /*! SPI Handle*/
                                           &respbuf[4],    /*! point of rx */
                                           4,                        /*! length of packet*/
                                           0xFFFFFFFF);              /*! Timeout */
                    
                    x_mem_print_bin("header", rxbuf, rxcnt);
                    x_mem_print_bin("header", &respbuf[4], 4);
                    
                    x_memzero(rxbuf, sizeof rxbuf);
                    remaing_len = 0;
                    rxcnt = 0;
                    continue;
                }

                //checker_bit = (rxbuf[0] & 0xF0) >> 4;

                // remaining packet 
                if( rxcnt == 4 && checker_bit == 0 && (rxbuf[1] == 0xD4)) {
                    // Original remaing_len = (rxbuf[0] & 0x3F) + 1;
                  //if((rxbuf[0] & 0x3F) < 4) {
                    remaing_len = ((rxbuf[0] & 0x3F) + 2);
                   //}/
                  //else {
                  //  remaing_len = ((rxbuf[0] & 0x3F) - 4);
                  //}
                    checker_bit = 1;
                    //printf("Remaing Len : %d, [0x%x] \r\n", remaing_len, rxbuf[0]);
                    //x_mem_print_bin("Sync packet", rxbuf, rxcnt);
                }

                if( remaing_len == rxcnt  ) {

                    remaing_len -= 1;

                    //is_processing = true;
                    //consume_spi_rxfifo(ifhnd.h_hostspi); 
                    //
                    //__HAL_SPI_DISABLE(ifhnd.h_hostspi);
                    // end of packet receving
                    //x_mem_print_bin("TPM packet", rxbuf, remaing_len);
                    // TO DO : TPM command processing 
                    athw_tpm_io_bypass((void *)rxbuf, remaing_len + 4, respbuf);
                    //athw_tpm_io((void *)rxbuf, remaing_len + 4, respbuf); 

                    //x_mem_print_bin("Resp packet", respbuf, remaing_len + 4);
                    // TO DO : Transfering to HOST
                    
                    //athw_tpm_io_bypass();
//                    ret = HAL_SPI_TransmitReceive(ifhnd.h_hostspi,          /*! SPI Handle*/
//                                                (uint8_t *)&respbuf,    /*! point of tx */
//                                                (uint8_t *)&respbuf,    /*! point of rx */
//                                                remaing_len + 4,                        /*! length of packet*/
//                                                0xFFFFFFFF);              /*! Timeout */


//                  ret = HAL_SPI_Transmit(ifhnd.h_hostspi,          /*! SPI Handle*/
//                                                  (uint8_t *)respbuf,    /*! point of rx */
//                                                  remaing_len + 4,                        /*! length of packet*/
//                                                  0xFFFFFFFF);              /*! Timeout */
                        


                    ret = athw_tpm_receive_transmit(ifhnd.h_hostspi, respbuf, remaing_len + 4);

//                  delay_ms(1);
//
//                  ret = HAL_SPI_Transmit(ifhnd.h_hostspi,          /*! SPI Handle*/
//                                         (uint8_t *)respbuf,    /*! point of rx */
//                                         remaing_len + 4,                        /*! length of packet*/
//                                         0xFFFFFFFF);              /*! Timeout */

                        
                    consume_spi_rxfifo(ifhnd.h_hostspi); 
                    x_mem_print_bin("-- RX packet ---", rxbuf, remaing_len);
                    x_mem_print_bin("-- RX Temp ---", rxtemp, ret);
                    x_mem_print_bin("-- TX packet ---", respbuf, remaing_len + 4);
                    printf("TX DONE ( with count : %d ) !!! \r\n", ret);
                    
                    x_memzero(rxbuf, sizeof rxbuf);
                    //x_memzero(respbuf, sizeof respbuf); 
                    checker_bit = 0;
                    rxcnt = 0;
                    //__HAL_SPI_DISABLE(ifhnd.h_hostspi); 
                    
                    

                }





                //_pkt_proc_resume_tick();
                break;
            case HAL_ERROR:
                printf("SPI RX HAL ERROR \r\n");
                //x_mem_print_bin("error", rxbuf, 12);;
                break; 
            case HAL_BUSY:
                printf("SPI BUSY \r\n");
                break; 
            case HAL_TIMEOUT:\
                printf("SPI Timeout \r\n");
                break;
            default:
                printf("Unknown Error : %d \r\n", ret);
                break;
            }

        }
#endif
    } while (1);

    //return 0;      
}
