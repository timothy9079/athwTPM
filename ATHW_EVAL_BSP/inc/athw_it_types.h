#ifndef _TPM_TYPES_H
#define _TPM_TYPES_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>

#include "athw_errno.h"
#include "x_common.h"

//#define _IFPACKED				(__attribute__((packed)))

#ifndef bool
typedef uint8_t    bool;
#endif

#ifndef true
#define true   (1==1)
#endif

#ifndef false
#define false   (!true)
#endif

#define min_t(type, x, y) ({            \
    type __min1 = (x);          \
    type __min2 = (y);          \
    __min1 < __min2 ? __min1: __min2; })

#define max_t(type, x, y) ({            \
    type __max1 = (x);          \
    type __max2 = (y);          \
    __max1 > __max2 ? __max1: __max2; })


#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#define BIT(nr)         (1 << (nr))

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define SPI_XFER_BEGIN      BIT(0)  /* Assert CS before transfer */
#define SPI_XFER_END        BIT(1)  /* Deassert CS after transfer */
#define SPI_XFER_ONCE       (SPI_XFER_BEGIN | SPI_XFER_END)

#define MAX_SPI_BYTES              32



/**
 * @brief Interface Handle access index
 */
typedef enum _e_if_hndtype {
	ATHW_HNDTYPE_HOST 			= 0,
	ATHW_HNDTYPE_TPM,
	ATHW_HNDTYPE_DBG,
	ATHW_HDNTYPE_UNKNOWN
}if_hdntype_e;


/**
 * @brief Interface callback calling procedure type
 */
#define ATHW_IFCB_PROCEDURE_TYPE_RX			(0x0101)
#define ATHW_IFCB_PROCEDURE_TYPE_TX 		(0x0102)
#define ATHW_IFCB_PROCEDURE_TYPE_RXTX 		(0x0103)

#ifdef __cplusplus
}
#endif

#endif
