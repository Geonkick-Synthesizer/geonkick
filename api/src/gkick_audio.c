/**
 * File name: gkick_audio.c
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

#include "gkick_audio.h"
#include "audio_output.h"
#ifdef GEONKICK_AUDIO_JACK
#include "gkick_jack.h"
#endif

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
        memset(*audio, 0, sizeof(struct gkick_audio));

        if (gkick_audio_output_create(&(*audio)->audio_output) != GEONKICK_OK) {
                gkick_log_error("can't create audio output");
                gkick_audio_free(audio);
                return GEONKICK_ERROR;
        }

#ifdef GEONKICK_AUDIO_JACK
        if (gkick_create_jack(&(*audio)->jack, (*audio)->audio_output) != GEONKICK_OK)
                gkick_log_warning("can't create jack module. Jack server is either not running or not installed");
#endif // GEONKICK_AUDIO_JACK
        return GEONKICK_OK;
}

void gkick_audio_free(struct gkick_audio** audio)
{
        if (audio != NULL && *audio != NULL) {
#ifdef GEONKICK_AUDIO_JACK
                gkick_jack_free(&(*audio)->jack);
#endif // GEONKICK_AUDIO_JACK
                gkick_audio_output_free(&(*audio)->audio_output);
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

        if (limit < 0)
                limit = 0;
        else if (limit > 10)
                limit = 10;

        return gkick_audio_output_set_limiter(audio->audio_output, limit);
}

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *audio, gkick_real *limit)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        return gkick_audio_output_get_limiter(audio->audio_output, limit);
}

struct gkick_buffer*
gkick_audio_get_buffer(struct gkick_audio *audio)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        return gkick_audio_output_get_buffer(audio->audio_output);
}

enum geonkick_error
gkick_audio_key_pressed(struct gkick_audio *audio,
                        bool pressed,
                        int note,
                        int velocity)
{
        if (audio == NULL) {
               gkick_log_error("wrong arguments");
               return GEONKICK_ERROR;
        }

        struct gkick_note_info key;
        key.channel     = 1;
        key.note_number = note;
        key.velocity    = velocity;
        key.state = pressed ? GKICK_KEY_STATE_PRESSED : GKICK_KEY_STATE_RELEASED;
        gkick_audio_output_key_pressed(audio->audio_output, &key);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_audio_get_frame(struct gkick_audio *audio, gkick_real *val)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        return gkick_audio_output_get_frame(audio->audio_output, val);
}

enum geonkick_error
gkick_audio_set_limiter_callback(struct gkick_audio *audio,
                                 void (*callback)(void*, gkick_real val),
                                 void *arg)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }
        return gkick_audio_output_set_limiter_callback(audio->audio_output, callback, arg);
}
