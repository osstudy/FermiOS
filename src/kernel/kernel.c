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



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include <kernel/tty.h>
#include <arch/i386/gdt.h>
#include <arch/i386/ports.h>

void dump_font();
void printf_tests();
void vga_color_test();
void dump_registers();
void print_mem(void* from, size_t size);
void cycle_delay(size_t cycles);
void run_tests();
extern void gdt_set(void* gdt, size_t gdt_size);
extern void gdt_activate();

// TODO: refactor GDT out of kernel main

struct tss
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

} __attribute__((packed));



uint64_t gdt[6];
struct tss my_tss;


void kernel_main(size_t mem_size)
{
	terminal_initialize();
	printf("\x1b[10m[ OK ]\x1b[15;0m VGA TTY initialized\r\n");

	gdt[0] = gdt_create_descriptor(0 ,0 ,0);						// NULL
	gdt[1] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_CODE_PL0));	// CS K
	gdt[2] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_DATA_PL0));	// DS K
	gdt[3] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_CODE_PL3));	// CS U
	gdt[4] = gdt_create_descriptor(0, 0xFFFFFFFF, (GDT_DATA_PL3));	// DS U
	gdt[5] = gdt_create_descriptor((uint32_t)&my_tss, sizeof(my_tss), 0x89);	//  TSS

	gdt_set(gdt, sizeof(gdt));
	gdt_activate();
	printf("\x1b[10m[ OK ]\x1b[15;0m GDT Activated\r\n");

	printf("\x1b[10m[ OK ]\x1b[13m FermiOS \x1b[15;0m");
	printf(_KERNEL_VERSION);
	printf(" kernel loaded\r\n");
	printf("\x1b[9;0m[INFO]\x1b[15;0m Available memory: %u MB\r\n",
			mem_size / 1024);
	printf("\r\n");

	cycle_delay(0xFFFFFFFF);

	io_wait();

	if(true)
		run_tests();

	printf("\r\n");
	printf("Nothing to do, aborting...\r\n");

	abort();
}

void run_tests()
{
	printf("\r\n");
	printf("Doing some tests: \r\n");
	cycle_delay(0x0F000000);
	dump_font();
	cycle_delay(0xFFF00000);
	printf("\r\n");
	printf_tests();
	cycle_delay(0xFFF00000);
	printf("\r\n");
	vga_color_test();
	cycle_delay(0xFFF00000);
	printf("\r\n");
	dump_registers();
	cycle_delay(0xFFF00000);
	printf("\r\n");
	print_mem((void*)0x100000, 16 * 20);
}

void vga_color_test()
{
	printf("Text Mode VGA colors test: \r\n");
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
			printf("\x1b[%d;%dm%c%c%c", i, j, 176, 177, 178);
		printf("\r\n");
	}

	printf("\x1b[15;0m\r\n");
}

void print_mem(void* from, size_t size)
{
	size_t cols = 16;
	size_t rest = size;
	printf("Printing mem from 0x%p to 0x%p: \r\n", from, from + size);

	for(size_t i = 0; i < size; i += cols)
	{
		printf("\x1b[11;0m0x%p\x1b[15;0m: ", from);

		for(size_t j = 0; j < cols; j++)
		{
			if(j % (cols / 2) == 0 && j != 0)
				printf(" ");

			if(j <= rest)
			{
				uint8_t d = *(uint8_t*)(from + j);
				if(d)
					printf("%X ", d);
				else
					printf("\x1b[7;0m%X\x1b[15;0m ", d);
			}
			else
				printf("   ");
		}

		printf("|");

		for(size_t j = 0; j < (cols >= rest ? rest : cols); j++)
		{
			char c = *(char*)(from + j);
			if(' ' <= c && c <= '~')
				printf("%c", c);
			else
				printf("\x1b[0m.\x1b[15;0m");
		}

		printf("|\r\n");
		from += cols;
		rest -= cols;
	}
}

void printf_tests()
{
	int val = 1337;
	int* ptr = &val;

	char c = 2;
	char* c_ptr = &c;

	printf("printf() tests:\r\n");
	printf("Test  c: %c\r\n",  'A');
	printf("Test  c: %c\r\n",  '#');
	printf("Test  c: %c\r\n",  2);
	printf("Test  c: %c\r\n",  '`');
	printf("Test  c: %c\r\n",  'j');
	printf("Test  d: %d\r\n",  val);
	printf("Test -d: %d\r\n", -val);
	printf("Test  i: %i\r\n",  val);
	printf("Test -i: %i\r\n", -val);
	printf("Test  u: %u\r\n",  val);
	printf("Test -u: %u\r\n", -val);
	printf("Test  o: %o\r\n",  val);
	printf("Test -o: %o\r\n", -val);
	printf("Test  x: %x\r\n",  val);
	printf("Test -x: %x\r\n", -val);
	printf("Test  X: %X\r\n",  val);
	printf("Test -X: %X\r\n", -val);
	printf("Test  p: %p\r\n",  ptr);
	printf("Test  s: %s\r\n",  "Hello, kernel World!");
	printf("Test  complex: \"%s0x%X%s; '%c' is at %p\"\r\n",
			"Ten is ", 10, " in hex",
			c, c_ptr);
}

void dump_font()
{
	printf("Dumping font:\r\n\x1b[15;0m");
	for(uint8_t i = 0; i < 255; i++)
	{
		if(i == '\n' || i == '\t' || i == '\r' || i == '\x1b')
			printf(" ");
		else
			printf("%c", i);

		if(i % 32 == 0 && i != 0)
			printf("\r\n");
	}

	printf("\r\n\x1b[15;0m");
}

void dump_registers() // FIXME: x86 arch specific!
{
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

void cycle_delay(size_t cycles)
{
	for(; cycles > 0; cycles--)
		asm("nop"); // FIXME: hey that's platform specific!
}
