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

#include <arch/i386/isr.h>


// TODO: move to keyboard driver
static const char keymap[128] =
{
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t',  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0,     'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
	'\\',  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*',
	0,
	' ',
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F keys
	0, 0, 0, 0, 0,
	'-',
	0, 0, 0,
	'+',
	0, 0, 0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
};

static const char* exception_names[] =
{
	"Divide by Zero",
	"Debug",
	"Non-Maskable Interupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exception",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"LEGACY: Coprocessors Segment Fault",
	"Inavlid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"RESERVED",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"Security Exception",
	"RESERVED"
};

static size_t ticks = 0;

void handle_interrupt(struct interrupt_cpu_state* state)
{
	uint32_t id = state->isr_id;

	// handle exceptions
	if(id < 32)
	{
		printf("\x1b[12mEXCEPTION %u: '%s' OCCURRED!\r\n", id, exception_names[id]);
		printf("ERR_CODE: %u\r\n\x1b[15;0m");

		printf("\r\n");
		print_cpu_state(state);

		abort();
	}

	if(id == 33)
	{
		uint8_t scancode = inb(0x60);

		if(!(scancode & 0x80))
			putchar(keymap[scancode]);

		if(keymap[scancode] == '`')
			printf("TICKS: %u\n", ticks);
	}
	else if(id == 32)
	{
		ticks++;
	}
	else
		printf("INT: %u\r\n", id);

	pic_eoi(id - IRQ_OFFSET);
}

void print_cpu_state(struct interrupt_cpu_state* state)
{
	printf("INTERRUPT CPU STATE:\r\n\r\n");

	printf("INSTRUCTION POINTER AND EFLAGS:\r\n");
	printf("EIP:    0x%p\r\n", state->eip);
	printf("EFLAGS: 0x%p\r\n", state->eflags);

	printf("\r\n");
	printf("GENERAL PURPOSE REGISTERS:\r\n");
	printf("EAX: 0x%p\tEBX: 0x%p\r\n", state->eax, state->ebx);
	printf("ECX: 0x%p\tEDX: 0x%p\r\n", state->ecx, state->edx);

	printf("\r\n");
	printf("INDEX REGISTERS:\r\n");
	printf("EDI: 0x%p\tESI: 0x%p\r\n", state->edi, state->esi);

	printf("\r\n");
	printf("STACK REGISTERS:\r\n");
	printf("EBP: 0x%p\tESP: 0x%p\r\n", state->ebp, state->esp);

	printf("\r\n");
	printf("SEGMENT REGISTERS:\r\n");
	printf("CS:  0x%p\tDS:  0x%p\r\n", state->cs, state->ds);
	printf("ES:  0x%p\tFS:  0x%p\r\n", state->es, state->fs);
	printf("GS:  0x%p\r\n", state->gs);
}




