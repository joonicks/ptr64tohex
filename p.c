#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void ptr64tohex(void *p, char *dest)
{
    uint64_t a, b;
    char    c;

    a = (uint64_t)p;
    for(int i=7;i>=0;--i)
    {
        c = (a & 0xF) + '0';
        if (c > '9')
            c += 'A' - '9' - 1;
        dest[i*2+1] = c;
        a = a >> 4;

        c = (a & 0xF) + '0';
        if (c > '9')
            c += 'A' - '9' - 1;
        dest[i*2] = c;
        a = a >> 4;
    }
}

union Mix {
    uint64_t p;
    unsigned char pch[8];
};

void ptohex(void *p, char *dest)
{
	uint64_t a, b;
	union Mix c;

	a =  (uint64_t)p       & 0x0F0f0f0f0F0f0f0f;
	b = ((uint64_t)p >> 4) & 0x0F0f0f0f0F0f0f0f;
	c.p = (a + 0x0606060606060606) & 0x3030303030303030;
	c.p = ((c.p * 7) >> 4) + a + 0x3030303030303030;
	dest[1] = c.pch[7];
	dest[3] = c.pch[6];
	dest[5] = c.pch[5];
	dest[7] = c.pch[4];
	dest[9] = c.pch[3];
	dest[11] = c.pch[2];
	dest[13] = c.pch[1];
	dest[15] = c.pch[0];
	c.p = (b + 0x0606060606060606) & 0x3030303030303030;
	c.p = ((c.p * 7) >> 4) + b + 0x3030303030303030;
	dest[0] = c.pch[7];
	dest[2] = c.pch[6];
	dest[4] = c.pch[5];
	dest[6] = c.pch[4];
	dest[8] = c.pch[3];
	dest[10] = c.pch[2];
	dest[12] = c.pch[1];
	dest[14] = c.pch[0];
}

int main(int argc, char **argv, char **envp)
{
	char	str[100],x,y;
	void	*t;
	uint64_t g;

	t = main;
	x = (((((0x0A & 0x0F) + 0x06) & 0x30) * 7) >> 4) + 0x0A + 0x30;
	y = (((((0x0B & 0x0F) + 0x06) & 0x30) * 7) >> 4) + 0x0B + 0x30;
	sprintf(str,"0xAB [%c%c] <0x%02x> %i\n",x,y,0,'A' - '9' - 1);write(1,str,strlen(str));
	ptr64tohex(main, str);
	write(1,str,16);
	write(1,"\n",1);
	g = 0x090a090a0a09090a;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	g = g + 0x0606060606060606;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	g = g & 0x1010101010101010;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	g = (g * 7) >> 4;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	g = g + 0x090a090a0a09090a;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	g = g + 0x3030303030303030;
	sprintf(str,"%016lx\n",g);write(1,str,strlen(str));
	ptr64tohex((void*)0x1a203b405c6d7e8f, str);write(1,str,16);write(1," ptr64tohex\n",12);
	ptohex((void*)0x1a203b405c6d7e8f, str);write(1,str,16);write(1," ptohex\n",8);
	ptr64tohex((void*)0x1234567890abcdef, str);
	write(1,str,16);
	write(1,"\n",1);
	ptr64tohex((void*)0xabcdef89, str);
	write(1,str,16);
	write(1,"\n",1);
}

