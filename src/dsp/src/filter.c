/**
 * File name: filter.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
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
gkick_filter_new(struct gkick_filter **filter, int sample_rate)
{
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *filter = (struct gkick_filter *)calloc(1, sizeof(struct gkick_filter));
        if (*filter == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        (*filter)->type = GEONKICK_FILTER_LOW_PASS;
        (*filter)->queue_empty = true;
        (*filter)->sample_rate = sample_rate;

        (*filter)->cutoff_env = gkick_envelope_create();
        if ((*filter)->cutoff_env == NULL) {
                gkick_log_error("can't create filter cutoff envelope");
                gkick_filter_free(filter);
                return GEONKICK_ERROR;
        } else {
                gkick_envelope_add_point((*filter)->cutoff_env, 0.0f, 1.0f);
                gkick_envelope_add_point((*filter)->cutoff_env, 1.0f, 1.0f);
        }

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
        filter->queue_empty = true;
        memset(filter->queue_l, 0, sizeof(filter->queue_l));
        memset(filter->queue_b, 0, sizeof(filter->queue_b));
        memset(filter->queue_h, 0, sizeof(filter->queue_h));
        gkick_filter_update_coefficents(filter);
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}

void gkick_filter_free(struct gkick_filter **filter)
{
        if (filter != NULL && *filter != NULL) {
                gkick_envelope_destroy((*filter)->cutoff_env);
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
        if (filter == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        filter->coefficients[1] = filter->factor;
        return GEONKICK_OK;
}

enum geonkick_error
gkick_filter_set_type(struct gkick_filter *filter,
                      enum gkick_filter_type type)
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
gkick_filter_get_type(struct gkick_filter *filter,
                      enum gkick_filter_type *type)
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
gkick_filter_set_cutoff_freq(struct gkick_filter *filter,
                             gkick_real cutoff)
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
gkick_filter_set_factor(struct gkick_filter *filter,
                        gkick_real factor)
{
        if (filter == NULL || factor < 0.5f) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);
        filter->factor = 10.0f / factor;
        gkick_filter_update_coefficents(filter);
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
        *factor = 10.0f / filter->factor;
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

/**
 * gkick_filter_val function
 *
 * Implements low, high, and band pass digital state variable filter.
 */
enum geonkick_error
gkick_filter_val(struct gkick_filter *filter,
                 gkick_real in_val,
                 gkick_real *out_val,
                 gkick_real env_x)
{
        if (isnan(in_val) || in_val > 1.0f || in_val < -1.0f) {
                *out_val = 0.0f;
                return GEONKICK_ERROR;
        }

        if (filter == NULL || out_val == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_filter_lock(filter);

        gkick_real *l = filter->queue_l;
        gkick_real *b = filter->queue_b;
        gkick_real *h = filter->queue_h;

	gkick_real env_val = gkick_envelope_get_value(filter->cutoff_env, env_x);
	enum gkick_envelope_apply_type apply_type = gkick_envelope_get_apply_type(filter->cutoff_env);
	gkick_real f;
	if (apply_type == GEONKICK_ENVELOPE_APPLY_LOGARITHMIC)
		f = pow(10, (log10(filter->cutoff_freq) - GKICK_LOG20) * env_val + GKICK_LOG20);
	else
		f = filter->cutoff_freq * env_val;
        gkick_real F = 2.0f * sin(M_PI * f / filter->sample_rate);
        gkick_real Q  = filter->coefficients[1];
	
        size_t n = 1;
        if (filter->queue_empty) {
                l[n - 1] = l[n] = 0;
                b[n - 1] = b[n] = 0;
                h[n - 1] = h[n] = 0;
                filter->queue_empty = false;
        } else {
                h[n - 1] = h[n];
                b[n - 1] = b[n];
                l[n - 1] = l[n];
        }
        h[n] = in_val - l[n - 1] - Q * b[n - 1];
        b[n] = F * h[n] + b[n - 1];
        l[n] = F * b[n] + l[n - 1];

        gkick_real val = 0.0f;
        if (filter->type == GEONKICK_FILTER_HIGH_PASS)
                val = h[n];
        else if (filter->type == GEONKICK_FILTER_BAND_PASS)
                val = b[n];
        else
                val = l[n];
        *out_val = val;
        gkick_filter_unlock(filter);

        return GEONKICK_OK;
}
