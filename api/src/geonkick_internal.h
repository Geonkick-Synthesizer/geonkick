#ifndef GEON_KICK_INTERNAL_H
#define GEON_KICK_INTERNAL_H

#include "geonkick.h"
#include "gkick_jack.h"

#include <pthread.h>

struct geonkick {
        pthread_mutex_t lock;
        char name[30];
        double length;
        double current_time;
        struct gkick_oscillator **oscillators;
        size_t oscillators_number;
        struct gkick_jack *jack;
        int midi_in_enabled;
        int is_play;
        gkick_envelope *amp_envelope;
        gkick_filter *filer;
};

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick);

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate);

double
geonkick_get_oscillators_value(struct geonkick *kick);

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index);

void
geonkick_lock(struct geonkick *kick);

void
geonkick_unlock(struct geonkick *kick);

void
gkick_jack_increment_time(struct gkick_jack *jack);

void
geonkick_reset_oscillators(struct geonkick *kick);

enum geonkick_error
geonkick_play(struct geonkick *kick, int play);

int
geonckick_is_play_stopped(struct geonkick *kick);

enum geonkick_error
geonkick_start_play(struct geonkick *kick);

enum geonkick_error
geonkick_stop_play(struct geonkick *kick);

enum geonkick_error
geonkick_increment_time(struct geonkick *kick, double dt);

double
geonkick_current_time(struct geonkick *kick);

double
geonkick_length(struct geonkick *kick);

void
geonkick_set_current_time(struct geonkick *kick,
			  double current_time);

#endif
