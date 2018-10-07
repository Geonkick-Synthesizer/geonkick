/**
 * File name: gkick_kack.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include "gkick_jack.h"
#include "oscillator.h"

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg)
{
        int i;
        gkick_real val;
        gkick_real limit;
        int note_pressed;
        struct gkick_jack *jack;
        jack_default_audio_sample_t *buffers[2]; /* Lef and right channels Jack buffers. */

        jack = (struct gkick_jack*)arg;

        note_pressed = gkick_jack_is_note_pressed(jack, nframes);
        if (note_pressed) {
                gkick_log_debug("note pressed");
                gkick_jack_set_play(jack, 1);
        }

        if (!gkick_jack_is_play(jack)) {
                return 0;
        }

        if (gkick_jack_get_output_buffers(jack, buffers, nframes)
           != GEONKICK_OK) {
                gkick_log_error("can't get output jack buffers");
                return 0;
        }

        for (i = 0; i < nframes; i++) {
                size_t is_end;
                val = gkick_buffer_get_at(jack->input, jack->buffer_index, &is_end);
                if (is_end) {
                        jack->buffer_index = 0;
                        gkick_jack_set_play(jack, 0);
                } else {
                        gkick_jack_get_limiter_val(jack, &limit);
                        val *= limit;
                        buffers[0][i] = (jack_default_audio_sample_t)val;
                        buffers[1][i] = (jack_default_audio_sample_t)val;
                        jack->buffer_index++;
                }
        }

        return 0;
}

jack_nframes_t
gkick_jack_sample_rate(struct gkick_jack *jack)
{
        jack_nframes_t sample_rate;

        if (jack == NULL) {
                gkick_log_error("wrong arguments");
                return 0;
        }

        gkick_jack_lock(jack);
        sample_rate = jack->sample_rate;
        gkick_jack_unlock(jack);

        return sample_rate;
}

enum geonkick_error
gkick_jack_get_output_buffers(struct gkick_jack *jack,
                              jack_default_audio_sample_t **channels_bufs,
                              jack_nframes_t nframes)
{
        enum geonkick_error error;

        if (jack == NULL || channels_bufs == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        error = GEONKICK_OK;
        gkick_jack_lock(jack);
        if (jack->output_port_r == NULL || jack->output_port_l == NULL) {
                gkick_log_error("output ports are undefined");
                error = GEONKICK_ERROR;
        } else {
                channels_bufs[0]
                        = (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port_l, nframes);
                channels_bufs[1]
                        = (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port_r, nframes);
        }
        gkick_jack_unlock(jack);

        if (channels_bufs[0] == NULL || channels_bufs[1] == NULL) {
                error = GEONKICK_ERROR;
        }

        return error;
}

int gkick_jack_is_note_pressed(struct gkick_jack *jack,
                               jack_nframes_t nframes)
{
        jack_nframes_t event_index;
        void* port_buf;
        jack_midi_event_t in_event;
        jack_nframes_t event_count;
        int note_pressed;

        if (gkick_jack_get_midi_in_port(jack) == NULL) {
                return 0;
        }

        port_buf    = jack_port_get_buffer(jack->midi_in_port, nframes);
        event_count = jack_midi_get_event_count(port_buf);

        note_pressed = 0;
        for (event_index = 0; event_index < event_count; event_index++) {
                jack_midi_event_get(&in_event, port_buf, 0);
                if ( ((*(in_event.buffer) & 0xf0)) == 0x90) {
                        note_pressed = 1;
                }
        }

        return note_pressed;
}

jack_port_t*
gkick_jack_get_midi_in_port(struct gkick_jack *jack)
{
        jack_port_t *port;

        if (jack == NULL) {
                gkick_log_error("wrong arguments");
                return NULL;
        }

        gkick_jack_lock(jack);
        port = jack->midi_in_port;
        gkick_jack_unlock(jack);

        return port;
}

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg)
{
	return 0;
}

