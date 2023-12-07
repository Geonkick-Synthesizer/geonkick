/**
 * File name: filter.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
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

#ifndef GEONKICK_FILTER_H
#define GEONKICK_FILTER_H

#include "geonkick_internal.h"
#include "envelope.h"

#define GEONKICK_DEFAULT_FILTER_CUTOFF_FREQ (350.0f)
#define GEONKICK_DEFAULT_FILTER_FACTOR      (1.0f)

struct gkick_filter {
        enum gkick_filter_type type;
        int sample_rate;

        /* Filter cuttoff frequency. */
        gkick_real cutoff_freq;

        /* Filter damping factor. */
        gkick_real factor;

        /* A queue of a three elements */
        gkick_real queue_l[2];
        gkick_real queue_b[2];
        gkick_real queue_h[2];
        bool queue_empty;

        /* Filter coefficients. */
        gkick_real coefficients[2];

        /* Filter cutoff envelope. */
        struct gkick_envelope *cutoff_env;
	struct gkick_envelope *q_env;
        pthread_mutex_t lock;
};

enum geonkick_error
gkick_filter_new(struct gkick_filter **filter, int sample_rate);

enum geonkick_error
gkick_filter_init(struct gkick_filter *filter);

void gkick_filter_free(struct gkick_filter **filter);

void gkick_filter_lock(struct gkick_filter *filter);

void gkick_filter_unlock(struct gkick_filter *filter);

enum geonkick_error
gkick_filter_update_coefficents(struct gkick_filter *filter);

enum geonkick_error
gkick_filter_set_type(struct gkick_filter *filter,
                      enum gkick_filter_type type);

enum geonkick_error
gkick_filter_get_type(struct gkick_filter *filter,
                      enum gkick_filter_type *type);

enum geonkick_error
gkick_filter_set_cutoff_freq(struct gkick_filter *filter,
                             gkick_real cutoff);

enum geonkick_error
gkick_filter_set_factor(struct gkick_filter *filter,
                        gkick_real factor);

enum geonkick_error
gkick_filter_get_cutoff_freq(struct gkick_filter *filter,
                             gkick_real *cutoff);

enum geonkick_error
gkick_filter_get_factor(struct gkick_filter *filter,
                        gkick_real *factor);

enum geonkick_error
gkick_filter_val(struct gkick_filter *filter,
                 gkick_real in_val,
                 gkick_real *out_val,
                 gkick_real env_x);

#endif // GEONKICK_FILTER_H
