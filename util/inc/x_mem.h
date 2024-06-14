#ifndef __XMEM_H
#define __XMEM_H

#ifdef __cplusplus
extern "C"
{
#endif

void *x_memcreate(size_t size);
void x_memdestory(void *buffer);
int x_memcpy(void *dst, int in_sizebytes, const void *src, int count);
int x_memset(void *dest, int count, uint8_t value);
int x_memzero(void *dest, int count);

#ifdef __cplusplus
}
#endif
#endif
