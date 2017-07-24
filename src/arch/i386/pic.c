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

#include <arch/i386/pic.h>


static isr_func_t irqs[] =
{
	irq0,  irq1, irq2,  irq3,  irq4,  irq5,  irq6,  irq7,
	irq8,  irq9, irq10, irq11, irq12, irq13, irq14, irq15
};


void pic_load_irqs()
{
	// load IRQs into IDT
	size_t i = 0;
	for(; i < sizeof(irqs) / sizeof(isr_func_t); i++)
		idt_set_gate(i + IRQ_OFFSET, (uint32_t)irqs[i], 0x08, // 0x08 = gdt[1] = CS PL0
				idt_flags_to_attr(true, 0x0 ,false, 0xE));
}

void pic_remap_irqs(uint32_t master_offset, uint32_t slave_offset)
{
	uint8_t mask_master, mask_slave;

	// save masks
	mask_master = inb(PIC_MASTER_PORT_DAT);
	mask_slave  = inb(PIC_SLAVE_PORT_DAT);

	// start init sequance (in cascade mode)
	outb(PIC_MASTER_PORT_CMD, PIC_ICW1_INIT + PIC_ICW1_ICW4);
	io_wait();
	outb(PIC_SLAVE_PORT_CMD,  PIC_ICW1_INIT + PIC_ICW1_ICW4);
	io_wait();

	// PICs vector (irq) offset
	outb(PIC_MASTER_PORT_DAT, master_offset);
	io_wait();
	outb(PIC_SLAVE_PORT_DAT, slave_offset);
	io_wait();

	// Tell master PIC there's a slave at IRQ2
	outb(PIC_MASTER_PORT_DAT, 4);
	io_wait();

	// Tell slave PIC it's cascade identity
	outb(PIC_SLAVE_PORT_DAT, 2);
	io_wait();

	// Set PICs into 8086 mode
	outb(PIC_MASTER_PORT_DAT, PIC_ICW4_8086);
	io_wait();
	outb(PIC_SLAVE_PORT_DAT, PIC_ICW4_8086);
	io_wait();

	// restore masks
	outb(PIC_MASTER_PORT_DAT, mask_master);
	outb(PIC_SLAVE_PORT_DAT, mask_slave);
}

void pic_eoi(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC_SLAVE_PORT_CMD, PIC_EOI);

	outb(PIC_MASTER_PORT_CMD, PIC_EOI);
}

void pic_disable()
{
	outb(PIC_SLAVE_PORT_DAT, 0xFF);
	outb(PIC_MASTER_PORT_DAT, 0xFF);
}


