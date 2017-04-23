#ifndef GKICK_OSCILLATOR_H
#define GKICK_OSCILLATOR_H
/*
  Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "geonkick_internal.h"
#include "envelope.h"

#include <pthread.h>

enum GKICK_OSC_FUNC_TYPE {
	GKICK_OSC_FUNC_SINE,
	GKICK_OSC_FUNC_SQARE, 
	GKICK_OSC_FUNC_TRIANGLE,
	GKICK_OSC_FUNC_SAWTOOTH,
	GKICK_OSC_FUNC_NOISE
};

struct gkick_oscillator {
  enum GKICK_OSC_FUNC_TYPE func;
  double phase;
  double sample_rate;
  double frequency;
  struct gkick_envelope *amp_envelope;
  struct gkick_envelope *freq_envelope;
  pthread_mutex_t lock;
};

struct gkick_oscillator
*gkick_osc_create(void);

void
gkick_osc_increment_phase(struct gkick_oscillator *osc,
			  double t);
double
gkick_osc_value(struct gkick_oscillator *osc, double t);

double
gkick_osc_func_sine(double phase);

double
gkick_osc_func_sqare(double phase);

#endif
