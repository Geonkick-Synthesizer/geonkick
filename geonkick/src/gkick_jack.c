#include "gkick_jack.h"
#include "oscillator.h"

int
gkick_jack_process_callback(jack_nframes_t nframes,
			    void *arg)
{
        int i;
        double val;
        int note_pressed;
        struct geonkick *kick;
        struct gkick_jack *jack;
        double kick_len;
        double delta_time;
        jack_nframes_t sample_rate;
        /* Lef and right channels buffers. */
        jack_default_audio_sample_t *buffers[2];

        kick = (struct geonkick*)arg;
        jack = kick->jack;

        note_pressed = gkick_jack_is_note_pressed(jack, nframes);
        if (note_pressed) {
                geonkick_start_play(kick);
        }

        if (geonckick_is_play_stopped(kick)) {
                /* Don't synthesize, just return. */
                return 0;
        }

        if (gkick_jack_get_output_buffers(jack, buffers, nframes)
            != GEONKICK_OK) {
                geonkick_log_error("can't get output buffers");
                return 0;
        }

        sample_rate = gkick_jack_sample_rate(jack);
        delta_time = 1.0 / sample_rate;
        for (i = 0; i < nframes; i++) {
                val = geonkick_get_oscillators_value(kick);
                buffers[0][i] = val;
                buffers[1][i] = val;
                if (geonckick_is_play_stopped(kick)) {
                        return 0;
                } else {
                        geonkick_increment_time(kick, delta_time);
                }
        }

        return 0;
}

jack_nframes_t
gkick_jack_sample_rate(struct gkick_jack *jack)
{
        jack_nframes_t sample_rate;

        if (jack == NULL) {
                geonkick_log_error("wrong arguments");
                return 0;
        }

        gkick_jack_lock(jack);
        sample_rate = jack->sample_rate;
        gkick_jack_lock(jack);

        return sample_rate;
}

enum geonkick_error
gkick_jack_get_output_buffers(struct gkick_jack *jack,
                              jack_default_audio_sample_t *channels_bufs,
                              jack_nframes_t nframes)
{
        enum geonkick_error error;

        if (jack == NULL || channels_bufs == NULL) {
                geonkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        error = GEONKICK_OK;
        gkick_jack_lock(jack);
        if (jack->output_port_r == NULL || jack->output_port_l == NULL) {
                geonkick_log_error("output ports are undefined");
                error = GEONKICK_ERROR;
        } else {
                *(channels_bufs[0])
                        = (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port_l, nframes);
                *(channels_bufs[1])
                        = (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port_r, nframes);
        }
        gkick_jack_unlock(jack);

        if (*channels_bufs[0] == NULL || *channels_bufs[1] == NULL) {
                error = GEONKICK_ERROR;
        }

        return error;
}

int gkick_jack_is_note_pressed(struct gkick_jack *jack,
                               jack_nframes_t nframes)
{
        int i;
        jack_nframes_t event_index;
        void* port_buf;
        jack_midi_event_t in_event;
        jack_nframes_t event_count;
        int note_pressed;

        if (gkick_jack_get_midi_in_port(jack) == NULL) {
                return 0;
        }

        port_buf    = jack_port_get_buffer(jack->midi_in_port, nframes);
        event_count = jack_midi_get_event_count(port_buf);

        note_pressed = 0;
        for (event_index = 0; event_index < event_count; event_index++) {
                jack_midi_event_get(&in_event, port_buf, 0);
                if ( ((*(in_event.buffer) & 0xf0)) == 0x90) {
                        note_pressed = 1;
                }
        }

        return note_pressed;
}

int gkick_jack_srate_callback(jack_nframes_t nframes, void *arg)
{
	return 0;
}

enum geonkick_error
gkick_jack_enable_midi_in(struct gkick_jack *jack, const char *name)
{
        const char *midi_name;
        enum geonkick_error error;

        if (jack == NULL) {
                geonkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        if (name == NULL) {
                midi_name = "midi_in";
        } else {
                midi_name = name;
        }

        error = GEONKICK_OK;
        gkick_jack_lock(jack);
        if (jack->midi_in_port == NULL) {
                jack->midi_in_port = jack_port_register(client, midi_name,
                                                JACK_DEFAULT_MIDI_TYPE,
                                                JackPortIsInput, 0);
                if (jack->midi_in_port == NULL) {
                        geonkick_log_error("can't register port: %s",
                                           midi_name);
                        error = GEONKICK_ERROR;
                }
        } else {
                geonkick_log_warning("midi in is already set");
        }
        gkick_jack_unlock(jack);

        return error;
}

enum geonkick_error
gkick_jack_create_output_ports(struct gkick_jack *jack)
{
        enum geonkick_error error;
        if (jack == NULL) {
                geonkick_log_error("wrong arguments");
                return GEONKICK_ERROR;
        }

        gkick_jack_lock(jack);
        if (jack->output_port_l == NULL && jack->output_port_r = NULL) {
                jack->output_port_l = jack_port_register(jack->client, "audio_out_L",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);
                jack->output_port_r = jack_port_register(jack->client, "audio_out_R",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);
                if (jack->output_port_l == NULL
                    || jack->output_port_r = NULL) {
                        geonkick_log_error("can't register output ports");
                        error = GEONKICK_ERROR;
                }
        } else {
                geonkick_log_warning("output ports already created");
        }
        gkick_jack_unlock(jack);

        return error;
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

        if (pthread_mutex_init(jack->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                gkick_jack_free(kick);
                return GEONKICK_ERROR_INIT_MUTEX;
        }

        kick->jack = jack;
        jack->client = jack_client_open(kick->name, JackNullOption, NULL);
        if (jack->client == NULL) {
                geonkick_log_error("can't create jack client");
                gkick_jack_free(kick);
                return GEONKICK_ERROR_OPEN_JACK;
        }

        jack_set_process_callback(jack->client,
                                  gkick_jack_process_callback,
                                  (void*)kick);
        jack_set_sample_rate_callback(jack->client,
                                      gkick_jack_srate_callback,
                                      (void*)kick);

        if (gkick_jack_create_output_ports(jack) != GEONKICK_OK) {
                geonkick_log_error("can't create output ports");
                gkick_jack_free(kick);
                return GEONKICK_ERROR;
        }

        if (geonkick_is_midi_in_enabled(kick)) {
                gkick_jack_enable_midi_in(jack);
        }

        if (jack_activate(jack->client)) {
                geonkick_log_error("cannot activate client");
                gkick_jack_free(kick);
                return GEONKICK_ERROR_ACTIVATE_JACK;
        }

	return GEONKICK_OK;
}

int gkick_jack_is_midi_in_enabled(struct gkick_jack *jack)
{
        int enabled;

        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        gkick_jack_lock(jack);
        enabled = (jack->midi_in_port != NULL) ? 1 : 0;
        gkick_jack_unlock(jack);

        return enabled;
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

void gkick_jack_lock(struct gkick_jack *jack)
{
        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }
        pthread_mutex_lock(&jack->lock);
}

void gkick_jack_unlock(struct gkick_jack *jack)
{
        if (jack == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }
        pthread_mutex_lock(&jack->lock);
}

