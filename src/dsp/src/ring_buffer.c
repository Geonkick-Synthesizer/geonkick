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

void ring_buffer_new(struct ring_buffer **ring, int size)
{
}

void ring_buffer_free(struct ring_buffer **ring)
{
}

void ring_buffer_clear(struct ring_buffer *ring)
{
        ring->index = 0;
        memset(ring->buff, 0, size * sizeof(gkick_real));
}

void ring_buffer_add_data(struct ring_buffer *buffer,
                          const gkick_real *data,
                          size_t data_size)
{
        for (size_t i = 0; i < data_size; i++)
                ring->buff[(ring->index + i) % ring->size] += data[i];
}

void ring_buffer_get_data(struct ring_buffer *buffer,
                          gkick_real *data,
                          size_t data_size)
{
        size_t max_data_size = min(ring->size, data_size);
        memcopy(data, ring->buff, data_size * sizeof(gkick_real));
        memset(ring->buff + ring->index, 0, max_data_size);
        ring->index = (ring->index + max_data_size) % ring->size;
}

size_t ring_buffer_get_size(struct ring_buffer *buffer)
{
        return ring->size;
}

size_t
ring_buffer_resize(struct ring_buffer *buffer, size_t size)
{
        ring->size = min(size, ring->max_size);
}

