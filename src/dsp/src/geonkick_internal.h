/**
 * File name: geonkick_internal.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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

#ifndef GEONKICK_INTERNAL_H
#define GEONKICK_INTERNAL_H

#include "geonkick.h"
#include "synthesizer.h"
#include "gkick_audio.h"
#include "gkick_buffer.h"

#include <pthread.h>
#include <stdatomic.h>
#include <math.h>

#define GKICK_2PI 6.2831853f
#define GKICK_LOG20 1.3010299f

#define GEONKICK_MAX_INSTANCES 500

#include "worker.h"

/* Kick maximum length in seconds. */
#define GEONKICK_MAX_LENGTH 4

struct geonkick {
        size_t id;
        char name[30];
	int sample_rate;
        /* The list of synths of available synths. */
        struct gkick_synth *synths[GEONKICK_MAX_INSTRUMENTS];
        struct gkick_audio *audio;

        /* Current controllable percussion index. */
        _Atomic size_t per_index;

        /**
         * Specifies if the synthesis is tuned off.
         * If it is false any updates of the synthesizers parameters
         * will not trigger the synthesis.
         */
        atomic_bool synthesis_on;
        pthread_mutex_t lock;
        struct gkick_worker *worker;
};

void
geonkick_lock(struct geonkick *kick);

void
geonkick_unlock(struct geonkick *kick);

void
geonkick_process(struct geonkick *kick);

void
geonkick_wakeup(struct geonkick *kick);

void
geonkick_usleep(unsigned long int usecods);

int
geonkick_rand(unsigned int *seed);

#define min(a, b) ((a) < (b) ? (a) : (b))

inline gkick_real geonkick_clamp(gkick_real value, gkick_real min_v, gkick_real max_v)
{
        return (value < min_v) ? min_v : (value > max_v) ? max_v : value;
}

#endif // GEONKICK_INTERNAL_H
