/**
 * File name: geonkick_synthesizer.h
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

#ifndef GEONKICK_SYNTHESIZER_H
#define GEONKICK_SYNTHESIZER_H

#define GEONKICK_SAMPLE_RATE 48000
#define GKICK_OSC_DEFAULT_AMPLITUDE   0.005
#define GKICK_OSC_DEFAULT_FREQUENCY   150

/* Kick maximum length in seconds. */
#define GEONKICK_MAX_LENGTH 4
#define GEONKICK_MAX_BUFFER_SIZE  (4 * GEONKICK_SAMPLE_RATE)

struct geonkick_synth {
        double current_time;
        struct gkick_oscillator **oscillators;
        size_t oscillators_number;

        /* General synthesizer amplitude envelope. */
        struct gkick_envelope *envelope;

        /* Time length of the kick in seconds. */
        gkick_real length;

        /**
         * Current kick samples buffer size which is calculated
         * from the kick length in seconds and should not
         * exceed the GEONKICK_MAX_BUFFER_SIZE.
         */
        size_t buffer_size;

        /* Kick smaples buffer. */
        gkick_real *buffer;

        /* Bool: is to update or not the buffer. */
        int update_buffer;

        /**
         * The synthesizer main thread.
         */
        pthread_t thread;

        /* Output buffer for the synthesizer.
         * The access to its internal buffer
         * can be locked by other threads.
         */
        gkick_buffer *output;

        pthread_mutex_t lock;
        pthread_cond_t condition_var;
};

void gkick_synth_free(struct gkick_synth **synth);

void geonkick_synth_new(struct geonkick_synth **synth);

void geonkick_synth_lock(struct geonkick_synth *synth);

void geonkick_synth_unlock(struct geonkick_synth *synth);

enum geonkick_error
gkick_synth_create_oscillators(struct geonkick_synth *synth);

struct gkick_oscillator*
gkick_synth_get_oscillator(struct gkick_synth *synth, size_t index);

enum geonkick_error
gkick_synthesizer_disable_oscilaltor(struct geonkick_synth *synth);

struct gkick_envelope*
gkick_synth_osc_get_env(struct gkick_synth *synth,
                        size_t osc_index,
                        size_t env_index);

enum geonkick_error
gkick_synth_osc_envelope_points(struct gkick_synth *synth,
                                int osc_index,
                                int env_index,
                                double **buf,
                                size_t *npoints);



enum geonkick_error
gkick_synth_osc_env_add_point(struct gkikc_synth *synth,
                              int osc_index,
                              int env_index,
                              double x,
                              double y);

enum geonkick_error
gkick_synth_osc_env_remove_point(struct gkick_synth *synth,
                                 size_t osc_index,
                                 size_t env_index,
                                 size_t index);

enum geonkick_error
gkick_synth_osc_env_update_point(struct gkikc_synth *synth,
                                 int osc_index,
                                 int env_index,
                                 int index,
                                 double x,
                                 double y);

enum geonkick_error
gkick_synth_set_osc_function(struct gkick_synth *synth,
                             size_t osc_index,
                             geonkick_osc_func_type type);

enum geonkick_error
gkick_synth_get_length(struct gkick_synth *synth, size_t *len);

enum geonkick_error
gkick_synth_set_length(struct gkick_synth *synth, size_t len);

enum geonkick_error
gick_synth_set_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             double v);

enum geonkick_error
gick_synth_get_osc_frequency(struct gkick_synth *synth,
                             size_t osc_index,
                             double *v);

enum geonkick_error
gkick_synth_set_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              double v);

enum geonkick_error
gkick_synth_get_osc_amplitude(struct gkick_synth *synth,
                              size_t osc_index,
                              double *v);

enum geonkick_error
gkick_synth_get_buffer(struct gkick_synth *synth,
                       double *buffer,
                       size_t size);

enum geonkick_error
gkick_synth_start(struct gkick_synth *synth);

enum geonkick_error
gkick_synth_stop(struct gkick_synth *synth);

void *gkick_synth_run(void *arg);

gkick_real
gkick_synth_get_value(struct geonkick *synth, gkick_real t);

void gkick_synth_copy_to_output(struct gkick_synth *synth);

void gkick_synth_reset_oscillators(struct gkick_synth *synth);

void gkick_set_update_buffer(struct gkick_synth *synth, int update);

#endif // GEONKICK_SYNTHESIZER_H
