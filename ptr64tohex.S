.intel_syntax noprefix
.text
.globl c_ptr64tohex
.type c_ptr64tohex,@function

/*
** rdi = *p
** rsi = char *pch
** x = (((((0x0A & 0x0F) + 0x06) & 0x30) * 7) >> 4) + 0x0A + 0x30;
*/
c_ptr64tohex:
	movabs	r10,0x0f0f0f0f0f0f0f0f
	movabs	r11,0x0606060606060606

	mov	rcx,rdi				/* A = p	*/
	and	rcx,r10				/* A & 0x0F...	*/

	shr	rdi,4				/* B = p >> 4	*/
	and	rdi,r10				/* B & 0x0F...	*/

	lea	r9,[rcx+r11]			/* A + 0x06...	*/
	shr	r9,4				/* A >> 4	*/
	and	r9,r10
	lea	rax,[r9*8]
	sub	rax,r9
	lea	rax,[rax+r11*8]			/* A + 0x30...	*/
	add	rax,rcx				/* A + (p & 0x0F) */

	mov	[rsi+15],al
	mov	[rsi+13],ah
	mov	[rsi+9],eax
	mov	[rsi+6],eax
	bswap	rax
	mov	[rsi+4],eax
	mov	[rsi+3],eax
	mov	[rsi+1],al
	mov	[rsi+3],ah

	lea	r9,[rdi+r11]			/* B + 0x06...	*/
	shr	r9,4				/* B >> 4	*/
	and	r9,r10
	lea	rax,[r9*8]
	sub	rax,r9
	lea	rax,[rax+r11*8]			/* B + 0x30...	*/
	add	rax,rdi				/* B + (p & 0x0F) */

	mov	[rsi+14],al
	mov	[rsi+12],ah
	shr	rax,16
	mov	[rsi+10],al
	mov	[rsi+ 8],ah
	shr	rax,16
	mov	[rsi+ 6],al
	mov	[rsi+ 4],ah
	shr	rax,16
	mov	[rsi+ 2],al
	mov	[rsi   ],ah

	mov	byte ptr [rsi + 16], 0
	ret
