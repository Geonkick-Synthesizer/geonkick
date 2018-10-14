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
#include "synthesizer.h"
#include "gkick_audio.h"

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

	if (gkick_audio_create(&(*kick)->audio) != GEONKICK_OK) {
                gkick_log_warning("can't create jack");
		geonkick_free(kick);
		return GEONKICK_ERROR;
	}

        if (gkick_synth_new(&(*kick)->synth) != GEONKICK_OK) {
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
        if (kick != NULL && *kick != NULL) {
                gkick_synth_free(&((*kick)->synth));
                gkick_audio_free(&((*kick)->audio));
                pthread_mutex_destroy(&(*kick)->lock);
                free(*kick);
                *kick = NULL;
        }
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
        enum geonkick_error res;
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        res = gkick_synth_enable_oscillator(kick->synth, index, 1);
        return res;
}

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_enable_oscillator(kick->synth, index, 0);
}

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick, size_t index, int *enabled)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_is_enabled(kick->synth, index, enabled);
}

enum geonkick_error
geonkick_get_oscillators_number(struct geonkick *kick, size_t *number)
{
        if (kick == NULL || number == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_oscillators_number(kick->synth, number);
}

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				gkick_real x,
				gkick_real y)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        gkick_synth_osc_env_add_point(kick->synth, osc_index,
                                      env_index, x, y);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_update_envelope_point(struct geonkick *kick,
			       enum geonkick_envelope_type type,
			       size_t index,
			       gkick_real x,
			       gkick_real y)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;

}

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, gkick_real rate)
{
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 gkick_real **buf,
				 size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_envelope_points(kick->synth, osc_index,
                                               env_index, buf, npoints);
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

        gkick_synth_osc_env_remove_point(kick->synth, osc_index,
                                         env_index, index);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_update_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
				   gkick_real x,
				   gkick_real y)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        gkick_synth_osc_env_update_point(kick->synth, osc_index,
                                         env_index, index, x, y);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type type)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_set_osc_function(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, gkick_real len)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_set_length(kick->synth, len);
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, gkick_real *len)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_length(kick->synth, len);
}

enum geonkick_error
geonkick_set_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_set_osc_frequency(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_set_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_set_osc_amplitude(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

        return gkick_synth_get_osc_amplitude(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	return gkick_synth_get_osc_frequency(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_play(struct geonkick *kick, int play)
{
        enum geonkick_error res;
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        res = gkick_audio_play(kick->audio, play);
        return res;
}

enum geonkick_error
geonkick_get_kick_buffer(struct geonkick *kick, gkick_real *buffer, size_t size)
{
        enum geonkick_error res;
        if (kick  == NULL || buffer == NULL || size < 1) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        res = gkick_synth_get_buffer(kick->synth, buffer, size);
        return res;
}

enum geonkick_error
geonkick_set_kick_buffer_callback(struct geonkick *kick, void (*callback)(void*), void *arg)
{
        if (kick  == NULL || callback == NULL || arg == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_set_buffer_callback(kick->synth, callback, arg);
}

enum geonkick_error
geonkick_set_limiter_value(struct geonkick *kick, gkick_real limit)
{
        enum geonkick_error res;
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        res = gkick_audio_set_limiter_val(kick->audio, limit);
        return res;
}

enum geonkick_error
geonkick_get_limiter_value(struct geonkick *kick, gkick_real *limit)
{
        enum geonkick_error res;
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        res = gkick_audio_get_limiter_val(kick->audio, limit);
        return res;
}

enum geonkick_error
geonkick_set_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type type)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_set_osc_filter_type(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_get_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_osc_filter_type(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_set_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real cutoff)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_log_debug("freq: %f", cutoff);
        return gkick_synth_set_osc_filter_cutoff(kick->synth, osc_index, cutoff);
}

enum geonkick_error
geonkick_get_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real *cutoff)
{
        if (kick == NULL || cutoff == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_osc_filter_cutoff(kick->synth, osc_index, cutoff);
}

enum geonkick_error
geonkick_set_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_log_debug("factor: %f", factor);
        return gkick_synth_set_osc_filter_factor(kick->synth, osc_index, factor);
}

enum geonkick_error
geonkick_get_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real *factor)
{
        if (kick == NULL || factor == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_osc_filter_factor(kick->synth, osc_index, factor);
}


enum geonkick_error
geonkick_enbale_osc_filter(struct geonkick *kick, size_t osc_index, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_enable_filter(kick->synth, osc_index, enable);
}

enum geonkick_error
geonkick_osc_filter_is_enabled(struct geonkick *kick, size_t osc_index, int *enable)
{
        if (kick == NULL || enable == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_is_enabled_filter(kick->synth, osc_index, enable);
}

