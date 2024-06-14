#ifndef _x_typedef_h
#define _x_typedef_h

#include <stdint.h>

//typedef signed char s8;
//typedef unsigned char u8;
//
//typedef signed short s16;
//typedef unsigned short u16;
//
//typedef signed int s32;
//typedef unsigned int u32;
//
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;


typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef unsigned int uint;
typedef unsigned long ulong;

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#ifdef __CHECK_ENDIAN__
#define __bitwise __bitwise__
#else
#define __bitwise
#endif

typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;

typedef __u16 __bitwise __sum16;
typedef __u32 __bitwise __wsum;

#endif

