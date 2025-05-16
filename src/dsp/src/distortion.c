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
	(*distortion)->drive_env   = NULL;
        (*distortion)->volume_env  = NULL;
	(*distortion)->drive       = 1.0f;
        (*distortion)->in_limiter  = 1.0f;
        (*distortion)->out_limiter = 1.0f;
        (*distortion)->sample_rate = sample_rate;
#if (GEONKICK_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // GEONKICK_VERSION_MAJOR
        (*distortion)->type        = GEONKICK_DISTORTION_SOFT_CLIPPING_TANH;

	struct gkick_envelope *env = gkick_envelope_create();
	if (env == NULL) {
		gkick_log_error("can't create distortion drive envelope");
		gkick_distortion_free(distortion);
		return GEONKICK_ERROR;
	} else {
		/* Add two default points. */
                struct gkick_envelope_point_info info = {0.0f, 1.0f, false};
		gkick_envelope_add_point(env, &info);
                info.x = 1.0f;
		gkick_envelope_add_point(env, &info);
		(*distortion)->drive_env = env;
	}

        env = gkick_envelope_create();
	if (env == NULL) {
		gkick_log_error("can't create distortion volume envelope");
		gkick_distortion_free(distortion);
		return GEONKICK_ERROR;
	} else {
		/* Add two default points. */
                struct gkick_envelope_point_info info = {0.0f, 1.0f, false};
		gkick_envelope_add_point(env, &info);
                info.x = 1.0f;
		gkick_envelope_add_point(env, &info);
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
gkick_distortion_enable(struct gkick_distortion *distortion, bool enable)
{
        gkick_distortion_lock(distortion);
        distortion->enabled = enable;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_is_enabled(struct gkick_distortion *distortion, bool *enabled)
{
        gkick_distortion_lock(distortion);
        *enabled = distortion->enabled;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_type(struct gkick_distortion *distortion,
                          enum gkick_distortion_type type)
{
        gkick_distortion_lock(distortion);
        distortion->type = type;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_type(struct gkick_distortion *distortion,
                          enum gkick_distortion_type *type)
{
        gkick_distortion_lock(distortion);
        *type = distortion->type;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

/* TODO: With old presets. In the next major release this to be removed. */
#if (GEONKICK_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // GEONKICK_VERSION_MAJOR
static void distortion_backward_compatible(struct gkick_distortion *distortion,
                                           gkick_real in_val,
                                           gkick_real *out_val,
                                           gkick_real env_x)
{
	gkick_real x = distortion->in_limiter * in_val;
	x *= 1.0f + (distortion->drive - 1.0f) * gkick_envelope_get_value(distortion->drive_env, env_x);

        x = GKICK_CLAMP(x, -1.0f, 1.0f);
        *out_val= (x < 0.0f ? -1.0f : 1.0f) * (1.0f - exp(-4.0f * log(10.0f) * fabs(x)));
        *out_val *= distortion->out_limiter * gkick_envelope_get_value(distortion->volume_env, env_x);
}

enum geonkick_error
gkick_distortion_val(struct gkick_distortion *distortion,
                     gkick_real in_val,
                     gkick_real *out_val,
                     gkick_real env_x)
{
        gkick_distortion_lock(distortion);
#if (GEONKICK_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // GEONKICK_VERSION_MAJOR
        if (distortion->type == GEONKICK_DISTORTION_BACKWARD_COMPATIBLE) {
                distortion_backward_compatible(distortion,
                                               in_val,
                                               out_val,
                                               env_x);
                gkick_distortion_unlock(distortion);
                return GEONKICK_OK;
        }

        gkick_real x = distortion->in_limiter * in_val;
        gkick_real drive_env_val = gkick_envelope_get_value(distortion->drive_env, env_x);
        gkick_real drive = distortion->drive * gkick_envelope_get_value(distortion->drive_env, env_x);
        switch (distortion->type) {
        case GEONKICK_DISTORTION_HARD_CLIPPING:
                *out_val = GKICK_CLAMP(x, -1.0f / drive, 1.0f / drive);
                break;
        case GEONKICK_DISTORTION_SOFT_CLIPPING_TANH:
                *out_val = tanh(drive * x);
                break;
        case GEONKICK_DISTORTION_ARCTAN:
                *out_val = atan(drive * x);
                break;
        case GEONKICK_DISTORTION_EXPONENTIAL:
                *out_val = (x < 0.0f ? -1.0f : 1.0f) * (1.0f - exp(-drive * fabs(x)));
                break;
        case GEONKICK_DISTORTION_POLYNOMIAL:
                *out_val = x - (0.1f * drive * x * x * x) / 3.0f;
                break;
        case GEONKICK_DISTORTION_LOGARITHMIC:
                *out_val = log(1.0f + drive * fabs(x)) * (x < 0.0f ? -1.0f : 1.0f);
                break;
        case GEONKICK_DISTORTION_FOLDBACK:
                {
                        gkick_real threshold = 1.0f;
                        *out_val = fabs(fmod(x + threshold, 2.0f * threshold) - threshold) - threshold;
                }
                break;
        case GEONKICK_DISTORTION_HALF_WAVE_RECT:
                *out_val = x > 0.0f ? x : 0.0f;
                break;
        case GEONKICK_DISTORTION_FULL_WAVE_RECT:
                *out_val = fabs(x);
                break;
        default:
                *out_val = x;
                break;
        }

        *out_val *= distortion->out_limiter * gkick_envelope_get_value(distortion->volume_env, env_x);
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_out_limiter(struct gkick_distortion *distortion,
                                 gkick_real value)
{
        gkick_distortion_lock(distortion);
        distortion->out_limiter = value;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_out_limiter(struct gkick_distortion *distortion,
                            gkick_real *value)
{
        gkick_distortion_lock(distortion);
        *value = distortion->out_limiter;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_set_in_limiter(struct gkick_distortion *distortion,
                                gkick_real value)
{
	gkick_distortion_lock(distortion);
        distortion->in_limiter = value;
        gkick_distortion_unlock(distortion);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_distortion_get_in_limiter(struct gkick_distortion *distortion,
                                gkick_real *value)
{
	gkick_distortion_lock(distortion);
        *value = distortion->in_limiter;
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
