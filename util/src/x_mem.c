
/*
 * \file        x_mem.c
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


/**
 * \fn void *x_mem_create(size_t)
 *
 * \brief memory allocation
 *
 * \param [in]size      - size of memory
 *
 * \return Allocated memory
 * \retval NULL         - Memory allocation fail
 * 
 */


// not approch on outside 

static void x_mem_prim_set (void *dest, uint32_t len, uint8_t value)
{
    uint8_t *dp;
    uint32_t count;
    uint32_t lcount;

    uint32_t *lp;
    uint32_t value32;

    count = len;

    dp = dest;

    value32 = value | (value << 8) | (value << 16) | (value << 24);

    /*
     * First, do the few bytes to get uint32_t aligned.
     */
    for (; count && ( (uintptr_t)dp & (sizeof(uint32_t)-1) ); count--) {
        *dp++ = value;
    }

    /*
     * Then do the uint32_ts, unrolled the loop for performance
     */
    lp = (uint32_t *)dp;
    lcount = count >> 2;

    while (lcount != 0) {

        switch (lcount) {
        /*
         * Here we do blocks of 8.  Once the remaining count
         * drops below 8, take the fast track to finish up.
         */
        default:
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            lcount -= 16;
            break;

        case 15:  *lp++ = value32;
        case 14:  *lp++ = value32;
        case 13:  *lp++ = value32;
        case 12:  *lp++ = value32;
        case 11:  *lp++ = value32;
        case 10:  *lp++ = value32;
        case 9:  *lp++ = value32;
        case 8:  *lp++ = value32;

        case 7:  *lp++ = value32;
        case 6:  *lp++ = value32;
        case 5:  *lp++ = value32;
        case 4:  *lp++ = value32;
        case 3:  *lp++ = value32;
        case 2:  *lp++ = value32;
        case 1:  *lp++ = value32;
            lcount = 0;
            break;
        }
    } /* end while */


    dp = (uint8_t *)lp;

    /*
     * compute the number of remaining bytes
     */
    count &= (sizeof(uint32_t)-1);

    /*
     * remaining bytes
     */
    for (; count; dp++, count--) {
        *dp = value;
    }

    return;
}

static void x_mem_prim_move (void *dest, const void *src, uint32_t len)
{

#define wsize   sizeof(uint32_t)
#define wmask   (wsize - 1)

    uint8_t *dp = dest;
    const uint8_t *sp = src;

    uint32_t tsp;

    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if ((uintptr_t)dp < (uintptr_t)sp) {
        /*
         * Copy forward.
         */

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.  This cannot be done
         * unless the low bits match.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {
            /*
             * determine how many bytes to copy to align operands
             */
            if ((tsp ^ (uintptr_t)dp) & wmask || len < wsize) {
                tsp = len;

            } else {
                tsp = wsize - (tsp & wmask);
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }

        /*
         * Now copy, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {

            do {
                *(uint32_t *)dp = *(uint32_t *)sp;

                sp += wsize;
                dp += wsize;
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;

        if (tsp > 0) {
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }

    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.  The alignment requires (tps&wmask) bytes to
         * align.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {

            if ((tsp ^ (uintptr_t)dp) & wmask || len <= wsize) {
                tsp = len;
            } else {
                tsp &= wmask;
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *--dp = *--sp;
            } while (--tsp);
        }

        /*
         * Now copy in uint32_t units, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {
            do {
                sp -= wsize;
                dp -= wsize;

                *(uint32_t *)dp = *(uint32_t *)sp;
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;
        if (tsp > 0) {
            tsp = len & wmask;
            do {
                *--dp = *--sp;
            } while (--tsp);
        }
    }

    return;
}
 
static void x_memfree_s(void *addr)
{
    if(addr == NULL) {
        return;
    }

    free(addr);
    addr = NULL;
}

static int x_memcpy_s(void *dest, int dmax, const char *src, int smax)
{
    uint8_t *dp;
    const uint8_t *sp;

    dp = dest;
    sp = (uint8_t *)src;

    X_RET_VAL_IF_FAIL((dp!=NULL), -1);
    X_RET_VAL_IF_FAIL((sp!=NULL), -1);

    X_RET_VAL_IF_FAIL((dmax>0), -1);
    X_RET_VAL_IF_FAIL((smax>0), -1);

    if(smax > dmax) {
        x_mem_prim_set(dp, dmax, 0);
        return -1;
    }

    if(((dp > sp) && (dp < (sp+smax))) ||
            ((sp > dp) && (sp<(dp+dmax)))) {
        x_mem_prim_set(dp, dmax, 0);
        return -1;
    }

    x_mem_prim_move(dp, sp, smax);

    return 0;
}



// end 

void *x_memcreate(size_t size)
{
    void *buffer = malloc(size);
    void *retbuf =  NULL;

    if(!buffer) {
        return NULL;
    }

    if(size >- sizeof(void *)) {
        retbuf = (void *)(((uintptr_t)buffer + sizeof(void *) - 1) & ~(sizeof(void *) -1));
        size -= (char *)retbuf - (char *)buffer;
        size &= ~(sizeof(void *) - 1);
    }
    else {
        // don't need aligin when size less then pointer size
        retbuf = buffer;
    }

    x_memzero(retbuf, size);
    return retbuf;
}


/**
 * \fn void x_mem_destory(void *buffer)
 *
 *
 */
void x_memdestory(void *buffer)
{
    if(buffer == NULL) {
        return;
    }

    free(buffer);
    buffer = NULL;

    return;
}


/**
 * \fn int x_memcpy(void *dst, int in_sizebytes, const void *src, int count)
 *
 * \brief Copy source buffer to destination buffer
 *
 * \param [out]dst - pointer to destination buffer
 * \param [in]in_sizebytes - size in byutes of the destination buffer
 * \param [in]src - pointer to source buffer
 * \param [in]count - number of bytes to copy
 *
 * \return Returns 0 If everything OK, else return the error code
 */
int x_memcpy(void *dst, int in_sizebytes, const void *src, int count)
{
    int ret = EOK;

    if(count == 0) {
        // TODO : error for size zero
        goto done;
    }

    if(src == NULL || in_sizebytes < count) {
        memset(dst, 0, in_sizebytes);


        ret = ERRNGATE(ESLEMAX | ESNULLP);
        //ret = -1;
        goto done;
    }

    x_memcpy_s(dst, count, src, count);

done:

    return ret;
}

int x_memset(void *dest, int count, uint8_t value)
{
    //X_RET_VALIDATE_ERRORCODE(dest != NULL, -EINVAL);

    //X_RET_VAL_IF_FAIL((sz_zero>0), -EINVAL);
    //X_RET_VAL_IF_FAIL((dest!=NULL), -EINVAL);
    
    X_RET_VAL_IF_FAIL((count!=0), -ESZEROL);
    X_RET_VAL_IF_FAIL((dest!=NULL), -ESNULLP);

    x_mem_prim_set(dest, count, value);
    //memset(dest, value, count);

    return 0;

}


int x_memzero(void *dest, int count)
{
    //X_RET_VALIDATE_ERRORCODE(dest != NULL, -EINVAL);

    X_RET_VAL_IF_FAIL((count!=0), -ESZEROL);
    X_RET_VAL_IF_FAIL((dest!=NULL), -ESNULLP);

    x_mem_prim_set(dest, count, 0);
    //memset(dest, 0, count);

    return 0;


}
