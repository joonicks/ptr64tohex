.intel_syntax noprefix
.text

	.globl	test
	.type	test,@function
test:
	mov	edx, 0x043C
	bextr	rax,rdi,rdx
	ret
//	and	rdi, 15
	lea	eax, [48+edi*8]
//	imul	ax,dh,7
	ret

/*
** rdi = *p
** rsi = char *pch
** x = (((((nibble & 0x0F) + 0x06) & 0x30) * 7) >> 4) + nibble + 0x30;
*/
	.globl	c_ptr64tohex
	.type	c_ptr64tohex,@function
c_ptr64tohex:
	mov	edx, 0x043C
.Lpu:
	bextr	rcx,rsi,rdx
	lea	rax, [-10+rcx]
	not	rax
        shr     rax, 61
        lea     eax, [48+rcx+rax]
        mov     [rdi], al
	inc	rdi
        sub	dl,4
        jnb     .Lpu
	mov	[rdi],ah
	ret








        mov     eax, 16
	mov	[rax+rsi], ah
.LC2:
        mov     edx, edi
        shr     rdi, 4
        and     edx, 15
	lea	rcx, [-10+rdx]
	not	rcx
        shr     rcx, 61
        lea     edx, [48+rdx+rcx]
        dec     rax
        mov     [rsi+rax], dl
        jne     .LC2
        ret

___c_ptr64tohex:
	movabs	r11,0x0606060606060606
	lea	r10,[r11+r11*4]
	shr	r10,1

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
	.size	c_ptr64tohex, .-c_ptr64tohex

	.globl	e_ptr64tohex
	.type	e_ptr64tohex, @function
e_ptr64tohex:
	mov	eax,0x10			/* upper bits of rax are cleared */
	mov	[rdi+0x10],ah
.L3:
	mov	ecx,esi
	and	ecx,0x0F
	lea	edx,[0x30+rcx]
	cmp	dl,0x39
	jbe	.L2
	lea	edx,[0x37+rcx]
.L2:
	mov	[rdi+rax-1],dl
	shr	rsi,4
	dec	rax
	jne	.L3
	ret
	.size	e_ptr64tohex, .-e_ptr64tohex

	.globl	g_ptr64tohex
	.type	g_ptr64tohex, @function
g_ptr64tohex:
	mov	eax,0x10			/* upper bits of rax are cleared */
	mov	[rsi+0x10],ah
.L6:
	mov	ecx,edi
	and	ecx,0x0F
	lea	edx,[0x30+rcx]
	cmp	dl,0x39
	jbe	.L8
	lea	edx,[0x37+rcx]
.L8:
	mov	[rsi+rax-1],dl
	shr	rdi,4
	dec	rax
	jne	.L6
	ret
	.size	g_ptr64tohex, .-g_ptr64tohex

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
