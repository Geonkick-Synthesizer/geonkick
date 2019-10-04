/**
 * File name: compressor.c
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

#include "compressor.h"

enum geonkick_error
gkick_compressor_new(struct gkick_compressor **compressor)
{
        if (compressor == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *compressor = (struct gkick_compressor*)malloc(sizeof(struct gkick_compressor));
        if (*compressor == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR;
        }

        (*compressor)->attack    = 0.01 * GEONKICK_SAMPLE_RATE;
        (*compressor)->release   = 0.01 * GEONKICK_SAMPLE_RATE;
        (*compressor)->threshold = 0.0;
        (*compressor)->ratio     = 1.0;
        (*compressor)->knee      = 0.0;
        (*compressor)->makeup    = 1.0;

        if (pthread_mutex_init(&(*compressor)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_compressor_free(compressor);
                return GEONKICK_ERROR;
	}

        return GEONKICK_OK;
}

void
gkick_compressor_free(struct gkick_compressor **compressor)
{
        if (compressor != NULL && *compressor != NULL) {
                pthread_mutex_destroy(&(*compressor)->lock);
                free(*compressor);
                *compressor = NULL;
        }
}

void gkick_compressor_lock(struct gkick_compressor *compressor)
{
        pthread_mutex_lock(&compressor->lock);
}

void gkick_compressor_unlock(struct gkick_compressor *compressor)
{
        pthread_mutex_unlock(&compressor->lock);
}

enum geonkick_error
gkick_compressor_enable(struct gkick_compressor *compressor, int enable)
{
        gkick_compressor_lock(compressor);
        compressor->enabled = enable;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_is_enabled(struct gkick_compressor *compressor, int *enabled)
{
        gkick_compressor_lock(compressor);
        *enabled = compressor->enabled;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}


enum geonkick_error
gkick_compressor_val(struct gkick_compressor *compressor,
                     gkick_real in_val,
                     gkick_real *out_val)
{
        gkick_compressor_lock(compressor);
        gkick_real threshold = compressor->threshold;
        if (fabs(compressor->threshold) < DBL_EPSILON || compressor->ratio < 1.0) {
                gkick_compressor_unlock(compressor);
                *out_val = compressor->makeup * in_val;
                return GEONKICK_OK;
        }

        gkick_real sign = in_val >= 0.0 ? 1.0 : -1.0;
        in_val = fabs(in_val);

        if (in_val > threshold) {
                gkick_real ratio = compressor->ratio;
                if (compressor->frames <= compressor->attack && compressor->attack > 0) {
                        // Linear increase of the ratio.
                        ratio = 1.0 + ((compressor->ratio - 1.0) / compressor->attack) * compressor->frames;
                        compressor->frames++;
                }
                gkick_real diff = (in_val - threshold) / ratio;
                *out_val = threshold + diff;
        } else {
                compressor->frames = 0;
                *out_val = in_val;
        }

        *out_val *= compressor->makeup;
        gkick_compressor_unlock(compressor);
        *out_val = sign * (*out_val);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_attack(struct gkick_compressor *compressor, gkick_real attack)
{
        gkick_compressor_lock(compressor);
        compressor->attack = GEONKICK_SAMPLE_RATE * attack;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_attack(struct gkick_compressor *compressor, gkick_real *attack)
{
        gkick_compressor_lock(compressor);
        *attack = (gkick_real)compressor->attack / GEONKICK_SAMPLE_RATE;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_release(struct gkick_compressor *compressor, gkick_real release)
{
        gkick_compressor_lock(compressor);
        compressor->release = GEONKICK_SAMPLE_RATE * release;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_release(struct gkick_compressor *compressor, gkick_real *release)
{
        gkick_compressor_lock(compressor);
        *release = (double)compressor->release / GEONKICK_SAMPLE_RATE;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_threshold(struct gkick_compressor *compressor, gkick_real threshold)
{
        gkick_compressor_lock(compressor);
        compressor->threshold = threshold;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_threshold(struct gkick_compressor *compressor, gkick_real *threshold)
{
        gkick_compressor_lock(compressor);
        *threshold = compressor->threshold;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_ratio(struct gkick_compressor *compressor, gkick_real ratio)
{
        gkick_compressor_lock(compressor);
        if (ratio < 1.0)
                compressor->ratio = 1.0;
        else
                compressor->ratio = ratio;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_ratio(struct gkick_compressor *compressor, gkick_real *ratio)
{
        gkick_compressor_lock(compressor);
        *ratio = compressor->ratio;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_knee(struct gkick_compressor *compressor, gkick_real knee)
{
        gkick_compressor_lock(compressor);
        compressor->knee = knee;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_knee(struct gkick_compressor *compressor, gkick_real *knee)
{
        gkick_compressor_lock(compressor);
        *knee = compressor->knee;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_set_makeup(struct gkick_compressor *compressor, gkick_real makeup)
{
        gkick_compressor_lock(compressor);
        compressor->makeup = makeup;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}

enum geonkick_error
gkick_compressor_get_makeup(struct gkick_compressor *compressor, gkick_real *makeup)
{
        gkick_compressor_lock(compressor);
        *makeup = compressor->makeup;
        gkick_compressor_unlock(compressor);
        return GEONKICK_OK;
}
