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

#ifndef KERNEL_HAL_TTY_H
#define KERNEL_HAL_TTY_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys_common.h>


// FIXME: I don't really feel like this should emulate real TTY

void tty_initialize();
void tty_clear();
void tty_putchar(char c);
void tty_write(const char* data, size_t size);
void tty_set_cursor(size_t col, size_t row);
void tty_set_color(uint8_t fg, uint8_t bg);
void tty_set_cursor_delta(int col, int row);
// FIXME: IMPLEMENR ME!
//size_t tty_get_cursor_x();
//size_t tty_get_cursor_y();
//uint8_t tty_get_color_fg();
//uint8_t tty_get_color_bg();
void tty_enable_cursor(bool enable);

#endif // KERNEL_HAL_TTY_H
