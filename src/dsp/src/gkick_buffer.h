/**
 * File name: gkick_buffer.h
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

#ifndef GKICK_BUFFER_H
#define GKICK_BUFFER_H

#include "geonkick_internal.h"

struct gkick_buffer {
        gkick_real *buff;

        /**
         * Real allocated fixed size, alwayse bigger than current size.
         * Never changes during the life-time of the buffer.
         */
        size_t max_size;

        /**
         * Current position in the buffer.
         */
        size_t currentIndex;
        gkick_real floatIndex;

        /**
         * Current size of the buffer. Can change during
         * the life-time of the buffer.
         */
        size_t size;
};

void
gkick_buffer_new(struct gkick_buffer **buffer, int size);

void
gkick_buffer_free(struct gkick_buffer **buffer);

void
gkick_buffer_reset(struct gkick_buffer *buffer);

void
gkick_buffer_set_data(struct gkick_buffer *buffer,
                      const gkick_real *data,
                      size_t size);

void
gkick_buffer_set_at(struct gkick_buffer *buffer,
                    size_t index,
                    gkick_real val);

gkick_real
gkick_buffer_get_at(struct gkick_buffer *buffer,
                    size_t index);

gkick_real
gkick_buffer_get_next(struct gkick_buffer *buffer);

gkick_real
gkick_buffer_stretch_get_next(struct gkick_buffer *buffer,
                                         gkick_real factor);

void
gkick_buffer_set_size(struct gkick_buffer *buffer,
                      size_t size);

size_t
gkick_buffer_size(struct gkick_buffer *buffer);

size_t
gkick_buffer_index(struct gkick_buffer *buffer);

void
gkick_buffer_push_back(struct gkick_buffer *buffer,
                       gkick_real val);

bool
gkick_buffer_is_end(struct gkick_buffer *buffer);

#endif // GKICK_BUFFER_H
