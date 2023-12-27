/**
 * File name: ring_buffer.c
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

#include "ring_buffer.h"

enum geonkick_error
ring_buffer_new(struct ring_buffer **ring,
                int size)
{
        if (ring == NULL) {
                gkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        *ring = (struct ring_buffer*)calloc(1, sizeof(struct ring_buffer));
        if (*ring == NULL) {
                gkick_log_error("can't allocate memory");
                return GEONKICK_ERROR;
        }
        (*ring)->max_size = size;
        (*ring)->size     = (*ring)->max_size;
        (*ring)->index    = 0;
        (*ring)->buff     = (gkick_real*)calloc(1, sizeof(gkick_real) * (*ring)->max_size);
        if ((*ring)->buff == NULL) {
                gkick_log_error("can't allocate memory");
                ring_buffer_free(ring);
                return GEONKICK_ERROR;
        }
        return GEONKICK_OK;
}

void
ring_buffer_free(struct ring_buffer **ring)
{
        if (ring == NULL || *ring == NULL)
                return;
        if ((*ring)->buff != NULL)
                free((*ring)->buff);
        free(*ring);
        *ring = NULL;
}

void
ring_buffer_reset(struct ring_buffer *ring)
{
        ring->index = 0;
        memset(ring->buff, 0, ring->size * sizeof(gkick_real));
}

void
ring_buffer_add_data(struct ring_buffer *ring,
                     const gkick_real *data,
                     size_t data_size)
{
        for (size_t i = 0; i < data_size; i++)
                ring->buff[(ring->index + i) % ring->size] += data[i];
}

void
ring_buffer_add_value(struct ring_buffer *ring,
                           size_t index,
                           gkick_real val)
{
        ring->buff[(ring->index + index) % ring->size] += val;
}

void
ring_buffer_get_data(struct ring_buffer *ring,
                     gkick_real *data,
                     size_t data_size)
{
        size_t size = min(ring->size, data_size);
        for (size_t i = 0; i < size; i++)
                data[i] += ring->buff[(ring->index + i) % ring->size];
}

void
ring_buffer_next(struct ring_buffer *ring,
                 size_t n)
{
        n = min(ring->size, n);
        memset(ring->buff + ring->index, 0, n * sizeof(gkick_real));
        ring->index = (ring->index + n) % ring->size;
}

size_t
ring_buffer_get_size(struct ring_buffer *ring)
{
        return ring->size;
}

size_t
ring_buffer_resize(struct ring_buffer *ring,
                   size_t size)
{
        ring->size = min(size, ring->max_size);
}
