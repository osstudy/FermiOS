/*
 * MIT License (MIT)
 *
 * Copyright (C) 2017 Dmytro Kalchenko <dmytro.v.kalchenko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef ARCH_I386_CPU_ISR_H
#define ARCH_I386_CPU_ISR_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <arch/i386/pic.h>
#include <sys_common.h>


#define IRQ_OFFSET       32
#define ISR_HANDERS_SIZE 255


typedef void(*isr_func_t)(void);
typedef void(*isr_handler_t)(void);

typedef struct
{
	size_t isr;
	isr_handler_t handler;

} isr_handler_entry_t;

typedef struct
{
	uint32_t gs, fs, es, ds;                       // pushed manually
	uint32_t edi, esi, ebp, esp,                   // pusha
			ebx, edx, ecx, eax;
	uint32_t isr_id, err_code;                     // pushed by isr stub
	uint32_t eip, cs, eflags, user_esp, user_ss;   // pushed by hardware
	// the above line is possibly
} __attribute__((packed)) interrupt_cpu_state_t;


extern isr_handler_entry_t isr_handlers[ISR_HANDERS_SIZE];

void isr_add_handler(size_t isr, isr_handler_t handler);
void handle_interrupt(interrupt_cpu_state_t*); // FIXME: add prefixes
void print_cpu_state(interrupt_cpu_state_t*);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


#endif //ARCH_i386_CPU_ISR_H
