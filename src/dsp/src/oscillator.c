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

#include "oscillator.h"

struct gkick_oscillator
*gkick_osc_create(int sample_rate)
{
        struct gkick_oscillator *osc;

        osc = (struct gkick_oscillator*)calloc(1, sizeof(struct gkick_oscillator));
        if (osc == NULL)
                return NULL;
        osc->state = GEONKICK_OSC_STATE_ENABLED;
        osc->func = GEONKICK_OSC_FUNC_SINE;
        osc->initial_phase = 0.0f;
        osc->phase = osc->initial_phase;
        osc->sample_rate = sample_rate;
        osc->amplitude = GKICK_OSC_DEFAULT_AMPLITUDE;
        osc->frequency = GKICK_OSC_DEFAULT_FREQUENCY;
        osc->pitch_shift = 0.0f;
        osc->env_number = 3;
        osc->brownian = 0;
        osc->is_fm = false;
        osc->fm_input = 0.0f;
        osc->seed = 100;
        osc->seedp = osc->seed;

        if (gkick_osc_create_envelopes(osc) != GEONKICK_OK) {
               gkick_osc_free(&osc);
                return NULL;
        }

        if (gkick_filter_new(&osc->filter, osc->sample_rate) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_osc_free(&osc);
		return NULL;
        }
        osc->filter_enabled = 0;
        return osc;
}

void
gkick_osc_free(struct gkick_oscillator **osc)
{
        if (osc == NULL || *osc == NULL)
                return;

        if ((*osc)->envelopes != NULL) {
                size_t i;
                for (i = 0; i < (*osc)->env_number; i++)
                        gkick_envelope_destroy((*osc)->envelopes[i]);
                free((*osc)->envelopes);
                gkick_filter_free(&(*osc)->filter);
                gkick_buffer_free(&(*osc)->sample);
        }

        free(*osc);
        *osc = NULL;
}

void
gkick_osc_set_state(struct gkick_oscillator *osc,
                         enum geonkick_osc_state state)
{
        osc->state = state;
}

enum geonkick_error
gkick_osc_create_envelopes(struct gkick_oscillator *osc)
{
        if (osc->env_number < 1)
                return GEONKICK_ERROR;

        osc->envelopes = (struct gkick_envelope**)calloc(1, sizeof(struct gkick_envelope*) *osc->env_number);
        if (osc->envelopes == NULL)
                return GEONKICK_ERROR_MEM_ALLOC;

        for (size_t i = 0; i < osc->env_number; i++) {
                struct gkick_envelope *env = gkick_envelope_create();
                if (env == NULL) {
                        return GEONKICK_ERROR;
                } else {
                        /* Add two default points. */
                        if (i == GKICK_OSC_PITCH_SHIFT_ENVELOPE) {
                                gkick_envelope_add_point(env, 0.0f, 0.5f);
                                gkick_envelope_add_point(env, 1.0f, 0.5f);
                        } else {
                                gkick_envelope_add_point(env, 0.0f, 1.0f);
                                gkick_envelope_add_point(env, 1.0f, 1.0f);
                        }
                        osc->envelopes[i] = env;
                }
        }

        return GEONKICK_OK;
}

struct gkick_envelope*
gkick_osc_get_envelope(struct gkick_oscillator *osc,
                       size_t env_index)
{
        if (osc == NULL)
                return NULL;

        switch (env_index) {
        case GEONKICK_FILTER_CUTOFF_ENVELOPE:
                return osc->filter->cutoff_env;
        case GEONKICK_AMPLITUDE_ENVELOPE:
                return osc->envelopes[GKICK_OSC_AMPLITUDE_ENVELOPE];
        case GEONKICK_FREQUENCY_ENVELOPE:
                return osc->envelopes[GKICK_OSC_FREQUENCY_ENVELOPE];
        case GEONKICK_PITCH_SHIFT_ENVELOPE:
                return osc->envelopes[GKICK_OSC_PITCH_SHIFT_ENVELOPE];
        default:
                return NULL;
        }
}

void gkick_osc_increment_phase(struct gkick_oscillator *osc,
                               gkick_real t,
			       gkick_real kick_len)
{
	struct gkick_envelope *frequency_envelope = osc->envelopes[GKICK_OSC_FREQUENCY_ENVELOPE];
        gkick_real env_val = gkick_envelope_get_value(frequency_envelope, t / kick_len);
	enum gkick_envelope_apply_type apply_type = gkick_envelope_get_apply_type(frequency_envelope);
	gkick_real f;
	if (apply_type == GEONKICK_ENVELOPE_APPLY_LOGARITHMIC)
		f = pow(10, (log10(osc->frequency) - GKICK_LOG20) * env_val + GKICK_LOG20);
	else
		f = osc->frequency * env_val;
        f += f * osc->fm_input;
        osc->phase += (GKICK_2PI * f) / (osc->sample_rate);
        if (osc->phase > GKICK_2PI)
	          osc->phase -= GKICK_2PI;
}

