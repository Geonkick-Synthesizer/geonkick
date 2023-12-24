/**
 * File name: geonkick.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
 *
 * This file is part of GeonKick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GEONKICK_H
#define GEONKICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>
#include <stdbool.h>

#ifdef __STDC_NO_ATOMICS__
#error atomic operations are not supported
#endif

#include "gkick_log.h"

#ifdef GEONKICK_DOUBLE_PRECISION
typedef double gkick_real;
#else
typedef float gkick_real;
#endif

#define GEONKICK_UNUSED(expr) (void)expr

#define GEONKICK_VERSION GEONKICK_VERSION_HEX
#define GEONKICK_NAME "Geonkick"
#define GEONKICK_APP_NAME "geonkick"
#define GEOKICK_VERSION_STRING GEONKICK_VERSION_STR

#ifdef GEONKICK_AUDIO_JACK
#define GKICK_IS_STANDALONE (1)
#else
#define GKICK_IS_STANDALONE (0)
#endif

#define GEONKICK_DEFAULT_SAMPLE_RATE 48000

enum geonkick_error {
  	GEONKICK_OK                    = 0,
	GEONKICK_ERROR                 = 1,
	GEONKICK_ERROR_MEM_ALLOC       = 2,
	GEONKICK_ERROR_WRONG_ARGUMENTS = 3
};

enum gkick_key_state {
        GKICK_KEY_STATE_DEFAULT  = 0,
        GKICK_KEY_STATE_PRESSED  = 1,
        GKICK_KEY_STATE_RELEASED = 2
};

enum geonkick_envelope_type {
        GEONKICK_AMPLITUDE_ENVELOPE         = 0,
        GEONKICK_FREQUENCY_ENVELOPE         = 1,
        GEONKICK_FILTER_CUTOFF_ENVELOPE     = 2,
	GEONKICK_DISTORTION_DRIVE_ENVELOPE  = 3,
        GEONKICK_DISTORTION_VOLUME_ENVELOPE = 4,
        GEONKICK_PITCH_SHIFT_ENVELOPE       = 5,
	GEONKICK_FILTER_Q_ENVELOPE          = 6,
};

enum gkick_filter_type {
        GEONKICK_FILTER_LOW_PASS  = 0,
        GEONKICK_FILTER_HIGH_PASS = 1,
        GEONKICK_FILTER_BAND_PASS = 2
};

enum geonkick_osc_func_type {
	GEONKICK_OSC_FUNC_SINE           = 0,
	GEONKICK_OSC_FUNC_SQUARE         = 1,
	GEONKICK_OSC_FUNC_TRIANGLE       = 2,
	GEONKICK_OSC_FUNC_SAWTOOTH       = 3,
        GEONKICK_OSC_FUNC_NOISE_WHITE    = 4,
        GEONKICK_OSC_FUNC_NOISE_PINK     = 5,
        GEONKICK_OSC_FUNC_NOISE_BROWNIAN = 6,
        GEONKICK_OSC_FUNC_SAMPLE         = 7
};

enum GEONKICK_MODULE {
        GEONKICK_MODULE_UNKNOWN = 0,
        GEONKICK_MODULE_JACK    = 1
};

#define GKICK_OSC_GROUPS_NUMBER 3
#define GKICK_OSC_GROUP_SIZE 3

enum geonkick_channel_type {
        GEONKICK_CHANNEL_AUDIO_INPUT  = 0,
        GEONKICK_CHANNEL_AUDIO_OUTPUT = 1,
        GEONKICK_CHANNEL_MIDI_INPUT   = 2,
        GEONKICK_CHANNEL_MIDI_OUTPUT  = 3
};

enum gkick_envelope_apply_type {
	GEONKICK_ENVELOPE_APPLY_LINEAR       = 0,
	GEONKICK_ENVELOPE_APPLY_LOGARITHMIC  = 1
};

#ifdef GEONKICK_SINGLE
#define GEONKICK_MAX_INSTRUMENTS 1
#else
#define GEONKICK_MAX_INSTRUMENTS 16
#endif

/**
* Maximum audio number of output stereo channels.
*/
#if defined(GEONKICK_SINGLE)
#define GEONKICK_MAX_CHANNELS 1
#else
#define GEONKICK_MAX_CHANNELS 16
#endif // if defined(GEONKICK_SINGLE)

#define GEONKICK_ANY_KEY -1

struct geonkick;

enum geonkick_error
geonkick_create(struct geonkick **kick, int sample_rate);

void
geonkick_free(struct geonkick **kick);

enum geonkick_error
geonkick_add_oscillator(struct geonkick* kick,
                        size_t *index);

enum geonkick_error
geonkick_remove_oscillator(struct geonkick* kick,
                           size_t index);

enum geonkick_error
geonkick_enable_oscillator(struct geonkick* kick,
                           size_t index);

