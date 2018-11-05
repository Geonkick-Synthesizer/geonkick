/**
 * File name: gkick_synthesizer.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#ifndef GKICK_SYNTHESIZER_H
#define GKICK_SYNTHESIZER_H

#include "geonkick_internal.h"

struct gkick_synth {
        gkick_real current_time;
        struct gkick_oscillator **oscillators;
        size_t oscillators_number;

        /* Kick general amplitude */
        gkick_real amplitude;

        /* Time length of the kick in seconds. */
        gkick_real length;

        /* Kick general filter */
        struct gkick_filter *filter;
        int filter_enabled;

        /* General synthesizer amplitude envelope. */
        struct gkick_envelope *envelope;

        /**
         * Current kick samples buffer size which is calculated
         * from the kick length in seconds and should not
         * exceed the GEONKICK_MAX_BUFFER_SIZE.
         */
        size_t buffer_size;

        /* To update or not the buffer. */
        int buffer_update;


        /* Kick smaples buffer. */
        gkick_real *buffer;

        /**
         * Pointer to a funtion to be
         * called when kick synthesis is finished.
         */
        void (*buffer_callback) (void*);
        void *callback_args;

        /* Output buffer for the synthesizer.
         * The access to its internal buffer
         * can be locked by other threads.
         */
        struct gkick_buffer *output;

        /**
         * The synthesizer main thread.
         */
        pthread_t thread;
        int is_running;

        pthread_mutex_t lock;
        pthread_cond_t condition_var;
        /**
         * Specifies if the kick synthesis is tuned off.
         * If is 0 any updates of the synthesizer parameters
         * will not trigger the kick synthesisys.
         */
        int synthesis_on;
};

enum geonkick_error
gkick_synth_new(struct gkick_synth **synth);

void gkick_synth_free(struct gkick_synth **synth);

void gkick_synth_lock(struct gkick_synth *synth);

void gkick_synth_unlock(struct gkick_synth *synth);

enum geonkick_error
gkick_synth_create_oscillators(struct gkick_synth *synth);

enum geonkick_error
gkick_synth_get_oscillators_number(struct gkick_synth *synth, size_t *number);

struct gkick_oscillator*
gkick_synth_get_oscillator(struct gkick_synth *synth, size_t index);

enum geonkick_error
gkick_synth_enable_oscillator(struct gkick_synth *synth, size_t index, int enable);

enum geonkick_error
gkick_synth_osc_is_enabled(struct gkick_synth *synth, size_t index, int *enabled);

struct gkick_envelope*
gkick_synth_osc_get_env(struct gkick_synth *synth,
                        size_t osc_index,
                        size_t env_index);

enum geonkick_error
gkick_synth_osc_envelope_points(struct gkick_synth *synth,
                                int osc_index,
                                int env_index,
                                gkick_real **buf,
                                size_t *npoints);



enum geonkick_error
gkick_synth_osc_env_add_point(struct gkick_synth *synth,
                              int osc_index,
                              int env_index,
                              gkick_real x,
                              gkick_real y);

enum geonkick_error
gkick_synth_osc_env_remove_point(struct gkick_synth *synth,
                                 size_t osc_index,
                                 size_t env_index,
                                 size_t index);

enum geonkick_error
gkick_synth_osc_env_update_point(struct gkick_synth *synth,
                                 int osc_index,
                                 int env_index,
                                 int index,
                                 gkick_real x,
                                 gkick_real y);

enum geonkick_error
gkick_synth_set_osc_function(struct gkick_synth *synth,
                             size_t osc_index,
                             enum geonkick_osc_func_type type);

enum geonkick_error
gkick_synth_get_osc_function(struct gkick_synth *synth,
                             size_t osc_index,
                             enum geonkick_osc_func_type *type);

enum geonkick_error
gkick_synth_get_length(struct gkick_synth *synth, gkick_real *len);

enum geonkick_error
gkick_synth_set_length(struct gkick_synth *synth, gkick_real len);

enum geonkick_error
gkick_synth_kick_set_amplitude(struct gkick_synth *synth, gkick_real amplitude);

