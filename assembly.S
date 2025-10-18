.intel_syntax noprefix
.text

	.globl	test
	.type	test,@function
test:
	mov	edx, 0x043C
	bextr	rax,rdi,rdx
	ret

	.globl	bogo
	.type	bogo,@function
bogo:
	mov	edx, 0x043C
	bextr	rax,rdi,rdx
	ret

	.align	64
	.globl	a_nobr_ptr64tohex
	.type	a_nobr_ptr64tohex,@function
a_nobr_ptr64tohex:
	mov	r11,0x0606060606060606
	lea	r10,[r11+r11*4]
	shr	r10,1

	mov	rcx,rsi				/* A = p	*/
	and	rcx,r10				/* A & 0x0F...	*/

	shr	rsi,4				/* B = p >> 4	*/
	and	rsi,r10				/* B & 0x0F...	*/

	lea	r9,[rcx+r11]			/* A + 0x06...	*/
	lea	r8,[rsi+r11]			/* B + 0x06...	*/

	shr	r9,4				/* A >> 4	*/
	shr	r8,4				/* B >> 4	*/
	and	r9,r10				/* r9 is now 1 for all A >= 10 */
	and	r8,r10				/* r8 is now 1 for all B >= 10 */

	lea	rax,[rcx+r9*8]			/* r9 * 8 + A	*/
	sub	rax,r9				/* r9 is now 7 for all A >= 10 */
	lea	rax,[rax+r11*8]			/* A + 0x30...	*/

	lea	rdx,[rsi+r8*8]
	sub	rdx,r8
	lea	rdx,[rdx+r11*8]			/* B + 0x30...	*/

	mov	byte ptr [rdi + 16], 0

	mov	[rdi+15],al
	mov	[rdi+13],ah
	mov	[rdi+9],eax
	mov	[rdi+6],eax
	bswap	rax
	mov	[rdi+4],eax
	mov	[rdi+3],eax
	mov	[rdi+1],al
	mov	[rdi+3],ah

	mov	[rdi+14],dl
	mov	[rdi+12],dh
	shr	rdx,16
	mov	[rdi+10],dl
	mov	[rdi+ 8],dh
	shr	rdx,16
	mov	[rdi+ 6],dl
	mov	[rdi+ 4],dh
	shr	rdx,16
	mov	[rdi+ 2],dl
	mov	[rdi   ],dh

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

	.globl	a_luta_ptr64tohex
	.type	a_luta_ptr64tohex, @function
a_luta_ptr64tohex:
	call	.YHex
        .string "0123456789ABCDEF"
.YHex:
	pop	rcx
        mov     eax, 16
        mov     [rdi+rax], ah
.LY0:
        mov     rdx, rsi
        shr     rsi, 4
        and     edx, 15
        mov     dl, [rcx+rdx]
        dec     rax
        mov     [rdi+rax], dl
        jne     .LY0
        ret
	.size	a_luta_ptr64tohex, .-a_luta_ptr64tohex

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

