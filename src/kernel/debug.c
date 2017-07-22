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

#include <kernel/debug.h>


void dbg_print_mem(void* from, size_t size)
{
#ifndef _DEBUG
	return;
#endif

	size_t cols = 16;
	size_t rest = size;
	printf("Printing mem from 0x%p to 0x%p: \r\n", from, from + size);

	for(size_t i = 0; i < size; i += cols)
	{
		printf("\x1b[11;0m0x%p\x1b[15;0m: ", from);

		for(size_t j = 0; j < cols; j++)
		{
			if(j % (cols / 2) == 0 && j != 0)
				printf(" ");

			if(j <= rest)
			{
				uint8_t d = *(uint8_t*)(from + j);
				if(d)
					printf("%X ", d);
				else
					printf("\x1b[7;0m%X\x1b[15;0m ", d);
			}
			else
				printf("   ");
		}

		printf("|");

		for(size_t j = 0; j < (cols >= rest ? rest : cols); j++)
		{
			char c = *(char*)(from + j);
			if(' ' <= c && c <= '~')
				printf("%c", c);
			else
				printf("\x1b[0m.\x1b[15;0m");
		}

		printf("|\r\n");
		from += cols;
		rest -= cols;
	}
}


