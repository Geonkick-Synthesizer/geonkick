/**
 * File name: audio_output.h
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

#ifndef GKICK_AUDO_OUTPUT_H
#define GKICK_AUDO_OUTPUT_H

#include "geonkick_internal.h"

/* Decay time measured in number of audio frames / samples. */
#define GEKICK_KEY_RELESE_DECAY_TIME 1000

struct gkick_note_info {
        enum gkick_key_state state;
        char channel;
        char note_number;
        char velocity;
};

struct gkick_audio_output
{
        struct gkick_buffer *buffer;

        pthread_mutex_t lock;
        gkick_real limiter;
        void (*limiter_callback) (void*, gkick_real val);
        void *limiter_callback_arg;
        size_t buffer_index;
        char key_velocity;
        enum gkick_key_state key_state;
        int is_play;

        /**
         * decay - note release time measured in number of sample frames.
         * Relaxation curve for audio is liniear:
         *   - 1.0 * (GEKICK_NOTE_RELEASE_TIME - decay) / GEKICK_NOTE_RELEASE_TIME + 1.0,
         *    decay from GEKICK_NOTE_RELEASE_TIME to 0;
         */
        int decay;
};

/**
 * Audio output functions module
 */
enum geonkick_error
gkick_audio_output_create(struct gkick_audio_output **audio_output);

void gkick_audio_output_free(struct gkick_audio_output **audio_output);

enum geonkick_error
gkick_audio_output_set_limiter(struct gkick_audio_output  *audio_output, gkick_real limit);

enum geonkick_error
gkick_audio_output_get_limiter(struct gkick_audio_output  *audio_output, gkick_real *limit);

struct gkick_buffer*
gkick_audio_output_get_buffer(struct gkick_audio_output  *audio_output);

enum geonkick_error
gkick_audio_output_key_pressed(struct gkick_audio_output *audio_output,
                               enum gkick_key_state pressed,
                               int velocity);

enum geonkick_error
gkick_audio_output_get_frame(struct gkick_audio_output *audio_output, gkick_real *val);

void gkick_audio_output_lock(struct gkick_audio_output *audio_output);

void gkick_audio_output_unlock(struct gkick_audio_output *audio_output);

enum geonkick_error
gkick_audio_output_set_limiter_callback(struct gkick_audio_output *audio_output,
                                        void (*callback)(void*, gkick_real val),
                                        void *arg);

#endif // GKICK_AUDO_OUTPUT_H
