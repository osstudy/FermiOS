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

#include <kernel/hal/kbd.h>
#include <arch/i386/cpu/ports.h>
#include <arch/i386/cpu/isr.h>


static bool shifted_l   = false;
static bool shifted_r   = false;
static bool caps_locked = false;

int kbd_event_id = -1;


void kbd_init()
{
	isr_add_handler(IRQ_OFFSET + 1, kbd_handler);
	kbd_event_id = event_add_type("keyboard");
}

void kbd_handler()
{
	char character = '\0';

	uint8_t scancode = inb(0x60);
	io_wait();

	if(scancode == 0x3A)
		caps_locked = !caps_locked;

	if(scancode == 0x36)
		shifted_r = true;
	if(scancode == 0x2A)
		shifted_l = true;

	if(scancode == (0x36 + 0x80))
		shifted_r = false;
	if(scancode == (0x2A + 0x80))
		shifted_l = false;

	bool caps = (shifted_l || shifted_r) ^ caps_locked;

	// FIXME: Only alpha chars should be affected by CAPS LOCK
	if((scancode >= 0x02 && scancode <= 0x37) || scancode == 0x39
			|| (scancode >= 0x47 && scancode <= 0x53))
	{
		if(caps)
			character = kbd_keymap_shft_us[scancode];
		else
			character = kbd_keymap_us[scancode];
	}

	kbd_event_msg_t msg = {scancode, character};
	event_trigger(kbd_event_id, (void*)&msg);
}
