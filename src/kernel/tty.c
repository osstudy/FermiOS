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

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <arch/i386/vga.h> // FIXME: shouldn't TTY be arch independant?


static int32_t   tty_row;
static int32_t   tty_column;
static uint8_t   tty_color;
static uint16_t* tty_buffer;

// TODO: ACTUALLY DO THIS CORRECT
static tty_state_t       tty_state            = TTY_ST_NORM;
static char              tty_esc_buffer[16]   = {0};
static size_t            tty_esc_buffer_index = 0;
static const vga_color_t tty_color_default    = VGA_COLOR_LIGHT_GREY |
	VGA_COLOR_BLACK << 14;


void tty_initialize()
{
	tty_row    = 0;
	tty_column = 0;
	tty_color  = tty_color_default;
	tty_buffer = VGA_MEMORY;

	tty_enable_cursor(true);
	tty_set_cursot(0, 0);
	tty_clear();
}

void tty_clear()
{
	for (int32_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (int32_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
}

void tty_setcolor(uint8_t color)
{
	tty_color = color;
}

void tty_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	tty_buffer[index] = vga_entry(c, color);
}

void tty_norm_handler(char c)
{
	switch(c)
	{
		case '\x1b':
		{
			tty_state = TTY_ST_ESC;
			break;
		}

		case '\r':
		{
			tty_column = 0;
			break;
		}

		case '\n':
		{
			tty_row++;
			break;
		}

		case '\t':
		{
			for(size_t i = 0; i < TTY_TAB_WIDTH; i++)
			{
				tty_putentryat(' ', tty_color, tty_column, tty_row);
				tty_column++;
			}

			break;
		}

		case '\b':
		{
			tty_column--;
			break;
		}

		case '\0':
			break;

		default:
		{
			tty_putentryat(c, tty_color, tty_column, tty_row);
			tty_column++;

			break;
		}
	}

	if(tty_column < 0)
	{
		tty_row--;
		tty_column = 0;
	}

	if(tty_row < 0)
		tty_row = 0;

	if(tty_column == VGA_WIDTH)
	{
		tty_column = 0;
		tty_row++;
	}

	if(tty_row == VGA_HEIGHT)
	{
		memcpy(tty_buffer, tty_buffer + VGA_WIDTH,
				(VGA_HEIGHT) * VGA_WIDTH * 2);

		tty_row--;
	}

	tty_set_cursot(tty_column, tty_row);
}

void tty_putchar(char c)
{
	switch(tty_state)
	{
		case TTY_ST_ESC:
		{
			memset(tty_esc_buffer, 0, 16);
			tty_esc_buffer_index = 0;

			if(c == '[')
				tty_state = TTY_ST_COL_FG;
			else if(c == 'm')
				tty_state = TTY_ST_NORM;
			else
				abort(); // FIXME: proper error?

			break;
		}

		case TTY_ST_COL_FG:
		{
			if(c == ';' || c == 'm')
			{
				int col = atoi(tty_esc_buffer);

				if(col == 0 && c == 'm')
					tty_color = tty_color_default;
				else
					tty_color = (tty_color & 0x70) | (col & 0x0F);

				memset(tty_esc_buffer, 0, 16);
				tty_esc_buffer_index = 0;

				if(c != 'm')
					tty_state = TTY_ST_COL_BG;
				else
					tty_state = TTY_ST_NORM;
			}
			else
				tty_esc_buffer[tty_esc_buffer_index++] = c;

			break;
		}

		case TTY_ST_COL_BG:
		{
			if(c == 'm')
			{
				int col = atoi(tty_esc_buffer);
				tty_color = (tty_color & 0x0F) | (col & 0x0F) << 4;
				tty_color &= 0x7F; //prevent blink

				tty_state = TTY_ST_NORM;
			}
			else
				tty_esc_buffer[tty_esc_buffer_index++] = c;

			break;
		}



		default:
		case TTY_ST_NORM:
		{
			tty_norm_handler(c);
			break;
		}
	}
}

void tty_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_set_cursot(size_t col, size_t row)
{
	// FIXME: don't hardcode the ports. get em from BIOS.

	uint16_t pos = row * VGA_WIDTH + col;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void tty_enable_cursor(bool enable)
{
	outb(0x3D4, 0x0A);
	if(enable)
		outb(0x3D5, 0x0D);
	else
		outb(0x3D5, 0x3F);
}
