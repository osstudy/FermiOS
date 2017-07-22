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
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif


int puts(const char* string)
{
	return printf("%s\n", string);
}

int putchar(int ic)
{
#if defined(__is_libk)
		char c = (char)ic;
		if(c == '\n')
			tty_putchar('\r');
		tty_putchar(c);
#else
		// TODO: Implement syscall.
#endif
	return ic;
}

static bool print(const char* data, size_t length)
{
	const unsigned char* bytes = (const unsigned char*) data;

	for (size_t i = 0; i < length; i++)
	{
		if (putchar(bytes[i]) == EOF)
			return false;
	}

	return true;
}

static void itoa(int num, char* buf, size_t buf_size, size_t base, bool sign,
		const char* digit, int padding)
{
	memset(buf, '0',buf_size);

	if(num < 0 && sign)
	{
		*buf = '-';
		buf++;

		num *= -1;
	}

	int shift = num;
	size_t i = 0;

	do
	{
		buf++;
		i++;
		shift /= base;

		if(i >= buf_size)
			abort(); // FIXME: proper errors
	}
	while(shift);

	int x = padding - (int)i;
	if(x < 0)
		x = 0;
	buf += x;

	*buf = '\0';

	do
	{
		buf--;
		*buf = digit[num % base];
		num /= base;
	}
	while(num);
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0')
	{
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%')
		{
			if (format[0] == '%')
				format++;

			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;

			if (maxrem < amount)
			{
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}


			if (!print(format, amount))
				return -1;

			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c')
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);

			if (!maxrem)
			{
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(&c, sizeof(c)))
				return -1;

			written++;
		}
		else if (*format == 's')
		{
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			if (maxrem < len)
			{
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len))
				return -1;

			written += len;
		}
		else if(*format == 'd' || *format == 'i' ||
				*format == 'u' || *format == 'o' ||
				*format == 'x' || *format == 'X' ||
				*format == 'p')
		{
			char f = *format;
			format++;
			int num = va_arg(parameters, int);
			char buf[32];

			switch(f)
			{
				case 'd':
				case 'i':
					itoa(num, buf, 32, 10, true,  "0123456789"      , 0);
					break;

				case 'u':
					itoa(num, buf, 32, 10, false, "0123456789"      , 0);
					break;

				case 'o':
					itoa(num, buf, 32,  8, false, "01234567"        , 0);
					break;

				case 'x':
					itoa(num, buf, 32, 16, false, "0123456789abcdef", 0);
					break;

				default:
				case 'p':
					itoa(num, buf, 32, 16, false, "0123456789ABCDEF", 8);
					break;

				case 'X':
					itoa(num, buf, 32, 16, false, "0123456789ABCDEF", 2);
					break;
			}
			size_t len = strlen(buf);

			if (maxrem < len)
			{
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(buf, len))
				return -1;

			written += len;
		}

		else
		{
			format = format_begun_at;
			size_t len = strlen(format);

			if (maxrem < len)
			{
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(format, len))
				return -1;

			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

