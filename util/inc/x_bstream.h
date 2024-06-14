#ifndef x_bstream_h
#define x_bstream_h

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \name Macros for converting a little endian byte buffer to integers.
 */
/**@{*/
/*! \brief convert little endian byte buffer to int16_t. */
#define ATHW_BYTES_TO_INT16(n, p)      {n = ((int16_t)(p)[0] + ((int16_t)(p)[1] << 8));}
/*! \brief convert little endian byte buffer to uint16_t. */
#define ATHW_BYTES_TO_UINT16(n, p)     {n = ((uint16_t)(p)[0] + ((uint16_t)(p)[1] << 8));}
/*! \brief convert little endian byte buffer to uint24_t. */
#define ATHW_BYTES_TO_UINT24(n, p)     {n = ((uint16_t)(p)[0] + ((uint16_t)(p)[1] << 8) + \
                                        ((uint16_t)(p)[2] << 16));}
/*! \brief convert little endian byte buffer to uint32_t. */
#define ATHW_BYTES_TO_UINT32(n, p)     {n = ((uint32_t)(p)[0] + ((uint32_t)(p)[1] << 8) + \
                                        ((uint32_t)(p)[2] << 16) + ((uint32_t)(p)[3] << 24));}
/**@}*/

/**
 * \name Macros for converting a big endian byte buffer to integers.
 */
/**@{*/
/*! \brief convert big endian byte buffer to uint16_t. */
#define ATHW_BYTES_BE_TO_UINT16(n, p)     {n = ((uint16_t)(p)[1] + ((uint16_t)(p)[0] << 8));}
/*! \brief convert big endian byte buffer to uint32_t. */
#define ATHW_BYTES_BE_TO_UINT32(n, p)     {n = ((uint32_t)(p)[3] + ((uint32_t)(p)[2] << 8) + \
                                           ((uint32_t)(p)[1] << 16) + ((uint32_t)(p)[0] << 24));}
/**@}*/


/**
 * \name Macros for converting little endian integers to array of bytes
 */
/**@{*/
/*! \brief convert little endian uint16_t to array of bytes. */
#define ATHW_UINT16_TO_BYTES(n)        ((uint8_t) (n)), ((uint8_t)((n) >> 8))
/*! \brief convert little endian uint32_t to array of bytes. */
#define ATHW_UINT32_TO_BYTES(n)        ((uint8_t) (n)), ((uint8_t)((n) >> 8)), ((uint8_t)((n) >> 16)), ((uint8_t)((n) >> 24))
/**@}*/

/**
 * \name Macros for converting big endian integers to array of bytes
 */
/**@{*/
/*! \brief convert big endian uint16_t to array of bytes. */
#define ATHW_UINT16_TO_BE_BYTES(n)       ((uint8_t)((n) >> 8)), ((uint8_t) (n))
/*! \brief convert big endian uint32_t to array of bytes. */
#define ATHW_UINT32_TO_BE_BYTES(n)       ((uint8_t)((n) >> 24)), ((uint8_t)((n) >> 16)), ((uint8_t)((n) >> 8)), ((uint8_t) (n))
/**@}*/

/**
 * \name Macros for converting little endian integers to single bytes
 */
/**@{*/
/*! \brief convert little endian uint16_t to byte 0. */
#define ATHW_UINT16_TO_BYTE0(n)        ((uint8_t) (n))
/*! \brief convert little endian uint16_t to byte 1. */
#define ATHW_UINT16_TO_BYTE1(n)        ((uint8_t) ((n) >> 8))

/*! \brief convert little endian uint32_t to byte 0. */
#define ATHW_UINT32_TO_BYTE0(n)        ((uint8_t) (n))
/*! \brief convert little endian uint32_t to byte 1. */
#define ATHW_UINT32_TO_BYTE1(n)        ((uint8_t) ((n) >> 8))
/*! \brief convert little endian uint32_t to byte 2. */
#define ATHW_UINT32_TO_BYTE2(n)        ((uint8_t) ((n) >> 16))
/*! \brief convert little endian uint32_t to byte 3. */
#define ATHW_UINT32_TO_BYTE3(n)        ((uint8_t) ((n) >> 24))
/**@}*/

