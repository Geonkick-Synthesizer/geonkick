/**
 * File name: worker.h
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

#ifndef GEONKICK_WORKER_H
#define GEONKICK_WORKER_H

#include "geonkick_internal.h"

struct geonkick;

struct gkick_worker {
	/* The worker thread. */
        pthread_t thread;

	/* Condition variable used for the worker thread. */
        pthread_cond_t condition_var;
	bool cond_var_initilized;

	/* Specifies if the worker is running. */
	atomic_bool running;

        /* Geonkick instances. */
        struct geonkick *instances[GEONKICK_MAX_INSTANCES];

        /* Reference count for Geonkick instances. */
        atomic_size_t ref_count;
        pthread_mutex_t lock;
};

bool
geonkick_worker_created();

size_t
geonkick_worker_reference_count();

enum geonkick_error
geonkick_worker_create();

enum geonkick_error
geonkick_worker_start();

void geonkick_worker_destroy();

void geonkick_worker_add_instance(struct geonkick *instance);

void geonkick_worker_remove_instance(struct geonkick *instance);

void *geonkick_worker_thread(void *arg);

void geonkick_worker_wakeup();


#endif // GEONKICK_WORKER_H
