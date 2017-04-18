/*
  Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
  Copyright (C) 2004 Ian Esten

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "common.h"
#include "envelope.h"

jack_port_t *input_port;
jack_port_t *output_port_l;
jack_port_t *output_port_r;
jack_default_audio_sample_t ramp=0.0;
jack_default_audio_sample_t note_on;
double T = 0.0;
double phase = 0.0;
unsigned char note = 0;
jack_default_audio_sample_t note_frqs[128];

jack_client_t *client;

struct gkick_envelope *envelope_fr;

static void signal_handler(int sig)
{
	jack_client_close(client);
	fprintf(stderr, "signal received, exiting ...\n");
	exit(0);
}

static void calc_note_frqs(jack_default_audio_sample_t srate)
{
	int i;
	for(i = 0; i < 128; i++) {
		note_frqs[i] = (2.0 * 440.0 / 32.0) * pow(2, (((jack_default_audio_sample_t)i - 9.0) / 12.0)) / srate;
	}
}

static int process(jack_nframes_t nframes, void *arg)
{
	int i;
	struct gkick_envelope *envelope = (struct gkick_envelope*)arg;
	
        jack_default_audio_sample_t *out_l = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_l, nframes);
	jack_default_audio_sample_t *out_r = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_r, nframes);

	double f = 100;
	double samplerate = 44800;
	for (i = 0; i < nframes; i++) {
	      	double amp = gkick_envelope_get_value(envelope, 1e6 * T);
		f = 250 * gkick_envelope_get_value(envelope_fr, 1e6 * T);
		out_l[i] = 0.5 * amp * sin(phase) + 0.1 * amp * sin(2 * phase);
	        out_r[i] = 0.5 * amp * sin(phase) + 0.1 * amp * sin(2 * phase);
		phase += ((2 * M_PI * f) / samplerate);
		
		if (phase > M_PI) {
			phase -= 2 * M_PI;
		}
		//				       	printf("[%f] phase = %f, v = %f\n", T, phase, out_r[i]);		
		T += 1.0 / samplerate;

	        if (T > 0.5) {
	              T = 0.0;
	        }
	}

	return 0;
}

static int srate(jack_nframes_t nframes, void *arg)
{
	printf("the sample rate is now %" PRIu32 "/sec\n", nframes);
	calc_note_frqs((jack_default_audio_sample_t)nframes);
	return 0;
}

static void jack_shutdown(void *arg)
{
	fprintf(stderr, "JACK shut down, exiting ...\n");
	exit(1);
}

int main(int narg, char **args)
{
	if ((client = jack_client_open(GEON_APP_NAME, JackNullOption, NULL)) == 0) {
		fprintf(stderr, "JACK server not running?\n");
		return 1;
	}

	calc_note_frqs(jack_get_sample_rate (client));

	struct gkick_envelope *envelope = gkick_envelope_create();
	envelope_fr = gkick_envelope_create();

	gkick_envelope_add_point(envelope_fr, 0.0, 1.0);
	gkick_envelope_add_point(envelope_fr, 0.375, 36.0/250.0);
	
	gkick_envelope_add_point(envelope, 0.0, 0.0);
	gkick_envelope_add_point(envelope, 0.01, 1.0);
	gkick_envelope_add_point(envelope, 0.360, 1.0);
	gkick_envelope_add_point(envelope, 0.375, 0.0);


	//gkick_envelope_add_point(envelope, 0.02, 1.0);
	//gkick_envelope_add_point(envelope, 0.03, 0.7);
	//gkick_envelope_add_point(envelope, 0.05, 0.5);
	//gkick_envelope_add_point(envelope, 0.1, 0.0);	
	/*	gkick_envelope_add_point(envelope, 4, 0.0);
	gkick_envelope_add_point(envelope, 5, 0.0);
	gkick_envelope_add_point(envelope, 7.8, 1.0);
	gkick_envelope_add_point(envelope, 3.5, 0.5);	
	gkick_envelope_add_point(envelope, 1.1, 0.0);
	gkick_envelope_add_point(envelope, 6, 1.0);
	gkick_envelope_add_point(envelope, 3, 0.5);	
	gkick_envelope_add_point(envelope, 4, 0.0);
	gkick_envelope_add_point(envelope, 5, 0.0);
	gkick_envelope_add_point(envelope, 7, 0.5);	
	gkick_envelope_add_point(envelope, 8, 0.0);
	gkick_envelope_add_point(envelope, 0.1, 0.0);	*/
	struct gkick_envelope_point *p = envelope_fr->first;
	while(p) {
		printf("\nx = %f, y = %f\n", p->x / 1e6, p->y);
		p = p->next;
	}
	
	jack_set_process_callback(client, process, (void*)envelope);
	jack_set_sample_rate_callback(client, srate, 0);
	jack_on_shutdown(client, jack_shutdown, 0);

	output_port_l = jack_port_register(client, "audio_out_L", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port_r = jack_port_register(client, "audio_out_R", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (jack_activate(client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}

	/* install a signal handler to properly quits jack client */
#ifndef WIN32
	signal(SIGQUIT, signal_handler);
	signal(SIGHUP, signal_handler);
#endif
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);

	/* run until interrupted */
	while(1) {
#ifdef WIN32
		Sleep(1*1000);
#else
		sleep(1);
#endif
	}
	jack_client_close(client);
	gkick_envelope_destroy(envelope);
	exit (0);
}

