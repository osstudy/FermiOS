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

#include <kernel/hal/tty.h>
#include <arch/i386/vga.h>
#include <arch/i386/cpu/ports.h>


static int tty_row;
static int tty_column;
static uint8_t tty_color_fg;
static uint8_t tty_color_bg;

static uint16_t* tty_buffer;

static const uint8_t tty_color_default_fg = VGA_COLOR_LIGHT_GREY;
static const uint8_t tty_color_default_bg = VGA_COLOR_BLACK;


void tty_initialize()
{
	tty_color_fg = tty_color_default_fg;
	tty_color_bg = tty_color_default_bg;
	tty_buffer   = VGA_MEMORY;

	tty_enable_cursor(true);
	tty_set_cursor(0, 0);
	tty_clear();
}

void tty_clear()
{
	for (int32_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (int32_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ',
					vga_entry_color(tty_color_fg, tty_color_bg));
		}
	}

	tty_set_cursor(0, 0);
}

void tty_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	tty_buffer[index] = vga_entry(c, color);
}

void tty_putchar(char c)
{
	switch(c)
	{
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
			tty_write("    ", 4); // FIXME: is it even my concenr?
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
			tty_putentryat(c, vga_entry_color(tty_color_fg, tty_color_bg),
						tty_column, tty_row);
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
		for(int i = 0; i < VGA_WIDTH; i++) // Painting BG with selected color
			tty_putentryat(' ', vga_entry_color(tty_color_fg,
						tty_color_bg), i, tty_row);
	}

	tty_set_cursor(tty_column, tty_row);
}

void tty_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		tty_putchar(data[i]);
}

void tty_set_cursor_delta(int col, int row)
{
	tty_set_cursor(tty_column += col, tty_row += row);
}

void tty_set_cursor(size_t col, size_t row)
{
	// FIXME: don't hardcode the ports. get em from BIOS.

	tty_column = col;
	tty_row = row;

	uint16_t pos = (row * VGA_WIDTH) + col;
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

void tty_set_color(uint8_t fg, uint8_t bg)
{
	tty_color_fg = fg;
	tty_color_bg = bg;
}
