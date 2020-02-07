/**
 * File name: mixer.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#ifndef GKICK_MIXER_H
#define GKICK_MIXER_H

#include "audio_output.h"

struct gkick_mixer {
	struct gkick_audio_output **audio_outputs;
	size_t connection_matrix[127];
	_Atomic int limiter;
        void (*limiter_callback) (void*, gkick_real val);
        void *limiter_callback_arg;
};

enum geonkick_error
gkick_mixer_create(struct gkick_mixer **mixer);

enum geonkick_error
gkick_mixer_key_pressed(struct gkick_mixer *mixer,
			struct gkick_note_info *note);

enum geonkick_error
gkick_mixer_get_frame(struct gkick_mixer *mixer,
		      gkick_real *out_channels,
		      size_t n_channels);

void
gkick_mixer_free(struct gkick_mixer **mixer);

enum geonkick_error
gkick_mixer_limiter_set(struct gkick_mixer *mixer, gkick_real val);

enum geonkick_error
gkick_mixer_limiter_get(struct gkick_mixer *mixer, gkick_real *val);

enum geonkick_error
gkick_mixer_set_limiter_callback(struct gkick_mixer *mixer,
				 void (*callback)(void*, gkick_real val),
				 void *arg);

#endif // GKICK_MIXER_H
