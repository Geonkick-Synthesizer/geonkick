/**
 * File name: geonkick_internal.h
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

#ifndef GEON_KICK_INTERNAL_H
#define GEON_KICK_INTERNAL_H

#include "geonkick.h"
#include "gkick_jack.h"
#include "envelope.h"

#include <pthread.h>

struct audio_device {
        struct gkick_jack *jack;
        int is_play;
        double limiter;
        pthread_mutex_t lock;
};

struct geonkick {
        char name[30];
        struct geonkick_synthesizer *synthesizer;
        struct gkick_audio_device *audio_device;
        pthread_mutex_t lock;
};

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick);

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate);

double
geonkick_get_oscillators_value(struct geonkick *kick);

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index);

void
geonkick_lock(struct geonkick *kick);

void
geonkick_unlock(struct geonkick *kick);

void
gkick_jack_increment_time(struct gkick_jack *jack);

void
geonkick_reset_oscillators(struct geonkick *kick);

enum geonkick_error
geonkick_play(struct geonkick *kick, int play);

int
geonckick_is_play_stopped(struct geonkick *kick);

enum geonkick_error
geonkick_start_play(struct geonkick *kick);

enum geonkick_error
geonkick_stop_play(struct geonkick *kick);

enum geonkick_error
geonkick_increment_time(struct geonkick *kick, double dt);

double
geonkick_current_time(struct geonkick *kick);

double
geonkick_length(struct geonkick *kick);

void
geonkick_set_current_time(struct geonkick *kick,
			  double current_time);

#endif
