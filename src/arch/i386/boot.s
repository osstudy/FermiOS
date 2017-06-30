; MIT License (MIT)
;
; Copyright (C) 2017 Dmytro Kalchenko <dmytro.v.kalchenko@gmail.com>
;
; Permission is hereby granted, free of charge, to any person obtaining a
; copy of this software and associated documentation files (the "Software"),
; to deal in the Software without restriction, including without limitation
; the rights to use, copy, modify, merge, publish, distribute, sublicense,
; and/or sell copies of the Software, and to permit persons to whom the
; Software is furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
; DEALINGS IN THE SOFTWARE.


; Declare constants for the multiboot header.
MBALIGN  equ  1<<0              ; align loaded modules on page boundaries
MEMINFO  equ  1<<1              ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Declare a multiboot header.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Reserver a stack for the initial thread.
section .bss
align 4
stack_bottom:
resb 16384 ; 16 KiB
stack_top:


section .data
gdtr dw 0 ; limit
	dd 0 ; base

section .text
global gdt_set:function
gdt_set:
	mov eax, [esp + 4]
	mov [gdtr + 2], eax
	mov ax, [esp + 8]
	mov [gdtr], ax
	lgdt [gdtr]
	ret

global gdt_activate:function
gdt_activate:
	jmp 0x08:.reload_CS
.reload_CS
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

global _start:function (_start.end - _start)
_start:
	mov esp, stack_top

	; check multiboot
	mov ecx, 0x2BADB002
	cmp ecx, eax
	jne .hang

	; calculate mem from multiboot info
	xor eax, eax
	add ebx, 0x4
	add eax, [ebx]
	add ebx, 0x4
	add eax, [ebx]
	push eax

	; disable interrupts till we have GDT and IDT
	cli

	; Call global ctor's.
	extern _init
	call _init

	; Transfer to main kernel.
	extern kernel_main
	call kernel_main

	cli
.hang:
	hlt
	jmp .hang
.end:
