/**
 * File name: geonkick.c
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

#include "geonkick_internal.h"
#include "synthesizer.h"
#include "gkick_audio.h"
#include "audio_output.h"
#include "envelope.h"
#include "mixer.h"

#include <unistd.h>
#include <stdlib.h>

enum geonkick_error
geonkick_create(struct geonkick **kick, int sample_rate)
{
	if (kick == NULL)
		return GEONKICK_ERROR;

	*kick = (struct geonkick*)calloc(1, sizeof(struct geonkick));
	if (*kick == NULL)
		return GEONKICK_ERROR_MEM_ALLOC;
	strcpy((*kick)->name, "Geonkick");
	(*kick)->sample_rate = sample_rate;
        (*kick)->synthesis_on = false;
        (*kick)->per_index = 0;

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

	if (gkick_audio_create(&(*kick)->audio, sample_rate) != GEONKICK_OK) {
                gkick_log_warning("can't create audio");
		geonkick_free(kick);
		return GEONKICK_ERROR;
	}

        (*kick)->sample_rate = (*kick)->audio->sample_rate;

        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                if (gkick_synth_new(&(*kick)->synths[i], (*kick)->sample_rate) != GEONKICK_OK) {
                        gkick_log_error("can't create synthesizer %u", i);
                        geonkick_free(kick);
                        return GEONKICK_ERROR;
                }
                (*kick)->synths[i]->id = i;
        }

        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                gkick_synth_set_output((*kick)->synths[i], (*kick)->audio->audio_outputs[i]);
                geonkick_set_percussion_channel(*kick, i, i % GEONKICK_MAX_CHANNELS);
        }

        if (!geonkick_worker_created()) {
                if (geonkick_worker_create() != GEONKICK_OK) {
                        gkick_log_error("can't init worker");
                        geonkick_free(kick);
                        return GEONKICK_ERROR;
                }

                if (geonkick_worker_start() != GEONKICK_OK) {
                        gkick_log_error("can't start worker");
                        geonkick_free(kick);
                        return GEONKICK_ERROR;
                }
        }
        geonkick_worker_add_instance(*kick);

        if (gkick_start_audio((*kick)->audio) != GEONKICK_OK) {
                gkick_log_error("can't start audio module");
                geonkick_free(kick);
        }

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
        if (kick != NULL && *kick != NULL) {
                geonkick_worker_remove_instance(*kick);
                if (geonkick_worker_reference_count() == 0)
                        geonkick_worker_destroy();
                gkick_log_debug("ref count: %d", geonkick_worker_reference_count());
                for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++)
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
geonkick_enable_oscillator(struct geonkick* kick, size_t index)
{
        if (kick == NULL)
                return GEONKICK_ERROR;
        enum geonkick_error res;
        res = gkick_synth_enable_oscillator(kick->synths[kick->per_index], index, 1);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
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
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick, size_t index, int *enabled)
{
        if (kick == NULL || enabled == NULL)
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
        res = gkick_synth_osc_env_add_point(kick->synths[kick->per_index],
                                            osc_index,
                                            env_index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
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

        return gkick_synth_osc_envelope_points(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               buf,
                                               npoints);
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
                geonkick_wakeup(kick);
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
        res = gkick_synth_osc_env_update_point(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);

        return res;
}

enum geonkick_error
geonkick_osc_envelope_set_apply_type(struct geonkick *kick, 
				     size_t osc_index,
				     size_t env_index,
				     enum gkick_envelope_apply_type apply_type)
{
	if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = synth_osc_env_set_apply_type(kick->synths[kick->per_index],
					   osc_index,
					   env_index,
					   apply_type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_osc_envelope_get_apply_type(struct geonkick *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum gkick_envelope_apply_type *apply_type)
{
        if (kick == NULL || apply_type == NULL)
                return GEONKICK_ERROR;
        return synth_osc_env_get_apply_type(kick->synths[kick->per_index],
					    osc_index,
					    env_index,
					    apply_type);
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
                geonkick_wakeup(kick);
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
                geonkick_wakeup(kick);
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
                geonkick_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_get_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real *phase)
{
        if (kick == NULL || phase == NULL)
                return GEONKICK_ERROR;

        return gkick_synth_get_osc_phase(kick->synths[kick->per_index],
                                         osc_index,
                                         phase);
}

enum geonkick_error
geonkick_set_osc_seed(struct geonkick *kick,
                      size_t osc_index,
                      unsigned int seed)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }


        enum geonkick_error res;
        res = gkick_synth_set_osc_seed(kick->synths[kick->per_index],
                                       osc_index,
                                       seed);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_seed(struct geonkick *kick,
                      size_t osc_index,
                      unsigned int *seed)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_get_osc_seed(kick->synths[kick->per_index],
                                        osc_index,
                                        seed);

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
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick,
                    gkick_real *len)
{
        if (kick == NULL || len == NULL)
                return GEONKICK_ERROR;
        return gkick_synth_get_length(kick->synths[kick->per_index], len);
}

enum geonkick_error
geonkick_get_max_length(struct geonkick *kick,
                        gkick_real *len)
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
geonkick_kick_set_amplitude(struct geonkick *kick,
                            gkick_real amplitude)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_kick_set_amplitude(kick->synths[kick->per_index], amplitude);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_get_amplitude(struct geonkick *kick,
                            gkick_real *amplitude)
{
        if (kick == NULL || amplitude == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_amplitude(kick->synths[kick->per_index], amplitude);
}

enum geonkick_error
geonkick_kick_set_filter_frequency(struct geonkick *kick,
                                   gkick_real frequency)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_kick_set_filter_frequency(kick->synths[kick->per_index], frequency);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_filter_enable(struct geonkick *kick,
                            int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = geonkick_synth_kick_filter_enable(kick->synths[kick->per_index], enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_filter_is_enabled(struct geonkick *kick,
                                int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return geonkick_synth_kick_filter_is_enabled(kick->synths[kick->per_index],
                                                     enabled);
}

enum geonkick_error
geonkick_kick_get_filter_frequency(struct geonkick *kick,
                                   gkick_real *frequency)
{
        if (kick == NULL || frequency == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_frequency(kick->synths[kick->per_index],
                                                     frequency);
}

enum geonkick_error
geonkick_kick_set_filter_factor(struct geonkick *kick,
                                gkick_real factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_set_filter_factor(kick->synths[kick->per_index], factor);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_get_filter_factor(struct geonkick *kick,
                                gkick_real *factor)
{
        if (kick == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_get_filter_factor(kick->synths[kick->per_index], factor);
}

enum geonkick_error
geonkick_set_kick_filter_type(struct geonkick *kick,
                              enum gkick_filter_type type)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_set_kick_filter_type(kick->synths[kick->per_index], type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_kick_filter_type(struct geonkick *kick,
                              enum gkick_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
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
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_kick_envelope_get_points(kick->synths[kick->per_index],
                                                    env_type,
                                                    buf,
                                                    npoints);
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
        res = gkick_synth_kick_envelope_set_points(kick->synths[kick->per_index],
                                                   env_type,
                                                   buff,
                                                   npoints);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_kick_add_env_point(struct geonkick *kick,
                            enum geonkick_envelope_type env_type,
                            gkick_real x,
                            gkick_real y)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_add_env_point(kick->synths[kick->per_index],
                                             env_type,
                                             x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_remove_env_point(struct geonkick *kick,
                               enum geonkick_envelope_type env_type,
                               size_t index)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_remove_env_point(kick->synths[kick->per_index],
                                                env_type,
                                                index);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
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
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_kick_update_env_point(kick->synths[kick->per_index],
                                                env_type,
                                                index, x, y);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_env_set_apply_type(struct geonkick *kick, 
				 enum geonkick_envelope_type env_type,
				 enum gkick_envelope_apply_type apply_type)
{
	if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = synth_kick_env_set_apply_type(kick->synths[kick->per_index],
					    env_type,
                                            apply_type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_kick_env_get_apply_type(struct geonkick *kick,
				 enum geonkick_envelope_type env_type,
				 enum gkick_envelope_apply_type *apply_type)
{
	if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return synth_kick_env_get_apply_type(kick->synths[kick->per_index],
					     env_type,
					     apply_type);
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
        res = gkick_synth_set_osc_frequency(kick->synths[kick->per_index],
                                            osc_index,
                                            v);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_set_osc_pitch_shift(struct geonkick *kick,
                             size_t osc_index,
                             gkick_real semitones)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

        enum geonkick_error res;
        res = gkick_synth_set_osc_pitch_shift(kick->synths[kick->per_index],
                                              osc_index,
                                              semitones);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
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
        res = gkick_synth_set_osc_amplitude(kick->synths[kick->per_index],
                                            osc_index,
                                            v);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
        return gkick_synth_get_osc_amplitude(kick->synths[kick->per_index],
                                             osc_index,
                                             v);
}

enum geonkick_error
geonkick_get_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v)
{
	if (kick == NULL || v == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_get_osc_frequency(kick->synths[kick->per_index],
                                             osc_index,
                                             v);
}

enum geonkick_error
geonkick_get_osc_pitch_shift(struct geonkick *kick,
                             size_t osc_index,
                             gkick_real *semitones)
{
	if (kick == NULL || semitones == NULL)
		return GEONKICK_ERROR;
	return gkick_synth_get_osc_pitch_shift(kick->synths[kick->per_index],
                                               osc_index,
                                               semitones);
}

enum geonkick_error
geonkick_play(struct geonkick *kick, size_t id)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_play(kick->audio, id);
}

enum geonkick_error
geonkick_key_pressed(struct geonkick *kick,
                     bool pressed,
                     int note,
                     int velocity)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_key_pressed(kick->audio,
                                       pressed,
                                       note,
                                       velocity);
}

enum geonkick_error
geonkick_get_kick_buffer_size(struct geonkick *kick, size_t *size)
{
        enum geonkick_error res;
        if (kick  == NULL || size == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        res = gkick_synth_get_buffer_size(kick->synths[kick->per_index],
                                          size);
        return res;
}

enum geonkick_error
geonkick_get_kick_buffer(struct geonkick *kick,
                         gkick_real *buffer,
                         size_t size)
{
        enum geonkick_error res;
        if (kick  == NULL || buffer == NULL || size < 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        res = gkick_synth_get_buffer(kick->synths[kick->per_index],
                                     buffer,
                                     size);
        return res;
}

enum geonkick_error
geonkick_set_kick_buffer_callback(struct geonkick *kick,
                                  void (*callback)(void*,
                                                   gkick_real* buff,
                                                   size_t size,
                                                   size_t id),
                                  void *arg)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	geonkick_lock(kick);
        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                kick->synths[i]->buffer_callback = callback;
                kick->synths[i]->callback_args = arg;
        }
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_kick_limiter_callback(struct geonkick *kick,
                                   void (*callback)(void*,
                                                    size_t index,
                                                    gkick_real val),
                                   void *arg)
{
        if (kick  == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_set_limiter_callback(kick->audio,
                                                callback,
                                                arg);
}

enum geonkick_error
geonkick_set_limiter_value(struct geonkick *kick,
                           gkick_real limit)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_set_limiter_val(kick->audio,
                                           kick->per_index,
                                           limit);
}

enum geonkick_error
geonkick_get_limiter_value(struct geonkick *kick,
                           gkick_real *limit)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_get_limiter_val(kick->audio,
                                           kick->per_index,
                                           limit);
}

enum geonkick_error
geonkick_set_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type type)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_type(kick->synths[kick->per_index],
                                              osc_index,
                                              type);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_get_osc_filter_type(kick->synths[kick->per_index],
                                               osc_index,
                                               type);
}

enum geonkick_error
geonkick_set_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real cutoff)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_cutoff(kick->synths[kick->per_index],
                                                osc_index,
                                                cutoff);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real *cutoff)
{
        if (kick == NULL || cutoff == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_get_osc_filter_cutoff(kick->synths[kick->per_index],
                                                 osc_index,
                                                 cutoff);
}

enum geonkick_error
geonkick_set_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real factor)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_set_osc_filter_factor(kick->synths[kick->per_index],
                                                osc_index,
                                                factor);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_get_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real *factor)
{
        if (kick == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_get_osc_filter_factor(kick->synths[kick->per_index],
                                                 osc_index,
                                                 factor);
}


enum geonkick_error
geonkick_enbale_osc_filter(struct geonkick *kick,
                           size_t osc_index,
                           int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_osc_enable_filter(kick->synths[kick->per_index],
                                            osc_index,
                                            enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_osc_filter_is_enabled(struct geonkick *kick,
                               size_t osc_index,
                               int *enable)
{
        if (kick == NULL || enable == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_osc_is_enabled_filter(kick->synths[kick->per_index],
                                                 osc_index,
                                                 enable);
}

enum geonkick_error
geonkick_get_sample_rate(struct geonkick *kick,
                         int *sample_rate)
{
        GEONKICK_UNUSED(kick);

        if (sample_rate == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        *sample_rate = kick->sample_rate;
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_enable_synthesis(struct geonkick *kick,
                          bool enable)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	kick->synthesis_on = enable;
        if (kick->synthesis_on) {
                for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                        if (kick->synths[i]->is_active)
                                kick->synths[i]->buffer_update = true;
                }
                geonkick_wakeup(kick);
        }
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_audio_process(struct geonkick *kick,
                       float **out,
                       size_t channel,
                       size_t size)
{
        return gkick_audio_process(kick->audio,
                                   out,
                                   channel,
                                   size);
}

enum geonkick_error
geonkick_compressor_enable(struct geonkick *kick,
                           int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_enable(kick->synths[kick->per_index],
                                            enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_compressor_is_enabled(struct geonkick *kick,
                               int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_is_enabled(kick->synths[kick->per_index],
                                                 enabled);
}

enum geonkick_error
geonkick_compressor_set_attack(struct geonkick *kick,
                               gkick_real attack)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_attack(kick->synths[kick->per_index],
                                                attack);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_attack(struct geonkick *kick,
                               gkick_real *attack)
{
        if (kick == NULL || attack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_attack(kick->synths[kick->per_index],
                                                 attack);
}

enum geonkick_error
geonkick_compressor_set_release(struct geonkick *kick,
                                gkick_real release)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_release(kick->synths[kick->per_index],
                                                 release);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_release(struct geonkick *kick,
                                gkick_real *release)
{
        if (kick == NULL || release == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_release(kick->synths[kick->per_index],
                                                  release);
}

enum geonkick_error
geonkick_compressor_set_threshold(struct geonkick *kick,
                                  gkick_real threshold)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_threshold(kick->synths[kick->per_index],
                                                   threshold);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_threshold(struct geonkick *kick,
                                  gkick_real *threshold)
{
        if (kick == NULL || threshold == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_threshold(kick->synths[kick->per_index],
                                                    threshold);
}

enum geonkick_error
geonkick_compressor_set_ratio(struct geonkick *kick,
                              gkick_real ratio)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_ratio(kick->synths[kick->per_index],
                                               ratio);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_ratio(struct geonkick *kick,
                              gkick_real *ratio)
{
        if (kick == NULL || ratio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_ratio(kick->synths[kick->per_index],
                                                ratio);
}

enum geonkick_error
geonkick_compressor_set_knee(struct geonkick *kick,
                             gkick_real knee)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_knee(kick->synths[kick->per_index],
                                              knee);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_compressor_get_knee(struct geonkick *kick,
                             gkick_real *knee)
{
        if (kick == NULL || knee == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_knee(kick->synths[kick->per_index], knee);
}

enum geonkick_error
geonkick_compressor_set_makeup(struct geonkick *kick,
                               gkick_real makeup)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_compressor_set_makeup(kick->synths[kick->per_index],
                                                makeup);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;

}

enum geonkick_error
geonkick_compressor_get_makeup(struct geonkick *kick,
                               gkick_real *makeup)
{
        if (kick == NULL || makeup == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_compressor_get_makeup(kick->synths[kick->per_index],
                                                 makeup);
}

enum geonkick_error
geonkick_distortion_enable(struct geonkick *kick, int enable)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_enable(kick->synths[kick->per_index],
                                            enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_is_enabled(struct geonkick *kick,
                               int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_is_enabled(kick->synths[kick->per_index],
                                                 enabled);
}

enum geonkick_error
geonkick_distortion_set_in_limiter(struct geonkick *kick,
                                   gkick_real limit)
{
	if (kick == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}

	enum geonkick_error res;
	res = gkick_synth_distortion_set_in_limiter(kick->synths[kick->per_index],
                                                    limit);
	if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
	return res;
}

enum geonkick_error
geonkick_distortion_get_in_limiter(struct geonkick *kick,
                                   gkick_real *limit)
{
	if (kick == NULL || limit == NULL) {
		gkick_log_error("wrong arguments");
		return GEONKICK_ERROR;
	}
	return gkick_synth_distortion_get_in_limiter(kick->synths[kick->per_index],
                                                     limit);
}

enum geonkick_error
geonkick_distortion_set_volume(struct geonkick *kick,
                               gkick_real volume)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_set_volume(kick->synths[kick->per_index],
                                                volume);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_get_volume(struct geonkick *kick, gkick_real *volume)
{
        if (kick == NULL || volume == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_volume(kick->synths[kick->per_index],
                                                 volume);
}

enum geonkick_error
geonkick_distortion_set_drive(struct geonkick *kick,
                              gkick_real drive)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        enum geonkick_error res;
        res = gkick_synth_distortion_set_drive(kick->synths[kick->per_index],
                                               drive);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_distortion_get_drive(struct geonkick *kick,
                              gkick_real *drive)
{
        if (kick == NULL || drive == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_synth_distortion_get_drive(kick->synths[kick->per_index],
                                                drive);
}

int geonkick_is_module_enabed(struct geonkick *kick,
                              enum GEONKICK_MODULE module)
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
        default:
                return 0;
        }
}

enum geonkick_error
geonkick_enable_group(struct geonkick *kick,
                      size_t index,
                      bool enable)
{
        if (kick == NULL || index >= GKICK_OSC_GROUPS_NUMBER) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = gkick_synth_enable_group(kick->synths[kick->per_index],
                                        index,
                                        enable);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_group_enabled(struct geonkick *kick,
                       size_t index,
                       bool *enabled)
{
        if (kick == NULL || enabled == NULL
            || index >= GKICK_OSC_GROUPS_NUMBER) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_synth_group_enabled(kick->synths[kick->per_index],
                                         index,
                                         enabled);
}

enum geonkick_error
geonkick_group_set_amplitude(struct geonkick *kick,
                             size_t index,
                             gkick_real amplitude)
{
        if (kick == NULL || index >= GKICK_OSC_GROUPS_NUMBER) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = geonkick_synth_group_set_amplitude(kick->synths[kick->per_index],
                                                  index,
                                                  amplitude);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
        return res;
}

enum geonkick_error
geonkick_group_get_amplitude(struct geonkick *kick,
                             size_t index,
                             gkick_real *amplitude)
{
        if (kick == NULL
            || index >= GKICK_OSC_GROUPS_NUMBER
            || amplitude == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return geonkick_synth_group_get_amplitude(kick->synths[kick->per_index],
                                                  index,
                                                  amplitude);
}

enum geonkick_error
geonkick_tune_audio_output(struct geonkick *kick,
                           size_t index,
                           bool tune)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_mixer_tune_output(kick->audio->mixer,
                                       index,
                                       tune);
}

enum geonkick_error
geonkick_is_audio_output_tuned(struct geonkick *kick,
                               size_t index,
                               bool *tune)
{
        if (kick == NULL || tune == NULL
            || index >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	return gkick_mixer_is_output_tuned(kick->audio->mixer,
                                           index,
                                           tune);
}

enum geonkick_error
geonkick_set_osc_sample(struct geonkick *kick,
                        size_t osc_index,
                        const gkick_real *data,
                        size_t size)
{
        if (kick == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        enum geonkick_error res;
        res  = geonkick_synth_set_osc_sample(kick->synths[kick->per_index],
                                             osc_index,
                                             data, size);
        if (res == GEONKICK_OK && kick->synths[kick->per_index]->buffer_update)
                geonkick_wakeup(kick);
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

        return geonkick_synth_get_osc_sample(kick->synths[kick->per_index],
                                             osc_index,
                                             data,
                                             size);
}

enum geonkick_error
geonkick_set_current_percussion(struct geonkick *kick,
                                size_t index)
{
        if (kick == NULL || index >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        kick->per_index = index;
        kick->audio->mixer->limiter_callback_index = index;
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_current_percussion(struct geonkick *kick,
                                size_t *index)
{
        if (kick == NULL || index == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	*index = kick->per_index;
	return GEONKICK_OK;
}

void geonkick_process(struct geonkick *kick)
{
        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                struct gkick_synth *synth = kick->synths[i];
                if (synth != NULL && synth->is_active && synth->buffer_update)
                        gkick_synth_process(synth);
        }
}

/**
 * Finds the first unused/free percussion index.
 */