enum geonkick_error
gkick_jack_enable_midi_in(struct gkick_jack *jack, const char *name)
{
        const char *midi_name;
        enum geonkick_error error;

        if (jack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (name == NULL) {
                midi_name = "midi_in";
        } else {
                midi_name = name;
        }

        error = GEONKICK_OK;
        gkick_jack_lock(jack);
        if (jack->midi_in_port == NULL) {
                jack->midi_in_port = jack_port_register(jack->client, midi_name,
                                                JACK_DEFAULT_MIDI_TYPE,
                                                JackPortIsInput, 0);
                if (jack->midi_in_port == NULL) {
                        gkick_log_error("can't register port: %s",
                                           midi_name);
                        error = GEONKICK_ERROR;
                }
        } else {
                gkick_log_warning("midi in is already set");
        }
        gkick_jack_unlock(jack);

        return error;
}

enum geonkick_error
gkick_jack_create_output_ports(struct gkick_jack *jack)
{
        enum geonkick_error error;
        if (jack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        error = GEONKICK_OK;
        gkick_jack_lock(jack);
        if (jack->output_port_l == NULL && jack->output_port_r == NULL) {
                jack->output_port_l = jack_port_register(jack->client, "audio_out_L",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);
                jack->output_port_r = jack_port_register(jack->client, "audio_out_R",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);
                if (jack->output_port_l == NULL
                    || jack->output_port_r == NULL) {
                        gkick_log_error("can't register output ports");
                        error = GEONKICK_ERROR;
                }
        } else {
                gkick_log_warning("output ports already created");
        }
        gkick_jack_unlock(jack);

        return error;
}

enum geonkick_error
gkick_create_jack(struct gkick_jack **jack)
{
        if (jack == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *jack = (struct gkick_jack*)malloc(sizeof(struct gkick_jack));
        if (*jack == NULL) {
                return GEONKICK_ERROR;
        }
        memset(*jack, 0, sizeof(struct gkick_jack));
        (*jack)->sample_rate = 48000;
        (*jack)->limiter = 1.0;
        gkick_buffer_new(&(*jack)->input, GEONKICK_MAX_KICK_BUFFER_SIZE);
        if ((*jack)->input == NULL) {
                gkick_log_error("can't create input buffer");
                gkick_jack_free(jack);
                return GEONKICK_ERROR;
        }

        if (pthread_mutex_init(&(*jack)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_jack_free(jack);
                return GEONKICK_ERROR;
        }

        (*jack)->client = jack_client_open(GEONKICK_API_NAME, JackNoStartServer, NULL);
        if ((*jack)->client == NULL) {
                gkick_log_error("can't create jack client");
                gkick_jack_free(jack);
                return GEONKICK_ERROR;
        }

        jack_set_process_callback((*jack)->client,
                                  gkick_jack_process_callback,
                                  (void*)(*jack));
        jack_set_sample_rate_callback((*jack)->client,
                                      gkick_jack_srate_callback,
                                      (void*)(*jack));

        if (gkick_jack_create_output_ports((*jack)) != GEONKICK_OK) {
                gkick_log_error("can't create output ports");
                gkick_jack_free(jack);
                return GEONKICK_ERROR;
        }

        gkick_jack_enable_midi_in(*jack, "midi_in");
        if (jack_activate((*jack)->client) != 0) {
                gkick_log_error("cannot activate client");
                gkick_jack_free(jack);
                return GEONKICK_ERROR;
        }

	return GEONKICK_OK;
}

int
gkick_jack_is_midi_in_enabled(struct gkick_jack *jack)
{
        int enabled;

        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return 0;
        }

        gkick_jack_lock(jack);
        enabled = (jack->midi_in_port != NULL) ? 1 : 0;
        gkick_jack_unlock(jack);

        return enabled;
}

void gkick_jack_free(struct gkick_jack **jack)
{
        if (jack != NULL && *jack != NULL) {
                if ((*jack)->client != NULL) {
                        jack_deactivate((*jack)->client);
                        if ((*jack)->output_port_l != NULL) {
                                jack_port_unregister((*jack)->client,
                                                     (*jack)->output_port_l);
                        }
                        if ((*jack)->output_port_r != NULL) {
                                jack_port_unregister((*jack)->client,
                                                     (*jack)->output_port_r);
                        }
                        jack_port_unregister((*jack)->client,
                                             (*jack)->output_port_r);
                        jack_client_close((*jack)->client);
                }

                if ((*jack)->input) {
                        gkick_buffer_free(&(*jack)->input);
                }
                free(*jack);
                *jack = NULL;
        }
}

void gkick_jack_lock(struct gkick_jack *jack)
{
        if (jack != NULL) {
                pthread_mutex_lock(&jack->lock);
        }
}

void gkick_jack_unlock(struct gkick_jack *jack)
{
        if (jack != NULL) {
                pthread_mutex_unlock(&jack->lock);
        }
}

void gkick_jack_set_play(struct gkick_jack *jack, int play)
{

        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return;
        }

        gkick_jack_lock(jack);
        jack->is_play = play;
        gkick_jack_unlock(jack);
}

int gkick_jack_is_play(struct gkick_jack *jack)
{
        int play;

        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return 0;
        }

        gkick_jack_lock(jack);
        play = jack->is_play;
        gkick_jack_unlock(jack);

        return play;
}

enum geonkick_error
gkick_jack_set_limiter_val(struct gkick_jack *jack, gkick_real limit)
{
        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        gkick_jack_lock(jack);
        jack->limiter = limit;
        gkick_jack_unlock(jack);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_jack_get_limiter_val(struct gkick_jack *jack, gkick_real *limit)
{
        if (jack == NULL || limit == NULL) {
                gkick_log_error("wrong arugments");
                return GEONKICK_ERROR;
        }

        gkick_jack_lock(jack);
        *limit = jack->limiter;
        gkick_jack_unlock(jack);
        return GEONKICK_OK;
}
