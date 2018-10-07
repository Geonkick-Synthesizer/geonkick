/**
 * File name: gkick_buffer.h
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

#ifndef GKICK_BUFFER_H
#define GKICK_BUFFER_H

#include "geonkick_internal.h"

typedef struct {
        gkick_real *buff;

        /* Real fixed size alwayse bigger than size. */
        size_t max_size;

        /* Current size set for the buffer. */
        size_t size;
        pthread_mutex_t lock;
} gkick_buffer;

void gkick_buffer_new(gkick_buffer **buffer, int size);

void gkick_buffer_free(gkick_buffer **buffer);

int gkick_buffer_set_data(gkick_buffer *buffer, gkick_real *data, size_t size);

gkick_real gkick_buffer_get_at(gkick_buffer *buffer,  size_t index, size_t *is_end);

#endif // GKICK_BUFFER_H
