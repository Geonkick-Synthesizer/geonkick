/**
 * File name: gkick_synthesizer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "synthesizer.h"
#include "oscillator.h"

enum geonkick_error
gkick_synth_new(struct gkick_synth **synth)
{
        size_t size;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *synth = (struct gkick_synth*)malloc(sizeof(struct gkick_synth));
	if (*synth == NULL) {
                gkick_log_error("can't allocate memory");
		return GEONKICK_ERROR_MEM_ALLOC;
	}
	memset(*synth, 0, sizeof(struct gkick_synth));
        (*synth)->length = 0.3;
	(*synth)->oscillators_number = 3;
        (*synth)->buffer_update = 1;
        (*synth)->amplitude = 1.0;
        (*synth)->synthesis_on = 1;

        if (gkick_filter_new(&(*synth)->filter) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_synth_free(synth);
		return GEONKICK_ERROR;
        }
        (*synth)->filter_enabled = 0;

        if (gkick_filter_new(&(*synth)->filter) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_synth_free(synth);
		return GEONKICK_ERROR;
        }

        (*synth)->envelope = gkick_envelope_create();
        if ((*synth)->envelope == NULL) {
                gkick_log_error("can't create envelope");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        } else {
                /* Add two default points. */
                gkick_envelope_add_point((*synth)->envelope, 0.0, 1.0);
                gkick_envelope_add_point((*synth)->envelope, 1.0, 1.0);
        }

        size = sizeof(gkick_real) * GEONKICK_MAX_KICK_BUFFER_SIZE;
        (*synth)->buffer = (gkick_real*)malloc(size);
        if ((*synth)->buffer == NULL) {
                gkick_log_error("can't allocate memory");
                gkick_synth_free(synth);
		return GEONKICK_ERROR_MEM_ALLOC;
        }
        (*synth)->buffer_size = (size_t)((*synth)->length * GEONKICK_SAMPLE_RATE);

        if (pthread_cond_init(&(*synth)->condition_var, NULL) != 0) {
                gkick_log_error("can't init condition variable");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        if (pthread_mutex_init(&(*synth)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
	}

        if (gkick_synth_create_oscillators(*synth) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;
}

void gkick_synth_free(struct gkick_synth **synth)
{
        size_t i;

        if (synth != NULL && *synth != NULL) {
                gkick_synth_stop(*synth);
                if ((*synth)->oscillators != NULL) {
                        for (i = 0; i < (*synth)->oscillators_number; i++) {
                                gkick_osc_free(&((*synth)->oscillators[i]));
                        }
                        free((*synth)->oscillators);
                        (*synth)->oscillators = NULL;

                        if ((*synth)->buffer != NULL) {
                                free((*synth)->buffer);
                                (*synth)->buffer = NULL;
                        }

                        if ((*synth)->filter) {
                                gkick_filter_free(&(*synth)->filter);
                        }

                        if ((*synth)->envelope) {
                                gkick_envelope_destroy((*synth)->envelope);
                                (*synth)->envelope = NULL;
                        }
                }

                pthread_cond_destroy(&(*synth)->condition_var);
                pthread_mutex_destroy(&(*synth)->lock);
                free(*synth);
                *synth = NULL;
        }
}

void gkick_synth_lock(struct gkick_synth *synth)
{
        pthread_mutex_lock(&synth->lock);
}

void gkick_synth_unlock(struct gkick_synth *synth)
{
        pthread_mutex_unlock(&synth->lock);
}

enum geonkick_error
gkick_synth_create_oscillators(struct gkick_synth *synth)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        size_t size;
        size_t i;
        struct gkick_oscillator *osc;

        size = sizeof(struct gkick_oscillator*) * synth->oscillators_number;
        synth->oscillators = (struct gkick_oscillator**)malloc(size);

        if (synth->oscillators == NULL) {
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        memset(synth->oscillators, 0, size);

        for (i = 0; i < synth->oscillators_number; i++) {
                osc = gkick_osc_create();
                if (osc == NULL) {
                        return GEONKICK_ERROR;
                }
                synth->oscillators[i] = osc;
        }

        return GEONKICK_OK;
}

struct gkick_oscillator*
gkick_synth_get_oscillator(struct gkick_synth *synth, size_t index)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        if (index >= 0 && index < synth->oscillators_number) {
                return synth->oscillators[index];
        } else {
                gkick_log_error("index = %d out of range", index);
        }

        return NULL;
}

enum geonkick_error
gkick_synth_get_oscillators_number(struct gkick_synth *synth, size_t *number)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        *number = synth->oscillators_number;
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_enable_oscillator(struct gkick_synth *synth, size_t index, int enable)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
	struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        if (enable) {
                gkick_osc_set_state(osc, GEONKICK_OSC_STATE_ENABLED);
        } else {
                gkick_osc_set_state(osc, GEONKICK_OSC_STATE_DISABLED);
        }

        wakeup_thread();
	gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

void gkick_synth_wakeup_thread(void)
{
        if (synth->synthesis_on) {
                synth->buffer_update = 1;
                pthread_cond_signal(&synth->condition_var);
        }
}

enum geonkick_error
gkick_synth_osc_is_enabled(struct gkick_synth *synth, size_t index, int *enabled)
{
        struct gkick_oscillator* osc;

        if (synth == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
	osc = gkick_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        *enabled = gkick_osc_enabled(osc);
	gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

struct gkick_envelope*
gkick_synth_osc_get_env(struct gkick_synth *synth,
                        size_t osc_index,
                        size_t env_index)
{
        struct gkick_oscillator *osc;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 gkick_log_error("can't get oscillator %d", osc_index);
                 return NULL;
        }

        return gkick_osc_get_envelope(osc, env_index);
}

enum geonkick_error
gkick_synth_osc_envelope_points(struct gkick_synth *synth,
                                int osc_index,
                                int env_index,
                                gkick_real **buf,
                                size_t *npoints)
{
        if (synth == NULL || buf == NULL || npoints == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *npoints = 0;
        *buf = NULL;

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                gkick_synth_unlock(synth);
                return GEONKICK_ERROR;
        }
        gkick_osc_get_envelope_points(osc, env_index, buf, npoints);
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_osc_env_add_point(struct gkick_synth *synth,
                              int osc_index,
                              int env_index,
                              gkick_real x,
                              gkick_real y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 gkick_log_error("can't get oscillator %d", osc_index);
                 gkick_synth_unlock(synth);
                 return GEONKICK_ERROR;
        }

        env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        if (gkick_envelope_add_point(env, x, y) == NULL) {
                gkick_log_error("can't add envelope point");
                gkick_synth_unlock(synth);
                return GEONKICK_ERROR;
        }

        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_osc_env_remove_point(struct gkick_synth *synth,
                                 size_t osc_index,
                                 size_t env_index,
                                 size_t index)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 gkick_log_error("can't get oscillator %d", osc_index);
                 gkick_synth_unlock(synth);
                 return GEONKICK_ERROR;
        }

        env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        gkick_envelope_remove_point(env, index);
        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_osc_env_update_point(struct gkick_synth *synth,
                                 int osc_index,
                                 int env_index,
                                 int index,
                                 gkick_real x,
                                 gkick_real y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);

        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 gkick_log_error("can't get oscillator %d", osc_index);
                 gkick_synth_unlock(synth);
                 return GEONKICK_ERROR;
        }

        env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        gkick_envelope_update_point(env, index, x, y);
        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_set_osc_function(struct gkick_synth *synth,
                             size_t osc_index,
                             enum geonkick_osc_func_type type)
{
        struct gkick_oscillator *osc;

        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscilaltor");
                gkick_synth_unlock(synth);
                return GEONKICK_ERROR;
        } else {
                osc->func = type;
        }

        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_function(struct gkick_synth *synth,
                             size_t osc_index,
                             enum geonkick_osc_func_type *type)
{
        struct gkick_oscillator *osc;

        if (synth == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscilaltor");
                gkick_synth_unlock(synth);
                return GEONKICK_ERROR;
        } else {
                *type = osc->func;
        }

        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_length(struct gkick_synth *synth, gkick_real *len)
{
        if (synth == NULL || len == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        *len = synth->length;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_set_length(struct gkick_synth *synth, gkick_real len)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        synth->length = len;
        synth->buffer_size = synth->length * GEONKICK_SAMPLE_RATE;
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_set_amplitude(struct gkick_synth *synth, gkick_real amplitude)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        synth->amplitude = amplitude;
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_get_amplitude(struct gkick_synth *synth, gkick_real *amplitude)
{
        if (synth == NULL || amplitude == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        *amplitude = synth->amplitude;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_kick_filter_enable(struct gkick_synth *synth, int enable)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        synth->filter_enabled = enable;
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_set_filter_frequency(struct gkick_synth *synth, gkick_real frequency)
{
        enum geonkick_error res;
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        res = gkick_filter_set_cutoff_freq(synth->filter, frequency);
        if (synth->filter_enabled) {
                gkick_synth_wakeup_thread();
        }
        gkick_synth_unlock(synth);
        return res;
}

enum geonkick_error
gkick_synth_kick_get_filter_frequency(struct gkick_synth *synth, gkick_real *frequency)
{
        if (synth == NULL || frequency == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_cutoff_freq(synth->filter, frequency);
}

enum geonkick_error
gkick_synth_kick_set_filter_factor(struct gkick_synth *synth, gkick_real factor)
{
        enum geonkick_error res;
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        res = gkick_filter_set_factor(synth->filter, factor);
        if (synth->filter_enabled) {
                gkick_synth_wakeup_thread();
        }
        gkick_synth_unlock(synth);
        return res;
}

enum geonkick_error
gkick_synth_kick_get_filter_factor(struct gkick_synth *synth, gkick_real *factor)
{
        if (synth == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_factor(synth->filter, factor);
}

enum geonkick_error
gkick_synth_set_kick_filter_type(struct gkick_synth *synth, enum gkick_filter_type type)
{
        enum geonkick_error res;
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        res = gkick_filter_set_type(synth->filter, type);
        if (synth->filter_enabled) {
                gkick_synth_wakeup_thread();
        }
        gkick_synth_unlock(synth);
        return res;
}

enum geonkick_error
gkick_synth_get_kick_filter_type(struct gkick_synth *synth, enum gkick_filter_type *type)
{
        if (synth == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_type(synth->filter, type);
}

enum geonkick_error
gkick_synth_kick_envelope_get_points(struct gkick_synth *synth,
                                     gkick_real **buf,
                                     size_t *npoints)
{
        if (synth == NULL || buf == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *npoints = 0;
        *buf = NULL;

        gkick_synth_lock(synth);
        gkick_envelope_get_points(synth->envelope, buf, npoints);
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_add_env_point(struct gkick_synth *synth, gkick_real x, gkick_real y)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        gkick_envelope_add_point(synth->envelope, x, y);
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_remove_env_point(struct gkick_synth *synth, size_t index)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        gkick_envelope_remove_point(synth->envelope, index);
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_update_env_point(struct gkick_synth *synth,
                                  size_t index,
                                  gkick_real x,
                                  gkick_real y)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        gkick_envelope_update_point(synth->envelope, index, x, y);
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_set_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             gkick_real v)
{
        struct gkick_oscillator* osc;

	if (synth == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
	osc->frequency = v;
        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             gkick_real *v)
{
        struct gkick_oscillator* osc;

	if (synth == NULL || v == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
	*v = osc->frequency;
	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_set_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real v)
{
        struct gkick_oscillator* osc;

	if (synth == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
	osc->amplitude = v;

        if (osc->state == GEONKICK_OSC_STATE_ENABLED) {
                gkick_synth_wakeup_thread();
        }

	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real *v)
{
        struct gkick_oscillator* osc;

	if (synth == NULL || v == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
	*v = osc->amplitude;
	gkick_synth_unlock(synth);

	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_buffer_size(struct gkick_synth *synth,
                            size_t *size)
{
        if (synth == NULL || size == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        *size = synth->buffer_size;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_buffer(struct gkick_synth *synth,
                       gkick_real *buffer,
                       size_t size)
{
	if (synth == NULL || buffer == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);

        if (size >= synth->buffer_size) {
                memcpy(buffer, synth->buffer, synth->buffer_size * sizeof(gkick_real));
        } else {
                memcpy(buffer, synth->buffer, size * sizeof(gkick_real));
        }

        gkick_synth_unlock(synth);

        return GEONKICK_ERROR;
}

enum geonkick_error
gkick_synth_set_buffer_callback(struct gkick_synth *synth, void (*callback) (void*), void *args)
{
	if (synth == NULL || callback == NULL || args == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        synth->buffer_callback = callback;
        synth->callback_args = args;
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

void gkick_synth_set_output(struct gkick_synth *synth, struct gkick_buffer *buffer)
{
        if (synth == NULL || buffer == NULL) {
                gkick_log_error("wrong arugment");
                return;
        }

        synth->output = buffer;
}

enum geonkick_error
gkick_synth_start(struct gkick_synth *synth)
{
        if (synth == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        if (pthread_create(&synth->thread, NULL, gkick_synth_run, synth) != 0) {
                gkick_log_error("can't create synthesizer thread");
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_stop(struct gkick_synth *synth)
{
        if (synth == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        gkick_synth_set_started(synth, 0);
        gkick_synth_lock(synth);
        pthread_cond_signal(&synth->condition_var);
        gkick_synth_unlock(synth);

        if (pthread_join(synth->thread, NULL) ) {
                gkick_log_error("error joining synth thread");
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;
}

void gkick_synth_set_started(struct gkick_synth *synth, int started)
{
        gkick_synth_lock(synth);
        synth->is_running = started;
        gkick_synth_unlock(synth);
}

int gkick_synth_is_running(struct gkick_synth *synth)
{
        int started;

        gkick_synth_lock(synth);
        started = synth->is_running;
        gkick_synth_unlock(synth);

        return started;
}

void *gkick_synth_run(void *arg)
{
        size_t size;
        gkick_real len;
        gkick_real dt;
        struct gkick_synth *synth;

        synth = (struct gkick_synth*)arg;
        if (synth == NULL) {
                gkick_log_error("can't run synth thread routine");
                return NULL;
        }
        gkick_synth_set_started(synth, 1);
        gkick_log_info("synth thread routine started");

        while (1) {
                if (!gkick_synth_is_running(synth)) {
                        break;
                } else if (!gkick_synth_is_update_buffer(synth)) {
                        gkick_synth_lock(synth);
                        pthread_cond_wait(&synth->condition_var, &synth->lock);
                        synth->buffer_update = 0;
                        gkick_synth_reset_oscillators(synth);
                        gkick_synth_unlock(synth);
                        if (!gkick_synth_is_running(synth)) {
                                break;
                        }
                }

                /**
                 * Lock happens for every cycle in order not to block
                 * for a long time the access to synth parameters.
                 * The synth parmaters is supposed to change even
                 * during the synthesis of the kick.
                 */
                size_t i = 0;
                while (1) {
                        gkick_synth_lock(synth);
                        if (i >= synth->buffer_size) {
                                gkick_synth_unlock(synth);
                                break;
                        } else {
                                size = synth->buffer_size;
                                len  = synth->length;
                                dt   =  len / size;
                                synth->buffer[i] = gkick_synth_get_value(synth, (gkick_real)(i * dt));
                                gkick_synth_unlock(synth);
                                i++;
                        }
                }

                gkick_synth_lock(synth);
                if (!gkick_buffer_set_data(synth->output, synth->buffer, synth->buffer_size)) {
                        gkick_log_warning("can't copy buffer to audio");
                }
                synth->buffer_update = 0;
                if (synth->buffer_callback != NULL || synth->callback_args != NULL) {
                        synth->buffer_callback(synth->callback_args);
                }
                gkick_synth_unlock(synth);
        }

        return NULL;
}

gkick_real gkick_synth_get_value(struct gkick_synth *synth, gkick_real t)
{
        gkick_real val;
        size_t i;

        val = 0.0;
        for (i = 0; i < synth->oscillators_number; i++) {
                if (gkick_osc_enabled(synth->oscillators[i])) {
                        val += gkick_osc_value(synth->oscillators[i],
                                              t, synth->length);
                }
                gkick_osc_increment_phase(synth->oscillators[i], t,
                                          synth->length);
        }

        val *= synth->amplitude * gkick_envelope_get_value(synth->envelope, t / synth->length);
        if (synth->filter_enabled) {
                gkick_filter_val(synth->filter, val, &val);
        }

        return val;
}

void gkick_synth_reset_oscillators(struct gkick_synth *synth)
{
        int i = 0;

        if (synth == NULL) {
                return;
        }

        for (i = 0; i < synth->oscillators_number; i++) {
                (synth->oscillators[i])->phase = 0.0;
                gkick_filter_init(synth->oscillators[i]->filter);
        }
}

int gkick_synth_is_update_buffer(struct gkick_synth *synth)
{
        int update;

        if (synth == NULL) {
                return 0;
        }

        gkick_synth_lock(synth);
        update = synth->buffer_update;
        gkick_synth_unlock(synth);

        return update;
}

enum geonkick_error
gkick_synth_set_osc_filter_type(struct gkick_synth *synth,
                             size_t osc_index,
                             enum gkick_filter_type type)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        res = gkick_filter_set_type(osc->filter, type);
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_get_osc_filter_type(struct gkick_synth *synth,
                                size_t osc_index,
                                enum gkick_filter_type *type)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL || type == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        res = gkick_filter_get_type(osc->filter, type);
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_set_osc_filter_cutoff(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real cutoff)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        res = gkick_filter_set_cutoff_freq(osc->filter, cutoff);
        gkick_synth_wakeup_thread();
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_get_osc_filter_cutoff(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real *cutoff)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL || cutoff == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        res = gkick_filter_get_cutoff_freq(osc->filter, cutoff);
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_set_osc_filter_factor(struct gkick_synth *synth,
                               size_t osc_index,
                               gkick_real factor)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        res = gkick_filter_set_factor(osc->filter, factor);
        synth->buffer_update = 1;
        pthread_cond_signal(&synth->condition_var);
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_get_osc_filter_factor(struct gkick_synth *synth,
                               size_t osc_index,
                               gkick_real *factor)
{
        struct gkick_oscillator *osc;
        enum geonkick_error res;

        if (synth == NULL || factor == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        res =  gkick_filter_get_factor(osc->filter, factor);
        gkick_synth_unlock(synth);

        return res;
}


enum geonkick_error
gkick_synth_osc_enable_filter(struct gkick_synth *synth,
                              size_t osc_index,
                              int enable)
{
        struct gkick_oscillator *osc;

        if (synth == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        osc->filter_enabled = enable;
        synth->buffer_update = 1;
        pthread_cond_signal(&synth->condition_var);
        gkick_synth_unlock(synth);

        return GEONKICK_ERROR;
}

enum geonkick_error
gkick_synth_osc_is_enabled_filter(struct gkick_synth *synth,
                                  size_t osc_index,
                                  int *enabled)
{
        struct gkick_oscillator *osc;

        if (synth == NULL || enabled == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        gkick_log_debug("enabled: %d", enabled);
        *enabled = osc->filter_enabled;
        synth->buffer_update = 1;
        pthread_cond_signal(&synth->condition_var);
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

void gkick_synth_enable_synthesis(struct gkick_synth *synth, int enable)
{
        if (synth ==)
}
