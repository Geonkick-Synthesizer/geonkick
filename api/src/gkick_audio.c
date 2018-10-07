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

        if (gkick_create_jack(&(*audio)->jack) != GEONKICK_OK) {
                gkick_log_error("can't create jack");
                gkick_audio_free(audio);
                return GEONKICK_ERROR;
        }

        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_free(struct gkick_audio** audio)
{
        if (audio != NULL && *audio != NULL) {
                gkick_jack_free(&(*audio)->jack);
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

        if (audio->jack != NULL) {
                gkick_jack_set_limit_val(audio->jack, limit);
        } else {
                // TODO: some other audio device.
        }

        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *audio, gkick_real *limit)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        if (audio->jack != NULL) {
                gkick_jack_get_limiter_val(audio->jack, limit);
        } else {
                // TODO: some other audio device.
        }
}

enum geonkick_error
gkick_audio_play(struct gkick_audio *audio, int play)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        if (audio->jack != NULL) {
                gkick_jack_set_play(audio->jack, play);
        } else {
                // TODO: some other audio device.
        }
}
