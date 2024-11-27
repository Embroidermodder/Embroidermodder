;
; Libembroidery 1.0.0-alpha
; https://www.libembroidery.org
;
; A library for reading, writing, altering and otherwise
; processing machine embroidery files and designs.
;
; Also, the core library supporting the Embroidermodder Project's
; family of machine embroidery interfaces.
;
; -----------------------------------------------------------------------------
;
; Copyright 2018-2024 The Embroidermodder Team
; Licensed under the terms of the zlib license.
;
; ------------------------------------------------------------------------------
;
; The elf64 testing platform is fully automated, create a log like this:
;	nasm -felf64 emb_elf64_test.asm -o emb_elf64_test
;	./emb_elf64_test > output.log
;
; ------------------------------------------------------------------------------
;
;


	global	_start

; Write message at argument 1 of length in argument 2.
%macro	write	2
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, %1
	mov	rdx, %2
	syscall
%endmacro

; Copy buffers in a similar way to memcpy but only for local memory.
;	argument 1: void* destination
;	argument 2: void* source
;	argument 3: number of bytes to copy
;	returns nothing.
;
%macro	copy_buffer	3
	mov	ecx, %3
	cdq
%%loop_top:
	mov	al, [%2]
	mov	[%1], al
	loop	%%loop_top
%endmacro

; Only works for 0-9
%macro	write_number	1
	copy_buffer	number, blank, 6
	mov     rax, $1
	mov     rbx, 10
	cdq
	idiv	ebx
	add	dl, '0'
	mov	[number+6], dl
	write	number, 7
%endmacro

; Exit with code in argument 1.
%macro	exit	1
	mov	rax, 60
	mov	rdi, $1
	syscall
%endmacro

; Uses the macros above.
%include "embroidery.inc"

	section .text

_start:
	startup
	write_number	30
	exit	0

	section .data

number:
	db	"       "
blank:
	db	"_______"

