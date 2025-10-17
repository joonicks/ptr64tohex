#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int fdwrite(int fd, const char *format, ...);
uint64_t test(uint64_t);

char binstr[100];

char *bin2str(uint64_t n, int lim)
{
	uint64_t f;
	int	i;
	char	*p = binstr;

	f = 1;
	f = f << (lim-1);
	for(i=0;i<lim;i++)
	{
		*(p++) = ((f & n) == 0) ? '0' : '1';
		f = f >> 1;
	}
	*p = 0;
	return(binstr);
}

/*
** Various C implementations (simple.c)
*/
void simpc_ptr64tohex(char *dst, uint64_t value);
void noobc_ptr64tohex(char *dst, uint64_t value);
void brless_ptr64tohex(char *dst, uint64_t value);

/*
** Assembler implementation ptr64tohex.S
*/
void c_ptr64tohex(char *dst, uint64_t value);
void e_ptr64tohex(char *dst, uint64_t value);

char	str[100];

void foo(uint64_t value)
{
	fdwrite(1,"\n     libc(%%016lX): %016lX\n",value);

	simpc_ptr64tohex(str,value);
	fdwrite(1," simpc_ptr64tohex: %s\n",str);

	noobc_ptr64tohex(str,value);
	fdwrite(1," noobc_ptr64tohex: %s\n",str);

	brless_ptr64tohex(str,value);
	fdwrite(1,"brless_ptr64tohex: %s\n",str);

	e_ptr64tohex(str,value);
	fdwrite(1,"     e_ptr64tohex: %s\n",str);
}

int main(int argc, char **argv, char **envp)
{
	uint64_t x;

	foo(0x1a203b405c6d7e8f);
	foo(0x1234567890abcdef);
	foo(0xabcdef896e5a23f0);
	foo((uint64_t)&main);

	for(int i=0;i<16;i++)
	{
		fdwrite(1,"%s   %2i   %X:   ",bin2str(i,4),i,i);
		x = test((uint64_t)i<<60);
		fdwrite(1,"%s\n",bin2str(x,64));
	}
}
