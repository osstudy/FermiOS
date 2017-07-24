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

#ifndef ARCH_I386_CPU_TSS_H
#define ARCH_I386_CPU_TSS_H

typedef struct
{
	uint16_t link;			uint16_t link_r;
	uint32_t esp0;
	uint16_t ss0;			uint16_t ss0_r;
	uint32_t esp1;
	uint16_t ss1;			uint16_t ss1_r;
	uint32_t esp2;
	uint16_t ss2;			uint16_t ss2_r;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;			uint16_t es_r;
	uint16_t cs;			uint16_t cs_r;
	uint16_t ss;			uint16_t ss_r;
	uint16_t ds;			uint16_t ds_r;
	uint16_t fs;			uint16_t fs_r;
	uint16_t gs;			uint16_t gs_r;
	uint16_t ldtr;			uint16_t ldtr_r;
	uint16_t iopb_off_r;	uint16_t iopb_off;

} __attribute__((packed)) tss_t;


#endif // ARCH_I386_CPU_TSS_H
