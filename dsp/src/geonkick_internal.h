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

#ifndef GEONKICK_INTERNAL_H
#define GEONKICK_INTERNAL_H

#include "geonkick.h"
#include "synthesizer.h"
#include "gkick_audio.h"
#include "gkick_buffer.h"

#include <pthread.h>
#include <stdatomic.h>

#define GEONKICK_SAMPLE_RATE 48000

/* Kick maximum length in seconds. */
#define GEONKICK_MAX_LENGTH 4.0
#define GEONKICK_MAX_KICK_BUFFER_SIZE  (4 * GEONKICK_SAMPLE_RATE)

struct gkick_worker {
	/* The worker thread. */
        pthread_t thread;

	/* Condition variable used for worker thread. */
        pthread_cond_t condition_var;
	bool cond_var_initilized;

	/* Specifies if the worker is running. */
	atomic_bool running;
};

struct geonkick {
        char name[30];
        // Current synth that can by controlled.
        struct gkick_synth *synth;

        // The list of synths of available synths.
        struct gkick_synth *synths[GEONKICK_MAX_PERCUSSIONS];
        struct gkick_audio *audio;

        /* Current controllable percussion index. */
        _Atomic size_t per_index;

        /**
         * Specifies if the synthesis is tuned off.
         * If is false any updates of the synthesizers parameters
         * will not trigger the perucssions synthesis.
         */
        atomic_bool synthesis_on;

	/* Global worker for all synths. */
	struct gkick_worker worker;

	/* Specifies if to update the percussions buffer. */
	atomic_bool update_buffers;
        pthread_mutex_t lock;
};

void geonkick_lock(struct geonkick *kick);

void geonkick_unlock(struct geonkick *kick);

enum geonkick_error
geonkick_worker_init(struct geonkick *kick);

enum geonkick_error
geonkick_worker_start(struct geonkick *kick);

void geonkick_worker_destroy(struct geonkick *kick);

void *geonkick_worker_thread(void *arg);

#endif // GEONKICK_INTERNAL_H
