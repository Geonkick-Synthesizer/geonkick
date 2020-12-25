### Version 2.6.1

#### Fixes

  - Fix potential race condition
  - Remove lib prefix for plugin shared library

### Version 2.6.0

#### Features

  - Set MDI key popup for instruments
  - Export instrument kit to SFZ
  - 16 stereo channels for standalone
  - Autoconnect ports to system playback for standalone

### Version 2.5.1

#### Fixes

  - Fix build for some distros (include iomanip)

### Version 2.5.0

#### Features

  - "Single" version of LV2 plugin (single instrument and single stereo channel)


### Version 2.4.1

#### Improvement

  - shared worker between plugin instances

### Version 2.4.0

#### Features:

  - sample browser with sample preview
  - save/restore the last visited path, also save UI visual state
  - loaded sample is amplified
  - pitch shift (+- 48 semitones) envelope for oscillator when it uses a sample
  - added sound level indication for kit mixer
  - UI updates, switch between envelopes is more easy, removed the envelopes menu
  - option to scale the UI by factor of 2 for so-called HiDPI cases

Updating to this version must be backward compatible.

### Version 2.3.8

#### Fixes:

   - fixed the looped continuous sound that is generated on some hosts (LV2 and VST3)

### Version 2.3.7

   - added manpage for geonkick executable

### Version 2.3.6

#### Fixes:

   - fix updating kit UI when selecting a preset from host
   - fix some compile warnings in the vst plugin

#### Other

   - remove old presets

### Version 2.3.5

#### Fixes:

   - fixes delays in the process loop and improve the process loop

#### Other

   - remove experimental file system support

### Version 2.3.4

#### Fixes:

   - fixes name label for output channels in LV2, fixes "Fanout" for channels (by Robin Gareus)

### Version 2.3.3

#### Fixes:

   - fix build on ARM architecture (by Andreas Müller)
   - fix showing limiter sound level
   - fix typos in documentation

### Version 2.3.2

#### Fixes:

   - add cmake architecture variable for compiler optimisation flags

### Version 2.3.1

#### Fixes:

   - add lv2:requiredFeature urid:map

### Changes from 2.2.3 to 2.3.0

#### Features:

   - reset to default button
   - add stereo output channels for LV2, VST and standalone

#### Improvements:

   -  added compiler optimization flags and fast math
   -  process audio routines for LV2, VST and standalone was made more efficient,
       now DSP should take less CPU

#### Fixes:

   - fix loading other audio format subtypes for samples
   - reset to default shortcut
   - FHS friendly LV2 and VST3 plugin installation paths
   - don't copy mute and solo settings when copy/paste percussion
   - reverse sawtooth

### Changes from 2.2.2 to 2.2.3

#### Fixes

   - fix compliers flags

### Changes from 2.2.1 to 2.2.2

#### Improvements

   - follow the standard https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html#variables
   - search for presets in the $XDG_DATA_DIRS and $XDG_CONFIG_HOME
   - use GEONKICK_DATA_DIR when geonkick is installed in non a nonstandard path

### Changes from 2.2.0 to 2.2.1

#### Fixes

   - Fix crash when .local/geonkick path can't be created
   - Fix loading presets from .local/geonkick/presets

### Changes from 2.1.1 to 2.2.0

#### New features

  - Kit mixer
  - Preset browser
  - Factory presets (installed under /usr/share/geonkick/presets or /usr/local/share/geonkick/presets)
  - Distortion volume envelope

#### Improvements

  - Range channels from 1 - 16 instead of 0 - 15
  - Added visual effect for play button and some other buttons

#### Fixes

  - Fix loading WAVEFORMATEX samples
  - Other small fixes

### Changes from 2.1.0 to 2.1.1

#### Fixes

  - fix install of *.desktop file
  - fix finding Redkite `lib64` in `FindRedkite.cmake`
  - add loading WAVEX samples

### Changes from 2.0.2 to 2.1.0

#### Fixes

  - fix loading stereo samples

#### Improvements

  - better quality of tuning

### Changes from 2.0.1 to 2.0.2

#### Fixes

  - fix saving kit state for standalone


### Changes from 2.0.0 to 2.0.1

#### Fixes

  - fixed the crash when opening a directory as kit preset

#### Other

  - moved presets into doc/examples

### Changes from 1.10.0 to 2.0.0

#### Features:

  - VST3 plugin support for GNU/Linux
  - seed for noise generator to have different noise patterns
  - add copy/paste between percussions
  - add shortcut keys to navigate and change the kit list order

#### Improvements:

  - noise pattern does not change when synthesizer parameters are changed.
    Load/save preset will preserve the noise pattern too
  - keep the order of percussion when saving/opening state or preset
  - add new percussion only at the the end
  - when starting to edit a percussion name move cursor by default to the end
  - reduce unnecessary updates of the UI when changing parameters

#### Fixes:

   - the shortcuts and key 'k'
   - a bug regarding "grabbing" window manager keys
   - set "any" key
   - updating the name in the toolbar
   - not updating the current percussion envelopes when adding a new default
   - reset to default percussion
   - limiter leveler for plugin
   - related to loading the preset from the host list of states (for LV2)
   - debug level when debugging
   - ttl LV2 validation errors
   - fixed wrong linking to experimental version of filesystem for GCC version >= 8

### Changes from 1.9.2 to 1.10.0

#### Freatures

   - percussion kit - supports up the 16 percussions in the kit
   - multichannel support - supports up to 16 audio output channels
   - distortion drive envelope
   - distortion effect input limiter
   - play percussion button
   - for build system check if Redkite is installed
   - enabled back the shortcut "k" key for triggering sound
   - documentation
   - backward compatibility, i.e. will load older presets

#### Improvements

   - improved the checkbox for effects
   - resonance knob was reversed

#### Fixes

   - fixed skipping notes for some hosts when notes are in a raw
   - fixed the crash for file dialog when going	to top file system '/' directory
   - fixed the label of FM checkbox
