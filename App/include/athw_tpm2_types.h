#ifndef _athw_tpm2_types_h
#define _athw_tpm2_types_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#if defined(__ICCARM__)
#include <cmsis_iccarm.h>
#elif defined(__GNUC__)
#include <fetures.h>
#include <cmsis_gccarm.h>
#endif

typedef uint8_t  UINT8;
typedef uint8_t  BYTE;
typedef int8_t   INT8;
#if !defined(BOOL) && !defined(HAVE_BOOL)
typedef int      BOOL;
#endif
typedef uint16_t UINT16;
typedef int16_t  INT16;
typedef uint32_t UINT32;
typedef int32_t  INT32;

#ifndef _W64
typedef uint64_t UINT64;
typedef int64_t  INT64;
#endif

#ifndef YES
#define YES 1
#endif
#ifndef NO
#define NO 0
#endif


/**
 * Performs a left circular arthmetic shift of x by y value
 * 
 * @author hyowon.cho (2024-03-25)
 * 
 * @param x      
 * @param y      
 * 
 * @return uint32_t 
 */
static inline uint32_t rotleft_fixed(uint32_t x, uint32_t y) {
	return (x << y) | (x >> (sizeof(y) * 8 - y));
}

static inline uint32_t rotright_fixed(uint32_t x, uint32_t y) {
	return (x >> y) | (x << (sizeof(y) * 8 - y));
}

static inline uint16_t _byte_swap_16(uint16_t value) {
//#if defined(__ICCARM__)
//	return (uint16_t)__REV16(value);
//#elif defined(__GNUC_PREREQ) && __GNUC_PREREQ(4, 3) 
//	return (uint16_t)__builtin_bswap16(value);
//#else
	return (value >> 8) | (value << 8);
//#endif
}

static inline uint32_t _byte_swap_32(uint32_t value) {
//#if defined(__ICCARM__)
//	return (uint32_t)__REV32(value);
//#elif defined(__GNUC_PREREQ) && __GNUC_PREREQ(4, 3)
//	return (uint32_t)__builtin_bswap32(value);
//#elif define(FAST_ROTATE)
//	return (rotright_fixed(value, 8U) & 0xff00ff00) |
//		   (rotleft_fixed(value, 8U) & 0x00ff00ff);
//#else
	value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
	return rotleft_fixed(value, 16U);
//#endif
}


#ifdef __cplusplus
}
#endif

#endif
