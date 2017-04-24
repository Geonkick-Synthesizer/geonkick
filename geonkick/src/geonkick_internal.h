#ifndef GEON_KICK_INTERNAL_H
#define GEON_KICK_INTERNAL_H

#include "geonkick.h"
//#include "gkick_jack.h"

#include <pthread.h>

struct geonkick {
  pthread_mutex_t lock;
  char name[30];
  double length;
  struct gkick_oscillator **oscillators;
  size_t oscillators_number;
  //struct gkick_jack *jack;
};

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick);

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate);

double
geonkick_get_oscillators_value(struct geonkick *kick, double t);

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index);

void geonkick_lock(struct geonkick *kick);

void geonkick_unlock(struct geonkick *kick);

#endif
