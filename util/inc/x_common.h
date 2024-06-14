#ifndef _x_mem_common_h
#define _x_mem_common_h

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "x_defmacro.h"
#include "x_typedef.h"
#include "athw_errno.h"
#include "x_mem.h"
#include "x_log.h"
#include "x_bstream.h"


void x_mem_print_bin(const uint8_t *title, const uint8_t *buffer, uint32_t length);
void print_byte_string(u8 *data, size_t count);


#endif
