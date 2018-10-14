/**
 * File name: oscillator.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#ifndef GKICK_OSCILLATOR_H
#define GKICK_OSCILLATOR_H

#include "geonkick_internal.h"
#include "envelope.h"
#include "filter.h"

#define GKICK_OSC_DEFAULT_AMPLITUDE   0.005
#define GKICK_OSC_DEFAULT_FREQUENCY   150

enum geonkick_osc_state {
        GEONKICK_OSC_STATE_DISABLED = 0,
        GEONKICK_OSC_STATE_ENABLED  = 1
};

struct gkick_oscillator {
        enum geonkick_osc_state state;
	enum geonkick_osc_func_type func;
	gkick_real phase;
	gkick_real sample_rate;
	gkick_real frequency;
	gkick_real amplitude;
	size_t env_number;
	struct gkick_envelope **envelopes;
        struct gkick_filter *filter;
        int filter_enabled;
	pthread_mutex_t lock;
};

struct gkick_oscillator
*gkick_osc_create(void);

void gkick_osc_free(struct gkick_oscillator **osc);

void gkick_osc_set_state(struct gkick_oscillator *osc,
                         enum geonkick_osc_state state);

int gkick_osc_enabled(struct gkick_oscillator *osc);

enum geonkick_error
gkick_osc_create_envelopes(struct gkick_oscillator *osc);

struct gkick_envelope*
gkick_osc_get_envelope(struct gkick_oscillator *osc,  size_t env_index);

void
gkick_osc_increment_phase(struct gkick_oscillator *osc,
			  gkick_real t,
			  gkick_real kick_len);
gkick_real
gkick_osc_value(struct gkick_oscillator *osc,
		gkick_real t,
		gkick_real kick_len);

gkick_real
gkick_osc_func_sine(gkick_real phase);

gkick_real
gkick_osc_func_square(gkick_real phase);

gkick_real gkick_osc_func_triangle(gkick_real phase);

gkick_real gkick_osc_func_sawtooth(gkick_real phase);

gkick_real gkick_osc_func_noise(void);

void
gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      gkick_real **buff,
			      size_t *npoints);

#endif
