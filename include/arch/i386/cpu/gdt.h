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

#ifndef ARCH_I386_CPU_GDT_H
#define ARCH_I386_CPU_GDT_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <arch/i386/cpu/tss.h>


#define GDT_SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define GDT_SEG_PRES(x)      ((x) << 0x07) // Present
#define GDT_SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define GDT_SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define GDT_SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define GDT_SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define GDT_SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)

#define GDT_SEG_DATA_RD        0x00 // Read-Only
#define GDT_SEG_DATA_RDA       0x01 // Read-Only, accessed
#define GDT_SEG_DATA_RDWR      0x02 // Read/Write
#define GDT_SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define GDT_SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define GDT_SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define GDT_SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define GDT_SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define GDT_SEG_CODE_EX        0x08 // Execute-Only
#define GDT_SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define GDT_SEG_CODE_EXRD      0x0A // Execute/Read
#define GDT_SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define GDT_SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define GDT_SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define GDT_SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define GDT_SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

#define GDT_CODE_PL0 GDT_SEG_DESCTYPE(1) | GDT_SEG_PRES(1) | GDT_SEG_SAVL(0) | \
                     GDT_SEG_LONG(0)     | GDT_SEG_SIZE(1) | GDT_SEG_GRAN(1) | \
                     GDT_SEG_PRIV(0)     | GDT_SEG_CODE_EXRD

#define GDT_DATA_PL0 GDT_SEG_DESCTYPE(1) | GDT_SEG_PRES(1) | GDT_SEG_SAVL(0) | \
                     GDT_SEG_LONG(0)     | GDT_SEG_SIZE(1) | GDT_SEG_GRAN(1) | \
                     GDT_SEG_PRIV(0)     | GDT_SEG_DATA_RDWR

#define GDT_CODE_PL3 GDT_SEG_DESCTYPE(1) | GDT_SEG_PRES(1) | GDT_SEG_SAVL(0) | \
                     GDT_SEG_LONG(0)     | GDT_SEG_SIZE(1) | GDT_SEG_GRAN(1) | \
                     GDT_SEG_PRIV(3)     | GDT_SEG_CODE_EXRD

#define GDT_DATA_PL3 GDT_SEG_DESCTYPE(1) | GDT_SEG_PRES(1) | GDT_SEG_SAVL(0) | \
                     GDT_SEG_LONG(0)     | GDT_SEG_SIZE(1) | GDT_SEG_GRAN(1) | \
                     GDT_SEG_PRIV(3)     | GDT_SEG_DATA_RDWR




uint64_t gdt_create_descriptor(uint32_t, uint32_t, uint16_t);

extern void gdt_set(void* gdt, size_t gdt_size);
extern void gdt_activate();


#endif // ARCH_I386_CPU_GDT_H
