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

#include <kernel/event.h>


static char*   event_types[EVENT_TYPES_SIZE] = {0};
static event_t events[EVENTS_SIZE]           = {0};


int event_add_type(char* name)
{
	for(size_t i = 0; i < EVENT_TYPES_SIZE; i++)
	{
		if(event_types[i] == NULL)
		{
			event_types[i] = name;
			return i;
		}
	}

	return -1;
}

int event_get_id(char* name)
{
	for(size_t i = 0; i < EVENT_TYPES_SIZE; i++)
		if(event_types[i] == name)
			return i;

	return -1;
}

char* event_get_name(int id)
{
	for(int i = 0; i < EVENT_TYPES_SIZE; i++)
		if(i == id)
			return event_types[i];

	return NULL;
}

void event_add_handler(int id, event_handler_t handler)
{
	bool added = false;

	for(size_t i = 0; i < EVENTS_SIZE; i++)
	{
		if(events[i].handler == NULL )
		{
			events[i].id = id;
			events[i].handler = handler;

			added = true;
			break;
		}
	}

	if(!added)
		abort(); // FIXME: return error
}

size_t event_get_num_handlers(int id)
{
	size_t ret = 0;

	for(size_t i = 0; i < EVENTS_SIZE; i++)
	{
		if(events[i].handler != NULL && events[i].id == id)
			ret++;
	}

	return ret;
}

void event_trigger(int id, void* msg)
{
	for(size_t i = 0; i < EVENTS_SIZE; i++)
	{
		if(events[i].id == id)
		{
			if(events[i].handler != NULL)
				events[i].handler(msg);
		}
	}
}
