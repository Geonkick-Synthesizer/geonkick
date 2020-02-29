/**
 * File name: mixer.c
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

#include "mixer.h"

enum geonkick_error
gkick_mixer_create(struct gkick_mixer **mixer)
{
	*mixer = (struct gkick_mixer*)calloc(1, sizeof(struct gkick_mixer));
	if (*mixer == NULL) {
		gkick_log_error("can't allocate memory");
		return GEONKICK_ERROR_MEM_ALLOC;
	}

	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_key_pressed(struct gkick_mixer *mixer,
			struct gkick_note_info *note)
{
	if (note->note_number < 0 || note->note_number > 127)
		return GEONKICK_ERROR;

        for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
                struct gkick_audio_output *output = mixer->audio_outputs[i];
                if (output->enabled && output->playing_key == note->note_number)
                        gkick_audio_output_key_pressed(output, note);
        }
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_get_frame(struct gkick_mixer *mixer,
		      gkick_real *out_channels,
		      size_t n_channels)
{
	gkick_real out_val = 0.0f;
	for (size_t i = 0; i < GEONKICK_MAX_PERCUSSIONS; i++) {
		struct gkick_audio_output *out = mixer->audio_outputs[i];
		if (out->enabled) {
			gkick_real val = 0.0f;
			gkick_audio_output_get_frame(out, &val);
			out_val += val;
		}
	}

	out_val *= (gkick_real)mixer->limiter / 1000000;
        if (mixer->limiter_callback != NULL && mixer->limiter_callback_arg != NULL)
                mixer->limiter_callback(mixer->limiter_callback_arg, out_val);

	for (size_t ch = 0; ch < n_channels; ch++)
		out_channels[ch] = out_val;

        return GEONKICK_OK;
}

void
gkick_mixer_free(struct gkick_mixer **mixer)
{
	if (mixer != NULL && *mixer != NULL) {
		free(*mixer);
		*mixer = NULL;
	}
}

enum geonkick_error
gkick_mixer_limiter_set(struct gkick_mixer *mixer, gkick_real val)
{
	mixer->limiter = 1000000 * val;
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_limiter_get(struct gkick_mixer *mixer, gkick_real *val)
{
	*val = (gkick_real)mixer->limiter / 1000000;
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_set_limiter_callback(struct gkick_mixer *mixer,
				 void (*callback)(void*, gkick_real val),
				 void *arg)
{
        mixer->limiter_callback = callback;
        mixer->limiter_callback_arg = arg;
        return GEONKICK_OK;
}
