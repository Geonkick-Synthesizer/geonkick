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

struct gkick_envelope_point {
	struct gkick_envelope_point *prev;
	struct gkick_envelope_point *next;
	float x;
	float y;
};

struct gkick_envelope {
	struct gkick_clock *clock;
	size_t npoints;
	struct gkick_envelope_point *first;
	struct gkick_envelope_point *last;
};

struct gkick_envelope*
gkick_envelope_create(void);

/**
 * This functions resetes the time elapsed since the envelope 
 * acts on the signal.
 */
void
gkick_envelope_activate(struct gkick_envelope* envelope);

/**
 * Compexity = O(N), where N are the nuber of evelope points.
 * TODO: to reduce the complexity with a hash table intead of the list.
 * With an array it can be reduced to O(log(N)) with binary search.
 */
float
gkick_envelope_get_value(const struct gkick_envelope* envelope);

struct gkick_envelope_point*
gkick_envelope_add_point(struct gkick_envelope *envelope, float x, float y);

void
gkick_envelope_destroy(struct gkick_envelope *envelope);

#endif
