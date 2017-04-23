#ifndef GKICK_JACK_H
#define GKICK_JACK_H

#include "geonkick_internal.h"

#include <jack/jack.h>
#include <jack/midiport.h>

struct gkick_jack {
  jack_port_t *output_port_l;
  jack_port_t *output_port_r;
  jack_client_t *client;
  double time;
  jack_nframes_t sample_rate;
};

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg);

int
gkick_jack_srate_callback(jack_nframes_t nframes, void *arg);

enum geonkick_error
gkick_create_jack(struct geonkick *kick);

#endif
