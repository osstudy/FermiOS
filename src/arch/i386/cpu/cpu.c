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

#include <cpuid.h>

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

// TODO: Read up on Intel, AMD, and GCC manuals
// FIXME: Make sure (!!!) all registers are correct
// TODO: Add flags support
static inline void cpuid(int code, uint32_t *a, uint32_t *d)
{
	asm volatile("cpuid":"=a"(*a),"=d"(*d):"a"(code):"ecx","ebx");
}

static inline int cpuid_string(int code, uint32_t where[4])
{
	asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
					"=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
	return (int)where[0];
}

static char buf[64];

cpu_info_t cpu_get_info()
{
	cpu_info_t info;
	uint32_t regs[4];

	cpuid_string(0x80000002, regs);

	// FIXME: automate

	buf[ 0] = (uint8_t)((regs[0] & 0x000000FF)      );
	buf[ 1] = (uint8_t)((regs[0] & 0x0000FF00) >> 8 );
	buf[ 2] = (uint8_t)((regs[0] & 0x00FF0000) >> 16);
	buf[ 3] = (uint8_t)((regs[0] & 0xFF000000) >> 24);
	buf[ 4] = (uint8_t)((regs[1] & 0x000000FF)      );
	buf[ 5] = (uint8_t)((regs[1] & 0x0000FF00) >> 8 );
	buf[ 6] = (uint8_t)((regs[1] & 0x00FF0000) >> 16);
	buf[ 7] = (uint8_t)((regs[1] & 0xFF000000) >> 24);
	buf[ 8] = (uint8_t)((regs[2] & 0x000000FF)      );
	buf[ 9] = (uint8_t)((regs[2] & 0x0000FF00) >> 8 );
	buf[10] = (uint8_t)((regs[2] & 0x00FF0000) >> 16);
	buf[11] = (uint8_t)((regs[2] & 0xFF000000) >> 24);
	buf[12] = (uint8_t)((regs[3] & 0x000000FF)      );
	buf[13] = (uint8_t)((regs[3] & 0x0000FF00) >> 8 );
	buf[14] = (uint8_t)((regs[3] & 0x00FF0000) >> 16);
	buf[15] = (uint8_t)((regs[3] & 0xFF000000) >> 24);

	cpuid_string(0x80000003, regs);

	buf[16] = (uint8_t)((regs[0] & 0x000000FF)      );
	buf[17] = (uint8_t)((regs[0] & 0x0000FF00) >> 8 );
	buf[18] = (uint8_t)((regs[0] & 0x00FF0000) >> 16);
	buf[19] = (uint8_t)((regs[0] & 0xFF000000) >> 24);
	buf[20] = (uint8_t)((regs[1] & 0x000000FF)      );
	buf[21] = (uint8_t)((regs[1] & 0x0000FF00) >> 8 );
	buf[22] = (uint8_t)((regs[1] & 0x00FF0000) >> 16);
	buf[23] = (uint8_t)((regs[1] & 0xFF000000) >> 24);
	buf[24] = (uint8_t)((regs[2] & 0x000000FF)      );
	buf[25] = (uint8_t)((regs[2] & 0x0000FF00) >> 8 );
	buf[26] = (uint8_t)((regs[2] & 0x00FF0000) >> 16);
	buf[27] = (uint8_t)((regs[2] & 0xFF000000) >> 24);
	buf[28] = (uint8_t)((regs[3] & 0x000000FF)      );
	buf[29] = (uint8_t)((regs[3] & 0x0000FF00) >> 8 );
	buf[30] = (uint8_t)((regs[3] & 0x00FF0000) >> 16);
	buf[31] = (uint8_t)((regs[3] & 0xFF000000) >> 24);

	cpuid_string(0x80000004, regs);

	buf[32] = (uint8_t)((regs[0] & 0x000000FF)      );
	buf[33] = (uint8_t)((regs[0] & 0x0000FF00) >> 8 );
	buf[34] = (uint8_t)((regs[0] & 0x00FF0000) >> 16);
	buf[35] = (uint8_t)((regs[0] & 0xFF000000) >> 24);
	buf[36] = (uint8_t)((regs[1] & 0x000000FF)      );
	buf[37] = (uint8_t)((regs[1] & 0x0000FF00) >> 8 );
	buf[38] = (uint8_t)((regs[1] & 0x00FF0000) >> 16);
	buf[39] = (uint8_t)((regs[1] & 0xFF000000) >> 24);
	buf[40] = (uint8_t)((regs[2] & 0x000000FF)      );
	buf[41] = (uint8_t)((regs[2] & 0x0000FF00) >> 8 );
	buf[42] = (uint8_t)((regs[2] & 0x00FF0000) >> 16);
	buf[43] = (uint8_t)((regs[2] & 0xFF000000) >> 24);
	buf[44] = (uint8_t)((regs[3] & 0x000000FF)      );
	buf[45] = (uint8_t)((regs[3] & 0x0000FF00) >> 8 );
	buf[46] = (uint8_t)((regs[3] & 0x00FF0000) >> 16);
	buf[47] = (uint8_t)((regs[3] & 0xFF000000) >> 24);


	buf[48] = '\0';

	// FIXME: malloc & strdup
	info.info = buf;

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


