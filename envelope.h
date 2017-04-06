#ifndef GKICK_ENVELOPE_H
#define GKICK_ENVELOPE_H
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
#include "point.h"

struct gkick_envelope {
	struct gkick_clock *clock;
	size_t npoints;
	gkick_point *point;
	gkick_point *next;
};

struct gkick_envelope* gkick_envelope_create(void)
{
	struct gkick_envelope *envelope;

	envelope = (struct gkick_envelope*)malloc(sizeof(struct gkick_envelope));
	if (envelope == NULL) {
		return NULL;
	}

	memset(envelope, 0, sizeof(struct gkick_envelope));
	envelope->clock = gkick_clock_create();
	if (envelope->clock == NULL) {
		free(envelope);
		return NULL;
	}
	
	return envelope;
}

/**
 * This functions resetes the time elapsed since the envelope 
 * acts on the signal.
 */
void gkick_envelope_activate(struct gkick_envelope* envelope);
{
	if (envelope != NULL && envelope->clock != NULL) {
		// The envelope clock is restarted.
		gkick_clock_start(envelope->clock);
	}
}

/**
 * Compexity = O(N), where N are the nuber of evelope points.
 * TODO: to reduce the complexity with a hash table intead of the list.
 * With an array it can be reduced to O(log(N)) with binary search.
 */
void gkick_envelope_get_value(const struct gkick_envelope* envelope)
{
	long int x1, y1, x2, y2, xm, ym;
	struct gkick_envelope_point *point;

	if (envelope == NULL ||
	    (envelope->points_first == NULL
	     || envelope->points_last == NULL)) {
		return 0.0;
	}

	xm = gkick_clock_get_value(envelope->clock);
	if (xm < envelope->points_first->x || xm > envelope->points_last->x)) {
		return 0.0;
	}

        x2 = x1 = xm;	
	point = envelope->points_first;
	while (point) {
		if (point->x > xm) {
			x2 = point->x;
			y2 = point->y;
			break;
		}
		point = point->next;
	}

	point = envelope->points_last;
	while (point) {
		if (point->x < xm) {
			x1 = point->x;
			y1 = point->y;
			break;
		}
		point = point->prev;
	}

	ym = (y1 * (x2 - xm) + y2 * (xm - x1)) / (x2 - x1);

	return ym;
}

static void gkick_envelope_add_point(gkick_envelope *envelope, float x, float y)
{
       
}

static void gkick_envelope_remove_point(gkick_envelope *envelope, struct gkickpoint *point)
{
	
}

#endif
