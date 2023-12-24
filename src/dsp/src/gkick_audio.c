/**
 * File name: gkick_audio.c
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

#include "gkick_audio.h"
#include "audio_output.h"
#include "mixer.h"
#ifdef GEONKICK_AUDIO_JACK
#include "gkick_jack.h"
#endif

enum geonkick_error
gkick_audio_create(struct gkick_audio** audio, int sample_rate)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }


        *audio = (struct gkick_audio*)calloc(1, sizeof(struct gkick_audio));
        if (*audio == NULL) {
                gkick_log_error("can't allocate memory");
		return GEONKICK_ERROR_MEM_ALLOC;
	}

        (*audio)->sample_rate = sample_rate;
#ifdef GEONKICK_AUDIO_JACK
        if (gkick_create_jack(&(*audio)->jack) != GEONKICK_OK) {
                gkick_log_warning("can't create jack module. "
				  "Jack server is either not running or not installed");
        } else {
                (*audio)->sample_rate = gkick_jack_sample_rate((*audio)->jack);
        }
#endif // GEONKICK_AUDIO_JACK
        gkick_log_debug("sample rate: %d", (*audio)->sample_rate);

        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS + 1; i++) {
                if (gkick_audio_output_create(&(*audio)->audio_outputs[i], sample_rate) != GEONKICK_OK) {
                        gkick_log_error("can't create audio output");
                        gkick_audio_free(audio);
                        return GEONKICK_ERROR;
                }
		(*audio)->audio_outputs[i]->enabled = true;
        }
        (*audio)->audio_outputs[GEONKICK_MAX_INSTRUMENTS]->limiter = 1000000;

	if (gkick_mixer_create(&(*audio)->mixer) != GEONKICK_OK) {
		gkick_log_error("can't create mixer");
		gkick_audio_free(audio);
		return GEONKICK_ERROR;
	}
	(*audio)->mixer->audio_outputs = (*audio)->audio_outputs;

        return GEONKICK_OK;
}

enum geonkick_error
gkick_start_audio(struct gkick_audio *audio)
{
#ifdef GEONKICK_AUDIO_JACK
        if (audio->jack != NULL
            && geonkick_jack_start(audio->jack, audio->mixer) != GEONKICK_OK) {
                gkick_log_warning("can't start jack module");
                return GEONKICK_ERROR;
        }
#endif // GEONKICK_AUDIO_JACK

        return GEONKICK_OK;
}

void gkick_audio_free(struct gkick_audio** audio)
{
        if (audio != NULL && *audio != NULL) {
#ifdef GEONKICK_AUDIO_JACK
                gkick_jack_free(&(*audio)->jack);
#endif // GEONKICK_AUDIO_JACK
		gkick_mixer_free(&(*audio)->mixer);
                for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++)
                        gkick_audio_output_free(&(*audio)->audio_outputs[i]);
                free(*audio);
                *audio = NULL;
        }
}

enum geonkick_error
gkick_audio_set_limiter_val(struct gkick_audio *audio,
                            size_t index,
                            gkick_real limit)
{
        if (limit < 0.0f)
                limit = 0.0f;
        else if (limit > 10.0f)
                limit = 10.0f;
        return gkick_mixer_limiter_set(audio->mixer,
                                       index,
                                       limit);
}

enum geonkick_error
gkick_audio_get_limiter_val(struct gkick_audio *audio,
                            size_t index,
                            gkick_real *limit)
{
	return gkick_mixer_limiter_get(audio->mixer,
                                       index,
                                       limit);
}

enum geonkick_error
gkick_audio_play(struct gkick_audio *audio,
                 size_t id)
{
        if (audio == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_log_info("play %d", id);
        if (id < GEONKICK_MAX_INSTRUMENTS && audio->audio_outputs[id]->enabled) {
                gkick_audio_output_play(audio->audio_outputs[id]);
        } else if (id == GEONKICK_MAX_INSTRUMENTS) {
                // Play sample preview.
                gkick_audio_output_play(audio->audio_outputs[id]);
        }
        return GEONKICK_OK;
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
        return gkick_mixer_key_pressed(audio->mixer, &key);
}

enum geonkick_error
gkick_audio_process(struct gkick_audio *audio,
                    float** out,
                    size_t channel,
                    size_t size)
{
        return gkick_mixer_process(audio->mixer, out, channel, size);
}

enum geonkick_error
gkick_audio_set_limiter_callback(struct gkick_audio *audio,
                                 void (*callback)(void*, size_t index, gkick_real val),
                                 void *arg)
{
        return gkick_mixer_set_limiter_callback(audio->mixer,
                                                callback,
                                                arg);
}
