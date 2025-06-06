/**
 * File name: oscillator.c
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

#ifndef GKICK_OSCILLATOR_H
#define GKICK_OSCILLATOR_H

#include "geonkick_internal.h"
#include "envelope.h"
#include "filter.h"

#define GKICK_OSC_DEFAULT_AMPLITUDE     1.0f
#define GKICK_OSC_DEFAULT_FREQUENCY     150.0f
#define GKICK_OSC_DEFAULT_PITCH         69.0f
#define GKICK_MAX_NOISE_DENSITY         400
#define GKICK_OSC_DEFAULT_NOISE_DENSITY GKICK_MAX_NOISE_DENSITY


enum geonkick_osc_state {
        GEONKICK_OSC_STATE_DISABLED = 0,
        GEONKICK_OSC_STATE_ENABLED  = 1
};

enum gkick_osc_envelope_type {
        GKICK_OSC_AMPLITUDE_ENVELOPE        = 0,
        GKICK_OSC_FREQUENCY_ENVELOPE        = 1,
        GKICK_OSC_PITCH_SHIFT_ENVELOPE      = 2,
        GKICK_OSC_NOISE_DENSITY_ENVELOPE    = 3,
        GKICK_OSC_DISTORTION_DRIVE_ENVELOPE = 4
};

struct gkick_oscillator {
        int sample_rate;
        enum geonkick_osc_state state;
	enum geonkick_osc_func_type func;
        /* Used for Brownian noise */
        gkick_real brownian;
        /* User as a seed for pseudo random generator. */
        unsigned int seedp;
        unsigned int seed;
        gkick_real initial_phase;
	gkick_real phase;
	gkick_real frequency;
	gkick_real amplitude;
        /* Pitch shift semitones. */
        gkick_real pitch_shift;

        /* Noise density. */
        gkick_real noise_density;

        struct gkick_buffer *sample;

        /* FM input value for this OSC. */
        gkick_real fm_input;
        /* FM input coefficient. Used for compatibity of presets. */
        gkick_real fm_k;

        /* Specifies if this OSC is a FM source to other oscillator. */
        bool is_fm;

	size_t env_number;
	struct gkick_envelope **envelopes;
        bool filter_enabled;
        struct gkick_filter *filter;
        bool distortion_enabled;
        struct gkick_distortion *distortion;
	pthread_mutex_t lock;
};

struct gkick_oscillator
*gkick_osc_create(int sample_rate);

void gkick_osc_free(struct gkick_oscillator **osc);

void gkick_osc_set_state(struct gkick_oscillator *osc,
                         enum geonkick_osc_state state);

int gkick_osc_enabled(struct gkick_oscillator *osc);

enum geonkick_error
gkick_osc_create_envelopes(struct gkick_oscillator *osc);

struct gkick_envelope*
gkick_osc_get_envelope(struct gkick_oscillator *osc,
                       size_t env_index);

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

gkick_real
gkick_osc_func_triangle(gkick_real phase);

gkick_real
gkick_osc_func_sawtooth(gkick_real phase);

gkick_real
gkick_osc_func_noise_white(unsigned int *seed, unsigned int density);

gkick_real
gkick_osc_func_noise_pink(void);

gkick_real
gkick_osc_func_noise_brownian(gkick_real *previous,
                              unsigned int *seed,
                              unsigned int density);

gkick_real
gkick_osc_func_sample(struct gkick_buffer *sample,
                      float pitch_shift);

void gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
                                   size_t env_index,
                                   struct gkick_envelope_point_info **buff,
                                   size_t *npoints);

void gkick_osc_set_envelope_points(struct gkick_oscillator *osc,
                                   size_t env_index,
                                   const struct gkick_envelope_point_info *buff,
                                   size_t npoints);

unsigned int gkick_osc_get_noise_density(struct gkick_oscillator *osc,
                                         gkick_real t,
                                         gkick_real kick_len);

#endif // GKICK_OSCILLATOR_H