enum geonkick_error
geonkick_disable_oscillator(struct geonkick* kick,
                            size_t index);

enum geonkick_error
geonkick_is_oscillator_enabled(struct geonkick* kick,
                               size_t index,
                               int *enabled);

enum geonkick_error
geonkick_get_oscillators_number(struct geonkick* kick,
                                size_t *number);

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 gkick_real **buf,
				 size_t *npoints);

enum geonkick_error
geonkick_osc_envelope_set_points(struct geonkick *kick,
                                 size_t osc_index,
                                 size_t env_index,
                                 const gkick_real *buff,
                                 size_t npoints);

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				gkick_real x,
				gkick_real y);

enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index);
enum geonkick_error
geonkick_osc_envelope_update_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
				   gkick_real x,
				   gkick_real y);

enum geonkick_error
geonkick_osc_envelope_set_apply_type(struct geonkick *kick, 
				     size_t osc_index,
				     size_t env_index,
				     enum gkick_envelope_apply_type apply_type);

enum geonkick_error
geonkick_osc_envelope_get_apply_type(struct geonkick *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum gkick_envelope_apply_type *apply_type);

enum geonkick_error
geonkick_osc_set_fm(struct geonkick *kick,
                    size_t index,
                    bool is_fm);

enum geonkick_error
geonkick_osc_is_fm(struct geonkick *kick,
                   size_t index,
                   bool *is_fm);

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type type);

enum geonkick_error
geonkick_get_osc_function(struct geonkick *kick,
			  size_t osc_index,
			  enum geonkick_osc_func_type *type);

enum geonkick_error
geonkick_set_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real phase);

enum geonkick_error
geonkick_get_osc_phase(struct geonkick *kick,
                       size_t osc_index,
                       gkick_real *phase);

enum geonkick_error
geonkick_set_osc_seed(struct geonkick *kick,
                      size_t osc_index,
                      unsigned int seed);

enum geonkick_error
geonkick_get_osc_seed(struct geonkick *kick,
                      size_t osc_index,
                      unsigned int *seed);

enum geonkick_error
geonkick_set_length(struct geonkick *kick,
                    gkick_real len);

enum geonkick_error
geonkick_get_length(struct geonkick *kick,
                    gkick_real *len);

enum geonkick_error
geonkick_get_max_length(struct geonkick *kick,
                        gkick_real *len);

enum geonkick_error
geonkick_kick_set_amplitude(struct geonkick *kick,
                            gkick_real amplitude);

enum geonkick_error
geonkick_kick_get_amplitude(struct geonkick *kick,
                            gkick_real *amplitude);

enum geonkick_error
geonkick_kick_filter_enable(struct geonkick *kick,
                            int enable);

enum geonkick_error
geonkick_kick_filter_is_enabled(struct geonkick *kick,
                                int *enabled);

enum geonkick_error
geonkick_kick_set_filter_frequency(struct geonkick *kick,
                                   gkick_real frequency);

enum geonkick_error
geonkick_kick_get_filter_frequency(struct geonkick *kick,
                                   gkick_real *frequency);

enum geonkick_error
geonkick_kick_set_filter_factor(struct geonkick *kick,
                                gkick_real factor);

enum geonkick_error
geonkick_kick_get_filter_factor(struct geonkick *kick,
                                gkick_real *factor);

enum geonkick_error
geonkick_set_kick_filter_type(struct geonkick *kick,
                              enum gkick_filter_type type);

enum geonkick_error
geonkick_get_kick_filter_type(struct geonkick *kick,
                              enum gkick_filter_type *type);

enum geonkick_error
geonkick_kick_envelope_get_points(struct geonkick *kick,
                                  enum geonkick_envelope_type env_type,
                                  gkick_real **buf,
                                  size_t *npoints);

enum geonkick_error
geonkick_kick_envelope_set_points(struct geonkick *kick,
                                  enum geonkick_envelope_type env_type,
                                  const gkick_real *buff,
                                  size_t npoints);

enum geonkick_error
geonkick_kick_add_env_point(struct geonkick *kick,
                            enum geonkick_envelope_type env_type,
                            gkick_real x, gkick_real y);

enum geonkick_error
geonkick_kick_remove_env_point(struct geonkick *kick,
                               enum geonkick_envelope_type env_type,
                               size_t index);

enum geonkick_error
geonkick_kick_update_env_point(struct geonkick *kick,
                               enum geonkick_envelope_type env_type,
                               size_t index,
                               gkick_real x,
                               gkick_real y);

enum geonkick_error
geonkick_kick_env_set_apply_type(struct geonkick *kick, 
				 enum geonkick_envelope_type env_type,
				 enum gkick_envelope_apply_type apply_type);

