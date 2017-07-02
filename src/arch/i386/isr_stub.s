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

section .text

global isr_ignore:function
isr_ignore:
	push byte  0      ; Dummy error code
	push word  0xFFFF ; Dummy ISR id/number
	jmp catch_interrupt


; Exceptions

global isr0:function
isr0:
	cli
	push byte 0
	push word 0
	jmp catch_interrupt

global isr1:function
isr1:
	cli
	push byte 0
	push word 1
	jmp catch_interrupt

global isr2:function
isr2:
	cli
	push byte 0
	push word 2
	jmp catch_interrupt

global isr3:function
isr3:
	cli
	push byte 0
	push word 3
	jmp catch_interrupt

global isr4:function
isr4:
	cli
	push byte 0
	push word 4
	jmp catch_interrupt

global isr5:function
isr5:
	cli
	push byte 0
	push word 5
	jmp catch_interrupt

global isr6:function
isr6:
	cli
	push byte 0
	push word 6
	jmp catch_interrupt

global isr7:function
isr7:
	cli
	push byte 0
	push word 7
	jmp catch_interrupt

global isr8:function
isr8:
	cli
	; err code is pushed by hardware
	push word 8
	jmp catch_interrupt

global isr9:function
isr9:
	cli
	push byte 0
	push word 9
	jmp catch_interrupt

global isr10:function
isr10:
	cli
	push word 10
	jmp catch_interrupt

global isr11:function
isr11:
	cli
	push word 11
	jmp catch_interrupt

global isr12:function
isr12:
	cli
	push word 12
	jmp catch_interrupt

global isr13:function
isr13:
	cli
	push word 13
	jmp catch_interrupt

global isr14:function
isr14:
	cli
	push word 14
	jmp catch_interrupt

global isr15:function
isr15:
	cli
	push byte 0
	push word 15
	jmp catch_interrupt

global isr16:function
isr16:
	cli
	push byte 0
	push word 16
	jmp catch_interrupt

global isr17:function
isr17:
	cli
	push byte 0
	push word 17
	jmp catch_interrupt

global isr18:function
isr18:
	cli
	push byte 0
	push word 18
	jmp catch_interrupt

global isr19:function
isr19:
	cli
	push byte 0
	push word 19
	jmp catch_interrupt

global isr20:function
isr20:
	cli
	push byte 0
	push word 20
	jmp catch_interrupt

global isr21:function
isr21:
	cli
	push byte 0
	push word 21
	jmp catch_interrupt

global isr22:function
isr22:
	cli
	push byte 0
	push word 22
	jmp catch_interrupt

global isr23:function
isr23:
	cli
	push byte 0
	push word 23
	jmp catch_interrupt

global isr24:function
isr24:
	cli
	push byte 0
	push word 24
	jmp catch_interrupt

global isr25:function
isr25:
	cli
	push byte 0
	push word 25
	jmp catch_interrupt

global isr26:function
isr26:
	cli
	push byte 0
	push word 26
	jmp catch_interrupt

global isr27:function
isr27:
	cli
	push byte 0
	push word 27
	jmp catch_interrupt

global isr28:function
isr28:
	cli
	push byte 0
	push word 28
	jmp catch_interrupt

global isr29:function
isr29:
	cli
	push byte 0
	push word 29
	jmp catch_interrupt

global isr30:function
isr30:
	cli
	push byte 0
	push word 30
	jmp catch_interrupt

global isr31:function
isr31:
	cli
	push byte 0
	push word 31
	jmp catch_interrupt

; ISR wrapper
catch_interrupt:
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10 ; PL0 DS descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax


	mov eax, esp
	push eax ; push stack to handler

	extern handle_interrupt
	cld
	call handle_interrupt

	pop eax

	pop gs
	pop fs
	pop es
	pop ds
	popa

	add esp, 8 ; clean up err code and ISR id from stack
	iret

