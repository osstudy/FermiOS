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

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>


enum vga_color
{
	VGA_COLOR_BLACK              = 0,
	VGA_COLOR_BLUE               = 1,
	VGA_COLOR_GREEN              = 2,
	VGA_COLOR_CYAN               = 3,
	VGA_COLOR_RED                = 4,
	VGA_COLOR_MAGENTA            = 5,
	VGA_COLOR_BROWN              = 6,
	VGA_COLOR_LIGHT_GREY         = 7,
	VGA_COLOR_DARK_GREY          = 8,
	VGA_COLOR_LIGHT_BLUE         = 9,
	VGA_COLOR_LIGHT_GREEN        = 10,
	VGA_COLOR_LIGHT_CYAN         = 11,
	VGA_COLOR_LIGHT_RED          = 12,
	VGA_COLOR_LIGHT_MAGENTA      = 13,
	VGA_COLOR_LIGHT_BROWN        = 14,
	VGA_COLOR_WHITE              = 15
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(uint8_t uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

#endif // ARCH_I386_VGA_H
