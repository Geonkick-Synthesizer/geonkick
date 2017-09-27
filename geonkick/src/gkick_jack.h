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
        jack_nframes_t sample_rate;
        pthread_mutex_t lock;
};

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg);

jack_nframes_t
gkick_jack_sample_rate(struct gkick_jack *jack);

enum geonkick_error
gkick_jack_get_output_buffers(struct gkick_jack *jack,
                              jack_default_audio_sample_t **channels_bufs,
                              jack_nframes_t nframes);

int gkick_jack_is_note_pressed(struct gkick_jack *jack,
                               jack_nframes_t nframes);

jack_port_t*
gkick_jack_get_midi_in_port(struct gkick_jack *jack);

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg);

enum geonkick_error
gkick_jack_enable_midi_in(struct gkick_jack *jack, const char *name);

enum geonkick_error
gkick_jack_create_output_ports(struct gkick_jack *jack);

enum geonkick_error
gkick_create_jack(struct geonkick *kick);

int gkick_jack_is_midi_in_enabled(struct gkick_jack *jack);

void gkick_jack_free(struct gkick_jack **jack);

void gkick_jack_lock(struct gkick_jack *jack);

void gkick_jack_unlock(struct gkick_jack *jack);


#endif
