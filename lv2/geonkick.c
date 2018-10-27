/**
 * File name: gluidgeon.c
 * Project: Geonkick
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of Geonkick.
 *
 * Geonkick is free software; you can redistribute it and/or modify
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

#include "geonkick.h"

/**
   In code, ports are referred to by index.  An enumeration of port indices
   should be defined for readability.
*/
typedef enum {
	AMP_GAIN   = 0,
	AMP_INPUT  = 1,
	AMP_OUTPUT = 2
} PortIndex;

/**
   Every plugin defines a private structure for the plugin instance.  All data
   associated with a plugin instance is stored here, and is available to
   every instance method.  In this simple plugin, only port buffers need to be
   stored, since there is no additional instance data.
*/
typedef struct {
	// Port buffers
	const float* gain;
	const float* input;
	float*       output;
} Amp;

/**
   The `instantiate()` function is called by the host to create a new plugin
   instance.  The host passes the plugin descriptor, sample rate, and bundle
   path for plugins that need to load additional resources (e.g. waveforms).
   The features parameter contains host-provided features defined in LV2
   extensions, but this simple plugin does not use any.

   This function is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
        UNUSED(descriptor);
	UNUSED(rate);
	UNUSED(bundle_path);
	UNUSED(features);
	
        fgeon_log_trace("start");
	Amp* amp = (Amp*)calloc(1, sizeof(Amp));
	
	if (amp == NULL) {
	  fgeon_log_critical("can't allocate memory for plugin");
	  return NULL;
	}

	return (LV2_Handle)amp;
}

/**
   The `connect_port()` method is called by the host to connect a particular
   port to a buffer.  The plugin must store the data location, but data may not
   be accessed except in run().

   This method is in the ``audio'' threading class, and is called in the same
   context as run().
*/
static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  const LV2_Atom_Sequence* control;
    //y  fgeon_log_trace("port: %u", port);
	if (instance == NULL) {
	  fgeon_log_error("Can't connect to port %d. Invalid argument (NULL pointer)", port);
	  return;
	}

  control = (const LV2_Atom_Sequence*)data;    
  if (port == 0 && control != NULL) {
    LV2_ATOM_SEQUENCE_FOREACH(control, ev) {
      //if (ev->body.type == self->uris.midi_MidiEvent) {
	const uint8_t* const msg = (const uint8_t*)(ev + 1);
	switch (lv2_midi_message_type(msg)) {
	case LV2_MIDI_MSG_NOTE_ON:
	  fgeon_log_info("LV2_MIDI_MSG_NOTE_ON: %d, %d", msg[1], msg[2]);
	  break;
	case LV2_MIDI_MSG_NOTE_OFF:
	  fgeon_log_info("LV2_MIDI_MSG_NOTE_OFF: %d, %d", msg[1], msg[2]);
	  break;
	case LV2_MIDI_MSG_PGM_CHANGE:
	  fgeon_log_info("LV2_MIDI_MSG_NOTE_OFF");
	  break;
	} // switch
	//} // if
    } // LV2_ATOM...
  } // if
  	
	/*Amp* amp = (Amp*)instance;

	switch ((PortIndex)port) {
	case AMP_GAIN:
		amp->gain = (const float*)data;
		break;
	case AMP_INPUT:
		amp->input = (const float*)data;
		break;
	case AMP_OUTPUT:
		amp->output = (float*)data;
		break;
		}*/
}

/**
   The `activate()` method is called by the host to initialise and prepare the
   plugin instance for running.  The plugin must reset all internal state
   except for buffer locations set by `connect_port()`.  Since this plugin has
   no other internal state, this method does nothing.

   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void
activate(LV2_Handle instance)
{
}

/**
   The `run()` method is the main process function of the plugin.  It processes
   a block of audio in the audio context.  Since this plugin is
   `lv2:hardRTCapable`, `run()` must be real-time safe, so blocking (e.g. with
   a mutex) or memory allocation are not allowed.
*/
static void
run(LV2_Handle instance, uint32_t n_samples)
{
  	if (instance == NULL) {
	  fgeon_log_error("Invalid argument (NULL pointer)");
	  return;
	}

	//fgeon_log_debug("n_smaples: %u", n_samples);
	
	/*	const Amp* amp = (const Amp*)instance;

	const float        gain   = *(amp->gain);
	//fgeon_log_debug("n_smaples1: %u", n_samples);
	const float* const input  = amp->input;
	//fgeon_log_debug("n_smaples2: %u", n_samples);
	float* const       output = amp->output;
	//fgeon_log_debug("n_smaples3: %u", n_samples);

	const float coef = DB_CO(gain);

	for (uint32_t pos = 0; pos < n_samples; pos++) {
		output[pos] = input[pos] * coef;
		}*/
}

/**
   The `deactivate()` method is the counterpart to `activate()`, and is called by
   the host after running the plugin.  It indicates that the host will not call
   `run()` again until another call to `activate()` and is mainly useful for more
   advanced plugins with ``live'' characteristics such as those with auxiliary
   processing threads.  As with `activate()`, this plugin has no use for this
   information so this method does nothing.

   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void
deactivate(LV2_Handle instance)
{
}

/**
   Destroy a plugin instance (counterpart to `instantiate()`).

   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void
cleanup(LV2_Handle instance)
{
	free(instance);
}

/**
   The `extension_data()` function returns any extension data supported by the
   plugin.  Note that this is not an instance method, but a function on the
   plugin descriptor.  It is usually used by plugins to implement additional
   interfaces.  This plugin does not have any extension data, so this function
   returns NULL.

   This method is in the ``discovery'' threading class, so no other functions
   or methods in this plugin library will be called concurrently with it.
*/
static const void*
extension_data(const char* uri)
{
	return NULL;
}

/**
   Every plugin must define an `LV2_Descriptor`.  It is best to define
   descriptors statically to avoid leaking memory and non-portable shared
   library constructors and destructors to clean up properly.
*/
static const LV2_Descriptor descriptor = {
	AMP_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

/**
   The `lv2_descriptor()` function is the entry point to the plugin library.  The
   host will load the library and call this function repeatedly with increasing
   indices to find all the plugins defined in the library.  The index is not an
   indentifier, the URI of the returned descriptor is used to determine the
   identify of the plugin.

   This method is in the ``discovery'' threading class, so no other functions
   or methods in this plugin library will be called concurrently with it.
*/
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	case 0:  return &descriptor;
	default: return NULL;
	}
}


