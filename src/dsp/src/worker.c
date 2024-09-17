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

#include "worker.h"

struct gkick_worker *geonkick_worker = NULL;

bool
geonkick_worker_created()
{
        return geonkick_worker != NULL;
}

size_t
geonkick_worker_reference_count()
{
        if (geonkick_worker != NULL)
                return geonkick_worker->ref_count;
        return 0;
}

enum geonkick_error
geonkick_worker_create()
{
        if (geonkick_worker != NULL)
                return GEONKICK_OK;

        geonkick_worker = (struct gkick_worker*)calloc(1, sizeof(struct gkick_worker));
	if (geonkick_worker == NULL)
		return GEONKICK_ERROR_MEM_ALLOC;

	geonkick_worker->running = false;
        if (pthread_cond_init(&geonkick_worker->condition_var, NULL) != 0) {
                gkick_log_error("can't init worker condition variable");
		return GEONKICK_ERROR;
	}
	geonkick_worker->cond_var_initilized = true;
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_worker_start()
{
        if (!geonkick_worker)
                return GEONKICK_ERROR;
        if (geonkick_worker->running)
                return GEONKICK_OK;
        geonkick_worker->running = true;
        if (pthread_create(&geonkick_worker->thread, NULL,
                           &geonkick_worker_thread, NULL) != 0) {
                gkick_log_error("can't create worker thread");
                geonkick_worker->running = false;
                return GEONKICK_ERROR;
        }
        return GEONKICK_OK;
}

void geonkick_worker_destroy()
{
	if (geonkick_worker->running)
		geonkick_worker->running = false;
        pthread_mutex_lock(&geonkick_worker->lock);
        pthread_cond_signal(&geonkick_worker->condition_var);
        pthread_mutex_unlock(&geonkick_worker->lock);
        gkick_log_debug("join thread: %d", geonkick_worker->running);
	pthread_join(geonkick_worker->thread, NULL);

	pthread_mutex_lock(&geonkick_worker->lock);
	if (geonkick_worker->cond_var_initilized)
		pthread_cond_destroy(&geonkick_worker->condition_var);
	geonkick_worker->cond_var_initilized = false;
	pthread_mutex_unlock(&geonkick_worker->lock);
        free(geonkick_worker);
        geonkick_worker = NULL;
}

void geonkick_worker_add_instance(struct geonkick *instance)
{
        if (geonkick_worker->ref_count < GEONKICK_MAX_INSTANCES) {
                pthread_mutex_lock(&geonkick_worker->lock);
                instance->id = geonkick_worker->ref_count;
                geonkick_worker->instances[geonkick_worker->ref_count++] = instance;
                pthread_mutex_unlock(&geonkick_worker->lock);
        }
}

void geonkick_worker_remove_instance(struct geonkick *instance)
{
        if (geonkick_worker->ref_count == 0)
                return;
        pthread_mutex_lock(&geonkick_worker->lock);
        if (geonkick_worker->ref_count) {
                geonkick_worker->instances[instance->id] =
                        geonkick_worker->instances[geonkick_worker->ref_count - 1];
                geonkick_worker->instances[instance->id]->id = instance->id;
        }
        geonkick_worker->instances[--geonkick_worker->ref_count] = NULL;
        pthread_mutex_unlock(&geonkick_worker->lock);
}

void *geonkick_worker_thread(void *arg)
{
        while (geonkick_worker->running) {
		/**
                 * Ignore too many updates.
                 * The last updates will be processed.
                 */
                geonkick_usleep(40000);
                gkick_log_debug("process...");
                pthread_mutex_lock(&geonkick_worker->lock);
                for (size_t i = 0; geonkick_worker->instances[i] != NULL && i < GEONKICK_MAX_INSTANCES; i++)
                        geonkick_process(geonkick_worker->instances[i]);

                if (!geonkick_worker->running) {
                        pthread_mutex_unlock(&geonkick_worker->lock);
                        break;
                }

                gkick_log_debug("wait...");
                pthread_cond_wait(&geonkick_worker->condition_var, &geonkick_worker->lock);
                pthread_mutex_unlock(&geonkick_worker->lock);
                gkick_log_debug("next");
	}
        gkick_log_debug("exit");
        return NULL;
}

void geonkick_worker_wakeup()
{
        pthread_mutex_lock(&geonkick_worker->lock);
        pthread_cond_signal(&geonkick_worker->condition_var);
        pthread_mutex_unlock(&geonkick_worker->lock);
}
