[bits 64]

; Significant parts of the code were taken from Intel AESNI Sample Library:
; https://software.intel.com/en-us/articles/download-the-intel-aesni-sample-library
; Parameter registers were changed to suit x86-64/Linux:
; RDI, RSI, RDX, RCX, R8, R9, XMM0–7:

%macro copy_round_keys 3
	movdqu xmm4,[%2 + ((%3)*16)]
	movdqa [%1 + ((%3)*16)],xmm4
%endmacro

%macro aesenc1 1
	aesenc	xmm0,%1
%endmacro

%macro aesenclast1 1
	aesenclast	xmm0,%1
%endmacro

%macro inversekey 1
	movdqu  xmm1,%1
	aesimc	xmm0,xmm1
	movdqu	%1,xmm0
%endmacro

%macro aesdec1_u 1
	movdqu xmm4,%1
	aesdec xmm0,xmm4
%endmacro

%macro load_and_xor4 2
	movdqa	xmm4,%2
	movdqu	xmm0,[%1 + 0*16]
	pxor	xmm0,xmm4
	movdqu	xmm1,[%1 + 1*16]
	pxor	xmm1,xmm4
	movdqu	xmm2,[%1 + 2*16]
	pxor	xmm2,xmm4
	movdqu	xmm3,[%1 + 3*16]
	pxor	xmm3,xmm4
%endmacro

%macro aesdec4 1
	movdqa	xmm4,%1

	aesdec	xmm0,xmm4
	aesdec	xmm1,xmm4
	aesdec	xmm2,xmm4
	aesdec	xmm3,xmm4
%endmacro

%macro aesdeclast4 1
	movdqa	xmm4,%1

	aesdeclast	xmm0,xmm4
	aesdeclast	xmm1,xmm4
	aesdeclast	xmm2,xmm4
	aesdeclast	xmm3,xmm4
%endmacro

%macro aesdeclast1_u 1
	movdqu xmm4,%1
	aesdeclast	xmm0,xmm4
%endmacro

%macro store4 1
	movdqu [%1 + 0*16],xmm0
	movdqu [%1 + 1*16],xmm1
	movdqu [%1 + 2*16],xmm2
	movdqu [%1 + 3*16],xmm3
%endmacro

section .data
align 16
shuffle_mask:
    DD 0FFFFFFFFh
    DD 03020100h
    DD 07060504h
    DD 0B0A0908h

align 16
key_expansion128: 
    pshufd xmm2, xmm2, 0xFF;
    movdqa xmm3, xmm1
    pshufb xmm3, xmm5
    pxor xmm1, xmm3
    pshufb xmm3, xmm5
    pxor xmm1, xmm3
    pshufb xmm3, xmm5
    pxor xmm1, xmm3
    pxor xmm1, xmm2

    ; storing the result in the key schedule array
    movdqu [rsi], xmm1
    add rsi, 0x10
    ret


align 16
global iEncExpandKey128
iEncExpandKey128:
    movdqu xmm1, [rdi]    ; loading the key
    movdqu [rsi], xmm1
    movdqa xmm5, [shuffle_mask]
    add rsi, 16

    aeskeygenassist xmm2, xmm1, 0x1     ; Generating round key 1
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x2     ; Generating round key 2
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x4     ; Generating round key 3
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x8     ; Generating round key 4
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x10    ; Generating round key 5
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x20    ; Generating round key 6
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x40    ; Generating round key 7
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x80    ; Generating round key 8
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x1b    ; Generating round key 9
    call key_expansion128
    aeskeygenassist xmm2, xmm1, 0x36    ; Generating round key 10
    call key_expansion128

    ret


align 16
global iDecExpandKey128
iDecExpandKey128:

	push rdi
	push rsi
	sub rsp,16+8

	call iEncExpandKey128

	add rsp,16+8
	pop rsi
	pop rdi

	inversekey [rsi + 1*16]
	inversekey [rsi + 2*16]
	inversekey [rsi + 3*16]
	inversekey [rsi + 4*16]
	inversekey [rsi + 5*16]
	inversekey [rsi + 6*16]
	inversekey [rsi + 7*16]
	inversekey [rsi + 8*16]
	inversekey [rsi + 9*16]

	ret


align 16
global iEnc128_CBC
iEnc128_CBC:
	sub rsp,16*16+8
	
	mov r9,rdi
	mov rax,[rdi+24]
	movdqu xmm1,[rax]
	
	mov eax,[rdi+32] ; numblocks
	mov rsi,[rdi]
	mov r8,[rdi+8]
	mov rdi,[rdi+16]
	
	sub r8,rsi


	test	rdi,0xf
	jz		lp128encsingle_CBC
	
	copy_round_keys rsp,rdi,0
	copy_round_keys rsp,rdi,1
	copy_round_keys rsp,rdi,2
	copy_round_keys rsp,rdi,3
	copy_round_keys rsp,rdi,4
	copy_round_keys rsp,rdi,5
	copy_round_keys rsp,rdi,6
	copy_round_keys rsp,rdi,7
	copy_round_keys rsp,rdi,8
	copy_round_keys rsp,rdi,9
	copy_round_keys rsp,rdi,10
	mov rdi,rsp	

