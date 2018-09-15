/**
 * File name: oscillator.c
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include <pthread.h>

#define GKICK_OSC_DEFAULT_SAMPLE_RATE 48000
#define GKICK_OSC_DEFAULT_AMPLITUDE   150
#define GKICK_OSC_DEFAULT_FREQUENCY   0.5

struct gkick_oscillator {
	enum geonkick_osc_func_type func;
	double phase;
	double sample_rate;
	double frequency;
	double amplitude;
	size_t env_number;
	struct gkick_envelope **envelopes;
	pthread_mutex_t lock;
};

struct gkick_oscillator
*gkick_osc_create(struct geonkick *kick);

void gkick_osc_free(struct gkick_oscillator **osc);

enum geonkick_error
gkick_osc_create_envelopes(struct gkick_oscillator *osc);

void
gkick_osc_increment_phase(struct gkick_oscillator *osc,
			  double t,
			  double kick_len);
double
gkick_osc_value(struct gkick_oscillator *osc,
		double t,
		double kick_len);

double
gkick_osc_func_sine(double phase);

double
gkick_osc_func_square(double phase);

double gkick_osc_func_triangle(double phase);

double gkick_osc_func_sawtooth(double phase);

double gkick_osc_func_noise(void);

void
gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      double **buff,
			      size_t *npoints);

#endif
