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

#include <kernel/hal/cpu.h>
#include <arch/i386/cpu/gdt.h>
#include <arch/i386/cpu/tss.h>
#include <arch/i386/cpu/idt.h>
#include <arch/i386/cpu/isr.h>
#include <arch/i386/cpu/ports.h>
#include <arch/i386/pic.h>


static uint64_t gdt[6];
static tss_t my_tss;

void cpu_initialize()
{
	// Set up GDT
	gdt[0] = gdt_create_descriptor(0 ,0 ,0);							// NULL
	gdt[1] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_CODE_PL0));		// CS K
	gdt[2] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_DATA_PL0));		// DS K
	gdt[3] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_CODE_PL3));		// CS U
	gdt[4] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_DATA_PL3));		// DS U
	gdt[5] = gdt_create_descriptor((uint32_t)&my_tss,
			sizeof(my_tss), 0x89);										//  TSS

	gdt_set(gdt, sizeof(gdt));
	gdt_activate();

	// Init IDT
	idt_init();

	// Init PIC
	pic_remap_irqs(IRQ_OFFSET, IRQ_OFFSET + 8);
	pic_load_irqs();

	outb(0x21,0xfc); // Enable keyboard and PIT
	outb(0xa1,0xff);

	idt_set();

	asm volatile("sti");
}

cpu_info_t cpu_get_info()
{
	cpu_info_t info;
	info.info = "unknown\n";

	return info;
}

void cpu_halt()
{
	asm volatile("hlt");
}

void cpu_hcf()
{
	while(true)
	{
		asm volatile("nop");
	}
}