enum geonkick_error
geonkick_kick_env_get_apply_type(struct geonkick *kick,
				 enum geonkick_envelope_type env_type,
				 enum gkick_envelope_apply_type *apply_type);

enum geonkick_error
geonkick_set_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v);
enum geonkick_error
geonkick_set_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real v);

enum geonkick_error
geonkick_set_osc_pitch_shift(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real semitones);

enum geonkick_error
geonkick_get_osc_amplitude(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v);
enum geonkick_error
geonkick_get_osc_frequency(struct geonkick *kick,
                           size_t osc_index,
                           gkick_real *v);

enum geonkick_error
geonkick_get_osc_pitch_shift(struct geonkick *kick,
                             size_t osc_index,
                             gkick_real *semitones);

enum geonkick_error
geonkick_play(struct geonkick *kick, size_t id);

/**
 * This function must be called
 * only from the audio thread.
 */
enum geonkick_error
geonkick_key_pressed(struct geonkick *kick,
                     bool pressed,
                     int note,
                     int velocity);

enum geonkick_error
geonkick_get_kick_buffer_size(struct geonkick *kick,
                              size_t *size);

enum geonkick_error
geonkick_get_kick_buffer(struct geonkick *kick,
                         gkick_real *buffer,
                         size_t size);

enum geonkick_error
geonkick_set_kick_buffer_callback(struct geonkick *kick,
                                  void (*callback)(void*,
                                                   gkick_real *buff,
                                                   size_t size,
                                                   size_t id),
                                  void *arg);

enum geonkick_error
geonkick_set_kick_limiter_callback(struct geonkick *kick,
                                   void (*callback)(void*,
                                                    size_t index,
                                                    gkick_real val),
                                   void *arg);

enum geonkick_error
geonkick_set_limiter_value(struct geonkick *kick,
                           gkick_real limit);

enum geonkick_error
geonkick_get_limiter_value(struct geonkick *kick,
                           gkick_real *limit);

enum geonkick_error
geonkick_set_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type type);
enum geonkick_error
geonkick_get_osc_filter_type(struct geonkick *kick,
                             size_t osc_index,
                             enum gkick_filter_type *type);

enum geonkick_error
geonkick_set_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real cutoff);
enum geonkick_error
geonkick_get_osc_filter_cutoff_freq(struct geonkick *kick,
                                    size_t osc_index,
                                    gkick_real *cutoff);

enum geonkick_error
geonkick_set_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real factor);

enum geonkick_error
geonkick_get_osc_filter_factor(struct geonkick *kick,
                               size_t osc_index,
                               gkick_real *factor);

enum geonkick_error
geonkick_enbale_osc_filter(struct geonkick *kick,
                           size_t osc_index,
                           int enable);

enum geonkick_error
geonkick_osc_filter_is_enabled(struct geonkick *kick,
                               size_t osc_index,
                               int *enable);

enum geonkick_error
geonkick_get_sample_rate(struct geonkick *kick,
                         int *sample_rate);

enum geonkick_error
geonkick_enable_synthesis(struct geonkick *kick,
                          bool enable);

enum geonkick_error
geonkick_audio_process(struct geonkick *kick,
                       float **out,
                       size_t channel,
                       size_t size);

enum geonkick_error
geonkick_compressor_enable(struct geonkick *kick,
                           int enable);

enum geonkick_error
geonkick_compressor_is_enabled(struct geonkick *kick,
                               int *enabled);

enum geonkick_error
geonkick_compressor_set_attack(struct geonkick *kick,
                               gkick_real attack);

enum geonkick_error
geonkick_compressor_get_attack(struct geonkick *kick,
                               gkick_real *attack);

enum geonkick_error
geonkick_compressor_set_release(struct geonkick *kick,
                                gkick_real release);

enum geonkick_error
geonkick_compressor_get_release(struct geonkick *kick,
                                gkick_real *release);

enum geonkick_error
geonkick_compressor_set_threshold(struct geonkick *kick,
                                  gkick_real threshold);

enum geonkick_error
geonkick_compressor_get_threshold(struct geonkick *kick,
                                  gkick_real *threshold);

enum geonkick_error
geonkick_compressor_set_ratio(struct geonkick *kick,
                              gkick_real ratio);

enum geonkick_error
geonkick_compressor_get_ratio(struct geonkick *kick,
                              gkick_real *ratio);

enum geonkick_error
geonkick_compressor_set_knee(struct geonkick *kick,
                             gkick_real knee);

enum geonkick_error
geonkick_compressor_get_knee(struct geonkick *kick,
                             gkick_real *knee);

enum geonkick_error
geonkick_compressor_set_makeup(struct geonkick *kick,
                               gkick_real makeup);

enum geonkick_error
geonkick_compressor_get_makeup(struct geonkick *kick,
                               gkick_real *makeup);

