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

#ifndef ARCH_I386_CPU_IDT_H
#define ARCH_I386_CPU_IDT_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <arch/i386/cpu/isr.h>

#define IDT_SIZE 256


typedef struct
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t reserved;
	uint8_t attr;
	uint16_t offset_high;

} __attribute__((packed)) idt_entry_t;

typedef struct
{
	uint16_t limit;
	uint32_t base;

} __attribute__((packed)) idt_ptr_t;


idt_entry_t idt[IDT_SIZE];
idt_ptr_t   idtp;

void idt_init();
uint8_t idt_flags_to_attr(bool present, uint8_t privilege, bool storage_seg,
		uint8_t type);
void idt_set_gate(uint8_t id, uint32_t offset, uint16_t selector,
		uint8_t attr);

extern void idt_set();


#endif // ARCH_I386_CPU_IDT_H
