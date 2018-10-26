/**
 * File name: gkick_kack.c
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

#ifndef GKICK_JACK_H
#define GKICK_JACK_H

#include "geonkick_internal.h"
#include "gkick_buffer.h"

#include <jack/jack.h>
#include <jack/midiport.h>

struct gkick_jack {
        /* Accessed only by jack thread. */
        jack_port_t *output_port_l;
        jack_port_t *output_port_r;
        jack_port_t *midi_in_port;
        jack_client_t *client;
        jack_nframes_t sample_rate;
        size_t buffer_index;
        char key_velocity;
        pthread_mutex_t lock;
        /**
         * Accessed by jack and other threads. input has it own mutex,
         * so there no need to used the current struct mutex lock.
         */
        gkick_real limiter;
        int is_play;
        struct gkick_buffer *input;
};

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg);

jack_nframes_t
gkick_jack_sample_rate(struct gkick_jack *jack);

enum geonkick_error
gkick_jack_get_output_buffers(struct gkick_jack *jack,
                              jack_default_audio_sample_t **channels_bufs,
                              jack_nframes_t nframes);

void gkick_jack_get_note_info(struct gkick_jack *jack,
                              jack_nframes_t nframes,
                              struct gkick_note_info *note);

jack_port_t*
gkick_jack_get_midi_in_port(struct gkick_jack *jack);

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg);

enum geonkick_error
gkick_jack_enable_midi_in(struct gkick_jack *jack, const char *name);

enum geonkick_error
gkick_jack_create_output_ports(struct gkick_jack *jack);

enum geonkick_error
gkick_create_jack(struct gkick_jack **jack);

int gkick_jack_is_midi_in_enabled(struct gkick_jack *jack);

void gkick_jack_free(struct gkick_jack **jack);

void gkick_jack_lock(struct gkick_jack *jack);

void gkick_jack_unlock(struct gkick_jack *jack);

void gkick_jack_set_play(struct gkick_jack *jack, int play);

int gkick_jack_is_play(struct gkick_jack *jack);

enum geonkick_error
gkick_jack_set_limiter_val(struct gkick_jack *jack, gkick_real limit);

enum geonkick_error
gkick_jack_get_limiter_val(struct gkick_jack *jack, gkick_real *limit);


#endif
