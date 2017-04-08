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

#include "envelope.h"

struct gkick_envelope*
gkick_envelope_create(void)
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
void
gkick_envelope_activate(struct gkick_envelope* envelope)
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
float
gkick_envelope_get_value(const struct gkick_envelope* envelope)
{
	long int x1, y1, x2, y2, xm, ym;
	struct gkick_envelope_point *point;

	if (envelope == NULL ||
	    (envelope->first == NULL
	     || envelope->last == NULL)) {
		return 0.0;
	}

	xm = gkick_clock_get_value(envelope->clock);
	if (xm < envelope->first->x || xm > envelope->last->x) {
		return 0.0;
	}

        x2 = x1 = xm;	
	point = envelope->first;
	while (point) {
		if (point->x > xm) {
			x2 = point->x;
			y2 = point->y;
			break;
		}
		point = point->next;
	}

	point = envelope->last;
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

struct gkick_envelope_point*
gkick_envelope_add_point(struct gkick_envelope *envelope, float x, float y)
{
	struct gkick_envelope_point *point;

	if (envelope == NULL) {
		return NULL;
	}

	point = (struct gkick_envelope_point*)malloc(sizeof(struct gkick_envelope_point));
	if (point == NULL) {
		return NULL;
	}
	memset(point, 0, sizeof(sizeof(struct gkick_envelope_point)));
	point->x = x;
	point->y = y;

	if (envelope->npoints == 0
	    || envelope->first == NULL || envelope->last == NULL) {
		envelope->first = envelope->last = point;
	} else {
		envelope->last->next = point;
		point->prev = envelope->last;
		envelope->last = point;
	}

	envelope->npoints++;

	return point;
}

void gkick_envelope_destroy(struct gkick_envelope *envelope)
{
	struct gkick_envelope_point *point;
	if (envelope == NULL) {
		return;
	}

	if (envelope->clock != NULL) {
		gkick_clock_destroy(envelope->clock);
	}

	if (envelope->npoints != 0) {
		while (envelope->first != NULL) {
			point = envelope->first;
			envelope->first = point->next;
			free(point);
		}
	}
	
	free(envelope);
}
