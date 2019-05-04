/**
 * File name: audio_output.c
 * Project: Geonkick (A kick synthesizer)
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

#include "audio_output.h"

enum geonkick_error
gkick_audio_output_create(struct gkick_audio_output **audio_output)
{
        if (audio_output == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *audio_output = (struct gkick_audio_output*)malloc(sizeof(struct gkick_audio_output));
        if (*audio_output == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR;
        }
        memset(*audio_output, 0, sizeof(struct gkick_audio_output));

        gkick_buffer_new(&(*audio_output)->buffer, GEONKICK_MAX_KICK_BUFFER_SIZE);
        if ((*audio_output)->buffer == NULL) {
                gkick_log_error("can't create kick buffer");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
        }

        if (pthread_mutex_init(&(*audio_output)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
	}

        return GEONKICK_OK;
}

void gkick_audio_output_free(struct gkick_audio_output **audio_output)
{
        if (audio_output != NULL && *audio_output != NULL) {
                gkick_buffer_free(&(*audio_output)->buffer);
                pthread_mutex_destroy(&(*audio_output)->lock);
                free(*audio_output);
                *audio_output = NULL;
        }
}

enum geonkick_error
gkick_audio_output_key_pressed(struct gkick_audio_output *audio_output,
                               enum gkick_key_state pressed,
                               int velocity)
{
        gkick_audio_output_lock(audio_output);
        audio_output->key_state = pressed;
        if (audio_output->key_state == GKICK_KEY_STATE_PRESSED) {
                audio_output->is_play = 1;
                audio_output->buffer_index = 0;
                audio_output->key_velocity = velocity;
        } else {
                audio_output->decay = GEKICK_KEY_RELESE_DECAY_TIME;
        }
        gkick_audio_output_unlock(audio_output);
        return GEONKICK_OK;
}

/**
 * Get autio frame gets only the current value of the kick from the kick buffer.
 * Not synthesis is done by this function.
 */
enum geonkick_error
gkick_audio_output_get_frame(struct gkick_audio_output *audio_output, gkick_real *val)
{
        size_t is_end;
        int release_time = GEKICK_KEY_RELESE_DECAY_TIME;
        gkick_real decay_val;

        if (!audio_output->is_play) {
                *val = 0.0;
        } else {
                gkick_audio_output_lock(audio_output);
                /* Get the value from the kick buffer at the current index. */
                *val = gkick_buffer_get_at(audio_output->buffer, audio_output->buffer_index, &is_end);
                if (is_end) {
                        audio_output->buffer_index = 0;
                        audio_output->is_play = 0;
                } else {
                        if (audio_output->key_state == GKICK_KEY_STATE_RELEASED) {
                                decay_val = - 1.0 * ((gkick_real)(release_time - audio_output->decay) / release_time) + 1.0;
                        } else {
                                decay_val = 1.0;
                        }
                        *val *= decay_val * ((gkick_real)audio_output->key_velocity / 127);
                        audio_output->buffer_index++;
                }

                if (audio_output->key_state == GKICK_KEY_STATE_RELEASED) {
                        audio_output->decay--;
                        if (audio_output->decay < 0) {
                                audio_output->is_play = 0;
                        }
                }
                gkick_audio_output_unlock(audio_output);
        }

        gkick_audio_output_lock(audio_output);
        *val *= audio_output->limiter;
        if (audio_output->limiter_callback != NULL
            && audio_output->limiter_callback_arg != NULL
            && audio_output->key_state == GKICK_KEY_STATE_PRESSED) {
                /* Callback is atomic. It sets the value and returns. */
                audio_output->limiter_callback(audio_output->limiter_callback_arg, *val);
        }
        gkick_audio_output_unlock(audio_output);

        return GEONKICK_OK;
}

void gkick_audio_output_lock(struct gkick_audio_output *audio_output)
{
        if (audio_output != NULL) {
                pthread_mutex_lock(&audio_output->lock);
        }
}

void gkick_audio_output_unlock(struct gkick_audio_output *audio_output)
{
        if (audio_output != NULL) {
                pthread_mutex_unlock(&audio_output->lock);
        }
}

struct gkick_buffer*
gkick_audio_output_get_buffer(struct gkick_audio_output  *audio_output)
{
        return audio_output->buffer;
}

enum geonkick_error
gkick_audio_output_set_limiter(struct gkick_audio_output  *audio_output, gkick_real limit)
{
        gkick_audio_output_lock(audio_output);
        audio_output->limiter = limit;
        gkick_audio_output_unlock(audio_output);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_get_limiter(struct gkick_audio_output  *audio_output, gkick_real *limit)
{
        gkick_audio_output_lock(audio_output);
        *limit = audio_output->limiter;
        gkick_audio_output_unlock(audio_output);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_set_limiter_callback(struct gkick_audio_output *audio_output,
                                        void (*callback)(void*, gkick_real val),
                                        void *arg)
{
        if (audio_output == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_audio_output_lock(audio_output);
        audio_output->limiter_callback = callback;
        audio_output->limiter_callback_arg = arg;
        gkick_audio_output_unlock(audio_output);
        return GEONKICK_OK;
}