enum geonkick_error
geonkick_distortion_enable(struct geonkick *kick,
                           int enable);

enum geonkick_error
geonkick_distortion_is_enabled(struct geonkick *kick,
                               int *enabled);

enum geonkick_error
geonkick_distortion_set_in_limiter(struct geonkick *kick,
                                   gkick_real limit);

enum geonkick_error
geonkick_distortion_get_in_limiter(struct geonkick *kick,
                                   gkick_real *limit);

enum geonkick_error
geonkick_distortion_set_volume(struct geonkick *kick,
                               gkick_real volume);

enum geonkick_error
geonkick_distortion_get_volume(struct geonkick *kick,
                               gkick_real *volume);

enum geonkick_error
geonkick_distortion_set_drive(struct geonkick *kick,
                              gkick_real drive);

enum geonkick_error
geonkick_distortion_get_drive(struct geonkick *kick,
                              gkick_real *drive);

int geonkick_is_module_enabed(struct geonkick *kick,
                              enum GEONKICK_MODULE module);

enum geonkick_error
geonkick_enable_group(struct geonkick *kick,
                      size_t index,
                      bool enable);

enum geonkick_error
geonkick_group_enabled(struct geonkick *kick,
                       size_t index,
                       bool *enabled);

enum geonkick_error
geonkick_group_set_amplitude(struct geonkick *kick,
                             size_t index,
                             gkick_real amplitude);

enum geonkick_error
geonkick_group_get_amplitude(struct geonkick *kick,
                             size_t index,
                             gkick_real *amplitude);

enum geonkick_error
geonkick_tune_audio_output(struct geonkick *kick,
                           size_t index,
                           bool tune);

enum geonkick_error
geonkick_is_audio_output_tuned(struct geonkick *kick,
                               size_t index,
                               bool *tune);

enum geonkick_error
geonkick_set_osc_sample(struct geonkick *kick,
                        size_t osc_index,
                        const gkick_real *data,
                        size_t size);

enum geonkick_error
geonkick_get_osc_sample(struct geonkick *kick,
                        size_t osc_index,
                        gkick_real **data,
                        size_t *size);

enum geonkick_error
geonkick_set_current_percussion(struct geonkick *kick,
                                size_t index);

enum geonkick_error
geonkick_get_current_percussion(struct geonkick *kick,
                                size_t *index);

enum geonkick_error
geonkick_unused_percussion(struct geonkick *kick,
                           int *index);

enum geonkick_error
geonkick_enable_percussion(struct geonkick *kick,
                           size_t index,
                           bool enable);

enum geonkick_error
geonkick_is_percussion_enabled(struct geonkick *kick,
                               size_t index,
                               bool *enable);

size_t
geonkick_percussion_number();

enum geonkick_error
geonkick_set_playing_key(struct geonkick *kick,
                         size_t id,
                         signed char key);

enum geonkick_error
geonkick_get_playing_key(struct geonkick *kick,
                         size_t id,
                         signed char *key);

enum geonkick_error
geonkick_set_percussion_name(struct geonkick *kick,
                             size_t id,
                             const char *name,
                             size_t size);

enum geonkick_error
geonkick_get_percussion_name(struct geonkick *kick,
                             size_t id,
                             char *name,
                             size_t size);

size_t
geonkick_channels_number();

enum geonkick_error
geonkick_set_percussion_channel(struct geonkick *kick,
                                size_t id,
                                size_t channel);

enum geonkick_error
geonkick_get_percussion_channel(struct geonkick *kick,
                                size_t id,
                                size_t *channel);

enum geonkick_error
geonkick_percussion_set_limiter(struct geonkick *kick,
                                size_t id,
                                gkick_real val);

enum geonkick_error
geonkick_percussion_get_limiter(struct geonkick *kick,
                                size_t id,
                                gkick_real *val);

enum geonkick_error
geonkick_percussion_mute(struct geonkick *kick,
                         size_t id,
                         bool b);

enum geonkick_error
geonkick_percussion_is_muted(struct geonkick *kick,
                             size_t id,
                             bool *b);

enum geonkick_error
geonkick_percussion_solo(struct geonkick *kick,
                         size_t id,
                         bool b);

enum geonkick_error
geonkick_percussion_is_solo(struct geonkick *kick,
                            size_t id,
                            bool *b);


enum geonkick_error
geonkick_set_preview_sample(struct geonkick *kick,
                            const gkick_real *data,
                            size_t size);

void
geonkick_play_sample_preview(struct geonkick *kick);

enum geonkick_error
geonkick_set_sample_preview_limiter(struct geonkick *kick, gkick_real val);

enum geonkick_error
geonkick_get_sample_preview_limiter(struct geonkick *kick, gkick_real *val);

bool
geonkick_is_plugin();

#ifdef __cplusplus
}
#endif

#endif
