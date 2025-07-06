/**
 * File name: mixer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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
#include "ring_buffer.h"

enum geonkick_error
gkick_mixer_create(struct gkick_mixer **mixer)
{
	*mixer = (struct gkick_mixer*)calloc(1, sizeof(struct gkick_mixer));
        (*mixer)->solo = false;
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

        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                struct gkick_audio_output *output = mixer->audio_outputs[i];
                if (!output->enabled)
                        continue;

                short forced_midi_channel = mixer->forced_midi_channel;
                signed char midi_channel;
                if (forced_midi_channel & 0x0100)
                        midi_channel = forced_midi_channel & 0x00ff;
                else
                        midi_channel = output->midi_channel;

                gkick_log_debug("output: index[%d], forced[%d], midi ch[%d], note_ch[%d]",
                                i,
                                (forced_midi_channel & 0x0100) != 0,
                                midi_channel,
                                note->channel);
                if ((midi_channel == GEONKICK_ANY_MIDI_CHANNEL
                     || midi_channel == note->channel)
                    && (output->playing_key == GEONKICK_ANY_KEY
                        || output->playing_key == note->note_number
                        || output->tune || note->state == GKICK_KEY_STATE_RELEASED)) {
                        gkick_audio_output_key_pressed(output, note);
                }
        }
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_tune_output(struct gkick_mixer *mixer,
                        size_t index,
                        bool tune)
{
	if (index < GEONKICK_MAX_INSTRUMENTS)
		gkick_audio_output_tune_output(mixer->audio_outputs[index],
                                               tune);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_is_output_tuned(struct gkick_mixer *mixer,
                            size_t index,
                            bool *tune)
{
	if (index < GEONKICK_MAX_INSTRUMENTS)
		*tune = gkick_audio_output_is_tune_output(mixer->audio_outputs[index]);
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_process(struct gkick_mixer *mixer,
                    float** out,
                    size_t offset,
                    size_t size)
{
        if (size < 1)
                return GEONKICK_OK;

        for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS + 1; i++) {
                struct gkick_audio_output *output = mixer->audio_outputs[i];

                if (output->start_play) {
                        gkick_audio_set_play(output);
                        output->start_play = false;
                }

                if (!output->enabled || output->muted
                    || mixer->solo != output->solo || !output->play) {
                        ring_buffer_next(output->ring_buffer, size);
                        gkick_mixer_set_leveler(mixer, i, 0);
                        continue;
                }

                size_t left_index  = 2 * output->channel;
                size_t right_index = left_index + 1;
                gkick_real *data[2] = {out[left_index] + offset, out[right_index] + offset};
                gkick_real leveler = 0.0f;
                gkick_audio_get_data(output, data, &leveler, size);
                gkick_mixer_set_leveler(mixer, i, fabsf(leveler));
        }

        return GEONKICK_OK;
}

void gkick_mixer_apply_limiter(float *out_left,
                               float *out_right,
                               size_t size,
                               float limiter)
{
}

void
gkick_mixer_set_leveler(struct gkick_mixer *mixer,
                        size_t index,
                        gkick_real val)
{
        if (index >= GEONKICK_MAX_INSTRUMENTS)
                return;
        if (mixer->limiter_callback != NULL && mixer->limiter_callback_arg != NULL)
                mixer->limiter_callback(mixer->limiter_callback_arg, index, val);
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
gkick_mixer_limiter_set(struct gkick_mixer *mixer,
                        size_t index,
                        gkick_real val)
{
        if (index <= GEONKICK_MAX_INSTRUMENTS)
                mixer->audio_outputs[index]->limiter = 1000000 * val;
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_limiter_get(struct gkick_mixer *mixer,
                        size_t index,
                        gkick_real *val)
{
        *val = 0.0f;
        if (index <= GEONKICK_MAX_INSTRUMENTS)
                *val = (gkick_real)mixer->audio_outputs[index]->limiter / 1000000;
	return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_mute(struct gkick_mixer *mixer, size_t id, bool b)
{
        mixer->audio_outputs[id]->muted = b;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_is_muted(struct gkick_mixer *mixer, size_t id, bool *b)
{
        if (id < GEONKICK_MAX_INSTRUMENTS)
                *b = mixer->audio_outputs[id]->muted;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_solo(struct gkick_mixer *mixer, size_t id, bool b)
{
        if (id < GEONKICK_MAX_INSTRUMENTS) {
                mixer->audio_outputs[id]->solo = b;
                bool is_solo = false;
                for (size_t i = 0; i < GEONKICK_MAX_INSTRUMENTS; i++) {
                        if (mixer->audio_outputs[i]->enabled && mixer->audio_outputs[i]->solo)
                                is_solo = true;
                }
                mixer->solo = is_solo;
        }

        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_is_solo(struct gkick_mixer *mixer, size_t id, bool *b)
{
        if (id < GEONKICK_MAX_INSTRUMENTS)
                *b = mixer->audio_outputs[id]->solo;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_set_limiter_callback(struct gkick_mixer *mixer,
				 void (*callback)(void*, size_t index, gkick_real val),
				 void *arg)
{
        mixer->limiter_callback = callback;
        mixer->limiter_callback_arg = arg;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_force_midi_channel(struct gkick_mixer *mixer,
                               signed char channel,
                               bool force)
{
        gkick_log_debug("forced %d midi ch %d", force, channel);
        short forced_midi = mixer->forced_midi_channel;
        forced_midi = (channel & 0x01ff) | (force << 8);
        short forced_midi_channel = mixer->forced_midi_channel;
        gkick_log_debug("forced [%d] midi ch: %d]",
                        (forced_midi & 0x0100) != 0,
                        forced_midi & 0x00ff);
        mixer->forced_midi_channel = forced_midi;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_get_forced_midi_channel(struct gkick_mixer *mixer,
                                    signed char *channel,
                                    bool *force)
{
        short forced_midi = mixer->forced_midi_channel;
        if (channel)
                *channel = forced_midi & 0x00ff;
        if (force)
                *force = forced_midi & 0x0100;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_enable_note_off(struct gkick_mixer *mixer,
                            size_t id,
                            bool enable)
{
        if (id < GEONKICK_MAX_INSTRUMENTS)
                gkick_audio_output_enable_note_off(mixer->audio_outputs[id], enable);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_mixer_note_off_enabled(struct gkick_mixer *mixer,
                             size_t id,
                             bool *enabled)
{
        if (id < GEONKICK_MAX_INSTRUMENTS)
                *enabled = gkick_audio_output_note_off(mixer->audio_outputs[id]);
        return GEONKICK_OK;
}
