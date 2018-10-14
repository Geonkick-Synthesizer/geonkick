/**
 * File name: filter.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "filter.h"

enum geonkick_error
gkick_filter_new(struct gkick_filter **filter)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *filter = (struct gkick_filter *) malloc(sizeof(struct gkick_filter));
        if (*filter == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        memset(*filter, 0, sizeof(struct gkick_filter));
        (*filter)->type = GEONKICK_FILTER_LOW_PASS;
        (*filter)->queue_empty = 1;

        if (pthread_mutex_init(&(*filter)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_filter_free(filter);
                return GEONKICK_ERROR;
	}

        (*filter)->cutoff_freq = GEONKICK_DEFAULT_FILTER_CUTOFF_FREQ;
        (*filter)->factor      = GEONKICK_DEFAULT_FILTER_FACTOR;
        gkick_filter_update_coefficents(*filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_init(struct gkick_filter *filter)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        filter->queue_empty = 1;
        memset(filter->queue, 0, sizeof(filter->queue));
        gkick_filter_update_coefficents(filter);
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

void gkick_filter_free(struct gkick_filter **filter)
{
        if (filter != NULL && *filter != NULL) {
                pthread_mutex_destroy(&(*filter)->lock);
                free(*filter);
                *filter = NULL;
        }
}

void gkick_filter_lock(struct gkick_filter *filter)
{
        pthread_mutex_lock(&filter->lock);
}

void gkick_filter_unlock(struct gkick_filter *filter)
{
        pthread_mutex_unlock(&filter->lock);
}

enum geonkick_error
gkick_filter_update_coefficents(struct gkick_filter *filter)
{
        gkick_real *k, C, p1, p2;

        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        k = filter->coefficients;
        C = 1.0 / tan(M_PI * filter->cutoff_freq / GEONKICK_SAMPLE_RATE);
        p1 = 2.0 * filter->factor * C;
        p2 = C * C;
        k[2] = 1.0 / (1.0 + p1 + p2);
        k[1] = 2.0 * k[2] * p2;
        k[0] = k[2] * (p1 - p2);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_set_type(struct gkick_filter *filter, enum gkick_filter_type type)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        filter->type = type;
        gkick_filter_update_coefficents(filter);
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_get_type(struct gkick_filter *filter, enum gkick_filter_type *type)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        *type = filter->type;
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_set_cutoff_freq(struct gkick_filter *filter, gkick_real cutoff)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        filter->cutoff_freq = cutoff;
        gkick_filter_update_coefficents(filter);
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_set_factor(struct gkick_filter *filter, gkick_real factor)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        filter->factor = factor;
        gkick_filter_update_coefficents(filter);
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_get_cutoff_freq(struct gkick_filter *filter, gkick_real *cutoff)
{
        if (filter == NULL || cutoff == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        *cutoff = filter->cutoff_freq;
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_get_factor(struct gkick_filter *filter, gkick_real *factor)
{
        if (filter == NULL || factor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        *factor = filter->factor;
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_val(struct gkick_filter *filter,
                 gkick_real in_val,
                 gkick_real *out_val)
{
        gkick_real *k;
        gkick_real *q;

        if (filter == NULL || out_val == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);

        q = filter->queue;
        k = filter->coefficients;
        if (filter->queue_empty) {
                q[0] = q[1] = q[2] = in_val;
                filter->queue_empty = 0;
        } else {
                q[0] = q[1];
                q[1] = q[2];
                q[2] = in_val;
        }
        q[2] = k[2] * q[2] +  k[1] * q[1] + k[0] * q[0];
        *out_val = q[2];

        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}
