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

#include <kernel/kshell.h>


// TODO: add strtol() to stdlib instead of this
static int32_t strtohex(const char* str)
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

void kernel_shell(boot_info_t info)
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
				printf("meminfo - prints memory information\n");
				printf("cpuinfo - prints CPU information\n");
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
			else if(strcmp(buffer, "meminfo") == 0)
			{
				printf("Detected memory size: %dMiB\n", info.mem_size / 1024);
				printf("Memory map:\n");
				for(size_t i = 0; i < info.ram_chunks; i++)
				{
					ram_chunk_t chunk = info.ram_map[i];
					uint64_t to = chunk.base + chunk.size;

					printf("From 0x%p%p to 0x%p%p - %s\n",
							(uint32_t)((chunk.base >> 32) & 0x00000000FFFFFFFF),
							(uint32_t)( chunk.base        & 0x00000000FFFFFFFF),
							(uint32_t)((        to >> 32) & 0x00000000FFFFFFFF),
							(uint32_t)(         to        & 0x00000000FFFFFFFF),
							chunk.available ? "Available" : "Reserved");
				}
			}
			else if(strcmp(buffer, "cpuinfo") == 0)
			{
				printf("%s\n", info.cpu_info.info);
			}
			else
				printf("'%s' - unknown command\n", buffer);
		}
	}
}


