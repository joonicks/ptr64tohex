#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int fdwrite(int fd, const char *format, ...);

/*
**  Simple Reference implementation in C
*/
void a_ptr64tohex(void *p, char *dest)
{
	uint64_t a;
	char	c,*d;

	a = (uint64_t)p;
	d = &dest[16];
	*(d--) = 0;
	do
	{
		c = (a & 0xF) + '0';
		if (c > '9')
			c += 'A' - '9' - 1;
		*(d--) = c;
		a = a >> 4;
	}
	while(d >= dest);
}

/*
** Branchless implementation in C
*/
union Mix {
    uint64_t p;
    unsigned char pch[8];
};

void b_ptr64tohex(void *p, char *dest)
{
	uint64_t	a;
	union Mix	c;

	a =  (uint64_t)p       & 0x0F0f0f0f0F0f0f0f;
	c.p = (a + 0x0606060606060606) & 0x3030303030303030;
	c.p = ((c.p * 7) >> 4) + a + 0x3030303030303030;
	dest[1]  = c.pch[7];
	dest[3]  = c.pch[6];
	dest[5]  = c.pch[5];
	dest[7]  = c.pch[4];
	dest[9]  = c.pch[3];
	dest[11] = c.pch[2];
	dest[13] = c.pch[1];
	dest[15] = c.pch[0];
	a = ((uint64_t)p >> 4) & 0x0F0f0f0f0F0f0f0f;
	c.p = (a + 0x0606060606060606) & 0x3030303030303030;
	c.p = ((c.p * 7) >> 4) + a + 0x3030303030303030;
	dest[0]  = c.pch[7];
	dest[2]  = c.pch[6];
	dest[4]  = c.pch[5];
	dest[6]  = c.pch[4];
	dest[8]  = c.pch[3];
	dest[10] = c.pch[2];
	dest[12] = c.pch[1];
	dest[14] = c.pch[0];
	dest[16] = 0;
}

/*
** Assembler implementation ptr64tohex.S
*/
void c_ptr64tohex(void *p, char *dest);

int main(int argc, char **argv, char **envp)
{
	char	str[100],x,y;
	uint64_t g;

	x = (((((0x0A & 0x0F) + 0x06) & 0x30) * 7) >> 4) + 0x0A + 0x30;
	y = (((((0x0B & 0x0F) + 0x06) & 0x30) * 7) >> 4) + 0x0B + 0x30;
	fdwrite(1,"0xAB [%c%c] <0x%02x> %i\n",x,y,0,'A' - '9' - 1);

	g = 0x090a090a0a09090a;
	fdwrite(1,"%016lx\n",g);
	g = g + 0x0606060606060606;
	fdwrite(1,"%016lx\n",g);
	g = g & 0x1010101010101010;
	fdwrite(1,"%016lx\n",g);
	g = (g * 7) >> 4;
	fdwrite(1,"%016lx\n",g);
	g = g + 0x090a090a0a09090a;
	fdwrite(1,"%016lx\n",g);
	g = g + 0x3030303030303030;
	fdwrite(1,"%016lx\n\n",g);

	fdwrite(1,"libc(%%016lX): %016lX\n",0x1a203b405c6d7e8f);
	a_ptr64tohex((void*)0x1a203b405c6d7e8f, str);
	fdwrite(1,"a_ptr64tohex: %s\n",str);
	b_ptr64tohex((void*)0x1a203b405c6d7e8f, str);
	fdwrite(1,"b_ptr64tohex: %s\n",str);
	c_ptr64tohex((void*)0x1a203b405c6d7e8f, str);
	fdwrite(1,"c_ptr64tohex: %s\n",str);
	fdwrite(1,"c_ptr64tohex:  1   1   1 1 1 1\n\n");

	fdwrite(1,"libc(%%016lX): %016lX\n",0x1234567890abcdef);
	a_ptr64tohex((void*)0x1234567890abcdef, str);
	fdwrite(1,"a_ptr64tohex: %s\n",str);
	b_ptr64tohex((void*)0x1234567890abcdef, str);
	fdwrite(1,"b_ptr64tohex: %s\n",str);
	c_ptr64tohex((void*)0x1234567890abcdef, str);
	fdwrite(1,"c_ptr64tohex: %s\n\n",str);

	fdwrite(1,"libc(%%016lX): %016lX\n",0xabcdef896e5a23f0);
	a_ptr64tohex((void*)0xabcdef896e5a23f0, str);
	fdwrite(1,"a_ptr64tohex: %s\n",str);
	b_ptr64tohex((void*)0xabcdef896e5a23f0, str);
	fdwrite(1,"b_ptr64tohex: %s\n",str);
	c_ptr64tohex((void*)0xabcdef896e5a23f0, str);
	fdwrite(1,"c_ptr64tohex: %s\n\n",str);

	fdwrite(1,"libc(%%016lX): %016lX\n",(uint64_t)&main);
	a_ptr64tohex(main, str);
	fdwrite(1,"a_ptr64tohex: %s\n",str);
	b_ptr64tohex(main, str);
	fdwrite(1,"b_ptr64tohex: %s\n",str);
	c_ptr64tohex(main, str);
	fdwrite(1,"c_ptr64tohex: %s\n",str);
}
