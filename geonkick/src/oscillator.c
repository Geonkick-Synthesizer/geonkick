#include "oscillator.h"

#include <math.h>

struct gkick_oscillator
*gkick_osc_create(void)
{
  struct gkick_oscillator *osc;

  osc = (struct gkick_oscillator*)malloc(sizeof(struct gkick_oscillator));
  if (osc == NULL) {
    return NULL;
  }
  memset(osc, 0, sizeof(struct gkick_oscillator));

  if (pthread_mutex_init(&osc->lock, NULL) == 0) {
    gkick_osc_free(&osc);
    return NULL;
  }

  osc->func = GKICK_OSC_FUNC_SINE;
  osc->phase = 0.0;
  osc->sample_rate = GKICK_OSC_DEFAULT_SAMPLE_RATE;
  osc->frequency = GKICK_OSC_DEFAULT_FREQUENCY;
  osc->env_number = 2;

  if (gkick_osc_create_envelopes(osc) != GEONKICK_OK) {
    gkick_osc_free(&osc);
    return NULL;
  }

  return osc;
}

void gkick_osc_free(struct gkick_oscillator **osc)
{
  size_t i;
  
  if (osc == NULL || *osc == NULL) {
    *osc = NULL;
    return;
  }

  for (i = 0; i < (*osc)->env_number; i++) {
    gkick_envelope_destroy((*osc)->envelopes[i]);
  }
  free((*osc)->envelopes);
  pthread_mutex_destroy(&(*osc)->lock);
  free(*osc);
  *osc = NULL;
}

enum geonkick_error gkick_osc_create_envelopes(struct gkick_oscillator *osc)
{
  size_t i;
  struct gkick_envelope *env;
  
  osc->envelopes = (struct gkick_envelope**)malloc(sizeof(struct gkick_envelope*) *osc->env_number);
  if (osc->envelopes == NULL) {
    return GEONKICK_ERROR_MEM_ALLOC;
  }
  memset(osc->envelopes, 0, sizeof(struct gkick_envelope*) * osc->env_number);

  for(i = 0; i < osc->env_number; i++) {
    env = gkick_envelope_create();
    if (env == NULL) {
      return GEONKICK_ERROR_CREATE_ENVELOPE;
    } else {
      osc->envelopes[i] = env;
    }
    
  }

  return GEONKICK_OK;
}

void gkick_osc_increment_phase(struct gkick_oscillator *osc,
			       double t)
{
  double freq;

  pthread_mutex_lock(&osc->lock);
  freq = osc->frequency * gkick_envelope_get_value(osc->envelopes[0], t);  

  osc->phase += ((2 * M_PI * freq) / osc->sample_rate);
  if (osc->phase > (2 * M_PI)) {
    osc->phase = osc->phase - (2 * M_PI);
  }
  pthread_mutex_unlock(&osc->lock);
}

double gkick_osc_value(struct gkick_oscillator *osc, double t)
{
  double amp;
  double v;

  pthread_mutex_lock(&(osc->lock));
  
  amp = gkick_envelope_get_value(osc->envelopes[1], t);  

  v = 0.0;
  if (osc->func == GKICK_OSC_FUNC_SINE) {
    v = amp * gkick_osc_func_sine(osc->phase);
  } else if (osc->func == GKICK_OSC_FUNC_SQARE)
  {
    v = amp * gkick_osc_func_sqare(osc->phase);
  }

  pthread_mutex_unlock(&(osc->lock));

  return v;
}

double gkick_osc_func_sine(double phase)
{
  return sin(2 * M_PI * phase);
}

double gkick_osc_func_sqare(double phase)
{
  if (phase < M_PI) {
    return 1;
  } else {
    return -1;
  }
}

void
gkick_osc_get_envelope_points(struct gkick_oscillator *osc,
			      size_t env_index,
			      double **buff,
			      size_t *npoints)
{
  *buff = NULL;
  if (env_index > -1 && env_index < osc->env_number) {
    gkick_envelope_get_points(osc->envelopes[env_index], buff, npoints);
  }
}
