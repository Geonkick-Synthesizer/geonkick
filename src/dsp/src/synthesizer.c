/**
 * File name: synthesizer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
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
gkick_synth_new(struct gkick_synth **synth, int sample_rate)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *synth = (struct gkick_synth*)calloc(1, sizeof(struct gkick_synth));
	if (*synth == NULL) {
                gkick_log_error("can't allocate memory");
		return GEONKICK_ERROR_MEM_ALLOC;
	}
	memset(*synth, 0, sizeof(struct gkick_synth));
	(*synth)->sample_rate = sample_rate;
        (*synth)->length = 0.3f;
	(*synth)->oscillators_number = GKICK_OSC_GROUPS_NUMBER * GKICK_OSC_GROUP_SIZE;
        (*synth)->buffer_update = 0;
        (*synth)->amplitude = 1.0f;
        (*synth)->buffer_update = false;
        (*synth)->is_active = false;
        memset((*synth)->name, '\0', sizeof((*synth)->name));
        for (size_t i = 0; i < GKICK_OSC_GROUPS_NUMBER; i++)
                (*synth)->osc_groups_amplitude[i] = 1.0f;

        if (gkick_filter_new(&(*synth)->filter, (*synth)->sample_rate) != GEONKICK_OK) {
                gkick_log_error("can't create filter");
                gkick_synth_free(synth);
		return GEONKICK_ERROR;
        }
        (*synth)->filter_enabled = 0;

        if (gkick_compressor_new(&(*synth)->compressor, (*synth)->sample_rate) != GEONKICK_OK) {
                gkick_log_error("can't create compressor");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        if (gkick_distortion_new(&(*synth)->distortion, (*synth)->sample_rate) != GEONKICK_OK) {
                gkick_log_error("can't create distortion");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        if (gkick_filter_new(&(*synth)->filter, (*synth)->sample_rate) != GEONKICK_OK) {
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
                gkick_envelope_add_point((*synth)->envelope, 0.0f, 1.0f);
                gkick_envelope_add_point((*synth)->envelope, 1.0f, 1.0f);
        }

        /* Create synthesizer kick buffer. */
        struct gkick_buffer *buff;
        gkick_buffer_new(&buff, (*synth)->sample_rate * GEONKICK_MAX_LENGTH);
        if (buff == NULL) {
                gkick_log_error("can't create synthesizer kick buffer");
                gkick_synth_free(synth);
        }
        gkick_buffer_set_size(buff, (size_t)((*synth)->length * (*synth)->sample_rate));
        (*synth)->buffer = buff;

        if (gkick_synth_create_oscillators(*synth) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                gkick_synth_free(synth);
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;
}

void gkick_synth_free(struct gkick_synth **synth)
{
        if (synth != NULL && *synth != NULL) {
                if ((*synth)->oscillators != NULL) {
                        for (size_t i = 0; i < (*synth)->oscillators_number; i++)
                                gkick_osc_free(&((*synth)->oscillators[i]));
                        free((*synth)->oscillators);
                        (*synth)->oscillators = NULL;

                        if ((*synth)->buffer != NULL)
                                gkick_buffer_free(&(*synth)->buffer);

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
                osc = gkick_osc_create(synth->sample_rate);
                if (osc == NULL)
                        return GEONKICK_ERROR;
                synth->oscillators[i] = osc;
        }

        return GEONKICK_OK;
}

struct gkick_oscillator*
gkick_synth_get_oscillator(struct gkick_synth *synth,
                           size_t index)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        if (index < synth->oscillators_number)
                return synth->oscillators[index];

        return NULL;
}

