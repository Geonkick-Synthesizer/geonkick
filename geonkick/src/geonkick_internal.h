#ifndef GEON_KICK_INTERNAL_H
#define GEON_KICK_INTERNAL_H

#include "geonkick.h"
#include "gkick_jack.h"

struct geonkick {
  pthread_mutex_t lock;
  char name[30];
  double lengh;
  struct gkick_oscillator *oscillators;
  struct gkick_jack *jack;
};

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate);

double
geonkick_get_oscillators_value(struct geonkick *kick, double t);

#endif
