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

#include "oscillator.h"
#include <math.h>

struct gkick_oscillator
*gkick_osc_create(void)
{
        struct gkick_oscillator *osc;

        osc = (struct gkick_oscillator*)malloc(sizeof(struct gkick_oscillator));
        if (osc == NULL) {
                return NULL;
        }
        memset(osc, 0, sizeof(struct gkick_oscillator));

        osc->state = GEONKICK_OSC_STATE_ENABLED;
        osc->func = GEONKICK_OSC_FUNC_SINE;
        osc->phase = 0.0;
        osc->sample_rate = GEONKICK_SAMPLE_RATE;
        osc->amplitude = GKICK_OSC_DEFAULT_AMPLITUDE;
        osc->frequency = GKICK_OSC_DEFAULT_FREQUENCY;
        osc->env_number = 2;

        if (gkick_osc_create_envelopes(osc) != GEONKICK_OK) {
                gkick_osc_free(&osc);
                return NULL;
        }

        if (gkick_filter_new(&osc->filter) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_osc_free(&osc);
		return NULL;
        }
        osc->filter_enabled = 0;

        return osc;
}

void gkick_osc_free(struct gkick_oscillator **osc)
{
        size_t i;

        if (osc == NULL || *osc == NULL) {
                return;
        }

        if ((*osc)->envelopes != NULL) {
                for (i = 0; i < (*osc)->env_number; i++) {
                        gkick_envelope_destroy((*osc)->envelopes[i]);
                }
                free((*osc)->envelopes);
                gkick_filter_free(&(*osc)->filter);
        }

        free(*osc);
        *osc = NULL;
}

void gkick_osc_set_state(struct gkick_oscillator *osc,
                         enum geonkick_osc_state state)
{
        osc->state = state;
}

enum geonkick_error gkick_osc_create_envelopes(struct gkick_oscillator *osc)
{
        size_t i;
        struct gkick_envelope *env;

        if (osc->env_number < 1) {
                return GEONKICK_ERROR;
        }

        osc->envelopes = (struct gkick_envelope**)malloc(sizeof(struct gkick_envelope*) *osc->env_number);
        if (osc->envelopes == NULL) {
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        memset(osc->envelopes, 0, sizeof(struct gkick_envelope*) * osc->env_number);

        for(i = 0; i < osc->env_number; i++) {
                env = gkick_envelope_create();
                if (env == NULL) {
                        return GEONKICK_ERROR;
                } else {
                        /* Add two default points. */
                        gkick_envelope_add_point(env, 0.0, 1.0);
                        gkick_envelope_add_point(env, 1.0, 1.0);
                        osc->envelopes[i] = env;
                }
        }

        return GEONKICK_OK;
}

struct gkick_envelope*
gkick_osc_get_envelope(struct gkick_oscillator *osc,  size_t env_index)
{
        if (osc == NULL) {
                return NULL;
        }

        if (env_index >= 0 && env_index < osc->env_number) {
                return osc->envelopes[env_index];
        }

        return NULL;
}

void gkick_osc_increment_phase(struct gkick_oscillator *osc,
			       gkick_real t,
			       gkick_real kick_len)
{
        gkick_real f;

        f = osc->frequency * gkick_envelope_get_value(osc->envelopes[1], t / kick_len);
        osc->phase += (2 * M_PI * f) / (osc->sample_rate);
        if (osc->phase > M_PI) {
                osc->phase -= 2 * M_PI;
        }
}

gkick_real gkick_osc_value(struct gkick_oscillator *osc,
		       gkick_real t,
		       gkick_real kick_len)
{
        gkick_real amp;
        gkick_real v;

        amp = osc->amplitude * gkick_envelope_get_value(osc->envelopes[0], t / kick_len);
        v = 0.0;
        switch (osc->func) {
        case GEONKICK_OSC_FUNC_SINE:
                v = amp * gkick_osc_func_sine(osc->phase);
                break;
        case GEONKICK_OSC_FUNC_SQUARE:
                v = amp * gkick_osc_func_square(osc->phase);
                break;
        case GEONKICK_OSC_FUNC_TRIANGLE:
                v = amp * gkick_osc_func_triangle(osc->phase);
                break;
        case GEONKICK_OSC_FUNC_SAWTOOTH:
                v = amp * gkick_osc_func_sawtooth(osc->phase);
                break;
        case GEONKICK_OSC_FUNC_NOISE_WHITE:
        case GEONKICK_OSC_FUNC_NOISE_PINK:
        case GEONKICK_OSC_FUNC_NOISE_BROWNIAN:
                v = amp * gkick_osc_func_noise();
                break;
        default:
                v = amp * gkick_osc_func_sine(osc->phase);
        };

        if (osc->filter_enabled) {
                gkick_filter_val(osc->filter, v, &v);
        }

        return v;
}

gkick_real gkick_osc_func_sine(gkick_real phase)
{
        return sin(phase);
}

gkick_real gkick_osc_func_square(gkick_real phase)
{
        if (phase < 0) {
                return -1;
        } else {
                return 1;
        }
}

gkick_real gkick_osc_func_triangle(gkick_real phase)
{
	gkick_real a = 1.0;

	if (phase < M_PI) {
		return -a + (2 * a / M_PI) * phase;
	}
	else {
		return 3 * a - (2 * a / M_PI) * phase;
	}
}

gkick_real gkick_osc_func_sawtooth(gkick_real phase)
{
        return 1 - (1 / M_PI) * (phase - M_PI);
}

gkick_real gkick_osc_func_noise(void)
{
        int sign = 1;
        if (rand() % 2) {
                sign = -1;
        }

        return sign * ((gkick_real)(rand() % RAND_MAX)) / RAND_MAX;
}

void
gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      gkick_real **buff,
			      size_t *npoints)
{
        if (buff != NULL) {
                *buff = NULL;
        }

        if (env_index >= 0 && env_index < osc->env_number) {
                gkick_envelope_get_points(osc->envelopes[env_index], buff, npoints);
        }
}

void
gkick_osc_set_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      gkick_real *buff,
			      size_t npoints)
{
        if (buff == NULL) {
                *buff = NULL;
        }

        if (env_index >= 0 && env_index < osc->env_number) {
                gkick_envelope_get_points(osc->envelopes[env_index], buff, npoints);
        }
}

int  gkick_osc_enabled(struct gkick_oscillator *osc)
{
        if (osc == NULL) {
                return 0;
        }

        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                return 1;
        }

        return 0;
}
