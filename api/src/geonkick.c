/**
 * File name: geonkick.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

enum geonkick_error
geonkick_create(struct geonkick **kick)
{
	if (kick == NULL)
		return GEONKICK_ERROR;

	*kick = (struct geonkick*)malloc(sizeof(struct geonkick));
	if (*kick == NULL)
		return GEONKICK_ERROR_MEM_ALLOC;
	memset(*kick, 0, sizeof(struct geonkick));
	strcpy((*kick)->name, "Geonkick");

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

        if (gkick_synth_new(&(*kick)->synth) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

        if ((*kick)->synth != NULL && (*kick)->audio != NULL)
                gkick_synth_set_output((*kick)->synth, (*kick)->audio->audio_output);


        if (gkick_synth_start((*kick)->synth)) {
                gkick_log_error("can't start synthesizer");
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
        if (kick == NULL)
                return GEONKICK_ERROR;
        res = gkick_synth_enable_oscillator(kick->synth, index, 1);
        return res;
}

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_enable_oscillator(kick->synth, index, 0);
}

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick, size_t index, int *enabled)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_is_enabled(kick->synth, index, enabled);
}

enum geonkick_error
geonkick_get_oscillators_number(struct geonkick *kick, size_t *number)
{
        if (kick == NULL || number == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_oscillators_number(kick->synth, number);
}

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				gkick_real x,
				gkick_real y)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        gkick_synth_osc_env_add_point(kick->synth, osc_index,
                                      env_index, x, y);
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
        if (kick == NULL || buf == NULL || npoints == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_envelope_points(kick->synth, osc_index,
                                               env_index, buf, npoints);
}

enum geonkick_error
geonkick_osc_envelope_set_points(struct geonkick *kick,
                                 size_t osc_index,
                                 size_t env_index,
                                 const gkick_real *buff,
                                 size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0)
                return GEONKICK_ERROR;
        return gkick_synth_osc_envelope_set_points(kick->synth, osc_index, env_index, buff, npoints);
}


enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
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
        if (kick == NULL)
                return GEONKICK_ERROR;
        gkick_synth_osc_env_update_point(kick->synth, osc_index,
                                         env_index, index, x, y);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_set_fm(struct geonkick *kick, size_t index, bool is_fm)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_set_fm(kick->synth, index, is_fm);
}


enum geonkick_error
geonkick_osc_is_fm(struct geonkick *kick, size_t index, bool *is_fm)
{
        if (kick == NULL || is_fm == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_osc_is_fm(kick->synth, index, is_fm);
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type type)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_set_osc_function(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_get_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type *type)
{
        if (kick == NULL || type == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_osc_function(kick->synth, osc_index, type);
}

enum geonkick_error
geonkick_set_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real phase)
{
        if (kick == NULL)
                return GEONKICK_ERROR;

        return gkick_synth_set_osc_phase(kick->synth, osc_index, phase);

}

enum geonkick_error
geonkick_get_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real *phase)
{
        if (kick == NULL || phase == NULL)
                return GEONKICK_ERROR;

        return gkick_synth_get_osc_phase(kick->synth, osc_index, phase);
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, gkick_real len)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_set_length(kick->synth, len);
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, gkick_real *len)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_length(kick->synth, len);
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
        return gkick_synth_kick_set_amplitude(kick->synth, amplitude);
}

enum geonkick_error
geonkick_kick_get_amplitude(struct geonkick *kick, gkick_real *amplitude)
{
        if (kick == NULL || amplitude == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_amplitude(kick->synth, amplitude);
}

enum geonkick_error
geonkick_kick_set_filter_frequency(struct geonkick *kick, gkick_real frequency)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_set_filter_frequency(kick->synth, frequency);
}

enum geonkick_error
geonkick_kick_filter_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return geonkick_synth_kick_filter_enable(kick->synth, enable);
}

enum geonkick_error
geonkick_kick_filter_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return geonkick_synth_kick_filter_is_enabled(kick->synth, enabled);
}

enum geonkick_error
geonkick_kick_get_filter_frequency(struct geonkick *kick, gkick_real *frequency)
{
        if (kick == NULL || frequency == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_frequency(kick->synth, frequency);
}

enum geonkick_error
geonkick_kick_set_filter_factor(struct geonkick *kick, gkick_real factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_set_filter_factor(kick->synth, factor);
}

enum geonkick_error
geonkick_kick_get_filter_factor(struct geonkick *kick, gkick_real *factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_factor(kick->synth, factor);
}

enum geonkick_error
geonkick_set_kick_filter_type(struct geonkick *kick, enum gkick_filter_type type)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_set_kick_filter_type(kick->synth, type);
}

enum geonkick_error
geonkick_get_kick_filter_type(struct geonkick *kick, enum gkick_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_get_kick_filter_type(kick->synth, type);
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
        return gkick_synth_kick_envelope_get_points(kick->synth, env_type, buf, npoints);
}

enum geonkick_error
geonkick_kick_envelope_set_points(struct geonkick *kick,
                                  enum geonkick_envelope_type env_type,
                                  const gkick_real *buff,
                                  size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0)
                return GEONKICK_ERROR;
        return gkick_synth_kick_envelope_set_points(kick->synth, env_type, buff, npoints);
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
        return gkick_synth_kick_add_env_point(kick->synth, env_type, x, y);
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
        return gkick_synth_kick_remove_env_point(kick->synth, env_type, index);
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
        return gkick_synth_kick_update_env_point(kick->synth, env_type, index, x, y);
}

enum geonkick_error
geonkick_set_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_set_osc_frequency(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_set_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v)
{
	if (kick == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_set_osc_amplitude(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
        return gkick_synth_get_osc_amplitude(kick->synth, osc_index, v);
}

enum geonkick_error
geonkick_get_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_get_osc_frequency(kick->synth, osc_index, v);
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
        res = gkick_synth_get_buffer_size(kick->synth, size);
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
geonkick_set_kick_buffer_callback(struct geonkick *kick,
                                  void (*callback)(void*, gkick_real* buff, size_t size),
                                  void *arg)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_set_buffer_callback(kick->synth, callback, arg);
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
geonkick_enable_synthesis(struct geonkick *kick, int enable)
{
        return gkick_synth_enable_synthesis(kick->synth, enable);
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
        return gkick_synth_compressor_enable(kick->synth, enable);
}

enum geonkick_error
geonkick_compressor_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_is_enabled(kick->synth, enabled);
}

enum geonkick_error
geonkick_compressor_set_attack(struct geonkick *kick, gkick_real attack)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_attack(kick->synth, attack);
}

enum geonkick_error
geonkick_compressor_get_attack(struct geonkick *kick, gkick_real *attack)
{
        if (kick == NULL || attack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_attack(kick->synth, attack);
}

enum geonkick_error
geonkick_compressor_set_release(struct geonkick *kick, gkick_real release)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_release(kick->synth, release);
}

enum geonkick_error
geonkick_compressor_get_release(struct geonkick *kick, gkick_real *release)
{
        if (kick == NULL || release == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_release(kick->synth, release);
}

enum geonkick_error
geonkick_compressor_set_threshold(struct geonkick *kick, gkick_real threshold)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_threshold(kick->synth, threshold);
}

enum geonkick_error
geonkick_compressor_get_threshold(struct geonkick *kick, gkick_real *threshold)
{
        if (kick == NULL || threshold == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_threshold(kick->synth, threshold);
}

enum geonkick_error
geonkick_compressor_set_ratio(struct geonkick *kick, gkick_real ratio)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_ratio(kick->synth, ratio);
}

enum geonkick_error
geonkick_compressor_get_ratio(struct geonkick *kick, gkick_real *ratio)
{
        if (kick == NULL || ratio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_ratio(kick->synth, ratio);
}

enum geonkick_error
geonkick_compressor_set_knee(struct geonkick *kick, gkick_real knee)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_knee(kick->synth, knee);
}

enum geonkick_error
geonkick_compressor_get_knee(struct geonkick *kick, gkick_real *knee)
{
        if (kick == NULL || knee == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_knee(kick->synth, knee);
}

enum geonkick_error
geonkick_compressor_set_makeup(struct geonkick *kick, gkick_real makeup)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_set_makeup(kick->synth, makeup);
}

enum geonkick_error
geonkick_compressor_get_makeup(struct geonkick *kick, gkick_real *makeup)
{
        if (kick == NULL || makeup == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_makeup(kick->synth, makeup);
}

enum geonkick_error
geonkick_distortion_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_enable(kick->synth, enable);
}

enum geonkick_error
geonkick_distortion_is_enabled(struct geonkick *kick, int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_is_enabled(kick->synth, enabled);
}

enum geonkick_error
geonkick_distortion_set_volume(struct geonkick *kick, gkick_real volume)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_set_volume(kick->synth, volume);
}

enum geonkick_error
geonkick_distortion_get_volume(struct geonkick *kick, gkick_real *volume)
{
        if (kick == NULL || volume == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_volume(kick->synth, volume);
}

enum geonkick_error
geonkick_distortion_set_drive(struct geonkick *kick, gkick_real drive)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_set_drive(kick->synth, drive);
}

enum geonkick_error
geonkick_distortion_get_drive(struct geonkick *kick, gkick_real *drive)
{
        if (kick == NULL || drive == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_drive(kick->synth, drive);
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

        if (kick->synth != NULL)
                return gkick_synth_enable_group(kick->synth, index, enable);
        return GEONKICK_ERROR;
}

enum geonkick_error
geonkick_group_enabled(struct geonkick *kick, size_t index, bool *enabled)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (kick->synth != NULL)
                return gkick_synth_group_enabled(kick->synth, index, enabled);
        return GEONKICK_ERROR;
}

enum geonkick_error
geonkick_group_set_amplitude(struct geonkick *kick, size_t index, gkick_real amplitude)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return geonkick_synth_group_set_amplitude(kick->synth, index, amplitude);
}

enum geonkick_error
geonkick_group_get_amplitude(struct geonkick *kick, size_t index, gkick_real *amplitude)
{
        if (kick == NULL || index < 0 || index > GKICK_OSC_GROUPS_NUMBER - 1
            || amplitude == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return geonkick_synth_group_get_amplitude(kick->synth, index, amplitude);
}

enum geonkick_error
geonkick_tune_audio_output(struct geonkick *kick, bool tune)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (kick->audio == NULL || kick->audio->audio_output == NULL) {
                gkick_log_error("audio output was not created");
                return GEONKICK_ERROR;
        }

        gkick_audio_tune_output(kick->audio->audio_output, tune);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_is_audio_output_tuned(struct geonkick *kick, bool *tune)
{
        if (kick == NULL || tune == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (kick->audio == NULL || kick->audio->audio_output == NULL) {
                gkick_log_error("audio output was not created");
                return GEONKICK_ERROR;
        } else {
                *tune = gkick_audio_is_tune_output(kick->audio->audio_output);
        }

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

        return geonkick_synth_set_osc_sample(kick->synth, osc_index, data, size);
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

        return geonkick_synth_get_osc_sample(kick->synth, osc_index, data, size);
}

