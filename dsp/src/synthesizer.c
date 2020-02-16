/**
 * File name: synthesizer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://geontime.com>
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
	(*synth)->oscillators_number = GKICK_OSC_GROUPS_NUMBER * GKICK_OSC_GROUP_SIZE;
        (*synth)->buffer_update = 0;
        (*synth)->amplitude = 1.0;
        (*synth)->buffer_size = (size_t)((*synth)->length * GEONKICK_SAMPLE_RATE);
        (*synth)->buffer_update = false;
        (*synth)->is_active = true;
        for (size_t i = 0; i < GKICK_OSC_GROUPS_NUMBER; i++)
                (*synth)->osc_groups_amplitude[i] = 1.0;

        if (gkick_filter_new(&(*synth)->filter) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_synth_free(synth);
		return GEONKICK_ERROR;
        }
        (*synth)->filter_enabled = 0;

        if (gkick_compressor_new(&(*synth)->compressor) != GEONKICK_OK) {
                gkick_log_error("can't create compressor");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        if (gkick_distortion_new(&(*synth)->distortion) != GEONKICK_OK) {
                gkick_log_error("can't create distortion");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

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

        /* Create synthesizer kick buffer. */
        struct gkick_buffer *buff;
        gkick_buffer_new(&buff, GEONKICK_MAX_KICK_BUFFER_SIZE);
        if (buff == NULL) {
                gkick_log_error("can't create synthesizer kick buffer");
                gkick_synth_free(synth);
        }
        gkick_buffer_set_size(buff, (*synth)->buffer_size);
        (*synth)->buffer = (char*)buff;

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

                        if ((*synth)->filter)
                                gkick_filter_free(&(*synth)->filter);

                        if ((*synth)->compressor)
                                gkick_compressor_free(&(*synth)->compressor);

                        if ((*synth)->distortion)
                                gkick_distortion_free(&(*synth)->distortion);

                        if ((*synth)->envelope) {
                                gkick_envelope_destroy((*synth)->envelope);
                                (*synth)->envelope = NULL;
                        }
                }

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

        if (synth->oscillators == NULL)
                return GEONKICK_ERROR_MEM_ALLOC;
        memset(synth->oscillators, 0, size);

        for (i = 0; i < synth->oscillators_number; i++) {
                osc = gkick_osc_create();
                if (osc == NULL)
                        return GEONKICK_ERROR;
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

        if (enable)
                gkick_osc_set_state(osc, GEONKICK_OSC_STATE_ENABLED);
        else
                gkick_osc_set_state(osc, GEONKICK_OSC_STATE_DISABLED);

        if (synth->osc_groups[index / GKICK_OSC_GROUP_SIZE])
                synth->buffer_update = true;

	gkick_synth_unlock(synth);

        return GEONKICK_OK;
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

enum geonkick_error
gkick_synth_osc_set_fm(struct gkick_synth *synth, size_t index, bool is_fm)
{
        gkick_synth_lock(synth);
	struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        osc->is_fm = is_fm;
        if (osc->state == GEONKICK_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_osc_is_fm(struct gkick_synth *synth, size_t index, bool *is_fm)
{
        gkick_synth_lock(synth);
	struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        *is_fm = osc->is_fm;
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
gkick_synth_osc_envelope_set_points(struct gkick_synth *synth,
                                    int osc_index,
                                    int env_index,
                                    const gkick_real *buf,
                                    size_t npoints)
{
        if (synth == NULL || buf == NULL || npoints == 0) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc = gkick_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                gkick_synth_unlock(synth);
                return GEONKICK_ERROR;
        }
        gkick_osc_set_envelope_points(osc, env_index, buf, npoints);
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

        if (env_index == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                env = osc->filter->cutoff_env;
        else
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

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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

        if (env_index == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                env = osc->filter->cutoff_env;
        else
                env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        gkick_envelope_remove_point(env, index);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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

        if (env_index == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                env = osc->filter->cutoff_env;
        else
                env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        gkick_envelope_update_point(env, index, x, y);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
                    && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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
gkick_synth_set_osc_phase(struct gkick_synth *synth,
                          size_t osc_index,
                          gkick_real phase)
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
                osc->initial_phase = phase;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_phase(struct gkick_synth *synth,
                          size_t osc_index,
                          gkick_real *phase)
{
        struct gkick_oscillator *osc;
        if (synth == NULL || phase == NULL) {
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
                *phase = osc->initial_phase;
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
        synth->buffer_update = true;
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
        synth->buffer_update = true;
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
        synth->buffer_update = true;
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_kick_filter_is_enabled(struct gkick_synth *synth, int *enabled)
{
        if (synth == NULL || enabled == NULL) {
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        *enabled = synth->filter_enabled;
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
        if (synth->filter_enabled)
                synth->buffer_update = true;
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
        if (synth->filter_enabled)
                synth->buffer_update = true;
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
        if (synth->filter_enabled)
                synth->buffer_update = true;
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
                                     enum geonkick_envelope_type env_type,
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
        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE)
                gkick_envelope_get_points(synth->envelope, buf, npoints);
        else if (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                gkick_envelope_get_points(synth->filter->cutoff_env, buf, npoints);
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_envelope_set_points(struct gkick_synth *synth,
                                     enum geonkick_envelope_type env_type,
                                     const gkick_real *buf,
                                     size_t npoints)
{
        if (synth == NULL || buf == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE)
                gkick_envelope_set_points(synth->envelope, buf, npoints);
        else if (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                gkick_envelope_set_points(synth->filter->cutoff_env, buf, npoints);
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_add_env_point(struct gkick_synth *synth,
                               enum geonkick_envelope_type env_type,
                               gkick_real x,
                               gkick_real y)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE)
                gkick_envelope_add_point(synth->envelope, x, y);
        else if (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                gkick_envelope_add_point(synth->filter->cutoff_env, x, y);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE && synth->filter_enabled))
                synth->buffer_update = true;

        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_remove_env_point(struct gkick_synth *synth,
                                  enum geonkick_envelope_type env_type,
                                  size_t index)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE)
                gkick_envelope_remove_point(synth->envelope, index);
        else if (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                gkick_envelope_remove_point(synth->filter->cutoff_env, index);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
                && synth->filter_enabled)) {
                synth->buffer_update = true;
        }
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_update_env_point(struct gkick_synth *synth,
                                  enum geonkick_envelope_type env_type,
                                  size_t index,
                                  gkick_real x,
                                  gkick_real y)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE)
                gkick_envelope_update_point(synth->envelope, index, x, y);
        else
                gkick_envelope_update_point(synth->filter->cutoff_env, index, x, y);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
                && synth->filter_enabled)) {
                synth->buffer_update = true;
	}

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

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
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

        if (size >= synth->buffer_size)
                memcpy(buffer, synth->buffer, synth->buffer_size * sizeof(gkick_real));
        else
                memcpy(buffer, synth->buffer, size * sizeof(gkick_real));

        gkick_synth_unlock(synth);

        return GEONKICK_ERROR;
}

void gkick_synth_set_output(struct gkick_synth *synth, struct gkick_audio_output *output)
{
        if (synth == NULL || output == NULL) {
                gkick_log_error("wrong arugment");
                return;
        }
        synth->output = output;
}

enum geonkick_error
gkick_synth_process(struct gkick_synth *synth,
                    void (*callback)(void*, gkick_real *buff, size_t size),
                    void *args)
{
	if (synth == NULL)
		return GEONKICK_ERROR;

	gkick_synth_lock(synth);
	synth->buffer_update = false;
	gkick_buffer_set_size((struct gkick_buffer*)synth->buffer, synth->buffer_size);
	gkick_real dt = synth->length / synth->buffer_size;
	gkick_synth_reset_oscillators(synth);
	gkick_filter_init(synth->filter);
	gkick_synth_unlock(synth);

	// Synthesize the percussion into the buffer.
	size_t i = 0;
	while (1) {
		gkick_synth_lock(synth);
		if (gkick_buffer_is_end((struct gkick_buffer*)synth->buffer)) {
			gkick_synth_unlock(synth);
			break;
		} else {
			gkick_real val = gkick_synth_get_value(synth, (gkick_real)(i * dt));
			if (isnan(val))
				val = 0;
			else if (val > 1.0)
				val = 1;
			else if (val < -1.0)
				val = -1;
			gkick_buffer_push_back((struct gkick_buffer*)synth->buffer, val);
			i++;
		}
                gkick_synth_unlock(synth);
	}

	gkick_synth_lock(synth);
        if (callback != NULL && args != NULL)
                callback(args, ((struct gkick_buffer*)synth->buffer)->buff, synth->buffer_size);
        // Check if the synthesizer parameters
        // were not updated during synthesis.
        if (!synth->buffer_update) {
                gkick_audio_output_lock(synth->output);
                char* buff = synth->output->updated_buffer;
                synth->output->updated_buffer = synth->buffer;
                synth->buffer = buff;
                gkick_audio_output_unlock(synth->output);
        }
	gkick_synth_unlock(synth);

	return GEONKICK_OK;
}

gkick_real gkick_synth_get_value(struct gkick_synth *synth, gkick_real t)
{
        gkick_real val = 0;
        gkick_real env_x = 0;
        gkick_real fm_val = 0;
        size_t n = synth->oscillators_number;
        for (size_t i = 0; i < n; i++) {
                if (synth->osc_groups[i / GKICK_OSC_GROUP_SIZE] && gkick_osc_enabled(synth->oscillators[i])) {
                        if (synth->oscillators[i]->is_fm && i % 3 == 0 && i + 1 < n) {
                                fm_val = gkick_osc_value(synth->oscillators[i], t, synth->length);
                                synth->oscillators[i + 1]->fm_input = fm_val;
                        } else {
                                gkick_real group_ampl = synth->osc_groups_amplitude[i / GKICK_OSC_GROUP_SIZE];
                                val += group_ampl * gkick_osc_value(synth->oscillators[i],
                                                                    t, synth->length);
                        }
                        gkick_osc_increment_phase(synth->oscillators[i], t, synth->length);
                }
        }

        env_x = t / synth->length;
        val *= synth->amplitude * gkick_envelope_get_value(synth->envelope, env_x);
        if (synth->filter_enabled)
                gkick_filter_val(synth->filter, val, &val, env_x);

        int enabled = 0;
        gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (enabled)
                gkick_distortion_val(synth->distortion, val, &val);

        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                gkick_compressor_val(synth->compressor, val, &val);

        return val;
}

void gkick_synth_reset_oscillators(struct gkick_synth *synth)
{
        if (synth == NULL)
                return;

        for (size_t i = 0; i < synth->oscillators_number; i++) {
                struct gkick_oscillator *osc = synth->oscillators[i];
                osc->phase = osc->initial_phase;
                osc->fm_input = 0;
                gkick_filter_init(osc->filter);
                if (osc->sample != NULL)
                        gkick_buffer_reset(osc->sample);
        }
}

int gkick_synth_is_update_buffer(struct gkick_synth *synth)
{
        return synth->buffer_update;
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
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

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
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

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
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

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
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

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
        *enabled = osc->filter_enabled;
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_compressor_enable(struct gkick_synth *synth, int enable)
{
        synth->buffer_update = true;
        return gkick_compressor_enable(synth->compressor, enable);
}

enum geonkick_error
gkick_synth_compressor_is_enabled(struct gkick_synth *synth, int *enabled)
{
        return gkick_compressor_is_enabled(synth->compressor, enabled);
}

enum geonkick_error
gkick_synth_compressor_set_attack(struct gkick_synth *synth, gkick_real attack)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_attack(synth->compressor, attack);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_attack(struct gkick_synth *synth, gkick_real *attack)
{
        return gkick_compressor_get_attack(synth->compressor, attack);
}

enum geonkick_error
gkick_synth_compressor_set_release(struct gkick_synth *synth, gkick_real release)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_release(synth->compressor, release);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_release(struct gkick_synth *synth, gkick_real *release)
{
        return gkick_compressor_get_release(synth->compressor, release);
}

enum geonkick_error
gkick_synth_compressor_set_threshold(struct gkick_synth *synth, gkick_real threshold)
{
                enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_threshold(synth->compressor, threshold);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_threshold(struct gkick_synth *synth, gkick_real *threshold)
{
        return gkick_compressor_get_threshold(synth->compressor, threshold);
}

enum geonkick_error
gkick_synth_compressor_set_ratio(struct gkick_synth *synth, gkick_real ratio)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_ratio(synth->compressor, ratio);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_ratio(struct gkick_synth *synth, gkick_real *ratio)
{
        return gkick_compressor_get_ratio(synth->compressor, ratio);
}

enum geonkick_error
gkick_synth_compressor_set_knee(struct gkick_synth *synth, gkick_real knee)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_knee(synth->compressor, knee);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_knee(struct gkick_synth *synth, gkick_real *knee)
{
        return gkick_compressor_get_threshold(synth->compressor, knee);
}

enum geonkick_error
gkick_synth_compressor_set_makeup(struct gkick_synth *synth, gkick_real makeup)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_compressor_set_makeup(synth->compressor, makeup);
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_makeup(struct gkick_synth *synth, gkick_real *makeup)
{
        return gkick_compressor_get_makeup(synth->compressor, makeup);
}

enum geonkick_error
gkick_synth_distortion_enable(struct gkick_synth *synth, int enable)
{
        return gkick_distortion_enable(synth->distortion, enable);
}

enum geonkick_error
gkick_synth_distortion_is_enabled(struct gkick_synth *synth, int *enabled)
{
        return gkick_distortion_is_enabled(synth->distortion, enabled);
}

enum geonkick_error
gkick_synth_distortion_set_in_limiter(struct gkick_synth *synth, gkick_real limit)
{
        enum geonkick_error res = gkick_distortion_set_in_limiter(synth->distortion, limit);
	int enabled = false;
        res = gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_distortion_get_in_limiter(struct gkick_synth *synth, gkick_real *limit)
{
	return gkick_distortion_get_in_limiter(synth->distortion, limit);
}

enum geonkick_error
gkick_synth_distortion_set_volume(struct gkick_synth *synth, gkick_real volume)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_distortion_set_volume(synth->distortion, volume);
        gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_distortion_get_volume(struct gkick_synth *synth, gkick_real *volume)
{
        return gkick_distortion_get_volume(synth->distortion, volume);
}

enum geonkick_error
gkick_synth_distortion_set_drive(struct gkick_synth *synth, gkick_real drive)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_distortion_set_drive(synth->distortion, drive);
        gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_distortion_get_drive(struct gkick_synth *synth, gkick_real *drive)
{
        return gkick_distortion_get_drive(synth->distortion, drive);
}

enum geonkick_error
gkick_synth_enable_group(struct gkick_synth *synth, size_t index, bool enable)
{
        gkick_synth_lock(synth);
        synth->osc_groups[index] = enable;
        synth->buffer_update = true;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_group_enabled(struct gkick_synth *synth, size_t index, bool *enabled)
{
        gkick_synth_lock(synth);
        *enabled = synth->osc_groups[index];
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_group_set_amplitude(struct gkick_synth *synth, size_t index, gkick_real amplitude)
{
        gkick_synth_lock(synth);
        synth->osc_groups_amplitude[index] = amplitude;
        synth->buffer_update = true;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_group_get_amplitude(struct gkick_synth *synth,
                                   size_t index,
                                   gkick_real *amplitude)
{
        gkick_synth_lock(synth);
        *amplitude = synth->osc_groups_amplitude[index];
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_set_osc_sample(struct gkick_synth *synth,
                              size_t osc_index,
                              const gkick_real *data,
                              size_t size)
{
        gkick_synth_lock(synth);
        struct gkick_oscillator *osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        if (osc->sample == NULL)
                gkick_buffer_new(&osc->sample, GEONKICK_MAX_KICK_BUFFER_SIZE);
        gkick_buffer_set_data(osc->sample, data, size);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED)
                synth->buffer_update = true;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_get_osc_sample(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real **data,
                              size_t *size)
{
        *data = NULL;
        *size = 0;
        gkick_synth_lock(synth);
        struct gkick_oscillator *osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        if (osc->sample == NULL) {
                gkick_synth_unlock(synth);
                return GEONKICK_OK;
        }

        *size = gkick_buffer_size(osc->sample);
        if (*size > 0) {
                *data = (gkick_real*)malloc(sizeof(gkick_real) * (*size));
                if (*data == NULL) {
                        gkick_log_error("can't allocate memory");
                        *size = 0;
                        gkick_synth_unlock(synth);
                        return GEONKICK_ERROR;
                }
                memcpy(*data, osc->sample->buff, sizeof(gkick_real) * (*size));
        }
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}
