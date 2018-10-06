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

	if (gkick_audio_device_create((*kick)->audio) != GEONKICK_OK) {
                gkick_log_warning("can't create jack");
		// geonkick_free(kick);
		// return GEONKICK_ERROR;
	}

        if (gkick_synth_create((*kick)->synth) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

        gkick_synth_set_output((*kick)->synth, (*kick)->audio->input);

        if (gkick_synth_start((*kick)->synth)) {
                gkick_log_warning("can't start synthesizer");
                geonkick_free(kick);
                return GEONKICK_ERROR;
        }

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
        int i = 0;

        if (kick != NULL && *kick != NULL) {
                gkick_audio_device_free(&((*kick)->audio_device));
                gkick_synthesizer_free(&((*kick)->synthesizer));
                pthread_mutex_destroy(&(*kick)->lock);
                free(*kick);
                *kick = NULL;
        }
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

        return gkick_synth_osc_env_add_point(osc_index, env_index, x, y);
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

        return gkick_synth_osc_env_get_points(kick->synt, osc,
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

        gkick_synth_set_length(kick->synth, len);
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, double *len)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_length(kick->synth, len);
}

enum geonkick_error
geonkick_set_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_set_osc_frequency(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_set_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_set_osc_amplitude(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

        return gkick_synth_get_osc_amplitude_val(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_frequency_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_get_osc_frequency(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_play(struct geonkick *kick, int play)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_audio_play(kick->audio_device, play);
}

enum geonkick_error
geonkick_get_kick_buffer(struct geonkick *kick, double *buffer, size_t size)
{
        if (kick  == NULL || buffer == NULL || size < 1) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_buffer(kick->synth, buffer, size);
}

enum geonkick_error
geonkick_set_limiter_value(struct geonkick *kick, double limit)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_audio_set_limiter_val(kick->audio_dev, limit);
}

enum geonkick_error
geonkick_get_limiter_value(struct geonkick *kick, double *limit)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_audio_set_limiter_val(kick->audio_dev, limit);
}
