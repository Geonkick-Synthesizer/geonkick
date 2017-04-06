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

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#define GEON_APP_NAME "geonckick"

jack_port_t *input_port;
jack_port_t *output_port_l;
jack_port_t *output_port_r;
jack_default_audio_sample_t ramp=0.0;
jack_default_audio_sample_t note_on;
unsigned char note = 0;
jack_default_audio_sample_t note_frqs[128];

jack_client_t *client;

/*struct gkick_point {
	int x;
	int y;
};

struct gkick_envelope {
	size_t count;
	gkick_point *point;
	gkick_point *next;
};

static void gkick_envelope_create(gkick_envelope** envelope)
{
	if (envelope == NULL) {
		return;
	}

	*envelope = (struct gkick_envelope*)malloc(sizeof(struct gkick_envelope));
	if (*envelope == NULL) {
		return;
	}

	memset(*envelope, 0, sizeof(struct gkick_envelope));
}

static void gkick_envelope_add_point(gkick_envelope *envelope, int x, int y)
{
	
}
*/
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
	
	//void* port_buf = jack_port_get_buffer(input_port, nframes);
        jack_default_audio_sample_t *out_l = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_l, nframes);
	jack_default_audio_sample_t *out_r = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_r, nframes);

	printf("nframes = %d \n", nframes);
	//	out_l[100] = 90;
	//volatile int s = 1;//nframes / 2;
	int f = 100;
	for (i = 0; i < nframes; i++) {
	       out_l[i] = 0.5 * sin(10 * 2 * M_PI * f * ramp);
	       out_r[i] = 0.5 * sin(10 * 2 * M_PI * f * ramp);
	       ramp += 1.0 / 48000.0;
	       if (ramp > 48000.0) {
	       	        ramp = 0.0;
	       }
	}

	//        out_l[nframes - 500] = 0; 
	
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

	//	calc_note_frqs(jack_get_sample_rate (client));
	jack_set_process_callback(client, process, 0);
	jack_set_sample_rate_callback(client, srate, 0);
	jack_on_shutdown(client, jack_shutdown, 0);

	//input_port = jack_port_register(client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
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
	exit (0);
}

