.intel_syntax noprefix
.text

	.globl	test
	.type	test,@function
test:
	mov	edx, 0x043C
	bextr	rax,rdi,rdx
	ret

	.globl	a_nobr_ptr64tohex
	.type	a_nobr_ptr64tohex,@function
a_nobr_ptr64tohex:
	movabs	r11,0x0606060606060606
	lea	r10,[r11+r11*4]
	shr	r10,1

	mov	rcx,rsi				/* A = p	*/
	and	rcx,r10				/* A & 0x0F...	*/

	shr	rsi,4				/* B = p >> 4	*/
	and	rsi,r10				/* B & 0x0F...	*/

	lea	r9,[rcx+r11]			/* A + 0x06...	*/
	shr	r9,4				/* A >> 4	*/
	and	r9,r10
	lea	rax,[r9*8]
	sub	rax,r9
	lea	rax,[rax+r11*8]			/* A + 0x30...	*/
	add	rax,rcx				/* A + (p & 0x0F) */

	mov	[rdi+15],al
	mov	[rdi+13],ah
	mov	[rdi+9],eax
	mov	[rdi+6],eax
	bswap	rax
	mov	[rdi+4],eax
	mov	[rdi+3],eax
	mov	[rdi+1],al
	mov	[rdi+3],ah

	lea	r9,[rsi+r11]			/* B + 0x06...	*/
	shr	r9,4				/* B >> 4	*/
	and	r9,r10
	lea	rax,[r9*8]
	sub	rax,r9
	lea	rax,[rax+r11*8]			/* B + 0x30...	*/
	add	rax,rsi				/* B + (p & 0x0F) */

	mov	[rdi+14],al
	mov	[rdi+12],ah
	shr	rax,16
	mov	[rdi+10],al
	mov	[rdi+ 8],ah
	shr	rax,16
	mov	[rdi+ 6],al
	mov	[rdi+ 4],ah
	shr	rax,16
	mov	[rdi+ 2],al
	mov	[rdi   ],ah

	mov	byte ptr [rdi + 16], 0
	ret
	.size	a_nobr_ptr64tohex, .-a_nobr_ptr64tohex

	.globl	a_noob_ptr64tohex
	.type	a_noob_ptr64tohex, @function
a_noob_ptr64tohex:
	mov	eax,0x10			/* upper bits of rax are cleared */
	mov	[rdi+0x10],ah
.LNoob1:
	mov	ecx,esi
	and	ecx,0x0F
	lea	edx,[0x30+rcx]
	cmp	dl,0x39
	jbe	.LNoob2
	lea	edx,[0x37+rcx]
.LNoob2:
	mov	[rdi+rax-1],dl
	shr	rsi,4
	dec	rax
	jne	.LNoob1
	ret
	.size	a_noob_ptr64tohex, .-a_noob_ptr64tohex

	.globl	y_ptr64tohex
	.type	y_ptr64tohex, @function
y_ptr64tohex:
	call	.YHex
        .string "0123456789ABCDEF"
.YHex:
	pop	rcx
        mov     eax, 16
        mov     [rsi+rax], ah
.LY0:
        mov     rdx, rdi
        shr     rdi, 4
        and     edx, 15
        mov     dl, [rcx+rdx]
        dec     rax
        mov     [rsi+rax], dl
        jne     .LY0
        ret

/*
** arg 1: rdi = char *pch
** arg 2: rsi = *p
*/
	.globl	a_bext_ptr64tohex
	.type	a_bext_ptr64tohex,@function
a_bext_ptr64tohex:
	mov	edx, 0x043C
.Lbext1:
	bextr	rcx,rsi,rdx
	lea	rax, [-10+rcx]
	not	rax
        shr     rax, 61
        lea     eax, [48+rcx+rax]
        mov     [rdi], al
	inc	rdi
        sub	dl,4
        jnb     .Lbext1
	mov	[rdi],ah
	ret
	.size	a_bext_ptr64tohex, .-a_bext_ptr64tohex

