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

/**
 * Audio output (which is maybe not the best name)
 * is an interface to an array that holds the all length synthesised kick.
 * Is has nothing to do with any real audio interfaces.
 * It provides access to the samples from the array in a "playable mode",
 * i.e. in a state machine fasion.
 */

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
        // On this buffer the synthesizer is  copying
        char* _Atomic updated_buffer;
        char* _Atomic playing_buffer;
        _Atomic bool buffer_updated;

        /* Callback must make only short atomic operations, no blocking. */
        void (*limiter_callback) (void*, gkick_real val);
        void *limiter_callback_arg;
        // Note info is changed only by the audio thread.
        struct gkick_note_info key;
        _Atomic bool is_play;

        /*
         * Specifies if to tune output in accordance with
         * the note (central note is A4).
         */
        _Atomic bool tune;

        /**
         * decay - note release time measured in number of sample frames.
         * Relaxation curve for audio is liniear:
         *   - 1.0 * (GEKICK_NOTE_RELEASE_TIME - decay) / GEKICK_NOTE_RELEASE_TIME + 1.0,
         *    decay from GEKICK_NOTE_RELEASE_TIME to 0;
         */
        _Atomic int decay;
        _Atomic int limiter;
        pthread_mutex_t lock;
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
gkick_audio_output_key_pressed(struct gkick_audio_output *audio_output, struct gkick_note_info *key);

gkick_real
gkick_audio_output_tune_factor(int note_number);

enum geonkick_error
gkick_audio_output_get_frame(struct gkick_audio_output *audio_output, gkick_real *val);

void gkick_audio_output_lock(struct gkick_audio_output *audio_output);

void gkick_audio_output_unlock(struct gkick_audio_output *audio_output);

enum geonkick_error
gkick_audio_output_set_limiter_callback(struct gkick_audio_output *audio_output,
                                        void (*callback)(void*, gkick_real val),
                                        void *arg);

void gkick_audio_swap_buffers(struct gkick_audio_output *audio_output);

void gkick_audio_tune_output(struct gkick_audio_output *audio_output, bool tune);

bool gkick_audio_is_tune_output(struct gkick_audio_output *audio_output);

#endif // GKICK_AUDO_OUTPUT_H
