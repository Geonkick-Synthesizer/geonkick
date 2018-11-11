/**
 * File name: gkick_audio.c
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "gkick_audio.h"
#include "gkick_jack.h"

enum geonkick_error
gkick_audio_create(struct gkick_audio** audio)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *audio = (struct gkick_audio*)malloc(sizeof(struct gkick_audio));
        if (*audio == NULL) {
                gkick_log_error("can't allocate memory");
		return GEONKICK_ERROR_MEM_ALLOC;
	}
        *audio->jack         = NULL;
        *audio->audio_outptut = NULL;

        if (gkick_audio_output_create(*audio) != GEONKICK_OK) {
                gkick_log_error("can't create audio output");
                gkick_audo_free(audio);
                return GEONKICK_ERROR;
        }

#ifdef GEONKICK_AUDIO_JACK
        if (gkick_create_jack(*audio) != GEONKICK_OK) {
                gkick_log_warning("can't create jack");
        }
#endif
        return GEONKICK_OK;
}

void gkick_audio_free(struct gkick_audio** audio)
{
        if (audio != NULL && *audio != NULL) {
                gkick_jack_free(&(*audio)->jack);
                gkick_audio_output_free(&(*audio)->audio_output);
                free(*audio);
                *audio = NULL;
        }
}

enum geonkick_error
gkick_audio_set_limiter_val(struct gkick_audio *audio, gkick_real limit)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        if (limit < 0) {
                limit = 0;
        } else if (limit > 1.0) {
                limit = 1.0;
        }

        return gkick_audio_output_set_limiter(audio->audio_output, limit);
}

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *audio, gkick_real *limit)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        return gkick_audio_output_get_limiter(audio->audio_ouptut, limit);
}

sutrct gkick_buffer*
gkick_audio_get_buffer(struct gkick_audio *adio)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        return gkick_audio_output_get_buffer(audio->audio_output);
}

enum geonkick_error
gkick_audio_key_pressed(struct gkick_audio *audio, int pressed, int velocity)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        gkick_audio_output_key_pressed(audio->audio_output, pressed, velocity);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_get_frame(struct gkick_audio *audio, gkick_real *val)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_audio_output_get_frame(audio->audio_output, val);
}

/**
 * Audio output module functions definition
 */
enum geonkick_error
gkick_audio_output_key_pressed(struct gkick_audio_output *audio_output, pressed, velocity)
{
        gkick_audio_output_lock(audio_output);
        if (pressed) {
                audio_output->key_state = GKICK_KEY_STATE_PRESSED;
                audio_output->is_play = 1;
                audio_output->buffer_index = 0;
        } else {
                audio_output->key_state = GKICK_KEY_STATE_RELESED;
                audio_output->decay = GEKICK_NOTE_RELEASE_TIME;
        }
        audio_output->key_velocity = velocity;
        gkick_audio_output_unlock(audio_output);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_get_frame(struct gkick_audio_output *audio_output, gkick_real *val)
{
        gkick_real val;
        int is_end;
        int release_time = GEKICK_NOTE_RELEASE_TIME;
        gkick_audio_output_lock(audio_output);
        if (!audio_output->is_play) {
                val = 0.0;
        } else {
                val = gkick_buffer_get_at(audio_output->buffer, jack->buffer_index, &is_end);
                if (is_end) {
                        audio_output->buffer_index = 0;
                        audio_output->is_play = 0;
                } else {
                        if (audio_output->key_state == GKICK_KEY_STATE_RELEASED) {
                                audio_output->decay = - 1.0 * ((gkick_real)(release_time - audio_output->decay) / release_time) + 1.0;
                        } else {
                                audio_output->decay = 1.0;
                        }
                        val *= audio_output->decay * ((gkick_real)audio_output->key_velocity / 127);
                        audio_output->buffer_index++;
                }

                if (audio_output->key_state == GKICK_KEY_STATE_RELEASED) {
                        audio_output->decay--;
                        if (audio_output->decay < 0) {
                                audio_output->is_play = 0;
                        }
                }
        }
        gkick_audio_output_unlock(audio_output);
}
