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

#include <kernel/debug.h>


void dbg_dump_cpu_state()
{
#ifndef _DEBUG
	return;
#endif

	register int eax asm("eax");
	register int ebx asm("ebx");
	register int ecx asm("ecx");
	register int edx asm("edx");
	register int esi asm("esi");
	register int edi asm("edi");

	printf("Register dump: \r\n");

	printf("EAX: 0x%p ", eax);
	asm volatile("mov %cs, %eax");
	printf("CS: 0x%p\r\n", eax);

	asm volatile("mov %ds, %eax");
	printf("EBX: 0x%p DS: 0x%p\r\n", ebx, eax);

	asm volatile("mov %es, %eax");
	printf("ECX: 0x%p ES: 0x%p\r\n", ecx, eax);

	asm volatile("mov %ss, %eax");
	printf("EDX: 0x%p SS: 0x%p\r\n", edx, eax);

	asm volatile("mov %gs, %eax");
	printf("ESI: 0x%p GS: 0x%p\r\n", esi, eax);

	asm volatile("mov %fs, %eax");
	printf("EDI: 0x%p FS: 0x%p\r\n", edi, eax);


	asm volatile("mov %cr0, %eax");
	asm volatile("mov %ebp, %ebx");
	asm volatile("mov %esp, %ecx");
	printf("CR0: 0x%p EBP: 0x%p ESP: 0x%p\r\n", eax, ebx, ecx);
}
