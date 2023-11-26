/**
 * File name: envelope.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GKICK_ENVELOPE_H
#define GKICK_ENVELOPE_H

#include "geonkick_internal.h"

struct gkick_envelope_point {
	struct gkick_envelope_point *prev;
	struct gkick_envelope_point *next;
	gkick_real x;
	gkick_real y;
};

struct gkick_envelope {
        enum gkick_envelope_apply_type apply_type;
	size_t npoints;
	struct gkick_envelope_point *first;
	struct gkick_envelope_point *last;
};

struct gkick_envelope*
gkick_envelope_create(void);

void gkick_envelope_set_apply_type(struct gkick_envelope* envelope,
				   enum gkick_envelope_apply_type apply_type);

enum gkick_envelope_apply_type
gkick_envelope_get_apply_type(const struct gkick_envelope* envelope);

gkick_real
gkick_envelope_get_value(const struct gkick_envelope* envelope,
                         gkick_real xm);

struct gkick_envelope_point*
gkick_envelope_add_point(struct gkick_envelope *envelope,
                         float x,
                         float y);

void gkick_envelope_add_sorted(struct gkick_envelope *envelope,
			       struct gkick_envelope_point *point);

void gkick_envelope_destroy(struct gkick_envelope *envelope);

void gkick_envelope_get_points(struct gkick_envelope *env,
                               gkick_real **buff,
                               size_t *npoints);

void gkick_envelope_set_points(struct gkick_envelope *env,
			  const gkick_real *buff,
			  size_t npoints);


void gkick_envelope_clear(struct gkick_envelope* env);

void gkick_envelope_remove_point(struct gkick_envelope *env,
                                 size_t index);

void gkick_envelope_update_point(struct gkick_envelope *env,
                                 size_t index,
                                 gkick_real x,
                                 gkick_real y);

#endif // GKICK_ENVELOPE_H
