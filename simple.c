#include <stdint.h>
#include <stdio.h>

/*
**  Wrapper for libc sprintf
*/
void x_libc_ptr64tohex(char *dst, uint64_t value)
{
	sprintf(dst,"%016lX",value);
}

/*
**  Naive noob implementation
*/
void c_noob_ptr64tohex(char *dst, uint64_t value)
{
	uint8_t c;
        char    *d;

        dst[16] = 0;
        d = &dst[15];
        do
        {
                c = (value & 0xF);
                if (c > 9)
                    *(d--) = 'A' - 10 + c;
                else
                    *(d--) = '0' + c;
                value = value >> 4;
        }
        while(d >= dst);
}

/*
** Conversion by lookup table
*/
void c_luta_ptr64tohex(char *dst, uint64_t value)
{
	char    *d;

	dst[16] = 0;
	d = &dst[15];
	do
	{
		*(d--) = "0123456789ABCDEF"[(value & 0xF)];
		value = value >> 4;
	}
	while(d >= dst);
}

/*
**  Simple Reference implementation
*/
void c_simp_ptr64tohex(char *dst, uint64_t value)
{
	uint32_t c;
	char	*d;

	dst[16] = 0;
	d = &dst[15];
	do
	{
		c = (value & 0xF);
		*(d--) = '0' + c + ((unsigned)(9 - c) >> 29);
		value = value >> 4;
	}
	while(d >= dst);
}

/*
** Branchless implementation in C
*/
union Mix {
    uint64_t value;
    unsigned char pch[8];
};

void c_nobr_ptr64tohex(char *dst, uint64_t value)
{
	uint64_t a;
	union Mix c;

	a = value & 0x0F0F0F0F0F0F0F0F;
	c.value = (a + 0x0606060606060606) & 0x3030303030303030;
	c.value = ((c.value * 7) >> 4) + a + 0x3030303030303030;
	dst[1]  = c.pch[7];
	dst[3]  = c.pch[6];
	dst[5]  = c.pch[5];
	dst[7]  = c.pch[4];
	dst[9]  = c.pch[3];
	dst[11] = c.pch[2];
	dst[13] = c.pch[1];
	dst[15] = c.pch[0];
	a = (value >> 4) & 0x0F0F0F0F0F0F0F0F;
	c.value = (a + 0x0606060606060606) & 0x3030303030303030;
	c.value = ((c.value * 7) >> 4) + a + 0x3030303030303030;
	dst[0]  = c.pch[7];
	dst[2]  = c.pch[6];
	dst[4]  = c.pch[5];
	dst[6]  = c.pch[4];
	dst[8]  = c.pch[3];
	dst[10] = c.pch[2];
	dst[12] = c.pch[1];
	dst[14] = c.pch[0];
	dst[16] = 0;
}

#include <immintrin.h>

void c_simd_ptr64tohex(char *dst, uint64_t value)
{
	__m128i lut = _mm_setr_epi8('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
	uint64_t mask = 0x0F0F0F0F0F0F0F0F;
	__m128i nibbles = _mm_set_epi64x(value & mask, value>>4 & mask);
	__m128i idxs = _mm_shuffle_epi8(nibbles, _mm_setr_epi8(7,15,6,14,5,13,4,12,3,11,2,10,1,9,0,8));
	__m128i results = _mm_shuffle_epi8(lut, idxs);
	_mm_storeu_si128((void*) dst, results);
}

