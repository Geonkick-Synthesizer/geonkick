/**
 * File name: distortion.c
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

#include "distortion.h"
#include "envelope.h"

enum geonkick_error
gkick_distortion_new(struct gkick_distortion **distortion, int sample_rate)
{
        if (distortion == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *distortion = (struct gkick_distortion*)calloc(1, sizeof(struct gkick_distortion));
        if (*distortion == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR;
        }
	(*distortion)->drive_env = NULL;
        (*distortion)->volume_env = NULL;
	(*distortion)->drive = 1.0f;
        (*distortion)->sample_rate = sample_rate;

	struct gkick_envelope *env = gkick_envelope_create();
	if (env == NULL) {
		gkick_log_error("can't create distortion drive envelope");
		gkick_distortion_free(distortion);
		return GEONKICK_ERROR;
	} else {
		/* Add two default points. */
		gkick_envelope_add_point(env, 0.0f, 1.0f);
		gkick_envelope_add_point(env, 1.0f, 1.0f);
		(*distortion)->drive_env = env;
	}

        env = gkick_envelope_create();
	if (env == NULL) {
		gkick_log_error("can't create distortion volume envelope");
		gkick_distortion_free(distortion);
		return GEONKICK_ERROR;
	} else {
		/* Add two default points. */
		gkick_envelope_add_point(env, 0.0f, 1.0f);
		gkick_envelope_add_point(env, 1.0f, 1.0f);
		(*distortion)->volume_env = env;
	}


        if (pthread_mutex_init(&(*distortion)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_distortion_free(distortion);
                return GEONKICK_ERROR;
	}

        return GEONKICK_OK;
}

void
gkick_distortion_free(struct gkick_distortion **distortion)
{
        if (distortion != NULL && *distortion != NULL) {
		if ((*distortion)->drive_env != NULL)
			gkick_envelope_destroy((*distortion)->drive_env);
                if ((*distortion)->volume_env != NULL)
                        gkick_envelope_destroy((*distortion)->volume_env);
                pthread_mutex_destroy(&(*distortion)->lock);
                free(*distortion);
                *distortion = NULL;
        }
}

void gkick_distortion_lock(struct gkick_distortion *distortion)
{
        pthread_mutex_lock(&distortion->lock);
}

void gkick_distortion_unlock(struct gkick_distortion *distortion)
{
        pthread_mutex_unlock(&distortion->lock);
}

enum geonkick_error
gkick_distortion_enable(struct gkick_distortion *distortion, int enable)
{
        gkick_distortion_lock(distortion);
        distortion->enabled = enable;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_is_enabled(struct gkick_distortion *distortion, int *enabled)
{
        gkick_distortion_lock(distortion);
        *enabled = distortion->enabled;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_val(struct gkick_distortion *distortion,
                     gkick_real in_val,
                     gkick_real *out_val,
		     gkick_real env_x)
{
        gkick_distortion_lock(distortion);
	gkick_real x = distortion->in_limiter * in_val;
	x *= 1.0f + (distortion->drive - 1.0f) * gkick_envelope_get_value(distortion->drive_env, env_x);

        if (x > 1.0f)
                x = 1.0f;
        else if (x < -1.0f)
                x = -1.0f;

        *out_val= (x < 0.0f ? -1.0f : 1.0f) * (1.0f - exp(-4.0f * log(10.0f) * fabs(x)));
        *out_val *= distortion->volume * gkick_envelope_get_value(distortion->volume_env, env_x);
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_volume(struct gkick_distortion *distortion,
                            gkick_real volume)
{
        gkick_distortion_lock(distortion);
        distortion->volume = volume;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_volume(struct gkick_distortion *distortion,
                            gkick_real *volume)
{
        gkick_distortion_lock(distortion);
        *volume = distortion->volume;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_in_limiter(struct gkick_distortion *distortion,
                                gkick_real limit)
{
	gkick_distortion_lock(distortion);
        distortion->in_limiter = limit;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_in_limiter(struct gkick_distortion *distortion,
                                gkick_real *limit)
{
	gkick_distortion_lock(distortion);
        *limit = distortion->in_limiter;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_drive(struct gkick_distortion *distortion,
                           gkick_real drive)
{
        gkick_distortion_lock(distortion);
        distortion->drive = drive;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_drive(struct gkick_distortion *distortion,
                           gkick_real *drive)
{
        gkick_distortion_lock(distortion);
        *drive = distortion->drive;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}