enum geonkick_error
geonkick_unused_percussion(struct geonkick *kick,
                           int *index)
{
        if (kick == NULL || index == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

	*index = -1;
        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                if (!kick->synths[i]->is_active) {
                        *index = i;
                        return GEONKICK_OK;
                }
        }
        return GEONKICK_ERROR;
}

enum geonkick_error
geonkick_enable_percussion(struct geonkick *kick,
                           size_t index,
                           bool enable)
{
        if (kick == NULL || index >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        kick->synths[index]->is_active  = enable;
        kick->audio->audio_outputs[index]->enabled = enable;
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_is_percussion_enabled(struct geonkick *kick,
                               size_t index,
                               bool *enable)
{
        if (kick == NULL || enable == NULL
            || index >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *enable = kick->synths[index]->is_active;
        return GEONKICK_OK;
}


size_t geonkick_percussion_number()
{
	return GEONKICK_MAX_INSTRUMENTS;
}

enum geonkick_error
geonkick_set_playing_key(struct geonkick *kick,
                         size_t id,
                         signed char key)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_set_playing_key(kick->synths[id]->output, key);
}

enum geonkick_error
geonkick_get_playing_key(struct geonkick *kick, size_t id, signed char *key)
{
        if (kick == NULL || key == NULL
            || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_get_playing_key(kick->synths[id]->output,
                                                  key);
}

enum geonkick_error
geonkick_set_percussion_name(struct geonkick *kick,
                             size_t id,
                             const char *name,
                             size_t size)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS
            || name == NULL || size < 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        struct gkick_synth *synth = kick->synths[id];
        gkick_synth_lock(synth);
        memset(synth->name, '\0', sizeof(synth->name));
        if (size < strlen(synth->name))
                strcpy(synth->name, name);
        else
                strncpy(synth->name, name, sizeof(synth->name) - 1);
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_percussion_name(struct geonkick *kick,
                             size_t id,
                             char *name,
                             size_t size)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS
            || name == NULL || size < 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        struct gkick_synth *synth = kick->synths[id];
        gkick_synth_lock(synth);
        memset(name, '\0', size);
        if (size > strlen(synth->name))
                strcpy(name, synth->name);
        else
                strncpy(name, synth->name, size - 1);
        gkick_synth_unlock(synth);
        return GEONKICK_OK;
}

size_t
geonkick_channels_number()
{
        return GEONKICK_MAX_CHANNELS;
}

enum geonkick_error
geonkick_set_percussion_channel(struct geonkick *kick,
                                size_t id,
                                size_t channel)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS
            || channel >= GEONKICK_MAX_CHANNELS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_set_channel(kick->synths[id]->output, channel);
}

enum geonkick_error
geonkick_get_percussion_channel(struct geonkick *kick,
                                size_t id,
                                size_t *channel)
{
        if (kick == NULL || channel == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_get_channel(kick->synths[id]->output,
                                              channel);
}

enum geonkick_error
geonkick_percussion_set_limiter(struct geonkick *kick,
                                size_t id,
                                gkick_real val)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_limiter_set(kick->audio->mixer, id, val);
}

enum geonkick_error
geonkick_percussion_get_limiter(struct geonkick *kick,
                                size_t id,
                                gkick_real *val)
{
        if (kick == NULL || val == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_limiter_get(kick->audio->mixer, id, val);
}

enum geonkick_error
geonkick_percussion_mute(struct geonkick *kick,
                         size_t id,
                         bool b)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_mute(kick->audio->mixer, id, b);
}

enum geonkick_error
geonkick_percussion_is_muted(struct geonkick *kick,
                             size_t id,
                             bool *b)
{
        if (kick == NULL || b == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_is_muted(kick->audio->mixer, id, b);
}

enum geonkick_error
geonkick_percussion_solo(struct geonkick *kick,
                         size_t id,
                         bool b)
{
        if (kick == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_solo(kick->audio->mixer, id, b);
}

enum geonkick_error
geonkick_percussion_is_solo(struct geonkick *kick,
                            size_t id,
                            bool *b)
{
        if (kick == NULL || b == NULL || id >= GEONKICK_MAX_INSTRUMENTS) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_mixer_is_solo(kick->audio->mixer, id, b);
}

enum geonkick_error
geonkick_set_preview_sample(struct geonkick *kick,
                            const gkick_real *data,
                            size_t size)
{
        if (kick == NULL || data == NULL || size < 1) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        struct gkick_audio_output *output = kick->audio->mixer->audio_outputs[GEONKICK_MAX_INSTRUMENTS];
        gkick_audio_output_lock(output);
        gkick_buffer_set_data((struct gkick_buffer*)output->updated_buffer, data, size);
        gkick_audio_output_unlock(output);
        return GEONKICK_OK;
}

void
geonkick_play_sample_preview(struct geonkick *kick)
{
        gkick_audio_play(kick->audio, GEONKICK_MAX_INSTRUMENTS);
}

enum geonkick_error
geonkick_set_sample_preview_limiter(struct geonkick *kick, gkick_real val)
{
        return gkick_audio_set_limiter_val(kick->audio, GEONKICK_MAX_INSTRUMENTS, val);
}

enum geonkick_error
geonkick_get_sample_preview_limiter(struct geonkick *kick, gkick_real *val)
{
        return gkick_audio_get_limiter_val(kick->audio, GEONKICK_MAX_INSTRUMENTS, val);
}

void
geonkick_wakeup(struct geonkick *kick)
{
        if (kick->synthesis_on)
                geonkick_worker_wakeup();
}

bool
geonkick_is_plugin()
{
        return !GKICK_IS_STANDALONE;
}

void
geonkick_usleep(unsigned long int usecods)
{
        usleep(usecods);
}


int geonkick_rand(unsigned int *seed)
{
#ifdef GEONKICK_OS_WINDOWS
        GEONKICK_UNUSED(seed);
        // Use rand & srand since the calls are only from a single thread.
        return rand();
#else
        return rand_r(seed);
#endif // GEONKICK_OS_GNU
}

