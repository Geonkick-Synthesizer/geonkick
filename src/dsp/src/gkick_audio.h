/**
 * File name: gkick_audio.h
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

#ifndef GKICK_AUDIO_H
#define GKICK_AUDIO_H

#include "geonkick_internal.h"

struct audio_output;
struct gkick_jack;
struct gkick_mixer;

struct gkick_audio {
        int sample_rate;
        /* Audio outputs. The last audio output is used as a sample preview. */
        struct gkick_audio_output *audio_outputs[GEONKICK_MAX_INSTRUMENTS + 1];
	struct gkick_mixer *mixer;
        struct gkick_jack *jack;
};

enum geonkick_error
gkick_audio_create(struct gkick_audio** audio, int sample_rate);

enum geonkick_error
gkick_start_audio(struct gkick_audio *audio);

void gkick_audio_free(struct gkick_audio** audio);

enum geonkick_error
gkick_audio_set_limiter_val(struct gkick_audio *audio,
                            size_t index,
                            gkick_real limit);

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *audio,
                            size_t index,
                            gkick_real *limit);

enum geonkick_error
gkick_audio_play(struct gkick_audio *audio,
                 size_t id);

enum geonkick_error
gkick_audio_key_pressed(struct gkick_audio *audio,
                        bool pressed,
                        int note,
                        int velocity);

enum geonkick_error
gkick_audio_process(struct gkick_audio *audio,
                    float** out,
                    size_t channel,
                    size_t size);

enum geonkick_error
gkick_audio_set_limiter_callback(struct gkick_audio *audio,
                                 void (*callback)(void*, size_t index, gkick_real val),
                                 void *arg);

#endif // GKICK_AUDIO_H
