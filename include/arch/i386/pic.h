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

#ifndef ARCH_I386_PIC_H
#define ARCH_I386_PIC_H

#include <stddef.h>
#include <stdint.h>
#include <arch/i386/ports.h>
#include <arch/i386/idt.h>
#include <arch/i386/isr.h>


#define PIC_MASTER_PORT_CMD 0x0020
#define PIC_MASTER_PORT_DAT 0x0021
#define PIC_SLAVE_PORT_CMD  0x00A0
#define PIC_SLAVE_PORT_DAT  0x00A1

#define PIC_EOI             0x20

#define PIC_ICW1_ICW4       0x01		/* ICW4 (not) needed */
#define PIC_ICW1_SINGLE     0x02		/* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4  0x04		/* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL      0x08		/* Level triggered (edge) mode */
#define PIC_ICW1_INIT       0x10		/* Initialization - required! */

#define PIC_ICW4_8086       0x01		/* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO       0x02		/* Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE  0x08		/* Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER 0x0C		/* Buffered mode/master */
#define PIC_ICW4_SFNM       0x10		/* Special fully nested (not) */


void pic_remap_irqs(uint32_t, uint32_t);
void pic_load_irqs();
void pic_eoi(uint8_t); // EOI = End Of Interrupt
void pic_disable();  // if using APIC and IOAPIC

// TODO: get PICs' internal regs and masks

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif // ARCH_I386_PIC_H