gkick_real gkick_osc_value(struct gkick_oscillator *osc,
		       gkick_real t,
		       gkick_real kick_len)
{
        gkick_real amp;
        gkick_real v;
        gkick_real env_x;
#ifdef GEONKICK_OS_WINDOWS
        // Use rand & srand since the calls are only from a single thread.
        if (t == 0.0)
                srand(osc->seed);
#endif // GEONKICK_OS_WINDOWS

        // Calculate the x coordinate between 0 and 1.0 for the envelope.
        env_x = t / kick_len;
        amp = osc->amplitude * gkick_envelope_get_value(osc->envelopes[GKICK_OSC_AMPLITUDE_ENVELOPE], env_x);
        v = 0.0f;
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
                v = amp * gkick_osc_func_noise_white(&osc->seedp);
                break;
        case GEONKICK_OSC_FUNC_NOISE_PINK:
                v = amp * gkick_osc_func_noise_pink();
                break;
        case GEONKICK_OSC_FUNC_NOISE_BROWNIAN:
                v = amp * gkick_osc_func_noise_brownian(&(osc)->brownian, &osc->seedp);
                break;
        case GEONKICK_OSC_FUNC_SAMPLE:
                if (osc->sample != NULL
                    && (t > (0.25f * osc->initial_phase / M_PI) * kick_len)) {
                        struct gkick_envelope *env = osc->envelopes[GKICK_OSC_PITCH_SHIFT_ENVELOPE];
                        float env_val = gkick_envelope_get_value(env, env_x);
                        float pitch_shift = 2.0f * (env_val - 0.5f) * osc->pitch_shift;
                        v = amp * gkick_osc_func_sample(osc->sample, pitch_shift);
                }
                break;
        default:
                v = amp * gkick_osc_func_sine(osc->phase);
        };

        if (osc->filter_enabled)
                gkick_filter_val(osc->filter, v, &v, env_x);

        return v;
}

gkick_real
gkick_osc_func_sine(gkick_real phase)
{
        return sin(phase);
}

gkick_real
gkick_osc_func_square(gkick_real phase)
{
        if (phase < M_PI)
                return -1.0f;
        else
                return 1.0f;
}

gkick_real
gkick_osc_func_triangle(gkick_real phase)
{
	if (phase < M_PI)
		return -1.0f + M_2_PI * phase;
	else
		return 3.0f - M_2_PI * phase;
}

gkick_real
gkick_osc_func_sawtooth(gkick_real phase)
{
        if (phase < M_PI)
                return M_1_PI * phase;
        else
                return M_1_PI * phase - 2.0f;
}

gkick_real gkick_osc_func_noise_white(unsigned int *seed)
{
        return 2.0f * ((gkick_real)(geonkick_rand(seed) % RAND_MAX)) / (gkick_real)RAND_MAX - 1.0f;
}

gkick_real gkick_osc_func_noise_pink(void)
{
        return 0.0f;
}

gkick_real
gkick_osc_func_noise_brownian(gkick_real *previous,
                              unsigned int *seed)
{
        gkick_real sign = 1.0f;
        gkick_real walk;
        if (geonkick_rand(seed) % 2)
                sign = -1.0f;

        walk = sign * 0.1f * (((gkick_real)(geonkick_rand(seed) % RAND_MAX)) / (gkick_real)RAND_MAX);
        if (*previous + walk > 1.0f || *previous + walk < -1.0f)
                *previous -= walk;
        else
                *previous += walk;
        return *previous;
}

gkick_real
gkick_osc_func_sample(struct gkick_buffer *sample,
                      float semitones)
{
        float factor = exp2f(semitones / 12.0f);
        return gkick_buffer_stretch_get_next(sample, factor);
}

void
gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      gkick_real **buff,
			      size_t *npoints)
{
        if (buff != NULL)
                *buff = NULL;
        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
        if (env != NULL)
                gkick_envelope_get_points(env, buff, npoints);
}

void
gkick_osc_set_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      const gkick_real *buff,
			      size_t npoints)
{
        if (buff == NULL)
                return;

        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
        if (env != NULL)
                gkick_envelope_set_points(env, buff, npoints);
}

int
gkick_osc_enabled(struct gkick_oscillator *osc)
{
        return osc != NULL && (osc->state == GEONKICK_OSC_STATE_ENABLED);
}
