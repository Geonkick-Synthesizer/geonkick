/**
 * File name: geonkick.c
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

#include "geonkick_internal.h"
#include "oscillator.h"

enum geonkick_error
geonkick_create(struct geonkick **kick)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	*kick = (struct geonkick*)malloc(sizeof(struct geonkick));
	if (*kick == NULL) {
		return GEONKICK_ERROR_MEM_ALLOC;
	}
	memset(*kick, 0, sizeof(struct geonkick));

	strcpy((*kick)->name, "GeonKick");
	(*kick)->length = 0.26;
	(*kick)->oscillators_number = 3;
	(*kick)->midi_in_enabled = 1;

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

        if (gkick_create_oscillators(*kick) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

	if (gkick_create_jack(*kick) != GEONKICK_OK) {
                gkick_log_warning("can't create jack");
		// geonkick_free(kick);
		// return GEONKICK_ERROR;
	}

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
        int i = 0;

        if (kick == NULL || *kick == NULL) {
                *kick = NULL;
                return;
        }

        gkick_jack_free(&((*kick)->jack));
        if ((*kick)->oscillators != NULL) {
                for (i = 0; i < (*kick)->oscillators_number; i++) {
                        gkick_osc_free(&((*kick)->oscillators[i]));
                }
                free((*kick)->oscillators);
        }

        pthread_mutex_destroy(&(*kick)->lock);
        free(*kick);
        *kick = NULL;
}

enum geonkick_error
geonkick_add_oscillator(struct geonkick* kick, size_t *index)
{
        GEONKICK_UNUSED(kick);
        GEONKICK_UNUSED(index);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_remove_oscillator(struct geonkick* kick, size_t index)
{
        GEONKICK_UNUSED(kick);
        GEONKICK_UNUSED(index);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_enable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}

	gkick_osc_set_state(osc, GEONKICK_OSC_STATE_ENABLED);
	geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	gkick_osc_set_state(osc, GEONKICK_OSC_STATE_DISABLED);
	geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick, size_t index, int *enabled)
{
        *enabled = 0;
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	if (gkick_osc_enabled(osc)) {
                *enabled = 1;
        }
	geonkick_unlock(kick);
        return GEONKICK_OK;
}

size_t
geonkick_get_oscillators_number(struct geonkick *kick)
{
        size_t n;

        if (kick == NULL || kick->oscillators == NULL) {
                return 0;
        }

        geonkick_lock(kick);
        n = kick->oscillators_number;
        geonkick_unlock(kick);

        return n;
}

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick)
{
        size_t size;
        size_t i;
        struct gkick_oscillator *osc;

        size = sizeof(struct gkick_oscillator*)
                * kick->oscillators_number;
        kick->oscillators = (struct gkick_oscillator**)malloc(size);

        if (kick->oscillators == NULL) {
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        memset(kick->oscillators, 0, size);

        for (i = 0; i < kick->oscillators_number; i++) {
                osc = gkick_osc_create(kick);
                if (osc == NULL) {
                        return GEONKICK_ERROR;
                }
                kick->oscillators[i] = osc;
        }

        return GEONKICK_OK;
}


enum geonkick_error
geonkick_start(struct geonkick *kick)
{
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				double x,
				double y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);

        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                geonkick_unlock(kick);
                return GEONKICK_ERROR;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        if (gkick_envelope_add_point(env, x, y) == NULL) {
                gkick_log_error("can't get envelope");
                geonkick_unlock(kick);
                return GEONKICK_ERROR;
        }

        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_update_envelope_point(struct geonkick *kick,
			       enum geonkick_envelope_type type,
			       size_t index,
			       double x,
			       double y)
{
        return GEONKICK_OK;
}

double geonkick_get_oscillators_value(struct geonkick *kick)
{
        double val = 0.0;
        size_t i;

        geonkick_lock(kick);
        val = 0.0;
        for (i = 0; i < kick->oscillators_number; i++) {
                if (gkick_osc_enabled(kick->oscillators[i])) {
                        val += gkick_osc_value(kick->oscillators[i],
                                               kick->current_time, kick->length);
                }
                gkick_osc_increment_phase(kick->oscillators[i],
                                          kick->current_time, kick->length);
        }
        geonkick_unlock(kick);

        return val;
}

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate)
{
        return GEONKICK_OK;
}

void geonkick_reset_oscillators(struct geonkick *kick)
{
        int i = 0;

        if (kick != NULL) {
                geonkick_lock(kick);
                for (i = 0; i < kick->oscillators_number; i++) {
                        (kick->oscillators[i])->phase = 0.0;
                }
                geonkick_unlock(kick);
        }
}

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 double **buf,
				 size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                return GEONKICK_ERROR;
        }

        *npoints = 0;
        *buf = NULL;
        geonkick_lock(kick);
        gkick_log_trace("get oscillator");
        struct gkick_oscillator *osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                geonkick_unlock(kick);
                return GEONKICK_ERROR;
        }
        gkick_osc_get_envelope_points(osc, env_index, buf, npoints);
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index)
{
        if (index >= 0 && index < kick->oscillators_number) {
                return kick->oscillators[index];
        } else {
                gkick_log_error("index = %d out of range", index);
        }

        return NULL;
}

void geonkick_lock(struct geonkick *kick)
{
        pthread_mutex_lock(&kick->lock);
}

void geonkick_unlock(struct geonkick *kick)
{
        pthread_mutex_unlock(&kick->lock);
}

enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get oscillator %d", osc_index);
                return GEONKICK_ERROR;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR;
        }

        gkick_envelope_remove_point(env, index);

        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_update_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
				   double x,
				   double y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get oscillator %d", osc_index);
                return GEONKICK_ERROR;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                gkick_log_error("can't get envelope");
                geonkick_unlock(kick);
                return GEONKICK_ERROR;
        }

        gkick_envelope_update_point(env, index, x, y);
        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t oscillatorIndex,
			  enum geonkick_osc_func_type type)
{
        struct gkick_oscillator *osc;
        gkick_log_debug("SET FUNCTION: %d", type);
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, oscillatorIndex);
        if (osc != NULL) {
                osc->func = type;
        }
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, double t)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->length = t;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, double *t)
{
        if (kick == NULL || t == 0) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        *t = kick->length;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_frequency_val(struct geonkick *kick,
                               size_t osc_index,
                               double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	osc->frequency = v;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_amplitude_val(struct geonkick *kick,
                               size_t osc_index,
                               double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	osc->amplitude = v;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_osc_amplitude_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	*v = 0.0;
	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	*v = osc->amplitude;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_osc_frequency_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	*v = 0.0;
	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	*v = osc->frequency;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_play(struct geonkick *kick, int play)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->is_play = play;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

int
geonckick_is_play_stopped(struct geonkick *kick)
{
        int is_play;

        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        is_play = 0;
        geonkick_lock(kick);
        is_play = kick->is_play;
        geonkick_unlock(kick);

        return !is_play;
}

enum geonkick_error
geonkick_start_play(struct geonkick *kick)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->current_time = 0.0;
        kick->is_play = 1;
        geonkick_unlock(kick);
        geonkick_reset_oscillators(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_stop_play(struct geonkick *kick)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }
        geonkick_lock(kick);
        kick->current_time = 0.0;
        kick->is_play = 0;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_increment_time(struct geonkick *kick, double dt)
{
        double current_time;

        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        current_time = geonkick_current_time(kick);
        current_time += dt;
        if (current_time > geonkick_length(kick)) {
                geonkick_stop_play(kick);
        } else {
                geonkick_set_current_time(kick, current_time);
        }

        return GEONKICK_OK;
}

double
geonkick_length(struct geonkick *kick)
{
        double len;

        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return 0.0;
        }

        geonkick_lock(kick);
        len = kick->length;
        geonkick_unlock(kick);

        return len;
}

double
geonkick_current_time(struct geonkick *kick)
{
        double current_time;

        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return 0.0;
        }

        geonkick_lock(kick);
        current_time = kick->current_time;
        geonkick_unlock(kick);

        return current_time;
}

void
geonkick_set_current_time(struct geonkick *kick,
			  double current_time)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return;
        }

        geonkick_lock(kick);
        kick->current_time = current_time;
        geonkick_unlock(kick);
}

enum geonkick_error
geonkick_get_kick_buffer(double *buffer, size_t *size)
{
        (void)buffer;
        (void)size;
        gkick_log_info("not implemented");
}
