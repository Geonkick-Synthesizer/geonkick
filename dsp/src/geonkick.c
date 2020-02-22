/**
 * File name: geonkick.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://geontime.com>
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

#include "geonkick_internal.h"
#include "synthesizer.h"
#include "gkick_audio.h"
#include "audio_output.h"

enum geonkick_error
geonkick_create(struct geonkick **kick)
{
	if (kick == NULL)
		return GEONKICK_ERROR;

	*kick = (struct geonkick*)calloc(1, sizeof(struct geonkick));
	if (*kick == NULL)
		return GEONKICK_ERROR_MEM_ALLOC;
	strcpy((*kick)->name, "Geonkick");
	(*kick)->update_buffers = false;
        (*kick)->synthesis_on = false;
        (*kick)->per_index = 0;

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

	if (gkick_audio_create(&(*kick)->audio) != GEONKICK_OK) {
                gkick_log_warning("can't create audio");
		geonkick_free(kick);
		return GEONKICK_ERROR;
	}

        for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
                if (gkick_synth_new(&(*kick)->synths[i]) != GEONKICK_OK) {
                        gkick_log_error("can't create synthesizer %u", i);
                        geonkick_free(kick);
                        return GEONKICK_ERROR;
                }
        }

        // Set the first synth as controllable.
        (*kick)->synth = (*kick)->synths[0];

        for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++)
                gkick_synth_set_output((*kick)->synths[i], (*kick)->audio->audio_outputs[i]);

	if (geonkick_worker_init(*kick) != GEONKICK_OK) {
		gkick_log_error("can't init worker");
		geonkick_free(kick);
		return GEONKICK_ERROR;
	}

	if (geonkick_worker_start(*kick) != GEONKICK_OK) {
		gkick_log_error("can't start worker");
		geonkick_free(kick);
		return GEONKICK_ERROR;
	}

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
        if (kick != NULL && *kick != NULL) {
		geonkick_worker_destroy(*kick);
                for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++)
                        gkick_synth_free(&((*kick)->synths[i]));
                gkick_audio_free(&((*kick)->audio));
		pthread_mutex_destroy(&(*kick)->lock);
                free(*kick);
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
        if (kick == NULL)
                return GEONKICK_ERROR;
        enum geonkick_error res;
        res = gkick_synth_enable_oscillator(kick->synths[kick->per_index], index, 1);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        enum geonkick_error res;
        res = gkick_synth_enable_oscillator(kick->synths[kick->per_index], index, 0);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick, size_t index, int *enabled)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_is_enabled(kick->synths[kick->per_index], index, enabled);
}

enum geonkick_error
geonkick_get_oscillators_number(struct geonkick *kick, size_t *number)
{
        if (kick == NULL || number == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_oscillators_number(kick->synths[kick->per_index], number);
}

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				gkick_real x,
				gkick_real y)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_osc_env_add_point(kick->synths[kick->per_index], osc_index,
                                            env_index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
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
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_envelope_points(kick->synths[kick->per_index], osc_index,
                                               env_index, buf, npoints);
}

enum geonkick_error
geonkick_osc_envelope_set_points(struct geonkick *kick,
                                 size_t osc_index,
                                 size_t env_index,
                                 const gkick_real *buff,
                                 size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_osc_envelope_set_points(kick->synths[kick->per_index],
                                                   osc_index,
                                                   env_index,
                                                   buff,
                                                   npoints);
}


enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_osc_env_remove_point(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               index);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
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
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_osc_env_update_point(kick->synths[kick->per_index], osc_index,
                                         env_index, index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);

        return res;
}

enum geonkick_error
geonkick_osc_set_fm(struct geonkick *kick, size_t index, bool is_fm)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_osc_set_fm(kick->synths[kick->per_index], index, is_fm);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_osc_is_fm(struct geonkick *kick, size_t index, bool *is_fm)
{
        if (kick == NULL || is_fm == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_is_fm(kick->synths[kick->per_index], index, is_fm);
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type type)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        enum geonkick_error res;
        res = gkick_synth_set_osc_function(kick->synths[kick->per_index], osc_index, type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type *type)
{
        if (kick == NULL || type == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_osc_function(kick->synths[kick->per_index], osc_index, type);
}

enum geonkick_error
geonkick_set_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real phase)
{
        if (kick == NULL)
                return GEONKICK_ERROR;

        enum geonkick_error res;
        res = gkick_synth_set_osc_phase(kick->synths[kick->per_index], osc_index, phase);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_get_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real *phase)
{
        if (kick == NULL || phase == NULL)
                return GEONKICK_ERROR;

        return gkick_synth_get_osc_phase(kick->synths[kick->per_index], osc_index, phase);
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, gkick_real len)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_set_length(kick->synths[kick->per_index], len);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, gkick_real *len)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_length(kick->synths[kick->per_index], len);
}

enum geonkick_error
geonkick_get_max_length(struct geonkick *kick, gkick_real *len)
{
        GEONKICK_UNUSED(kick);
        if (kick == NULL || len == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        *len = GEONKICK_MAX_LENGTH;
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_kick_set_amplitude(struct geonkick *kick, gkick_real amplitude)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_kick_set_amplitude(kick->synths[kick->per_index], amplitude);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_get_amplitude(struct geonkick *kick, gkick_real *amplitude)
{
        if (kick == NULL || amplitude == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_amplitude(kick->synths[kick->per_index], amplitude);
}

enum geonkick_error
geonkick_kick_set_filter_frequency(struct geonkick *kick, gkick_real frequency)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_kick_set_filter_frequency(kick->synths[kick->per_index], frequency);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_filter_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = geonkick_synth_kick_filter_enable(kick->synths[kick->per_index], enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_filter_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return geonkick_synth_kick_filter_is_enabled(kick->synths[kick->per_index], enabled);
}

enum geonkick_error
geonkick_kick_get_filter_frequency(struct geonkick *kick, gkick_real *frequency)
{
        if (kick == NULL || frequency == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_frequency(kick->synths[kick->per_index], frequency);
}

enum geonkick_error
geonkick_kick_set_filter_factor(struct geonkick *kick, gkick_real factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_set_filter_factor(kick->synths[kick->per_index], factor);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_get_filter_factor(struct geonkick *kick, gkick_real *factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_factor(kick->synths[kick->per_index], factor);
}

enum geonkick_error
geonkick_set_kick_filter_type(struct geonkick *kick, enum gkick_filter_type type)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_set_kick_filter_type(kick->synths[kick->per_index], type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_kick_filter_type(struct geonkick *kick, enum gkick_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_get_kick_filter_type(kick->synths[kick->per_index], type);
}

enum geonkick_error
geonkick_kick_envelope_get_points(struct geonkick *kick,
                                  enum geonkick_envelope_type env_type,
                                  gkick_real **buf,
                                  size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_envelope_get_points(kick->synths[kick->per_index], env_type, buf, npoints);
}

enum geonkick_error
geonkick_kick_envelope_set_points(struct geonkick *kick,
                                  enum geonkick_envelope_type env_type,
                                  const gkick_real *buff,
                                  size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0)
                return GEONKICK_ERROR;
        enum geonkick_error res;
        res = gkick_synth_kick_envelope_set_points(kick->synths[kick->per_index], env_type, buff, npoints);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_kick_add_env_point(struct geonkick *kick,
                            enum geonkick_envelope_type env_type,
                            gkick_real x,
                            gkick_real y)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_add_env_point(kick->synths[kick->per_index], env_type, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_remove_env_point(struct geonkick *kick,
                               enum geonkick_envelope_type env_type,
                               size_t index)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_remove_env_point(kick->synths[kick->per_index], env_type, index);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_update_env_point(struct geonkick *kick,
                               enum geonkick_envelope_type env_type,
                               size_t index,
                               gkick_real x,
                               gkick_real y)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_update_env_point(kick->synths[kick->per_index], env_type, index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_set_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res = gkick_synth_set_osc_frequency(kick->synths[kick->per_index], osc_index, v);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_set_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res = gkick_synth_set_osc_amplitude(kick->synths[kick->per_index], osc_index, v);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
        return gkick_synth_get_osc_amplitude(kick->synths[kick->per_index], osc_index, v);
}

enum geonkick_error
geonkick_get_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_get_osc_frequency(kick->synths[kick->per_index], osc_index, v);
}

enum geonkick_error
geonkick_play(struct geonkick *kick)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_play(kick->audio);
}

enum geonkick_error
geonkick_key_pressed(struct geonkick *kick,
                     bool pressed,
                     int note,
                     int velocity)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_key_pressed(kick->audio, pressed, note, velocity);
}

enum geonkick_error
geonkick_get_kick_buffer_size(struct geonkick *kick, size_t *size)
{
        enum geonkick_error res;
        if (kick  == NULL || size == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        res = gkick_synth_get_buffer_size(kick->synths[kick->per_index], size);
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
        res = gkick_synth_get_buffer(kick->synths[kick->per_index], buffer, size);
        return res;
}

enum geonkick_error
geonkick_set_kick_buffer_callback(struct geonkick *kick,
                                  void (*callback)(void*, gkick_real* buff, size_t size),
                                  void *arg)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

	geonkick_lock(kick);
	kick->buffer_callback = callback;
        kick->callback_args = arg;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_kick_limiter_callback(struct geonkick *kick,
                                   void (*callback)(void*, gkick_real val),
                                   void *arg)
{
        if (kick  == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_set_limiter_callback(kick->audio, callback, arg);
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

        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_type(kick->synths[kick->per_index], osc_index, type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
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
        return gkick_synth_get_osc_filter_type(kick->synths[kick->per_index], osc_index, type);
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

        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_cutoff(kick->synths[kick->per_index], osc_index, cutoff);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
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
        return gkick_synth_get_osc_filter_cutoff(kick->synths[kick->per_index], osc_index, cutoff);
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
        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_factor(kick->synths[kick->per_index], osc_index, factor);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
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
        return gkick_synth_get_osc_filter_factor(kick->synths[kick->per_index], osc_index, factor);
}


enum geonkick_error
geonkick_enbale_osc_filter(struct geonkick *kick, size_t osc_index, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_osc_enable_filter(kick->synths[kick->per_index], osc_index, enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_osc_filter_is_enabled(struct geonkick *kick, size_t osc_index, int *enable)
{
        if (kick == NULL || enable == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_osc_is_enabled_filter(kick->synths[kick->per_index], osc_index, enable);
}

enum geonkick_error
geonkick_get_sample_rate(struct geonkick *kick, int *sample_rate)
{
        GEONKICK_UNUSED(kick);

        if (sample_rate == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        *sample_rate = GEONKICK_SAMPLE_RATE;
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_enable_synthesis(struct geonkick *kick, bool enable)
{
	if (kick == NULL) {
		gkick_log_error("");
		return GEONKICK_ERROR;
	}

	kick->synthesis_on = enable;
        for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
                if (kick->synths[i]->is_active && kick->synthesis_on)
                        kick->synths[i]->buffer_update = true;
        }
        geonkick_worker_wakeup(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_audio_frame(struct geonkick *kick, gkick_real *val)
{
        return gkick_audio_get_frame(kick->audio, val);
}

enum geonkick_error
geonkick_compressor_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_enable(kick->synths[kick->per_index], enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_compressor_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_is_enabled(kick->synths[kick->per_index], enabled);
}

enum geonkick_error
geonkick_compressor_set_attack(struct geonkick *kick, gkick_real attack)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_attack(kick->synths[kick->per_index], attack);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_attack(struct geonkick *kick, gkick_real *attack)
{
        if (kick == NULL || attack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_attack(kick->synths[kick->per_index], attack);
}

enum geonkick_error
geonkick_compressor_set_release(struct geonkick *kick, gkick_real release)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_release(kick->synths[kick->per_index], release);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_release(struct geonkick *kick, gkick_real *release)
{
        if (kick == NULL || release == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_release(kick->synths[kick->per_index], release);
}

enum geonkick_error
geonkick_compressor_set_threshold(struct geonkick *kick, gkick_real threshold)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_threshold(kick->synths[kick->per_index], threshold);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_threshold(struct geonkick *kick, gkick_real *threshold)
{
        if (kick == NULL || threshold == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_threshold(kick->synths[kick->per_index], threshold);
}

enum geonkick_error
geonkick_compressor_set_ratio(struct geonkick *kick, gkick_real ratio)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_ratio(kick->synths[kick->per_index], ratio);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_ratio(struct geonkick *kick, gkick_real *ratio)
{
        if (kick == NULL || ratio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_ratio(kick->synths[kick->per_index], ratio);
}

enum geonkick_error
geonkick_compressor_set_knee(struct geonkick *kick, gkick_real knee)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_knee(kick->synths[kick->per_index], knee);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_knee(struct geonkick *kick, gkick_real *knee)
{
        if (kick == NULL || knee == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_knee(kick->synths[kick->per_index], knee);
}

enum geonkick_error
geonkick_compressor_set_makeup(struct geonkick *kick, gkick_real makeup)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_makeup(kick->synths[kick->per_index], makeup);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_compressor_get_makeup(struct geonkick *kick, gkick_real *makeup)
{
        if (kick == NULL || makeup == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_makeup(kick->synths[kick->per_index], makeup);
}

enum geonkick_error
geonkick_distortion_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_enable(kick->synths[kick->per_index], enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_is_enabled(kick->synths[kick->per_index], enabled);
}

enum geonkick_error
geonkick_distortion_set_in_limiter(struct geonkick *kick, gkick_real limit)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}
	return gkick_synth_distortion_set_in_limiter(kick->synth, limit);
}

enum geonkick_error
geonkick_distortion_get_in_limiter(struct geonkick *kick, gkick_real *limit)
{
	if (kick == NULL || limit == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}
	return gkick_synth_distortion_get_in_limiter(kick->synth, limit);
}

enum geonkick_error
geonkick_distortion_set_volume(struct geonkick *kick, gkick_real volume)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_set_volume(kick->synths[kick->per_index], volume);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_get_volume(struct geonkick *kick, gkick_real *volume)
{
        if (kick == NULL || volume == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_volume(kick->synths[kick->per_index], volume);
}

enum geonkick_error
geonkick_distortion_set_drive(struct geonkick *kick, gkick_real drive)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_set_drive(kick->synths[kick->per_index], drive);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_get_drive(struct geonkick *kick, gkick_real *drive)
{
        if (kick == NULL || drive == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_drive(kick->synths[kick->per_index], drive);
}

int geonkick_is_module_enabed(struct geonkick *kick, enum GEONKICK_MODULE module)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return 0;
        }

        switch (module)
        {
        case GEONKICK_MODULE_JACK:
                if (kick->audio != NULL && kick->audio->jack != NULL)
                        return 1;
                else
                        return 0;
        break;
        default:
                return 0;
        }
}

enum geonkick_error
geonkick_enable_group(struct geonkick *kick, size_t index, bool enable)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = gkick_synth_enable_group(kick->synths[kick->per_index], index, enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_group_enabled(struct geonkick *kick, size_t index, bool *enabled)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (kick->synth != NULL)
                return gkick_synth_group_enabled(kick->synths[kick->per_index], index, enabled);
        return GEONKICK_ERROR;
}

enum geonkick_error
geonkick_group_set_amplitude(struct geonkick *kick, size_t index, gkick_real amplitude)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = geonkick_synth_group_set_amplitude(kick->synths[kick->per_index], index, amplitude);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_group_get_amplitude(struct geonkick *kick, size_t index, gkick_real *amplitude)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1
            || amplitude == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return geonkick_synth_group_get_amplitude(kick->synths[kick->per_index], index, amplitude);
}

enum geonkick_error
geonkick_tune_audio_output(struct geonkick *kick, bool tune)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        /* if (kick->audio == NULL || kick->audio->audio_output == NULL) { */
        /*         gkick_log_error("audio output was not created"); */
        /*         return GEONKICK_ERROR; */
        /* } */

        /* gkick_audio_tune_output(kick->audio->audio_output, tune); */
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_is_audio_output_tuned(struct geonkick *kick, bool *tune)
{
        if (kick == NULL || tune == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        /* if (kick->audio == NULL || kick->audio->audio_output == NULL) { */
        /*         gkick_log_error("audio output was not created"); */
        /*         return GEONKICK_ERROR; */
        /* } else { */
        /*         *tune = gkick_audio_is_tune_output(kick->audio->audio_output); */
        /* } */

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_sample(struct geonkick *kick,
                        size_t osc_index,
                        const gkick_real *data,
                        size_t size)
{
        if (kick == NULL || data == NULL || size < 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = geonkick_synth_set_osc_sample(kick->synths[kick->per_index], osc_index, data, size);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_worker_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_sample(struct geonkick *kick,
                        size_t osc_index,
                        gkick_real **data,
                        size_t *size)
{
        if (kick == NULL || data == NULL || size == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return geonkick_synth_get_osc_sample(kick->synths[kick->per_index], osc_index, data, size);
}

enum geonkick_error
geonkick_set_current_percussion(struct geonkick *kick, size_t index)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	kick->per_index = index;
	struct gkick_synth *synth = kick->synths[kick->per_index];
	gkick_synth_lock(synth);
        if (kick->buffer_callback != NULL && kick->callback_args != NULL) {
                kick->buffer_callback(kick->callback_args,
				      ((struct gkick_buffer*)synth->buffer)->buff,
				      synth->buffer_size);
	}
	gkick_synth_unlock(synth);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_current_percussion(struct geonkick *kick, size_t *index)
{
        if (kick == NULL || index == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	*index = kick->per_index;
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_worker_init(struct geonkick *kick)
{
	if (kick == NULL)
		return GEONKICK_ERROR;

	geonkick_lock(kick);
	struct gkick_worker *worker = &kick->worker;
	worker->running = false;
        if (pthread_cond_init(&worker->condition_var, NULL) != 0) {
                gkick_log_error("can't init worker condition variable");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	worker->cond_var_initilized = true;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_worker_start(struct geonkick *kick)
{
        kick->worker.running = true;
        if (pthread_create(&kick->worker.thread, NULL, geonkick_worker_thread, kick) != 0) {
                gkick_log_error("can't create worker thread");
                kick->worker.running = false;
                return GEONKICK_ERROR;
        }
        return GEONKICK_OK;
}

void geonkick_worker_destroy(struct geonkick *kick)
{
	struct gkick_worker *worker = &kick->worker;
	if (worker->running)
		worker->running = false;
        geonkick_lock(kick);
        pthread_cond_signal(&kick->worker.condition_var);
        geonkick_unlock(kick);
	pthread_join(worker->thread, NULL);

	geonkick_lock(kick);
	if (worker->cond_var_initilized)
		pthread_cond_destroy(&worker->condition_var);
	worker->cond_var_initilized = false;
	geonkick_unlock(kick);
}

void *geonkick_worker_thread(void *arg)
{
	if (arg == NULL) {
		gkick_log_error("wrong arugments");
		return NULL;
	}
        gkick_log_debug("worker thread started");

	struct geonkick *kick = (struct geonkick*)arg;
	struct gkick_worker *worker = &kick->worker;
	while (worker->running) {
		/* Ignore too many updates. The last udpates will be processed. */
                usleep(40000);
		if (!kick->update_buffers) {
		        pthread_cond_wait(&worker->condition_var, &kick->lock);
                        geonkick_unlock(kick);
                        if (!worker->running)
                                break;
                }

                /* Prioritize the synthesis for the current controllable synthesizer. */
                struct gkick_synth *synth = kick->synths[kick->per_index];
                if (synth != NULL && synth->is_active && synth->buffer_update) {
                        kick->update_buffers = false;
                        gkick_synth_process(synth, kick->buffer_callback, kick->callback_args);
                }

                for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
                        if (i == kick->per_index)
                                continue;
                        synth = kick->synths[i];
                        if (synth != NULL && synth->is_active && synth->buffer_update) {
                                kick->update_buffers = false;
                                gkick_synth_process(synth, NULL, NULL);
                        }
                }
	}

        gkick_log_debug("worker thread stopped");
        return NULL;
}

void geonkick_worker_wakeup(struct geonkick *kick)
{
        if (kick->synthesis_on) {
                kick->update_buffers = true;
                geonkick_lock(kick);
                pthread_cond_signal(&kick->worker.condition_var);
                geonkick_unlock(kick);
        }
}

enum geonkick_error
geonkick_unused_percussion(struct geonkick *kick, int *index)
{
        gkick_log_info("called");
        if (kick == NULL || index == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	*index = -1;
        for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
                if (!kick->synths[i]->is_active) {
                        gkick_log_info("%d", i);
                        *index = i;
                        return GEONKICK_OK;
                }
        }
        return GEONKICK_ERROR;
}

enum geonkick_error
geonkick_enable_percussion(struct geonkick *kick, size_t index, bool enable)
{
        if (kick == NULL || index > GEONKICK_MAX_PERCUSSIONS - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        kick->synths[index]->is_active = enable;
	return GEONKICK_OK;
}

size_t geonkick_percussion_number(struct geonkick *kick)
{
	return GEONKICK_MAX_PERCUSSIONS;
}

enum geonkick_error
geonkick_set_playing_key(struct geonkick *kick, size_t id, char key)
{
        if (kick == NULL || id > GEONKICK_MAX_PERCUSSIONS - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_set_playing_key(kick->synths[id]->output, key);
}
