/*
  Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "clock.h"

struct gkick_clock* gkick_clock_create(void)
{
	struct gkick_clock* clock;

	clock = (struct gkick_clock*)malloc(sizeof(struct gkick_clock));
	if (clock == NULL) {
		return NULL;
	}

	clock->started_at = -1;
	clock->started = 0;

	
	return clock;
}

void gkick_clock_start(struct gkick_clock *clock)
{
	struct timespec start;
	
	if (clock == NULL) {
		return;
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	clock->started = 1;
	clock->started_at = start.tv_sec * (1000000000L) + start.tv_nsec;
}

int gkick_clock_stop(struct gkick_clock *clock)
{
	if (clock == NULL) {
		return 0;
	}

	clock->started = 0;
	
	return 0;
}

long int gkick_clock_get_value(struct gkick_clock *clock)
{
	struct timespec start;
	long int current;
	
	if (clock == NULL) {
		return 0;
	}
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	current = start.tv_sec * (1000000000L) + start.tv_nsec;
	return current - clock->started_at;
}

void gkick_clock_destroy(struct gkick_clock *clock)
{
	if (clock != NULL) {
		free(clock);
	}
}
