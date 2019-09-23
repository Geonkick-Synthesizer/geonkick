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
        (*audio_output)->decay = -1;

        gkick_buffer_new((struct gkick_buffer**)&(*audio_output)->updated_buffer, GEONKICK_MAX_KICK_BUFFER_SIZE);
        if ((*audio_output)->updated_buffer == NULL) {
                gkick_log_error("can't create updated buffer");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
        }
        gkick_buffer_set_size((struct gkick_buffer*)(*audio_output)->updated_buffer, 0);

        gkick_buffer_new((struct gkick_buffer**)&(*audio_output)->playing_buffer, GEONKICK_MAX_KICK_BUFFER_SIZE);
        if ((*audio_output)->playing_buffer == NULL) {
                gkick_log_error("can't create playing buffer");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
        }
        gkick_buffer_set_size((struct gkick_buffer*)(*audio_output)->playing_buffer, 0);

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
                struct gkick_buffer *p = (struct gkick_buffer*)((*audio_output)->playing_buffer);
                gkick_buffer_free(&p);
                p = (struct gkick_buffer*)((*audio_output)->updated_buffer);
                gkick_buffer_free(&p);
                pthread_mutex_destroy(&(*audio_output)->lock);
                free(*audio_output);
                *audio_output = NULL;
        }
}

enum geonkick_error
gkick_audio_output_key_pressed(struct gkick_audio_output *audio_output,
                               struct gkick_note_info *key)
{
        if (key->state == GKICK_KEY_STATE_PRESSED) {
                audio_output->key = *key;
                audio_output->is_play = true;
                gkick_audio_swap_buffers(audio_output);
        } else {
                audio_output->decay = GEKICK_KEY_RELESE_DECAY_TIME;
                audio_output->key.state = key->state;
        }

        return GEONKICK_OK;
}

gkick_real
gkick_audio_output_tune_factor(int note_number)
{
        return pow(2.0, (gkick_real)(note_number - 69) / 12.0);
}

/**
 * Gets autio frame gets only the current value
 * of the kick from the kick buffer.
 * Not synthesis is done by this function. There is no lock called in this function.
 */
enum geonkick_error
gkick_audio_output_get_frame(struct gkick_audio_output *audio_output, gkick_real *val)
{
        int release_time = GEKICK_KEY_RELESE_DECAY_TIME;
        gkick_real decay_val;

        *val = 0;
        if (audio_output->is_play) {
                if (gkick_buffer_is_end((struct gkick_buffer*)audio_output->playing_buffer)) {
                        audio_output->is_play = false;
                } else {
                        struct gkick_buffer *buff = (struct gkick_buffer*)audio_output->playing_buffer;
                        gkick_real factor = gkick_audio_output_tune_factor(audio_output->key.note_number);
                        if (audio_output->tune)
                                *val = gkick_buffer_stretch_get_next(buff, factor);
                        else
                                *val = gkick_buffer_get_next(buff);

                        if (gkick_buffer_size(buff) - gkick_buffer_index(buff) == GEKICK_KEY_RELESE_DECAY_TIME) {
                                audio_output->decay = GEKICK_KEY_RELESE_DECAY_TIME;
                                audio_output->key.state = GKICK_KEY_STATE_RELEASED;
                        }

                        if (audio_output->key.state == GKICK_KEY_STATE_RELEASED)
                                decay_val = - 1.0 * ((gkick_real)(release_time - audio_output->decay) / release_time) + 1.0;
                        else
                                decay_val = 1.0;
                        *val *= decay_val * ((gkick_real)audio_output->key.velocity / 127);

                        if (audio_output->key.state == GKICK_KEY_STATE_RELEASED) {
                                audio_output->decay--;
                                if (audio_output->decay < 0)
                                        audio_output->is_play = false;
                        }
                }
        }

        *val *= (gkick_real)audio_output->limiter / 1000000;
        if (audio_output->limiter_callback != NULL
            && audio_output->limiter_callback_arg != NULL) {
                /* Callback is atomic. It sets the value and returns. */
                audio_output->limiter_callback(audio_output->limiter_callback_arg, *val);
        }

        return GEONKICK_OK;
}

void gkick_audio_output_lock(struct gkick_audio_output *audio_output)
{
        if (audio_output != NULL)
                pthread_mutex_lock(&audio_output->lock);
}

void gkick_audio_output_unlock(struct gkick_audio_output *audio_output)
{
        if (audio_output != NULL)
                pthread_mutex_unlock(&audio_output->lock);
}

struct gkick_buffer*
gkick_audio_output_get_buffer(struct gkick_audio_output  *audio_output)
{
        return (struct gkick_buffer*)audio_output->playing_buffer;
}

enum geonkick_error
gkick_audio_output_set_limiter(struct gkick_audio_output  *audio_output, gkick_real limit)
{
        audio_output->limiter = limit * 1000000;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_get_limiter(struct gkick_audio_output  *audio_output, gkick_real *limit)
{
        int val = audio_output->limiter;
        *limit = (gkick_real)val / 1000000;
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

void gkick_audio_swap_buffers(struct gkick_audio_output *audio_output)
{
        gkick_buffer_reset((struct gkick_buffer*)audio_output->playing_buffer);
        // Try lock. If succesfull, swap buffers. If not, continue with the current one to play
        // until the next key press (i.e. next bit).
        if (pthread_mutex_trylock(&audio_output->lock) == 0) {
                // Test if the updated buffer is full. Otherwise it means that it was not updated.
                if (gkick_buffer_size((struct gkick_buffer*)audio_output->updated_buffer) > 0
                    && gkick_buffer_is_end((struct gkick_buffer*)audio_output->updated_buffer)) {
                                        char *buff = audio_output->updated_buffer;
                                        audio_output->updated_buffer = audio_output->playing_buffer;
                                        audio_output->playing_buffer = buff;
                }
                gkick_buffer_reset((struct gkick_buffer*)audio_output->playing_buffer);
                gkick_audio_output_unlock(audio_output);
        }
}

void gkick_audio_tune_output(struct gkick_audio_output *audio_output, bool tune)
{
        audio_output->tune = tune;
}

bool gkick_audio_is_tune_output(struct gkick_audio_output *audio_output)
{
        return audio_output->tune;
}