enum geonkick_error
gkick_synth_kick_get_amplitude(struct gkick_synth *synth, gkick_real *amplitude);

enum geonkick_error
geonkick_synth_kick_filter_enable(struct gkick_synth *synth, int enable);

enum geonkick_error
gkick_synth_kick_set_filter_frequency(struct gkick_synth *synth, gkick_real frequency);

enum geonkick_error
gkick_synth_kick_get_filter_frequency(struct gkick_synth *synth, gkick_real *frequency);

enum geonkick_error
gkick_synth_kick_set_filter_factor(struct gkick_synth *synth, gkick_real factor);

enum geonkick_error
gkick_synth_kick_get_filter_factor(struct gkick_synth *synth, gkick_real *factor);

enum geonkick_error
gkick_synth_set_kick_filter_type(struct gkick_synth *synth, enum gkick_filter_type type);

enum geonkick_error
gkick_synth_get_kick_filter_type(struct gkick_synth *synth, enum gkick_filter_type *type);

enum geonkick_error
gkick_synth_kick_envelope_get_points(struct gkick_synth *synth,
                                     gkick_real **buf,
                                     size_t *npoints);

enum geonkick_error
gkick_synth_kick_add_env_point(struct gkick_synth *synth, gkick_real x, gkick_real y);

enum geonkick_error
gkick_synth_kick_remove_env_point(struct gkick_synth *synth, size_t index);

enum geonkick_error
gkick_synth_kick_update_env_point(struct gkick_synth *synth,
                                  size_t index,
                                  gkick_real x,
                                  gkick_real y);

enum geonkick_error
gkick_synth_set_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             gkick_real v);

enum geonkick_error
gkick_synth_get_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             gkick_real *v);

enum geonkick_error
gkick_synth_set_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real v);

enum geonkick_error
gkick_synth_get_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              gkick_real *v);

enum geonkick_error
gkick_synth_get_buffer_size(struct gkick_synth *synth,
                            size_t *size);

enum geonkick_error
gkick_synth_get_buffer(struct gkick_synth *synth,
                       gkick_real *buffer,
                       size_t size);

enum geonkick_error
gkick_synth_set_buffer_callback(struct gkick_synth *synth, void (*callback) (void*), void *args);

void gkick_synth_set_output(struct gkick_synth *synth,
                            struct gkick_buffer *buffer);

enum geonkick_error
gkick_synth_start(struct gkick_synth *synth);

enum geonkick_error
gkick_synth_stop(struct gkick_synth *synth);

void gkick_synth_set_started(struct gkick_synth *synth, int started);

int gkick_synth_is_running(struct gkick_synth *synth);

void *gkick_synth_run(void *arg);

gkick_real
gkick_synth_get_value(struct gkick_synth *synth, gkick_real t);

void gkick_synth_reset_oscillators(struct gkick_synth *synth);

int gkick_synth_is_update_buffer(struct gkick_synth *synth);

enum geonkick_error
gkick_synth_set_osc_filter_type(struct gkick_synth *synth,
                                size_t osc_index,
                                enum gkick_filter_type type);

enum geonkick_error
gkick_synth_get_osc_filter_type(struct gkick_synth *synth,
                                size_t osc_index,
                                enum gkick_filter_type *type);

enum geonkick_error
gkick_synth_set_osc_filter_cutoff(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real cutoff);

enum geonkick_error
gkick_synth_get_osc_filter_cutoff(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real *cutoff);

enum geonkick_error
gkick_synth_set_osc_filter_factor(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real factor);

enum geonkick_error
gkick_synth_get_osc_filter_factor(struct gkick_synth *synth,
                                  size_t osc_index,
                                  gkick_real *factor);

enum geonkick_error
gkick_synth_osc_enable_filter(struct gkick_synth *synth,
                              size_t osc_index,
                              int enable);

enum geonkick_error
gkick_synth_osc_is_enabled_filter(struct gkick_synth *synth,
                                  size_t osc_index,
                                  int *enabled);

void gkick_synth_wakeup_thread(struct gkick_synth *synth);

void gkick_synth_enable_synthesis(struct gkick_synth *synth, int enable);


#endif // GEONKICK_SYNTHESIZER_H
