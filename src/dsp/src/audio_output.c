/**
 * File name: audio_output.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
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
gkick_audio_output_create(struct gkick_audio_output **audio_output, int sample_rate)
{
        if (audio_output == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *audio_output = (struct gkick_audio_output*)calloc(1, sizeof(struct gkick_audio_output));
        if (*audio_output == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR;
        }
        (*audio_output)->decay   = -1;
        (*audio_output)->play    = false;
	(*audio_output)->enabled = true;
        (*audio_output)->muted   = false;
        (*audio_output)->solo    = false;
        (*audio_output)->channel = 0;
        (*audio_output)->sample_rate = sample_rate;

        gkick_buffer_new(&(*audio_output)->updated_buffer,
                         (*audio_output)->sample_rate * GEONKICK_MAX_LENGTH);
        if ((*audio_output)->updated_buffer == NULL) {
                gkick_log_error("can't create updated buffer");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
        }
        gkick_buffer_set_size((*audio_output)->updated_buffer, 0);

        gkick_buffer_new(&(*audio_output)->playing_buffer,
                         (*audio_output)->sample_rate * GEONKICK_MAX_LENGTH);
        if ((*audio_output)->playing_buffer == NULL) {
                gkick_log_error("can't create playing buffer");
                gkick_audio_output_free(audio_output);
                return GEONKICK_ERROR;
        }
        gkick_buffer_set_size((*audio_output)->playing_buffer, 0);

        /**
         * Create the instrument ring buffer.
         * The ring buffer is twice bigger than the
         * maximum instrument sound length. This permits
         * to tune the instrument sound up to an octave.
         */
        enum geonkick_error err;
        err = ring_buffer_new(&(*audio_output)->ring_buffer,
                              (*audio_output)->sample_rate * 2 * GEONKICK_MAX_LENGTH);
        if (err != GEONKICK_OK) {
                gkick_log_error("can'r create ring buffer");
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
                gkick_buffer_free(&(*audio_output)->playing_buffer);
                gkick_buffer_free(&(*audio_output)->updated_buffer);
                ring_buffer_free(&(*audio_output)->ring_buffer);
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
                gkick_audio_output_swap_buffers(audio_output);
                gkick_audio_add_playing_buffer_to_ring(audio_output);
        } else {
                audio_output->decay = GEKICK_KEY_RELESE_DECAY_TIME;
                audio_output->key.state = key->state;
        }

        return GEONKICK_OK;
}

/* This funciton is called from the audio thread. */
void
gkick_audio_set_play(struct gkick_audio_output *audio_output)
{
        struct gkick_note_info key;
        key.channel     = 1;
        key.note_number = 69;
        key.velocity    = 127;
        key.state       = GKICK_KEY_STATE_PRESSED;
        audio_output->play = false;
        gkick_audio_output_key_pressed(audio_output, &key);
}


void
gkick_audio_add_playing_buffer_to_ring(struct gkick_audio_output *audio_output)
{
        gkick_buffer_reset(audio_output->playing_buffer);
        size_t i = 0;
        gkick_real factor = gkick_audio_output_tune_factor(audio_output->key.note_number);
        factor = geonkick_clamp(factor, 0.5f, 2.0f);
        while (!gkick_buffer_is_end(audio_output->playing_buffer)) {
                gkick_real val;
                if (audio_output->tune) {
                        val = gkick_buffer_stretch_get_next(audio_output->playing_buffer,
                                                            factor);
                } else {
                        val = gkick_buffer_get_next(audio_output->playing_buffer);
                }
                val *= (gkick_real)audio_output->key.velocity / 127;
                ring_buffer_add_value(audio_output->ring_buffer, i++, val);
        }
}

enum geonkick_error
gkick_audio_output_play(struct gkick_audio_output *audio_output)
{
        audio_output->play = true;
        return GEONKICK_OK;
}

gkick_real
gkick_audio_output_tune_factor(int note_number)
{
        return exp2f((gkick_real)(note_number - 69) / 12.0f);
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
        return audio_output->playing_buffer;
}

void gkick_audio_output_swap_buffers(struct gkick_audio_output *audio_output)
{
        gkick_buffer_reset(audio_output->playing_buffer);

        /**
         * Try lock. If successful, swap buffers. If not, continue with
         * the current one to play until the next press of the key.
         */
        if (pthread_mutex_trylock(&audio_output->lock) == 0) {
                /* Test if the updated buffer is full. Otherwise it means that it was not updated. */
                if (gkick_buffer_size(audio_output->updated_buffer) > 0
                    && gkick_buffer_is_end(audio_output->updated_buffer)) {
                        struct gkick_buffer *buff = audio_output->updated_buffer;
                        audio_output->updated_buffer = audio_output->playing_buffer;
                        audio_output->playing_buffer = buff;
                }
                gkick_buffer_reset(audio_output->playing_buffer);
                gkick_audio_output_unlock(audio_output);
        }
}

enum geonkick_error
gkick_audio_output_set_playing_key(struct gkick_audio_output *audio_output, signed char key)
{
        if (key < 21 || key > 108)
                key = GEONKICK_ANY_KEY;
        audio_output->playing_key = key;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_get_playing_key(struct gkick_audio_output *audio_output, signed char *key)
{
        *key = audio_output->playing_key;
        return GEONKICK_OK;
}


void gkick_audio_output_tune_output(struct gkick_audio_output *audio_output, bool tune)
{
        audio_output->tune = tune;
}

bool gkick_audio_output_is_tune_output(struct gkick_audio_output *audio_output)
{
        return audio_output->tune;
}

enum geonkick_error
gkick_audio_output_set_channel(struct gkick_audio_output *audio_output,
                               size_t channel)
{
        audio_output->channel = channel;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_output_get_channel(struct gkick_audio_output *audio_output,
                               size_t *channel)
{
        *channel = audio_output->channel;
        return GEONKICK_OK;
}
