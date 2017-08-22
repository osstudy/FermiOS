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

#ifndef KERNEL_HAL_BOOT_H
#define KERNEL_HAL_BOOT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel/hal/cpu.h>
#include <sys_common.h>


#define RAM_CHUNKS_SIZE 256

typedef struct
{
	uint64_t base;
	uint64_t size;
	bool available;

} ram_chunk_t;

typedef struct // TODO: Expand
{
	size_t mem_size;
	size_t ram_chunks;
	ram_chunk_t ram_map[RAM_CHUNKS_SIZE];
	cpu_info_t cpu_info;

} boot_info_t;

#endif // KERNEL_HAL_BOOT_H
