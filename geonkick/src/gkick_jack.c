#include "gkick_jack.h"

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg)
{
	int i;
	double t;
	double val;
	struct geonkick *kick = (struct geonkick*)arg;
	struct gkick_jack *jack = kick->jack;

	if (kick == NULL) {
	  return 0;
	}
	
        jack_default_audio_sample_t *out_l
	  = (jack_default_audio_sample_t *)jack_port_get_buffer(jack->output_port_l, nframes);
	jack_default_audio_sample_t *out_r
	  = (jack_default_audio_sample_t *)jack_port_get_buffer(jack->output_port_r, nframes);

	t = 0.0;
	for (i = 0; i < nframes; i++) {
	  //t = gkick_get_jack_time(jack);
	  val = geonkick_get_oscillators_value(kick, t);
	  out_l[i] = val;
	  out_r[i] = val;
	  //gkick_jack_increment_time(jack);
	}

	return 0;
}

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg)
{
  //geonkick_set_sampe_rate((struct geonkick*)arg), (double)nframes);
	return 0;
}

enum geonkick_error
gkick_create_jack(struct geonkick *kick)
{

  struct gkick_jack *jack;

  jack = kick->jack;
  //name = geonkick_get_opt_name(kick);
  jack->client = jack_client_open(kick->name, JackNullOption, NULL);
  
  if (jack->client == NULL) {
    fprintf(stderr, "JACK server not running?\n");
    return GEONKICK_ERROR_OPEN_JACK;
  }
	
  jack_set_process_callback(jack->client, gkick_jack_process_callback, (void*)kick);
  jack_set_sample_rate_callback(jack->client, gkick_jack_srate_callback, (void*)kick);

  jack->output_port_l = jack_port_register(jack->client, "audio_out_L",
					   JACK_DEFAULT_AUDIO_TYPE,
					   JackPortIsOutput, 0);
  jack->output_port_r = jack_port_register(jack->client, "audio_out_R",
					   JACK_DEFAULT_AUDIO_TYPE,
					   JackPortIsOutput, 0);

  if (jack_activate(jack->client)) {
    fprintf(stderr, "cannot activate client");
    return GEONKICK_ERROR_ACTIVATE_JACK;
  }
	return GEONKICK_OK;
}
