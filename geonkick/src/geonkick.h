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

#include "gkick_log.h"

#define GEON_APP_VERSION 0x000100
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
	GEONKICK_OSC_FUNC_SQARE    = 1, 
	GEONKICK_OSC_FUNC_TRIANGLE = 2,
	GEONKICK_OSC_FUNC_SAWTOOTH = 3,
	GEONKICK_OSC_FUNC_NOISE    = 4
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
enum geonkick_error
geonkick_remove_envelope_point(struct geonkick *kick,
				enum geonkick_envelope_type type,
				size_t index);

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

#ifdef __cplusplus
}
#endif

#endif
