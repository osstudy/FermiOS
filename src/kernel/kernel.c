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
#include <string.h>

#include <sys_common.h>
#include <kernel/hal/boot.h>
#include <kernel/hal/cpu.h>
#include <kernel/hal/timer.h>
#include <kernel/hal/kbd.h>
#include <kernel/hal/tty.h>
#include <kernel/system.h>
#include <kernel/debug.h>


char getchar_buffer = '\0';
bool getchar_lock = true;
static size_t ticks = 0;


void kbd_event_getchar(void* msg);
void timer_event_tick(void* msg);
void timer_event_tick2(void* msg);
int kernel_shell_input(const char* prompt, char* buffer, const size_t size);
int32_t strtohex(const char* str);
void kernel_shell();

void kernel_main(boot_info_t boot_info)
{
	tty_initialize();
	timer_init();
	kbd_init();

	tty_set_color(0xF, 0x0); // For cursor color TODO: add set_cursor_col()
	tty_clear();
	tty_set_color(0xD, 0x0);
	printf("%c FermiOS ", 0x02);
	tty_set_color(0xF, 0x0);

	printf("%s loaded.\n", _KERNEL_VERSION);
	printf("MIT LICENSE Copyright (C) 2017 Dmytro Kalchenko\n");
	printf("Avaiable RAM: %u MB\n", boot_info.mem_size / 1000);
	printf("CPU: %s\n", boot_info.cpu_info.info);

	event_add_handler(kbd_event_id, kbd_event_getchar);
	event_add_handler(timer_event_id, timer_event_tick);
	//event_add_handler(timer_event_id, timer_event_tick2);

	kernel_shell();

	while(true)
		cpu_halt();
}

// FIXME: return only alphanum
// FIXME: I have NO idea what I am doing with these events
void kbd_event_getchar(void* msg)
{
	kbd_event_msg_t m = *((kbd_event_msg_t*)msg);

	if(!(m.scancode & 0x80))
	{
		getchar_lock = false;
		getchar_buffer = m.character;
	}
}

void timer_event_tick(void* msg)
{
	ticks = *((size_t*)msg);
}

void timer_event_tick2(void* msg)
{
	size_t ticks = *((size_t*)msg);

	if((ticks % 500) == 0)
		printf("\nANOTHER 500 TICKS PASSED!\n");
}

int kernel_shell_input(const char* prompt, char* buffer, const size_t size)
{
	size_t i = 0;
	char c = '\0';

	printf("%s", prompt);
	while((c = getchar()) != '\n')
	{
		if(c == '\b')
		{
			if(i <= 0)
				return -1;

			tty_set_cursor_delta(-1, 0);
			putchar(' ');
			tty_set_cursor_delta(-1, 0);

			buffer[--i] = '\0';
		}
		else
		{
			buffer[i++] = c;
			putchar(buffer[i - 1]);
		}

		if(i >= size)
			PANIC("buffer overflow");
	}

	return i;
}

void kernel_shell()
{
	printf("Welcome to kernel shell. It is used for debugging.\n");
	printf("Type 'help' for the list of available commands.\n");
	printf("\n");

	while(true)
	{
		char buffer[255] = {'\0'};
		int i = kernel_shell_input("kernel> ", buffer, 255);

		if(i < 0)
			putchar('\r');
		else if(i == 0)
			putchar('\n');
		else
		{
			putchar('\n');

			if(strcmp(buffer, "help") == 0)
			{
				printf("help    - prints this message\n");
				printf("ver     - prints kernel version\n");
				printf("regs    - prints register dump\n");
				printf("dumpmem - prints memory dump\n");
				printf("author  - prints developer's name\n");
				printf("clear   - clears the screen\n");
				printf("timer   - print timer ticks since start\n");
				printf("events  - print registered events info\n");
				printf("panic   - test kernel panic\n");
			}
			else if(strcmp(buffer, "ver") == 0)
				printf("FermiOS kenerel ver. %s\n", _KERNEL_VERSION);
			else if(strcmp(buffer, "regs") == 0)
				cpu_dump_state();
			else if(strcmp(buffer, "dumpmem") == 0)
			{
				// TODO: better yet, implement malloc() and strtok()

				size_t from = 0;
				size_t size = 0;

				memset(buffer, '\0', 255);

				kernel_shell_input("From (base 16): ", buffer, 255);
				printf("\n");

				from = strtohex(buffer);
				memset(buffer, '\0', 255);

				kernel_shell_input("Size (base 10): ", buffer, 255);
				printf("\n");

				size = atoi(buffer);

				dbg_print_mem((void*)from, size);
			}
			else if(strcmp(buffer, "author") == 0)
			{
				printf("Made by: Dmytro Kalchenko\n");
				printf("Under MIT LICENSE Copyright (C) 2017\n");
				printf("Contact at: dmytro.v.kalchenko@gmail.com\n");
			}
			else if(strcmp(buffer, "clear") == 0)
				tty_clear();
			else if(strcmp(buffer, "timer") == 0)
				printf("TICKS: %d\n", ticks);
			else if(strcmp(buffer, "events") == 0)
			{
				printf("Registered events: \n");
				for(int i = 0; i < EVENT_TYPES_SIZE; i++)
				{
					char* name;
					if((name = event_get_name(i)) != NULL)
					{
						int subs = event_get_num_handlers(i);
						printf("EVENT[%d]: '%s' Subs: %d\n", i, name, subs);
					}
				}
			}
			else if(strcmp(buffer, "panic") == 0)
			{
				PANIC("user induced panic, everything is normal.");
			}
			else
				printf("'%s' - unknown command\n", buffer);
		}
	}
}

// TODO: add strtol() to stdlib instead of this
int32_t strtohex(const char* str)
{
	int32_t num = 0;

	if((*str) == '0')
	{
		str++;

		if((*str) == 'x' || (*str) == 'X')
			str++;
	}

	while(*str)
	{
		int32_t n = 0;
		if(*str >= '0' && *str <= '9')
			n = (*str) - '0';
		else if(*str >= 'A' && *str  <= 'F')
			n = (*str) - 'A' + 10;
		else if(*str >= 'a' && *str <= 'f')
			n = (*str) - 'a' + 10;
		else
			PANIC("couldn't parse the hex string");

		num = num * 16 + n;
		str++;
	}

	return num;
}


