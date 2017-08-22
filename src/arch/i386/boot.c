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

#include <kernel/hal/boot.h>
#include <kernel/hal/cpu.h>
#include <arch/i386/multiboot.h>


extern void kernel_main();

typedef struct multiboot_memory_map
{
	uint32_t size;
	uint32_t base_addr_low, base_addr_high;
	uint32_t length_low, length_high;
	uint32_t type;

} multiboot_memory_map_t;

void boot_i386(multiboot_info_t* mbd)
{
	cpu_initialize();

	boot_info_t info;
	info.mem_size = mbd->mem_lower + mbd->mem_upper;
	info.cpu_info = cpu_get_info();

	size_t i = 0;
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbd->mmap_addr;
	while((uint32_t)mmap < mbd->mmap_addr + mbd->mmap_length)
	{
		info.ram_map[i].base = ((uint64_t)mmap->base_addr_high << 32)
			| (uint64_t)mmap->base_addr_low;
		info.ram_map[i].size = ((uint64_t)mmap->length_high    << 32)
			| (uint64_t)mmap->length_low;
		info.ram_map[i].available = mmap->type == 1;

		i++;
		mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size
				+ sizeof(mmap->size));
	}

	info.ram_chunks = i;
	kernel_main(info);
}

