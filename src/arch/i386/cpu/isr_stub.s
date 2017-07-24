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

; Exceptions:
global isr0:function
isr0:
	cli
	push byte 0
	push byte 0
	jmp catch_interrupt

global isr1:function
isr1:
	cli
	push byte 0
	push byte 1
	jmp catch_interrupt

global isr2:function
isr2:
	cli
	push byte 0
	push byte 2
	jmp catch_interrupt

global isr3:function
isr3:
	cli
	push byte 0
	push byte 3
	jmp catch_interrupt

global isr4:function
isr4:
	cli
	push byte 0
	push byte 4
	jmp catch_interrupt

global isr5:function
isr5:
	cli
	push byte 0
	push byte 5
	jmp catch_interrupt

global isr6:function
isr6:
	cli
	push byte 0
	push byte 6
	jmp catch_interrupt

global isr7:function
isr7:
	cli
	push byte 0
	push byte 7
	jmp catch_interrupt

global isr8:function
isr8:
	cli
	; err code is pushed by hardware
	push byte 8
	jmp catch_interrupt

global isr9:function
isr9:
	cli
	push byte 0
	push byte 9
	jmp catch_interrupt

global isr10:function
isr10:
	cli
	push byte 10
	jmp catch_interrupt

global isr11:function
isr11:
	cli
	push byte 11
	jmp catch_interrupt

global isr12:function
isr12:
	cli
	push byte 12
	jmp catch_interrupt

global isr13:function
isr13:
	cli
	push byte 13
	jmp catch_interrupt

global isr14:function
isr14:
	cli
	push byte 14
	jmp catch_interrupt

global isr15:function
isr15:
	cli
	push byte 0
	push byte 15
	jmp catch_interrupt

global isr16:function
isr16:
	cli
	push byte 0
	push byte 16
	jmp catch_interrupt

global isr17:function
isr17:
	cli
	push byte 0
	push byte 17
	jmp catch_interrupt

global isr18:function
isr18:
	cli
	push byte 0
	push byte 18
	jmp catch_interrupt

global isr19:function
isr19:
	cli
	push byte 0
	push byte 19
	jmp catch_interrupt

global isr20:function
isr20:
	cli
	push byte 0
	push byte 20
	jmp catch_interrupt

global isr21:function
isr21:
	cli
	push byte 0
	push byte 21
	jmp catch_interrupt

global isr22:function
isr22:
	cli
	push byte 0
	push byte 22
	jmp catch_interrupt

global isr23:function
isr23:
	cli
	push byte 0
	push byte 23
	jmp catch_interrupt

global isr24:function
isr24:
	cli
	push byte 0
	push byte 24
	jmp catch_interrupt

global isr25:function
isr25:
	cli
	push byte 0
	push byte 25
	jmp catch_interrupt

global isr26:function
isr26:
	cli
	push byte 0
	push byte 26
	jmp catch_interrupt

global isr27:function
isr27:
	cli
	push byte 0
	push byte 27
	jmp catch_interrupt

global isr28:function
isr28:
	cli
	push byte 0
	push byte 28
	jmp catch_interrupt

global isr29:function
isr29:
	cli
	push byte 0
	push byte 29
	jmp catch_interrupt

global isr30:function
isr30:
	cli
	push byte 0
	push byte 30
	jmp catch_interrupt

global isr31:function
isr31:
	cli
	push byte 0
	push byte 31
	jmp catch_interrupt

; (Maskable) Hardware Interrupt Requests:
%define IRQ_OFFSET 32 ; we map IRQs to ISRs after CPU exceptions

global irq0:function
irq0:
	push byte 0
	push byte 0 + IRQ_OFFSET
	jmp catch_interrupt

global irq1:function
irq1:
	push byte 0
	push byte 1 + IRQ_OFFSET
	jmp catch_interrupt

global irq2:function
irq2:
	push byte 0
	push byte 2 + IRQ_OFFSET
	jmp catch_interrupt

global irq3:function
irq3:
	push byte 0
	push byte 3 + IRQ_OFFSET
	jmp catch_interrupt

global irq4:function
irq4:
	push byte 0
	push byte 4 + IRQ_OFFSET
	jmp catch_interrupt

global irq5:function
irq5:
	push byte 0
	push byte 5 + IRQ_OFFSET
	jmp catch_interrupt

global irq6:function
irq6:
	push byte 0
	push byte 6 + IRQ_OFFSET
	jmp catch_interrupt

global irq7:function
irq7:
	push byte 0
	push byte 7 + IRQ_OFFSET
	jmp catch_interrupt

global irq8:function
irq8:
	push byte 0
	push byte 8 + IRQ_OFFSET
	jmp catch_interrupt

global irq9:function
irq9:
	push byte 0
	push byte 9 + IRQ_OFFSET
	jmp catch_interrupt

global irq10:function
irq10:
	push byte 0
	push byte 10 + IRQ_OFFSET
	jmp catch_interrupt

global irq11:function
irq11:
	push byte 0
	push byte 11 + IRQ_OFFSET
	jmp catch_interrupt

global irq12:function
irq12:
	push byte 0
	push byte 12 + IRQ_OFFSET
	jmp catch_interrupt

global irq13:function
irq13:
	push byte 0
	push byte 13 + IRQ_OFFSET
	jmp catch_interrupt

global irq14:function
irq14:
	push byte 0
	push byte 14 + IRQ_OFFSET
	jmp catch_interrupt

global irq15:function
irq15:
	push byte 0
	push byte 15 + IRQ_OFFSET
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

