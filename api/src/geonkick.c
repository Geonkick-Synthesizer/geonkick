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
	strcpy((*kick)->name, "Geonkick");

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

        if (gkick_synthesizer_create((*kick)->synthesizer) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

	if (gkick_audio_device_create((*kick)->audio_device) != GEONKICK_OK) {
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

        gkick_audio_device_free(&((*kick)->audio_device));
        gkick_synthesizer_free(&((*kick)->synthesizer);
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

        return gkick_synthesizer_enable_oscilaltor(kick->synthesizer, index, 1);
}

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synthesizer_enable_oscilaltor(kick->synthesizer, index, 0);
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
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				double x,
				double y)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_envelope_add_point(osc_index, env_index, x, y);
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

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate)
{
        return GEONKICK_OK;
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

        return gkick_synth_osc_envelope_points(kick->synt, osc,
                                               env, buf, npoints);
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
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_env_remove_point(kick->synth, osc_index,
                                                env_index, index);
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

        return gkick_synth_osc_env_update_point(kick->synth, osc_index,
                                                env_index, index, x, y);
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type type)
{
        if (kick == NULL) {
                return GEONKIK_ERROR;
        }

        return gkick_synth_set_osc_function(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, double len)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synthe_set_length(kick->synth, len);
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, double *len)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synthe_get_length(kick->synth, len);
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
geonkick_get_kick_buffer(struct geonkick *kick, double *buffer, size_t size)
{
        size_t i;
        for (i = 0; i < size; i++) {
                buffer[i] = (0.5 * (double)i  / size) * sin(0.01 * cos(0.1 * i));
        }

        return GEONKICK_OK;
}

void geonkick_set_limiter_value(struct geonkick *kick, double limit)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
        }

        gkick_log_debug("limit: %.5f", limit);
        if (limit < 0) {
                limit = 0;
        } else if (limit > 1.0) {
                limit = 1.0;
        } else {
                geonkick_lock(kick);
                kick->limiter = 0.1 * limit;
                geonkick_unlock(kick);
        }
}

double geonkick_get_limiter_value(struct geonkick *kick)
{
        double limit;
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
        }

        geonkick_lock(kick);
        limit = kick->limiter;
        geonkick_unlock(kick);

        return limit;
}
