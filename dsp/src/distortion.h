/**
 * File name: distortion.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://geontime.com>
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

#ifndef GEONKICK_DISTORTION_H
#define GEONKICK_DISTORTION_H

#include "geonkick_internal.h"

struct gkick_distortion {
        int enabled;
	/* Input limiter for distortion. */
	gkick_real in_limiter;
        gkick_real volume;
        gkick_real drive;
	struct gkick_envelope *drive_env;
        struct gkick_envelope *volume_env;
        pthread_mutex_t lock;
};

enum geonkick_error
gkick_distortion_new(struct gkick_distortion **distortion);

void
gkick_distortion_free(struct gkick_distortion **distortion);

void gkick_distortion_lock(struct gkick_distortion *distortion);

void gkick_distortion_unlock(struct gkick_distortion *distortion);

enum geonkick_error
gkick_distortion_enable(struct gkick_distortion *distortion, int enable);

enum geonkick_error
gkick_distortion_is_enabled(struct gkick_distortion *distortion, int *enabled);

enum geonkick_error
gkick_distortion_val(struct gkick_distortion *distortion,
                     gkick_real in_val,
                     gkick_real *out_val,
		     gkick_real env_x);

enum geonkick_error
gkick_distortion_set_volume(struct gkick_distortion *distortion,
                            gkick_real volume);

enum geonkick_error
gkick_distortion_get_volume(struct gkick_distortion *distortion,
                            gkick_real *volume);

enum geonkick_error
gkick_distortion_set_in_limiter(struct gkick_distortion *distortion,
                                gkick_real limit);

enum geonkick_error
gkick_distortion_get_in_limiter(struct gkick_distortion *distortion,
                                gkick_real *limit);

enum geonkick_error
gkick_distortion_set_drive(struct gkick_distortion *distortion,
                           gkick_real drive);

enum geonkick_error
gkick_distortion_get_drive(struct gkick_distortion *distortion,
                           gkick_real *drive);

#endif // GEONKICK_DISTORTION_H
