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

#include <arch/i386/idt.h>


static isr_func isrs[] =
{
	isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,  isr8,  isr9,  isr10,
	isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21,
	isr22, isr23, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

void idt_init()
{
	// set up the IDT pointer
	idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
	idtp.base = (uint32_t)&idt;

	// clean the IDT
	memset(&idt, 0, sizeof(struct idt_entry) * IDT_SIZE);

	// load ISRs and IRQs into IDT
	size_t i = 0;
	for(; i < sizeof(isrs) / sizeof(isr_func); i++)
		idt_set_gate(i, (uint32_t)isrs[i], 0x08, // 0x08 = gdt[1] = CS PL0
				idt_flags_to_attr(true, 0x0 ,false, 0xE));

	// load the IDT for the CPU
	idt_set();
}

uint8_t idt_flags_to_attr(bool present, uint8_t privilege, bool storage_seg,
		uint8_t type)
{
	uint8_t attr = type & 0x0F;
	attr |= (storage_seg ? 1 : 0) << 4;
	attr |= (privilege & 0x03) << 5;
	attr |= (present ? 1 : 0) << 7;

	return attr;
}

void idt_set_gate(uint8_t id, uint32_t offset, uint16_t selector, uint8_t attr)
{
	idt[id].offset_low  = offset & 0xFFFF;
	idt[id].offset_high = (offset >> 16) & 0xFFFF;
	idt[id].reserved    = 0;
	idt[id].selector    = selector;
	idt[id].attr        = attr;
}




