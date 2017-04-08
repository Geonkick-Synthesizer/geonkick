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
	
	return envelope;
}

/**
 * This functions resetes the time elapsed since the envelope 
 * acts on the signal.
 */
void
gkick_envelope_activate(struct gkick_envelope* envelope)
{
}

/**
 * Compexity = O(N), where N are the nuber of evelope points.
 * TODO: to reduce the complexity with a hash table intead of the list.
 * With an array it can be reduced to O(log(N)) with binary search.
 */
float
gkick_envelope_get_value(const struct gkick_envelope* envelope, double xm)
{
	double x1, y1, x2, y2, ym;
	struct gkick_envelope_point *point;

	if (envelope == NULL ||
	    (envelope->first == NULL
	     || envelope->last == NULL)) {
		return 0.0;
	}

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
	
	if (fabsl(x2 - x1) < 1e-40) {
		return 0.0;
	} else {
		ym = (y1 * (x2 - xm) + y2 * (xm - x1)) / (x2 - x1);
	}

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
	point->x = 1e6 * x;
	point->y = y;

	if (envelope->npoints == 0
	    || envelope->first == NULL || envelope->last == NULL) {
		envelope->first = envelope->last = point;
		printf("here 0\n");
	} else {
		gkick_envelope_add_sorted(envelope, point);
	}

	envelope->npoints++;

	return point;
}

void gkick_envelope_add_sorted(struct gkick_envelope *envelope, struct gkick_envelope_point *point)
{
	struct gkick_envelope_point *p;
	printf("gkick_envelope_add_sorted");
	
	p = envelope->first;

	if (point->x > envelope->last->x) {
		printf("here[1]\n");
		envelope->last->next = point;
	        point->prev = envelope->last;
		envelope->last = point;
	} else {	
		while (p) {
			printf("here1\n");
			if (point->x < p->x) {
				if (p == envelope->first) {
					point->next = p;
					p->prev = point;
					envelope->first = point;
								printf("here2\n");
				} else {
					point->prev = p->prev;
					point->next = p;
					p->prev->next = point;
					p->prev = point;
				}
				break;
			}
			p = p->next;
		}
	}
}

void gkick_envelope_destroy(struct gkick_envelope *envelope)
{
	struct gkick_envelope_point *point;
	if (envelope == NULL) {
		return;
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
