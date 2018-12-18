/**
 * File name: geonkick_internal.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#ifndef GEON_KICK_INTERNAL_H
#define GEON_KICK_INTERNAL_H

#include "geonkick.h"
#include "synthesizer.h"
#include "gkick_audio.h"
#include "gkick_buffer.h"

#include <pthread.h>

#define GEONKICK_SAMPLE_RATE 48000

/* Kick maximum length in seconds. */
#define GEONKICK_MAX_LENGTH 4.0
#define GEONKICK_MAX_KICK_BUFFER_SIZE  (4 * GEONKICK_SAMPLE_RATE)

struct geonkick {
        char name[30];
        struct gkick_synth *synth;
        struct gkick_audio *audio;
        pthread_mutex_t lock;
};

void geonkick_lock(struct geonkick *kick);
void geonkick_unlock(struct geonkick *kick);

#endif // GEON_KICK_INTERNAL_H
