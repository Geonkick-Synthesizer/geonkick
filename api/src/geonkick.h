/**
 * File name: geonkick.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#ifndef GEONKICK_H
#define GEONKICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>

#include "gkick_log.h"

#define GEON_APP_VERSION 0x000301
#define GEON_APP_NAME "geonkick"

enum geonkick_error {
  	GEONKICK_OK,
	GEONKICK_ERROR,
	GEONKICK_ERROR_MEM_ALLOC,
	GEONKICK_ERROR_NULL_POINTER,
	GEONKICK_ERROR_INIT_MUTEX,
	GEONKICK_ERROR_CREATE_ENVELOPE,
	GEONKICK_ERROR_OPEN_JACK,
	GEONKICK_ERROR_CREATE_JACK,
	GEONKICK_ERROR_CANT_CREATE_OSC,
	GEONKICK_ERROR_ACTIVATE_JACK
};

enum geonkick_envelope_type {
  GEONKICK_AMPLITUDE_ENVELOPE = 0,
  GEONKICK_FREQUENCY_ENVELOPE = 1
};

enum geonkick_osc_func_type {
	GEONKICK_OSC_FUNC_SINE     = 0,
	GEONKICK_OSC_FUNC_SQUARE   = 1,
	GEONKICK_OSC_FUNC_TRIANGLE = 2,
	GEONKICK_OSC_FUNC_SAWTOOTH = 3,
	GEONKICK_OSC_FUNC_NOISE    = 4,
        GEONKICK_OSC_FUNC_UNKNOWN  = 5
};

struct geonkick;

enum geonkick_error
geonkick_create(struct geonkick **kick);

void
geonkick_free(struct geonkick **kick);

size_t
geonkick_get_oscillators_number(struct geonkick* kick);

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 double **buf,
				 size_t *npoints);

        /*enum geonkick_error
geonkick_envelope_get_points(struct geonkick *kick,
				 double **buf,
				 size_t *npoints);*/

        /*enum geonkick_error
geonkick_remove_envelope_point(struct geonkick *kick,
				enum geonkick_envelope_type type,
				size_t index);*/

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				double x,
				double y);

enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index);
enum geonkick_error
geonkick_osc_envelope_update_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
				   double x,
				   double y);

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t oscillatorIndex,
			  enum geonkick_osc_func_type type);

enum geonkick_error
geonkick_set_length(struct geonkick *kick, double t);

enum geonkick_error
geonkick_get_length(struct geonkick *kick, double *len);

enum geonkick_error
geonkick_set_osc_amplitude_val(struct geonkick *kick,
			       size_t osc_index,
			       double v);
enum geonkick_error
geonkick_set_osc_frequency_val(struct geonkick *kick,
			       size_t osc_index,
			       double v);

enum geonkick_error
geonkick_get_osc_amplitude_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v);
enum geonkick_error
geonkick_get_osc_frequency_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v);

enum geonkick_error
geonkick_play(struct geonkick *kick, int play);

int
geonkick_is_play(struct geonkick *kick);

#ifdef __cplusplus
}
#endif

#endif
