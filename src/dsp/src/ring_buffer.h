/**
 * File name: ring_buffer.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2023 Iurie Nistor 
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

#ifndef GKICK_RING_BUFFER_H
#define GKICK_RING_BUFFER_H

#include "geonkick_internal.h"

struct ring_buffer {
        gkick_real *buff;

        /**
         * Real allocated fixed size, alwayse bigger than current size.
         * Never changes during the life-time of the buffer.
         */
        size_t max_size;

        /**
         * Current size of the buffer. Can change during
         * the life-time of the buffer.
         */
        size_t size;

        /**
         * Current position in the buffer.
         */
        size_t index;
};

enum geonkick_error
ring_buffer_new(struct ring_buffer **ring,
                int size);

void
ring_buffer_free(struct ring_buffer **ring);

void
ring_buffer_reset(struct ring_buffer *ring);

void
ring_buffer_add_data(struct ring_buffer *ring,
                     const gkick_real *data,
                     size_t data_size);

void
ring_buffer_add_value(struct ring_buffer *ring,
                      size_t index,
                      gkick_real val);

void
ring_buffer_get_data(struct ring_buffer *ring,
                     gkick_real *data,
                     size_t data_size);

void
ring_buffer_next(struct ring_buffer *ring,
                 size_t n);

size_t
ring_buffer_get_size(struct ring_buffer *ring);

size_t
ring_buffer_resize(struct ring_buffer *ring,
                   size_t size);

#endif // GKICK_RING_BUFFER_H
