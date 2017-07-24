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

#include <arch/i386/cpu/isr.h>


isr_handler_entry_t isr_handlers[ISR_HANDERS_SIZE] = {0};

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

// FIXME: Check shit yo!
void isr_add_handler(size_t isr, isr_handler_t handler)
{
	bool added = false;

	for(size_t i = 0; i < ISR_HANDERS_SIZE; i++)
	{
		if(isr_handlers[i].handler == NULL)
		{
			isr_handlers[i].isr = isr;
			isr_handlers[i].handler = handler;

			added = true;
			break;
		}
	}

	if(!added)
		abort(); // FIXME: error results and stuff
}

void handle_interrupt(interrupt_cpu_state_t* state)
{
	uint32_t id = state->isr_id;

	// TODO: handle exceptions
	if(id < 32)
	{
		printf("\x1b[12mEXCEPTION %u: '%s' OCCURRED!\r\n", id,
				exception_names[id]);
		printf("ERR_CODE: %u\r\n\x1b[15;0m");

		printf("\r\n");
		print_cpu_state(state);

		abort();
	}

	// TODO: call handlers/transform into events
	for(size_t i = 0; i < ISR_HANDERS_SIZE; i++)
	{
		if(isr_handlers[i].isr == id)
			isr_handlers[i].handler();
	}

	pic_eoi(id - IRQ_OFFSET);
}

void print_cpu_state(interrupt_cpu_state_t* state) // FIXME: move out of isr
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




