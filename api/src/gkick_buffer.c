/**
 * File name: gkick_buffer.c
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

#include "gkick_buffer.h"

void gkick_buffer_new(struct gkick_buffer **buffer, int size)
{
        if (buffer == NULL || size < 1) {
                gkick_log_error("wrong argumnets");
                return;
        }

        *buffer = (struct gkick_buffer*)malloc(sizeof(struct gkick_buffer));
        if (*buffer == NULL) {
                gkick_log_error("can't allocate memory");
                return;
        }
        (*buffer)->max_size = size;
        (*buffer)->size = size;

        if (pthread_mutex_init(&(*buffer)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_buffer_free(buffer);
                return;
	}

        (*buffer)->buff = (gkick_real*)malloc(sizeof(gkick_real) * size);
        if ((*buffer)->buff == NULL) {
                gkick_log_error("can't allocate memory");
                gkick_buffer_free(buffer);
                return;
        }
}

void gkick_buffer_free(struct gkick_buffer **buffer)
{
        if (buffer == NULL || *buffer == NULL) {
                return;
        }

        if ((*buffer)->buff != NULL) {
                free((*buffer)->buff);
        }
        pthread_mutex_destroy(&(*buffer)->lock);
        free(*buffer);
        *buffer = NULL;
}

int gkick_buffer_set_data(struct gkick_buffer *buffer, gkick_real *data, size_t size)
{
        if (buffer == NULL || data == NULL) {
                return 0;
        }

        if (size < 1) {
                return 1;
        }

        pthread_mutex_lock(&buffer->lock);
        if (size <= buffer->max_size) {
                memcpy(buffer->buff, data, sizeof(gkick_real) * size);
                buffer->size = size;
        } else {
                gkick_log_error("maximum allowed buffer size");
                pthread_mutex_unlock(&buffer->lock);
                return 0;
        }

        pthread_mutex_unlock(&buffer->lock);
        return 1;
}

gkick_real gkick_buffer_get_at(struct gkick_buffer *buffer,  size_t index, size_t *is_end)
{
        gkick_real val;

        if (buffer == NULL) {
                if (is_end != NULL) {
                        *is_end = 1;
                }
                return 0;
        }

        if (is_end != NULL) {
                *is_end = 1;
        }

        val = 0;
        pthread_mutex_lock(&buffer->lock);
        if (index < buffer->size) {
                if (is_end != NULL) {
                        *is_end = 0;
                }
                val = buffer->buff[index];
        }
        pthread_mutex_unlock(&buffer->lock);

        return val;
}
