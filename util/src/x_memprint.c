/*
 * \file        x_memprint.c
 * \author      Trustkey
 * \anchor      ATHW utilty
 * \version     1.0-rc
 * \date        2024-11-20
 * \brief       Copy source buffer to destination buffer
 * \bug         No known bug
 * \note
 *
 * \copyright Copyright(c) 2020 Trustkey,Co.,Ltd All right reserved.
 *
 * This software contains confidential information of eWBM, Co., Ltd.
 * and Unauthorized distribution of this software, or any protion of it, are 
 * prohibited.
 *
 */

#include "x_common.h"

#define LINE_LEN 16
void x_mem_print_bin(
        const uint8_t *title /**< [in] display name strings */, 
        const uint8_t* buffer /**< [in] print buffer  */, 
        uint32_t length /**< [in] length of buffer */
        )
{
    uint32_t i, sz;
    
    if(title) {
        printf("\t%s (length of buffer: %d) \r\r\n", title, length) ;
    }

    if (!buffer) {
        printf("\tNULL\r\n");
        return;
    }

    while (length > 0) {
        sz = length;
        if (sz > LINE_LEN)
            sz = LINE_LEN;

        printf("\t");
        for (i = 0; i < LINE_LEN; i++) {
            if (i < length)
                printf("%02x ", buffer[i]);
            else
                printf("   ");
        }
        printf("| ");
        for (i = 0; i < sz; i++) {
            if (buffer[i] > 31 && buffer[i] < 127)
                printf("%c", buffer[i]);
            else
                printf(".");
        }
        printf("\r\r\n");


        buffer += sz;
        length -= sz;
    }
}

/**
 * \fn void print_byte_string(u8 *data, size_t count)
 * \brief Print a byte string in hexdecimal format, 16-bytes per line.
 *
 * \param data		byte string to be printed
 * \param count		number of bytes to be printed
 */
void print_byte_string(u8 *data, size_t count)
{
//  int i, print_newline = 0;
//
//  for (i = 0; i < count; i++) {
//  	printf(" %02x", data[i]);
//  	print_newline = (i % 16 == 15);
//  	if (print_newline)
//  		putc('\n');
//  }
//  /* Avoid duplicated newline at the end */
//  if (!print_newline)
//  	putc('\n');

    return;
}

