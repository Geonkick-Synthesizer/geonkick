#include "geonkick_internal.h"
#include "oscillator.h"

enum geonkick_error
geonkick_create(struct geonkick **kick)
{
	if (kick == NULL) {
		return GEONKICK_ERROR_NULL_POINTER;
	}

	*kick = (struct geonkick*)malloc(sizeof(struct geonkick));
	if (*kick == NULL) {
		return GEONKICK_ERROR_MEM_ALLOC;
	}
	memset(*kick, 0, sizeof(struct geonkick));

	strcpy((*kick)->name, "GeonKick");
	(*kick)->lengh = 1.0; // One second;
	(*kick)->oscillators_number = 2;

	if (pthread_mutex_init(&(*kick)->lock, NULL) == 0) {
	  geonkick_free(kick);
	  return GEONKICK_ERROR_INIT_MUTEX;
	}


        if (gkick_create_oscillators(*kick) != GEONKICK_OK) {
	  geonkick_free(kick);
	  return GEONKICK_ERROR_CANT_CREATE_OSC;
	}
	//	if (gkick_create_jack(*kick) != GEONKICK_OK) {
	//  geonkick_free(kick);
	//}

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
  size_t i;
  
  if (kick == NULL || *kick == NULL) {
    *kick = NULL;
    return;
  }

  // gkick_jack_free((*kick)->jack);
  for (i = 0; i < (*kick)->oscillators_number; i++) {
    gkick_osc_free(&((*kick)->oscillators[i]));
  }
  free((*kick)->oscillators);
  pthread_mutex_destroy(&(*kick)->lock);
  free(*kick);
  *kick = NULL;
}

size_t
geonkick_get_oscillators_number(struct geonkick *kick)
{
  size_t n;
  
  if (kick == NULL || kick->oscillators == NULL) {
    return 0;
  }

  pthread_mutex_lock(&kick->lock);
  n = 10; //kick->oscillators_number;
  pthread_mutex_unlock(&kick->lock);  
  
  return n;
}

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick)
{
  size_t size;
  size_t i;
  struct gkick_oscillator *osc;
  
  size = sizeof(struct gkick_oscillator*)
    * kick->oscillators_number;
  kick->oscillators = (struct gkick_oscillator**)malloc(size);

  if (kick->oscillators == NULL) {
	  return GEONKICK_ERROR_MEM_ALLOC;
  }
  memset(kick->oscillators, 0, size);

  for (i = 0; i < kick->oscillators_number; i++) {
    osc = gkick_osc_create();
    if (osc == NULL) {
      return GEONKICK_ERROR_CANT_CREATE_OSC;
    }
    kick->oscillators[i] = osc;
  }

  return GEONKICK_OK;
}


enum geonkick_error
geonkick_start(struct geonkick *kick)
{
     return gkick_create_jack(kick);
}

enum geonkick_error
geonkick_get_envelope_points(struct geonkick *kick,
			      enum gkick_envelope_type type,
			      double **points,
			      size_t *npoints)
{
  /*  if (kick == NULL && points != NULL && npoints != NULL) {
	  return GEONKICK_ERROR_NULL_POINTER;    
  }

  *points = NULL;
  *npoints = 0;
  if (type == GKICK_AMPLITUDE_ENVELOPE) {
    *points = gkick_envelope_get_points(kick)
  }
  */
  return GEONKICK_OK;
}

enum geonkick_error
geonkick_remove_envelope_point(struct geonkick *kick,
				enum gkick_envelope_type type,
				size_t index)
{
    return GEONKICK_OK;
}

enum geonkick_error
geonkick_add_envelope_point(struct geonkick *kick,
			    enum gkick_envelope_type type,
			    double x,
			    double y)
{
    return GEONKICK_OK;
}

enum geonkick_error
geonkick_update_envelope_point(struct geonkick *kick,
			       enum gkick_envelope_type type,
			       size_t index,
			       double x,
			       double y)
{
    return GEONKICK_OK;
}

/*double geonkick_get_graph(struct geonckick kick)
{
  geonkick_lock(kick);
  geonkick_save_state(kick);
  geonkick_set_begin(kick);

  t = 0.0;
  while( t < samplerate) {
    out[i] = t;
    out[i + 1] = gkick_get_oscialltors_value(kick, t);
    if (t > geonkick_get_length(kick)) {
      break;
    }
    t += 1.0 / samplerate;
  }
  
  /// get graph
  gkick_restore_state(kick);
  geonkick_ulock(kick);

  return out;
}
*/

double geonkick_get_oscillators_value(struct geonkick *kick, double t)
{
  double val = 0.0;

  /*  val = gkick_osc_value(kick->base_oscillator, t);
  val += gkick_osc_value(kick->noise_oscillator, t);
  
  gkick_osc_increment_phase(kick->base_oscillator, t);
  gkick_osc_increment_phase(kick->noise_oscillator, t);*/

  return val;  
}

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate)
{
  //gkick_set_osc_sample_rate(kick->base_oscillator, rate);
  //gkick_set_osc_sample_rate(kick->noise_oscillator, rate);
  return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 double **buf,
				 size_t *npoints)
{
  if (kick == NULL || buf == NULL || npoints == NULL) {
    return GEONKICK_ERROR_NULL_POINTER;
  }

  pthread_mutex_lock(&kick->lock);
  struct gkick_oscillator *osc = geonkick_get_oscillator(kick, osc_index);
  if (osc == NULL) {
    return GEONKICK_ERROR_NULL_POINTER;
  }
  gkick_osc_get_envelope_points(osc, env_index, buf, npoints);
  pthread_mutex_unlock(&kick->lock);  

  return GEONKICK_OK;
}

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index)
{
  if (index > -1 && index < kick->oscillators_number) {
    return kick->oscillators[index];
  }

  return NULL;
}

