/**
 * File name: compressor.h
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

#ifndef GEONKICK_COMPRESSOR_H
#define GEONKICK_COMPRESSOR_H

#include "geonkick_internal.h"

struct gkick_compressor {
        int enabled;

        /* Attack and release time in number of frames/samples. */
        uint64_t attack;
        uint64_t release;

        /* Threshold in -dB. */
        gkick_real threshold;
        /* Ratio from 1.0 to 60. */
        gkick_real ratio;
        /* Knee in dB. */
        gkick_real knee;
        /* Makeup in dB. */
        gkick_real makeup;

        /* Number of frames/smaples elapsed since the start of compression. */
        uint64_t frames;
        /* The frame/sample at which the compressor is deactivated. */
        uint64_t deactivation;
        pthread_mutex_t lock;
};

enum geonkick_error
gkick_compressor_new(struct gkick_compressor **compressor);

void
gkick_compressor_free(struct gkick_compressor **compressor);

void gkick_compressor_lock(struct gkick_compressor *compressor);

void gkick_compressor_unlock(struct gkick_compressor *compressor);

enum geonkick_error
gkick_compressor_enable(struct gkick_compressor *compressor, int enable);

enum geonkick_error
gkick_compressor_is_enabled(struct gkick_compressor *compressor, int *enabled);

enum geonkick_error
gkick_compressor_val(struct gkick_compressor *compressor,
                     gkick_real in_val,
                     gkick_real *out_val);

enum geonkick_error
gkick_compressor_set_attack(struct gkick_compressor *compressor, gkick_real attack);

enum geonkick_error
gkick_compressor_get_attack(struct gkick_compressor *compressor, gkick_real *attack);

enum geonkick_error
gkick_compressor_set_release(struct gkick_compressor *compressor, gkick_real release);

enum geonkick_error
gkick_compressor_get_release(struct gkick_compressor *compressor, gkick_real *release);

enum geonkick_error
gkick_compressor_set_threshold(struct gkick_compressor *compressor, gkick_real threshold);

enum geonkick_error
gkick_compressor_get_threshold(struct gkick_compressor *compressor, gkick_real *threshold);

enum geonkick_error
gkick_compressor_set_ratio(struct gkick_compressor *compressor, gkick_real ratio);

enum geonkick_error
gkick_compressor_get_ratio(struct gkick_compressor *compressor, gkick_real *ratio);

enum geonkick_error
gkick_compressor_set_knee(struct gkick_compressor *compressor, gkick_real knee);

enum geonkick_error
gkick_compressor_get_knee(struct gkick_compressor *compressor, gkick_real *knee);

enum geonkick_error
gkick_compressor_set_makeup(struct gkick_compressor *compressor, gkick_real makeup);

enum geonkick_error
gkick_compressor_get_makeup(struct gkick_compressor *compressor, gkick_real *makeup);

#endif // GEONKICK_COMPRESSOR_H
