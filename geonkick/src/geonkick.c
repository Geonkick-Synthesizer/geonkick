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

	strcpy((*kick)->name, "GeonKick");
	(*kick)->lengh = 1.0; // One second;
	
	//(*kick)->aplitude_envelope = gkick_envelope_create();
	//if ((*kick)->aplitude_envelope == NULL ) {
	//	geonkick_free(kick);
	//	return GEONKICK_ERROR_CREATE_ENVELOPE;
	//}
	
	/* Add two default points. */
	//gkick_envelope_add_point((*kick)->aplitude_envelope, 0.0, 1.0);
	//gkick_envelope_add_point((*kick)->aplitude_envelope, (*kick)->length, 1.0);

	//(*kick)->aplitude_envelope = gkick_envelope_create();
	//if ((*kick)->aplitude_envelope == NULL ) {
	//	geonkick_free(kick);
	//	return GEONKICK_ERROR_CREATE_ENVELOPE;
	//}
	
	/* Add two default points. */
	//gkick_envelope_add_point((*kick)->aplitude_envelope, 0.0, 1.0);
	//gkick_envelope_add_point((*kick)->aplitude_envelope, (*kick)->length, 1.0);

	return GEONKICK_OK;
}

enum geonkick_error
geonkick_start(struct geonkick *kick)
{
     return gkick_create_jack(kick);
}

enum geonkick_error
geonkick_free(struct geonkick **kick)
{
	if (*kick == NULL) {
	  return GEONKICK_ERROR_NULL_POINTER;
	}

	//geonkick_stop();

	//gkick_envelope_free((*kick)->ampilutde_envelope);
	//gkick_envelope_free((*kick)->frequency_envelope);
	free(*kick);
	*kick = NULL;

	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_envelope_points(struct geonkick *kick,
			      enum gkick_envelope_type type,
			      double **points,
			      size_t *npoints)
{
  if (kick == NULL && points != NULL && npoints != NULL) {
	  return GEONKICK_ERROR_NULL_POINTER;    
  }

  *points = NULL;
  *npoints = 0;
  if (type == GKICK_AMPLITUDE_ENVELOPE) {
    *points = gkick_envelope_get_points(kick)
  }
  
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
  double val;

  val = gkick_osc_value(kick->base_oscillator, t);
  val += gkick_osc_value(kick->noise_oscillator, t);
  
  gkick_osc_increment_phase(kick->base_oscillator, t);
  gkick_osc_increment_phase(kick->noise_oscillator, t);

  return val;  
}

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate)
{
  //gkick_set_osc_sample_rate(kick->base_oscillator, rate);
  //gkick_set_osc_sample_rate(kick->noise_oscillator, rate);
  return GEONKICK_OK;
}
