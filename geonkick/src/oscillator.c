#include "oscillator.h"

#include <math.h>

void gkick_osc_increment_phase(struct gkick_oscillator *osc,
			       double t)
{
  double freq;

  pthread_mutex_lock(&osc->lock);
  freq = osc->frequency * gkick_envelope_get_value(osc->freq_envelope, t);  

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
  
  amp = gkick_envelope_get_value(osc->amp_envelope, t);  

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
