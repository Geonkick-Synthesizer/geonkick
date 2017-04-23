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
#include <pthread.h>

#define GEON_APP_VERSION 0x000100
#define GEON_APP_NAME "geonkick"  
  
enum geonkick_error {
  	GEONKICK_OK,
	GEONKICK_ERROR_MEM_ALLOC,
	GEONKICK_ERROR_NULL_POINTER,
	GEONKICK_ERROR_CREATE_ENVELOPE,
	GEONKICK_ERROR_OPEN_JACK,
	GEONKICK_ERROR_ACTIVATE_JACK
};

enum gkick_envelope_type {
  GKICK_AMPLITUDE_ENVELOPE = 0,
  GKICK_FREQUENCY_ENVELOPE = 1
};

struct geonkick;

enum geonkick_error
geonkick_create(struct geonkick **kick);

enum geonkick_error
geonkick_free(struct geonkick **kick);

enum geonkick_error
geonkick_get_envelope_points(struct geonkick *kick,
			      enum gkick_envelope_type type,
			      double **points,
			      size_t *npoints);

enum geonkick_error
geonkick_remove_envelope_point(struct geonkick *kick,
				enum gkick_envelope_type type,
				size_t index);

enum geonkick_error
geonkick_add_envelope_point(struct geonkick *kick,
			    enum gkick_envelope_type type,
			    double x,
			    double y);

enum geonkick_error
geonkick_update_envelope_point(struct geonkick *kick,
			       enum gkick_envelope_type type,
			       size_t index,
			       double x,
			       double y);
  
#ifdef __cplusplus
}
#endif

#endif
