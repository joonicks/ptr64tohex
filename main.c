#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>

uint64_t rand64(void)
{
	long long unsigned int val;
	while (_rdrand64_step(&val) == 0) ;  // keep trying until success
	return val;
}

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

uint64_t rdtsc(void)
{
	uint32_t lo, hi;
	asm volatile (
		"rdtsc" : "=a"(lo), "=d"(hi)
	);
	return ((uint64_t)hi << 32) | lo;
}

/*
** Various C implementations (simple.c)
*/
void x_libc_ptr64tohex(char *dst, uint64_t value); /* just a simple wrapper for libc sprintf */
void c_noob_ptr64tohex(char *dst, uint64_t value);
void c_luta_ptr64tohex(char *dst, uint64_t value);
void c_simp_ptr64tohex(char *dst, uint64_t value);
void c_nobr_ptr64tohex(char *dst, uint64_t value);
void c_simd_ptr64tohex(char *dst, uint64_t value);

/*
** Various assembler implementation (assembly.S)
*/
void a_bext_ptr64tohex(char *dst, uint64_t value);
void a_noob_ptr64tohex(char *dst, uint64_t value);
void a_nobr_ptr64tohex(char *dst, uint64_t value);
void a_luta_ptr64tohex(char *dst, uint64_t value);
void a_luro_ptr64tohex(char *dst, uint64_t value);
void a_ssse_ptr64tohex(char *dst, uint64_t value);

char	str[100];

typedef void (hexfunc)(char *, uint64_t);

void bar(const char *name, hexfunc func, uint64_t value, const char *credit)
{
	uint64_t r[100000];
	uint64_t t1, t2;
	uint32_t i, t;

	for(i=0;i<100000;i++)
	{
		r[i] = rand64();
	}

	t1 = rdtsc();
	for(i=0;i<100000;i++)
	{
		func(str,r[i]);
	}
	t2 = rdtsc();
	t = t2 - t1;

	func(str,value);
	fdwrite(1,"%s: %s (avg ~%.2f)%s\n",name,str,((double)t/100000),(credit == NULL) ? "": credit);
}

void foo(uint64_t value)
{
	bar("     libc(%016lX)",x_libc_ptr64tohex,value,NULL);
	bar("c_noob_ptr64tohex",c_noob_ptr64tohex,value,NULL);
	bar("c_luta_ptr64tohex",c_luta_ptr64tohex,value,NULL);
	bar("c_simp_ptr64tohex",c_simp_ptr64tohex,value,NULL);
	bar("c_nobr_ptr64tohex",c_nobr_ptr64tohex,value,NULL);
	bar("c_simd_ptr64tohex",c_simd_ptr64tohex,value," (dzaima)");
	bar("a_noob_ptr64tohex",a_noob_ptr64tohex,value,NULL);
	bar("a_luta_ptr64tohex",a_luta_ptr64tohex,value,NULL);
	bar("a_luro_ptr64tohex",a_luro_ptr64tohex,value,NULL);
	bar("a_bext_ptr64tohex",a_bext_ptr64tohex,value,NULL);
	bar("a_nobr_ptr64tohex",a_nobr_ptr64tohex,value,NULL);
	bar("a_ssse_ptr64tohex",a_ssse_ptr64tohex,value,NULL);

	fdwrite(1,"%c",'\n');
}

int main(int argc, char **argv, char **envp)
{
	fdwrite(1,"%c",'\n');

	foo(0x1a203b405c6d7e8f);
	foo(0x1234567890abcdef);
	foo(0xabcdef896e5a23f0);
	foo((uint64_t)&main);

#ifdef FALSE
	for(int i=0;i<16;i++)
	{
		uint64_t x;
		fdwrite(1,"%s   %2i   %X:   ",bin2str(i,4),i,i);
		x = test((uint64_t)i<<60);
		fdwrite(1,"%s\n",bin2str(x,64));
	}
#endif
}
