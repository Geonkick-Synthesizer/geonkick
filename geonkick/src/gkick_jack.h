#ifndef GKICK_JACK_H
#define GKICK_JACK_H

#include "geonkick_internal.h"

#include <jack/jack.h>
#include <jack/midiport.h>

struct gkick_jack {
  jack_port_t *output_port_l;
  jack_port_t *output_port_r;
  jack_port_t *midi_in_port;
  jack_client_t *client;
  double time;
  double kick_len;
  jack_nframes_t sample_rate;
  int end;
  pthread_mutex_t lock;
};

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg);

int
gkick_jack_srate_callback(jack_nframes_t nframes, void *arg);

enum geonkick_error
gkick_create_jack(struct geonkick *kick);

void gkick_jack_free(struct gkick_jack **jack);

#endif
