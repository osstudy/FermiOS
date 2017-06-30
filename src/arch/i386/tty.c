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
#include <arch/i386/vga.h>


static const int32_t VGA_WIDTH = 80;
static const int32_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static int32_t terminal_row;
static int32_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// TODO: ACTUALLY DO THIS CORRECT
static enum terminal_state tty_state = TTY_ST_NORM;
static char tty_esc_buffer[16] = {0};
static size_t tty_esc_buffer_index = 0;
static const uint8_t terminal_color_default = VGA_COLOR_LIGHT_GREY |
	VGA_COLOR_BLACK << 14;


void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = terminal_color_default;
	terminal_buffer = VGA_MEMORY;

	terminal_clear();
}

void terminal_clear()
{
	for (int32_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (int32_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_norm_handler(char c)
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
			terminal_column = 0;
			break;
		}

		case '\n':
		{
			terminal_row++;
			break;
		}

		case '\t':
		{
			for(size_t i = 0; i < TTY_TAB_WIDTH; i++)
			{
				terminal_putentryat(' ', terminal_color,
						terminal_column, terminal_row);
				terminal_column++;
			}

			break;
		}

		case '\b':
		{
			terminal_column--;
			break;
		}

		default:
		{
			terminal_putentryat(c, terminal_color,
					terminal_column, terminal_row);
			terminal_column++;

			break;
		}
	}

	if(terminal_column < 0)
	{
		terminal_row--;
		terminal_column = 0;
	}

	if(terminal_row < 0)
		terminal_row = 0;

	if(terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		terminal_row++;
	}

	// TODO: last row is always empty
	if(terminal_row == VGA_HEIGHT)
	{
		memcpy(terminal_buffer, terminal_buffer + VGA_WIDTH, (VGA_HEIGHT)
				* VGA_WIDTH * 2);

		terminal_row--;
	}
}

void terminal_putchar(char c)
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
					terminal_color = terminal_color_default;
				else
					terminal_color = (terminal_color & 0x70) | (col & 0x0F);

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
				terminal_color = (terminal_color & 0x0F) | (col & 0x0F) << 4;

				tty_state = TTY_ST_NORM;
			}
			else
				tty_esc_buffer[tty_esc_buffer_index++] = c;

			break;
		}



		default:
		case TTY_ST_NORM:
		{
			terminal_norm_handler(c);
			break;
		}
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