enum geonkick_error
gkick_synth_get_oscillators_number(struct gkick_synth *synth,
                                   size_t *number)
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
gkick_synth_enable_oscillator(struct gkick_synth *synth,
                              size_t index,
                              int enable)
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
gkick_synth_osc_is_enabled(struct gkick_synth *synth,
                           size_t index,
                           int *enabled)
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
gkick_synth_osc_set_fm(struct gkick_synth *synth,
                       size_t index,
                       bool is_fm)
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
gkick_synth_osc_is_fm(struct gkick_synth *synth,
                      size_t index,
                      bool *is_fm)
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
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        struct gkick_oscillator *osc;
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
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
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
        if (synth == NULL) {
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
        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
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
        if (synth == NULL) {
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
        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
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
        if (synth == NULL) {
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

        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
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
synth_osc_env_set_apply_type(struct gkick_synth *synth, 
			     size_t osc_index,
			     size_t env_index,
			     enum gkick_envelope_apply_type apply_type)
{
	if (synth == NULL) {
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

        struct gkick_envelope *env = gkick_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }
        gkick_envelope_set_apply_type(env, apply_type);
	osc = gkick_synth_get_oscillator(synth, osc_index);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
synth_osc_env_get_apply_type(struct gkick_synth *synth,
			     size_t osc_index,
			     size_t env_index,
			     enum gkick_envelope_apply_type *apply_type)
{
	if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
	struct gkick_envelope *env = gkick_synth_osc_get_env(synth, osc_index, env_index);
        if (env == NULL) {
                gkick_synth_unlock(synth);
                gkick_log_error("can't get envelope %d", env_index);
                return GEONKICK_ERROR;
        }
	*apply_type = gkick_envelope_get_apply_type(env);
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
gkick_synth_set_osc_seed(struct gkick_synth *synth,
                         size_t osc_index,
                         unsigned int seed)
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
                osc->seed = seed;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_seed(struct gkick_synth *synth,
                         size_t osc_index,
                         unsigned *seed)
{
        struct gkick_oscillator *osc;
        if (synth == NULL || seed == NULL) {
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
                *seed = osc->seed;
        }

        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}


enum geonkick_error
gkick_synth_get_length(struct gkick_synth *synth,
                       gkick_real *len)
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
gkick_synth_set_length(struct gkick_synth *synth,
                       gkick_real len)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        synth->length = len;
        gkick_buffer_set_size(synth->buffer, synth->length * synth->sample_rate);
        synth->buffer_update = true;
        gkick_synth_unlock(synth);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_kick_set_amplitude(struct gkick_synth *synth,
                               gkick_real amplitude)
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
gkick_synth_kick_get_amplitude(struct gkick_synth *synth,
                               gkick_real *amplitude)
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
geonkick_synth_kick_filter_enable(struct gkick_synth *synth,
                                  int enable)
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
geonkick_synth_kick_filter_is_enabled(struct gkick_synth *synth,
                                      int *enabled)
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
gkick_synth_kick_set_filter_frequency(struct gkick_synth *synth,
                                      gkick_real frequency)
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
gkick_synth_kick_get_filter_frequency(struct gkick_synth *synth,
                                      gkick_real *frequency)
{
        if (synth == NULL || frequency == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_cutoff_freq(synth->filter, frequency);
}

enum geonkick_error
gkick_synth_kick_set_filter_factor(struct gkick_synth *synth,
                                   gkick_real factor)
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
gkick_synth_kick_get_filter_factor(struct gkick_synth *synth,
                                   gkick_real *factor)
{
        if (synth == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_factor(synth->filter, factor);
}

enum geonkick_error
gkick_synth_set_kick_filter_type(struct gkick_synth *synth,
                                 enum gkick_filter_type type)
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
gkick_synth_get_kick_filter_type(struct gkick_synth *synth,
                                 enum gkick_filter_type *type)
{
        if (synth == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_filter_get_type(synth->filter, type);
}

struct gkick_envelope*
synth_get_kick_envelope(struct gkick_synth *synth,
			enum geonkick_envelope_type env_type)
{
	switch(env_type) {
	case GEONKICK_AMPLITUDE_ENVELOPE:
                return synth->envelope;
	case GEONKICK_FILTER_CUTOFF_ENVELOPE:
                return synth->filter->cutoff_env;
	case GEONKICK_FILTER_Q_ENVELOPE:
                return synth->filter->q_env;
	case GEONKICK_DISTORTION_DRIVE_ENVELOPE:
		return synth->distortion->drive_env;
	case GEONKICK_DISTORTION_VOLUME_ENVELOPE:
                return synth->distortion->volume_env;
	default:
		return NULL;
	}
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
	struct gkick_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		gkick_envelope_get_points(env, buf, npoints);
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
	struct gkick_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		gkick_envelope_set_points(env, buf, npoints);

	if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || ((env_type == GEONKICK_FILTER_Q_ENVELOPE
		 || env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == GEONKICK_DISTORTION_DRIVE_ENVELOPE
                 || env_type == GEONKICK_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
        }
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
	struct gkick_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		gkick_envelope_add_point(env, x, y);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || ((env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
		 || env_type == GEONKICK_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == GEONKICK_DISTORTION_DRIVE_ENVELOPE
                 || env_type == GEONKICK_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled))
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
	struct gkick_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		gkick_envelope_remove_point(env, index);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || ((env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
		|| env_type == GEONKICK_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == GEONKICK_DISTORTION_DRIVE_ENVELOPE
                 || env_type == GEONKICK_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
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
	struct gkick_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		gkick_envelope_update_point(env, index, x, y);

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || ((env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
		 || env_type == GEONKICK_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == GEONKICK_DISTORTION_DRIVE_ENVELOPE
                 || env_type == GEONKICK_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
	}
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
synth_kick_env_set_apply_type(struct gkick_synth *synth, 
			      enum geonkick_envelope_type env_type,
			      enum gkick_envelope_apply_type apply_type)
{
        gkick_synth_lock(synth);
        switch (env_type) {
	case GEONKICK_FILTER_CUTOFF_ENVELOPE:
		gkick_envelope_set_apply_type(synth->filter->cutoff_env, apply_type);
		break;
	}

        if (env_type == GEONKICK_AMPLITUDE_ENVELOPE
            || (env_type == GEONKICK_FILTER_CUTOFF_ENVELOPE
                && synth->filter_enabled)
	    || ((env_type == GEONKICK_DISTORTION_DRIVE_ENVELOPE
                 || env_type == GEONKICK_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
        }
        gkick_synth_unlock(synth);
        return GEONKICK_OK;	
}

enum geonkick_error
synth_kick_env_get_apply_type(struct gkick_synth *synth,
			      enum geonkick_envelope_type env_type,
			      enum gkick_envelope_apply_type *apply_type)
{
	gkick_synth_lock(synth);
        switch (env_type) {
	case GEONKICK_FILTER_CUTOFF_ENVELOPE:
		*apply_type = gkick_envelope_get_apply_type(synth->filter->cutoff_env);
		break;
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
	if (synth == NULL || v == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, osc_index);
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
gkick_synth_set_osc_pitch_shift(struct gkick_synth *synth,
                                size_t osc_index,
                                gkick_real semitones)
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
	osc->pitch_shift = semitones;
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == GEONKICK_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_get_osc_pitch_shift(struct gkick_synth *synth,
                                size_t osc_index,
                                gkick_real *semitones)
{
	if (synth == NULL || semitones == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
	struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
	*semitones = osc->pitch_shift;
	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_set_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real v)
{
	if (synth == NULL) {
                gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	gkick_synth_lock(synth);
        struct gkick_oscillator* osc = gkick_synth_get_oscillator(synth, osc_index);
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
        *size = gkick_buffer_size(synth->buffer);
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
        size = min(gkick_buffer_size(synth->buffer), size);
        memcpy(buffer, synth->buffer->buff, size * sizeof(gkick_real));
        gkick_synth_unlock(synth);

        return GEONKICK_ERROR;
}

void
gkick_synth_set_output(struct gkick_synth *synth,
                       struct gkick_audio_output *output)
{
        if (synth == NULL || output == NULL) {
                gkick_log_error("wrong argument");
                return;
        }
        synth->output = output;
}

enum geonkick_error
gkick_synth_process(struct gkick_synth *synth)
{
	if (synth == NULL)
		return GEONKICK_ERROR;

	gkick_synth_lock(synth);
	synth->buffer_update = false;
        gkick_buffer_set_size(synth->buffer, synth->length * synth->sample_rate);
	gkick_real dt = synth->length / gkick_buffer_size(synth->buffer);
	gkick_synth_reset_oscillators(synth);
	gkick_filter_init(synth->filter);
	gkick_synth_unlock(synth);

	/* Synthesize the percussion into the synthesizer buffer. */
	size_t i = 0;
        size_t tries = 0;
	while (1) {
                /**
                 * Try lock in order not too block the access
                 * of the synthesizer parameters for too long time.
                 */
                if (pthread_mutex_trylock(&synth->lock) != 0) {
                        geonkick_usleep(50);
                        /**
                         * Check how many tries for locking to avoind infinite loop.
                         * It should be maximum around 30ms.
                         */
                        if (++tries > 600)
                                break;
                        else
                                continue;
                }

		if (gkick_buffer_is_end((struct gkick_buffer*)synth->buffer)) {
			gkick_synth_unlock(synth);
			break;
		} else {
			gkick_real val = gkick_synth_get_value(synth, (gkick_real)(i * dt));
			if (isnan(val))
				val = 0.0f;
			else if (val > 1.0f)
				val = 1.0f;
			else if (val < -1.0f)
				val = -1.0f;
			gkick_buffer_push_back(synth->buffer, val);
			i++;
		}
                gkick_synth_unlock(synth);
	}

	gkick_synth_lock(synth);
        if (synth->buffer_callback != NULL && synth->callback_args != NULL) {
                gkick_log_debug("synth->buffer_callback");
                synth->buffer_callback(synth->callback_args,
                                       synth->buffer->buff,
                                       gkick_buffer_size(synth->buffer),
                                       synth->id);
        }

        /**
         * Don't update the output audio buffer if
         * the synthesizer parameters
         * were updated during the synthesis.
         */
        if (!synth->buffer_update) {
                gkick_audio_output_lock(synth->output);
                struct gkick_buffer *buff = synth->output->updated_buffer;
                synth->output->updated_buffer = synth->buffer;
                synth->buffer = buff;
                gkick_audio_output_unlock(synth->output);
        }
	gkick_synth_unlock(synth);

	return GEONKICK_OK;
}

gkick_real
gkick_synth_get_value(struct gkick_synth *synth,
                      gkick_real t)
{
        gkick_real val = 0.0f;
        gkick_real env_x = 0.0f;
        gkick_real fm_val = 0.0f;
        size_t n = synth->oscillators_number;
        for (size_t i = 0; i < n; i++) {
                if (synth->osc_groups[i / GKICK_OSC_GROUP_SIZE]
                    && gkick_osc_enabled(synth->oscillators[i])) {
                        if (synth->oscillators[i]->is_fm
                            && i % 3 == 0 && i + 1 < n) {
                                fm_val = gkick_osc_value(synth->oscillators[i],
                                                         t,
                                                         synth->length);
                                synth->oscillators[i + 1]->fm_input = fm_val;
                        } else {
                                gkick_real group_ampl
                                        = synth->osc_groups_amplitude[i / GKICK_OSC_GROUP_SIZE];
                                val += group_ampl * gkick_osc_value(synth->oscillators[i],
                                                                    t,
                                                                    synth->length);
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
                gkick_distortion_val(synth->distortion, val, &val, env_x);

        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (enabled)
                gkick_compressor_val(synth->compressor, val, &val);

        return val;
}

void
gkick_synth_reset_oscillators(struct gkick_synth *synth)
{
        if (synth == NULL)
                return;

        for (size_t i = 0; i < synth->oscillators_number; i++) {
                struct gkick_oscillator *osc = synth->oscillators[i];
                osc->phase = osc->initial_phase;
                osc->fm_input = 0.0f;
                osc->seedp = osc->seed;
                gkick_filter_init(osc->filter);
                if (osc->sample != NULL)
                        gkick_buffer_reset(osc->sample);
        }
}

int
gkick_synth_is_update_buffer(struct gkick_synth *synth)
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
        if (synth == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res = gkick_filter_get_type(osc->filter, type);
        gkick_synth_unlock(synth);

        return res;
}

enum geonkick_error
gkick_synth_set_osc_filter_cutoff(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real cutoff)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}
        enum geonkick_error res;
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
        if (synth == NULL || cutoff == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res = gkick_filter_get_cutoff_freq(osc->filter, cutoff);
        gkick_synth_unlock(synth);
        return res;
}

enum geonkick_error
gkick_synth_set_osc_filter_factor(struct gkick_synth *synth,
                               size_t osc_index,
                               gkick_real factor)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
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
        if (synth == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res =  gkick_filter_get_factor(osc->filter, factor);
        gkick_synth_unlock(synth);
        return res;
}


enum geonkick_error
gkick_synth_osc_enable_filter(struct gkick_synth *synth,
                              size_t osc_index,
                              int enable)
{
        if (synth == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
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
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_osc_is_enabled_filter(struct gkick_synth *synth,
                                  size_t osc_index,
                                  int *enabled)
{
        if (synth == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_synth_lock(synth);
        struct gkick_oscillator *osc;
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
gkick_synth_compressor_enable(struct gkick_synth *synth,
                              int enable)
{
        synth->buffer_update = true;
        return gkick_compressor_enable(synth->compressor,
                                       enable);
}

enum geonkick_error
gkick_synth_compressor_is_enabled(struct gkick_synth *synth,
                                  int *enabled)
{
        return gkick_compressor_is_enabled(synth->compressor
                                           , enabled);
}

enum geonkick_error
gkick_synth_compressor_set_attack(struct gkick_synth *synth,
                                  gkick_real attack)
{
        enum geonkick_error res;
        res = gkick_compressor_set_attack(synth->compressor,
                                          attack);
	int enabled  = 0;
        gkick_compressor_is_enabled(synth->compressor,
                                    &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_attack(struct gkick_synth *synth,
                                  gkick_real *attack)
{
        return gkick_compressor_get_attack(synth->compressor,
                                           attack);
}

enum geonkick_error
gkick_synth_compressor_set_release(struct gkick_synth *synth,
                                   gkick_real release)
{
        enum geonkick_error res;
        res = gkick_compressor_set_release(synth->compressor,
                                           release);
	int enabled = 0;
        gkick_compressor_is_enabled(synth->compressor,
                                    &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_release(struct gkick_synth *synth,
                                   gkick_real *release)
{
        return gkick_compressor_get_release(synth->compressor,
                                            release);
}

enum geonkick_error
gkick_synth_compressor_set_threshold(struct gkick_synth *synth,
                                     gkick_real threshold)
{
	enum geonkick_error res;
        res = gkick_compressor_set_threshold(synth->compressor,
                                             threshold);
        int enabled = 0;
        gkick_compressor_is_enabled(synth->compressor,
                                    &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_threshold(struct gkick_synth *synth,
                                     gkick_real *threshold)
{
        return gkick_compressor_get_threshold(synth->compressor,
                                              threshold);
}

enum geonkick_error
gkick_synth_compressor_set_ratio(struct gkick_synth *synth,
                                 gkick_real ratio)
{
        enum geonkick_error res;
        res = gkick_compressor_set_ratio(synth->compressor, ratio);
        int enabled = 0;
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_ratio(struct gkick_synth *synth,
                                 gkick_real *ratio)
{
        return gkick_compressor_get_ratio(synth->compressor,
                                          ratio);
}

enum geonkick_error
gkick_synth_compressor_set_knee(struct gkick_synth *synth,
                                gkick_real knee)
{
        enum geonkick_error res;
        res = gkick_compressor_set_knee(synth->compressor, knee);
        int enabled = false;
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_knee(struct gkick_synth *synth,
                                gkick_real *knee)
{
        return gkick_compressor_get_threshold(synth->compressor, knee);
}

enum geonkick_error
gkick_synth_compressor_set_makeup(struct gkick_synth *synth,
                                  gkick_real makeup)
{
        enum geonkick_error res;
        res = gkick_compressor_set_makeup(synth->compressor, makeup);
        int enabled;
        gkick_compressor_is_enabled(synth->compressor, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_compressor_get_makeup(struct gkick_synth *synth,
                                  gkick_real *makeup)
{
        return gkick_compressor_get_makeup(synth->compressor,
                                           makeup);
}

enum geonkick_error
gkick_synth_distortion_enable(struct gkick_synth *synth,
                              int enable)
{
	synth->buffer_update = true;
        return gkick_distortion_enable(synth->distortion,
                                       enable);
}

enum geonkick_error
gkick_synth_distortion_is_enabled(struct gkick_synth *synth,
                                  int *enabled)
{
        return gkick_distortion_is_enabled(synth->distortion,
                                           enabled);
}

enum geonkick_error
gkick_synth_distortion_set_in_limiter(struct gkick_synth *synth,
                                      gkick_real limit)
{
        gkick_distortion_set_in_limiter(synth->distortion,
                                              limit);
	int enabled = 0;
        gkick_distortion_is_enabled(synth->distortion,
                                          &enabled);
        if (enabled)
                synth->buffer_update = true;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_distortion_get_in_limiter(struct gkick_synth *synth,
                                      gkick_real *limit)
{
	return gkick_distortion_get_in_limiter(synth->distortion,
                                               limit);
}

enum geonkick_error
gkick_synth_distortion_set_volume(struct gkick_synth *synth, gkick_real volume)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_distortion_set_volume(synth->distortion, volume);
        gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_distortion_get_volume(struct gkick_synth *synth,
                                  gkick_real *volume)
{
        return gkick_distortion_get_volume(synth->distortion,
                                           volume);
}

enum geonkick_error
gkick_synth_distortion_set_drive(struct gkick_synth *synth,
                                 gkick_real drive)
{
        enum geonkick_error res;
        int enabled;
        res = gkick_distortion_set_drive(synth->distortion, drive);
        gkick_distortion_is_enabled(synth->distortion, &enabled);
        if (res == GEONKICK_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum geonkick_error
gkick_synth_distortion_get_drive(struct gkick_synth *synth,
                                 gkick_real *drive)
{
        return gkick_distortion_get_drive(synth->distortion, drive);
}

enum geonkick_error
gkick_synth_enable_group(struct gkick_synth *synth,
                         size_t index,
                         bool enable)
{
        gkick_synth_lock(synth);
        synth->osc_groups[index] = enable;
        synth->buffer_update = true;
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_synth_group_enabled(struct gkick_synth *synth,
                          size_t index,
                          bool *enabled)
{
        gkick_synth_lock(synth);
        *enabled = synth->osc_groups[index];
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_synth_group_set_amplitude(struct gkick_synth *synth,
                                   size_t index,
                                   gkick_real amplitude)
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
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		gkick_synth_unlock(synth);
		return GEONKICK_ERROR;
	}

        if (osc->sample == NULL)
                gkick_buffer_new(&osc->sample, osc->sample_rate * GEONKICK_MAX_LENGTH);
        gkick_buffer_set_data(osc->sample, data, size);
        gkick_buffer_reset(osc->sample);
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
        struct gkick_oscillator *osc;
        osc = gkick_synth_get_oscillator(synth, osc_index);
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
