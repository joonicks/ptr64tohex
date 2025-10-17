#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
**  Simple Reference implementation in C
*/
void simpc_ptr64tohex(char *dst, uint64_t value)
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
**  Naive noob implementation in C
*/
void noobc_ptr64tohex(char *dst, uint64_t value)
{
	char	c,*d;

	dst[15] = 0;
	d = &dst[15];
	do
	{
		c = (value & 0xF);
		*(d--) = '0' + c + ((c > 9) ? 7 : 0);
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

void brless_ptr64tohex(char *dst, uint64_t value)
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