align 16
lp128encsingle_CBC:

	movdqu xmm0, [rsi]
	movdqu xmm4,[rdi+0*16]
	add rsi, 16
	pxor xmm0, xmm1
	pxor xmm0, xmm4
	aesenc1 [rdi+1*16]
	aesenc1 [rdi+2*16]
	aesenc1 [rdi+3*16]
	aesenc1 [rdi+4*16]     
	aesenc1 [rdi+5*16]
	aesenc1 [rdi+6*16]
	aesenc1 [rdi+7*16]
	aesenc1 [rdi+8*16]
	aesenc1 [rdi+9*16]
	aesenclast1 [rdi+10*16]
	movdqa xmm1,xmm0

	; Store output encrypted data into CIPHERTEXT array
	movdqu  [r8+rsi-16], xmm0
	dec eax
	jnz lp128encsingle_CBC

	mov	   r9,[r9+24]
	movdqu [r9],xmm1
	add rsp,16*16+8
	ret



align 16
global iDec128_CBC
iDec128_CBC:
	
	sub rsp,16*16+8

	mov r9,rdi
	mov rax,[rdi+24]
	movdqu	xmm5,[rax]
	
	mov eax,[rdi+32] ; numblocks
	mov rsi,[rdi]
	mov r8,[rdi+8]
	mov rdi,[rdi+16]
	
	
	sub r8,rsi


	test eax,eax
	jz end_dec128_CBC

	cmp eax,4
	jl	lp128decsingle_CBC

	test	rdi,0xf
	jz		lp128decfour_CBC
	
	copy_round_keys rsp,rdi,0
	copy_round_keys rsp,rdi,1
	copy_round_keys rsp,rdi,2
	copy_round_keys rsp,rdi,3
	copy_round_keys rsp,rdi,4
	copy_round_keys rsp,rdi,5
	copy_round_keys rsp,rdi,6
	copy_round_keys rsp,rdi,7
	copy_round_keys rsp,rdi,8
	copy_round_keys rsp,rdi,9
	copy_round_keys rsp,rdi,10
	mov rdi,rsp	


align 16
lp128decfour_CBC:
	
	test eax,eax
	jz end_dec128_CBC

	cmp eax,4
	jl	lp128decsingle_CBC

	load_and_xor4 rsi, [rdi+10*16]
	add rsi,16*4
	aesdec4 [rdi+9*16]
	aesdec4 [rdi+8*16]
	aesdec4 [rdi+7*16]
	aesdec4 [rdi+6*16]
	aesdec4 [rdi+5*16]
	aesdec4 [rdi+4*16]
	aesdec4 [rdi+3*16]
	aesdec4 [rdi+2*16]
	aesdec4 [rdi+1*16]
	aesdeclast4 [rdi+0*16]

	pxor	xmm0,xmm5
	movdqu	xmm4,[rsi - 16*4 + 0*16]
	pxor	xmm1,xmm4
	movdqu	xmm4,[rsi - 16*4 + 1*16]
	pxor	xmm2,xmm4
	movdqu	xmm4,[rsi - 16*4 + 2*16]
	pxor	xmm3,xmm4
	movdqu	xmm5,[rsi - 16*4 + 3*16]
	
	sub eax,4
	store4 r8+rsi-(16*4)
	jmp lp128decfour_CBC


align 16
lp128decsingle_CBC:

	movdqu xmm0, [rsi]
	movdqa	xmm1,xmm0
	movdqu xmm4,[rdi+10*16]
	pxor xmm0, xmm4
	aesdec1_u [rdi+9*16]
	aesdec1_u [rdi+8*16]
	aesdec1_u [rdi+7*16]
	aesdec1_u [rdi+6*16]
	aesdec1_u [rdi+5*16]
	aesdec1_u [rdi+4*16]
	aesdec1_u [rdi+3*16]
	aesdec1_u [rdi+2*16]
	aesdec1_u [rdi+1*16]
	aesdeclast1_u [rdi+0*16]

	pxor	xmm0,xmm5
	movdqa	xmm5,xmm1
	add rsi, 16
	movdqu  [r8 + rsi - 16], xmm0
	dec eax
	jnz lp128decsingle_CBC

end_dec128_CBC:

	mov	   r9,[r9+24]
	movdqu [r9],xmm5
	add rsp,16*16+8
	ret