/**
 * \name Macros for converting a little endian byte stream to integers, with increment.
 */
/**@{*/
/*! \brief convert little endian byte stream to uint8_t, incrementing one byte. */
#define ATHW_BSTREAM_TO_INT8(n, p)     {n = (int8_t)(*(p)++);}
/*! \brief convert little endian byte stream to int8_t, incrementing one byte. */
#define ATHW_BSTREAM_TO_UINT8(n, p)    {n = (uint8_t)(*(p)++);}
/*! \brief convert little endian byte stream to int16_t, incrementing two bytes. */
#define ATHW_BSTREAM_TO_INT16(n, p)    {ATHW_BYTES_TO_INT16(n, p); p += 2;}
/*! \brief convert little endian byte stream to uint16_t, incrementing two bytes. */
#define ATHW_BSTREAM_TO_UINT16(n, p)   {ATHW_BYTES_TO_UINT16(n, p); p += 2;}
/*! \brief convert little endian byte stream to uint24_t, incrementing three bytes. */
#define ATHW_BSTREAM_TO_UINT24(n, p)   {ATHW_BYTES_TO_UINT24(n, p); p += 3;}
/*! \brief convert little endian byte stream to uint32_t, incrementing four bytes. */
#define ATHW_BSTREAM_TO_UINT32(n, p)   {ATHW_BYTES_TO_UINT32(n, p); p += 4;}
/*! \brief convert little endian byte stream to uint40_t, incrementing five bytes. */
/**@}*/

/**
 * \name Macros for converting a big endian byte stream to integers, with increment.
 */
/**@{*/
/*! \brief convert big endian byte stream to uint16_t, incrementing one byte. */
#define ATHW_BSTREAM_BE_TO_UINT16(n, p)   {ATHW_BYTES_BE_TO_UINT16(n, p); p += 2;}
/*! \brief convert big endian byte stream to 24-bit uint32_t (MSB 0), incrementing one byte. */
#define ATHW_BSTREAM_BE_TO_UINT24(n, p)   {ATHW_BYTES_BE_TO_UINT24(n, p); p += 3;}
/**@}*/

/**
 * \name Macros for converting integers to a little endian byte stream, without increment.
 */
/**@{*/
/*! \brief convert uint16_t to little endian byte stream. */
#define ATHW_UINT16_TO_BUF(p, n)       {(p)[0] = (uint8_t)(n); (p)[1] = (uint8_t)((n) >> 8);}
/*! \brief convert uint24_t to little endian byte stream. */
#define ATHW_UINT24_TO_BUF(p, n)       {(p)[0] = (uint8_t)(n); (p)[1] = (uint8_t)((n) >> 8); \
                                  (p)[2] = (uint8_t)((n) >> 16);}
/*! \brief convert uint32_t to little endian byte stream. */
#define ATHW_UINT32_TO_BUF(p, n)       {(p)[0] = (uint8_t)(n); (p)[1] = (uint8_t)((n) >> 8); \
                                  (p)[2] = (uint8_t)((n) >> 16); (p)[3] = (uint8_t)((n) >> 24);}

/**@}*/


/**
 * \name Macros for converting integers to a big endian byte stream, without increment.
 */
/**@{*/
/*! \brief convert uint16_t to big endian byte stream. */
#define ATHW_UINT16_TO_BE_BUF(p, n)    {(p)[0] = (uint8_t)((n) >> 8); (p)[1] = (uint8_t)(n);}
/*! \brief convert 24-bit uint32_t (MSB 0) to big endian byte stream. */
#define ATHW_UINT24_TO_BE_BUF(p, n)    {(p)[0] = (uint8_t)((n) >> 16); (p)[1] = (uint8_t)((n) >> 8); \
                                  (p)[2] = (uint8_t)(n);}
/*! \brief convert uint32_t to big endian byte stream. */
#define ATHW_UINT32_TO_BE_BUF(p, n)    {(p)[0] = (uint8_t)((n) >> 24); (p)[1] = (uint8_t)((n) >> 16); \
                                  (p)[2] = (uint8_t)((n) >> 8); (p)[3] = (uint8_t)(n);}
/**@}*/




#ifdef __cplusplus
}
#endif



#endif
