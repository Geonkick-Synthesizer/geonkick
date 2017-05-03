#include "gkick_jack.h"
#include "oscillator.h"
double phase = 0.0;
int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg)
{
	int i;
	double val;
	struct geonkick *kick = (struct geonkick*)arg;
	struct gkick_jack *jack = kick->jack;
	
	//if (!jack->end) {
	//  return 0;
	//}
	
        jack_default_audio_sample_t *out_l
	  = (jack_default_audio_sample_t *)jack_port_get_buffer(jack->output_port_l, nframes);
	jack_default_audio_sample_t *out_r
	  = (jack_default_audio_sample_t *)jack_port_get_buffer(jack->output_port_r, nframes);
	
	for (i = 0; i < nframes; i++) {
	  val = geonkick_get_oscillators_value(kick, jack->time);
	  out_l[i] = val;
	  out_r[i] = val;
	  if (jack->time > 1.5) {
	    //jack->end = 1;
	    jack->time = 0.0;
	    geonkick_reset_oscillators(kick);
	  } else {
	    jack->time += 1.0 / jack->sample_rate;
	  }

	  //gkick_jack_increment_time(jack);
	}


	return 0;
}

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg)
{
  printf("sample rate: %u", nframes );
  //geonkick_set_sampe_rate((struct geonkick*)arg), (double)nframes);
	return 0;
}

void gkick_jack_increment_time(struct gkick_jack *jack)
{
  if (jack->time > 1.5 * jack->kick_len) {
    //jack->end = 1;
    jack->time = 0.0;
  } else {
    jack->time += 1.0/jack->sample_rate;
  }
}

enum geonkick_error
gkick_create_jack(struct geonkick *kick)
{

  struct gkick_jack *jack;

  jack = (struct gkick_jack*)malloc(sizeof(struct gkick_jack));
  if (jack == NULL) {
    return GEONKICK_ERROR_NULL_POINTER;
  }
  memset(jack, 0, sizeof(struct gkick_jack));
  jack->sample_rate = 48000;
  jack->kick_len = kick->length;
  jack->end = 0;

  kick->jack = jack;
  jack->client = jack_client_open(kick->name, JackNullOption, NULL);
  
  if (jack->client == NULL) {
    fprintf(stderr, "JACK server not running?\n");
    return GEONKICK_ERROR_OPEN_JACK;
  }
	
  jack_set_process_callback(jack->client,
			    gkick_jack_process_callback,
			    (void*)kick);
  jack_set_sample_rate_callback(jack->client,
				gkick_jack_srate_callback,
				(void*)kick);

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

void gkick_jack_free(struct gkick_jack **jack)
{
  if (jack != NULL && *jack != NULL) {
    jack_deactivate((*jack)->client);
    jack_port_unregister((*jack)->client, (*jack)->output_port_l);
    jack_port_unregister((*jack)->client, (*jack)->output_port_r);
    jack_client_close((*jack)->client);
    free(*jack);
    *jack = NULL;
  }
}
