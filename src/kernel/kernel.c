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
#include <kernel/kshell.h>


char getchar_buffer = '\0';
bool getchar_lock = true;
size_t ticks = 0;


void kbd_event_getchar(void* msg);
void timer_event_tick(void* msg);

void kernel_main(boot_info_t boot_info)
{
	tty_initialize();
	timer_init();
	kbd_init();

	tty_set_color(0xF, 0x0); // For cursor color TODO: add set_cursor_col()
	tty_clear();
	tty_set_color(0xD, 0x0);

	// logo lol
	printf("d88888b d88888b d8888b. .88b  d88. d888888b  .d88b.  .d8888. \n");
	printf("88'     88'     88  `8D 88'YbdP`88   `88'   .8P  Y8. 88'  YP \n");
	printf("88ooo   88ooooo 88oobY' 88  88  88    88    88    88 `8bo.   \n");
	printf("88~~~   88~~~~~ 88`8b   88  88  88    88    88    88   `Y8b. \n");
	printf("88      88.     88 `88. 88  88  88   .88.   `8b  d8' db   8D \n");
	printf("YP      Y88888P 88   YD YP  YP  YP Y888888P  `Y88P'  `8888Y' \n");
	printf("\n  \t\t\t\tversion %s loaded.\n\n", _KERNEL_VERSION);

	tty_set_color(0xF, 0x0);
	printf("\n");

	event_add_handler(kbd_event_id, kbd_event_getchar);
	event_add_handler(timer_event_id, timer_event_tick);

	kernel_shell(boot_info);

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

