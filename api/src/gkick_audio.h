/**
 * File name: gkick_audio.h
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

#ifndef GKICK_AUDIO_H
#define GKICK_AUDIO_H

#include "geonkick_internal.h"

struct gkick_audio {
        struct gkick_jack *jack;
        struct gkick_buffer *input;
        // other audio device
};

enum geonkick_error
gkick_audio_create(struct gkick_audio** audio);

void gkick_audio_free(struct gkick_audio** audio);

enum geonkick_error
gkick_audio_set_limiter_val(struct gkick_audio *audio, gkick_real limit);

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *adio, gkick_real *limit);

enum geonkick_error
gkick_audio_play(struct gkick_audio *audio, int play);

#endif // GKICK_AUDIO_H
