## [3.0.1]

### Fixes

- Fix for VST3 plugin that caused fails to load in many hosts
- Fix the VST3 plugin window size that on some hosts cause problems
- Fix oscillator frequency envelope for <=20 Hz

### Improvements

- Disable building the vst3sdk examples and gui (merge request from Christian Heusel @christian-heusel)
- Pipelines scripts cleanups related to	vst3sdk

## [3.0.0]

### Features

- Windows port of LV2 and VST3

### Fixes

- Added GEONKICK_OS_GNU preprocessor to enable memory lock for standalone on GNU/Linux

### Improvements

- Removed OpenSSL, using own code for Base64 encoding

### Other

- For Windows, export to audio file is turned off (available only through SFZ/WAV export)
- Added config files for pipeline builds

## [2.10.2]

### Fixes

- Added lock memory for standalone.
- Added external sub directory path to VST3 SDK for building.
- Error check on parsing JSON.
- Fixed info about vst plugin.

### Features

- Added Geonkick Bells presets.

### Other

- Cleanups, adding new info


## [2.10.1]

### Fixes

- Use OpenSSL for base64 encoding/decoding and eliminated third-party code related to base64.
- Generate PNG resources during the build process and removed the generated C files from the repository.
- Sanitized all SVG images by removing sensitive data.
- Relocated the 'redkite,' 'dsp,' 'plugin,' and 'standalone' folders to the 'src' directory.
- Eliminated the '3rdparty' folder and excluded 'vst3sdk' from the repository.
- Introduced a CMake option to specify the path to the VST3 SDK.
- Additional clean-up to remove sensitive data and unnecessary files.
- Added licensing (CC0) information for all images, documentation, and other non-code materials.

## [2.10.0]

### Fixes

- Fix linking VST3 against wrong Redkite library path.
- Fix conversion from frequency to note (more fixes).
- Fix shortcut for hide/show envelope.

### Features

- Set manually the exact value of envelope points.
- Display the current envelope point coordinates (value, time).

### Improvements

- Improve the precision for saving into presets.


## [2.9.2]

### Fixes

- Fix conversion from frequency to note in the frequency envelope graph.

### Other

- Moved redkite to 3rdparty folder.


## [2.9.1]

### Fixes

- Fix crash in Reaper for LV2 (if the event frame is given wrong by the host).


## [2.9.0]

### Features

  - Bring back VST3 plugin.
  - Support the session sample rate for VST3 plugin.
  - Integrate VST3 SDK into Geonkick build by providing the library into the 3rdparty dir.

### Fixes

  - export ModuleEntry/ModuleExit symbols for VST3 SDK
  - don't try to build standalone if there is no libjack dev library installed
  - fix a potential crash if the GUI graph thread was not created
  - for LV2 plugin tell the host that there is required ui:parent and ui:resize (manifest file changes)


## [2.8.1]

### Fixes

  - Remove redundant lock in the DSP worker.


## [2.8.0]

### Fixes

  - Fix crash when opening multiple GUI for LV2 Find rapidjson when.
  - Configuring at build add musl compatibility patch (Teldra / @teldra@likeable.space).

### Features

  - Add limiter to samples preview.

### Improvements

  - Integrate Redkite into Geonkick code.


## [2.7.3]

### Fixes

  - Fix selection of the last MIDI column in the MIDI key popup.

### Improvements

  - Move Redkite GUI toolkit into the Geonkick source and integrate
    it in the build system (no need to install Redkite now to build Geonkick).

## [2.7.2]

### Improvements

  - Show Geonkick version in the UI

### Fixes

  - Fix crash for standalone when jack server is not running.


## [2.7.1]

### Features

  - Add scale factor 1.5x for UI.


## [2.7.0]

### Features

  - Sample rate agnostic.


## [2.6.1]

### Fixes

  - Fix potential race condition.
  - Remove lib prefix for plugin shared library.


## [2.6.0]

### Features

  - Set MDI key popup for instruments.
  - Export instrument kit to SFZ.
  - 16 stereo channels for standalone.
  - Autoconnect ports to system playback for standalone.


## [2.5.1]

### Fixes

  - Fix build for some distros (include iomanip).


## [2.5.0]

### Features

  - "Single" version of LV2 plugin (single instrument and single stereo channel).


## [2.4.1]

### Improvement

  - Shared worker between plugin instances/


## [2.4.0]

### Features

  - Sample browser with sample preview.
  - Save/restore the last visited path, also save UI visual state.
  - Loaded sample is amplified.
  - Pitch shift (+- 48 semitones) envelope for oscillator when it uses a sample.
  - Added sound level indication for kit mixer.
  - UI updates, switch between envelopes is more easy, removed the envelopes menu.
  - Option to scale the UI by factor of 2 for so-called HiDPI cases.


## [2.3.8]

### Fixes:

   - Fixed the looped continuous sound that is generated on some hosts (LV2 and VST3).

## [2.3.7]

   - Added manpage for geonkick executable.


## [2.3.6]

### Fixes:

   - Fix updating kit UI when selecting a preset from host.
   - Fix some compile warnings in the vst plugin.

### Other

   - Remove old presets.


## [2.3.5]

### Fixes:

   - Fixes delays in the process loop and improve the process loop.

### Other

   - remove experimental file system support.

## [2.3.4]

### Fixes

   - Fixes name label for output channels in LV2, fixes "Fanout" for channels (by Robin Gareus).

## [2.3.3]

### Fixes:

   - Fix build on ARM architecture (by Andreas Müller)
   - Fix showing limiter sound level
   - Fix typos in documentation


## [2.3.2]

### Fixes:

   - Add cmake architecture variable for compiler optimisation flags.


## [2.3.1]

### Fixes:

   - Add lv2:requiredFeature urid:map.


### [2.3.0]

### Features

   - Reset to default button.
   - Add stereo output channels for LV2, VST and standalone.

### Improvements

   -  Added compiler optimization flags and fast math.
   -  Process audio routines for LV2, VST and standalone was made more efficient,
       now DSP should take less CPU.

### Fixes:

   - Fix loading other audio format subtypes for samples.
   - Reset to default shortcut.
   - FHS friendly LV2 and VST3 plugin installation paths.
   - Don't copy mute and solo settings when copy/paste percussion.
   - Reverse sawtooth.


### [2.2.3]

### Fixes

   - Fix compilers flags


### [2.2.2]

### Improvements

   - Follow the standard https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html#variables.
   - Search for presets in the $XDG_DATA_DIRS and $XDG_CONFIG_HOME.
   - Use GEONKICK_DATA_DIR when geonkick is installed in non a nonstandard path.

### [2.2.1]

### Fixes

   - Fix crash when .local/geonkick path can't be created.
   - Fix loading presets from .local/geonkick/presets.


### [2.2.0]

### Features

  - Kit mixer.
  - Preset browser.
  - Factory presets (installed under /usr/share/geonkick/presets or /usr/local/share/geonkick/presets).
  - Distortion volume envelope.

### Improvements

  - Range channels from 1 - 16 instead of 0 - 15.
  - Added visual effect for play button and some other buttons.

### Fixes

  - Fix loading WAVEFORMATEX samples
  - Other small fixes

### Changes from 2.1.0 to 2.1.1

### Fixes

  - Fix install of *.desktop file.
  - Fix finding Redkite `lib64` in `FindRedkite.cmake`.
  - Add loading WAVEX samples.


### [2.1.0]

### Fixes

  - Fix loading stereo samples.

### Improvements

  - Better quality of tuning.


### Changes from [2.0.2]

### Fixes

  - Fix saving kit state for standalone.


### [2.0.1]

### Fixes

  - Fixed the crash when opening a directory as kit preset.

### Other

  - Moved presets into doc/examples.


### [2.0.0]

### Features

  - VST3 plugin support for GNU/Linux.
  - Seed for noise generator to have different noise patterns.
  - Add copy/paste between percussions.
  - Add shortcut keys to navigate and change the kit list order.

### Improvements

  - Noise pattern does not change when synthesizer parameters are changed.
    Load/save preset will preserve the noise pattern too.
  - Keep the order of percussion when saving/opening state or preset.
  - Add new percussion only at the the end.
  - When starting to edit a percussion name move cursor by default to the end.
  - Reduce unnecessary updates of the UI when changing parameters.

### Fixes

   - The shortcuts and key 'k'.
   - A bug regarding "grabbing" window manager keys.
   - Set "any" key.
   - Updating the name in the toolbar.
   - Not updating the current percussion envelopes when adding a new default.
   - Reset to default percussion.
   - Limiter leveler for plugin.
   - Related to loading the preset from the host list of states (for LV2).
   - Deebug level when debugging.
   - ttl LV2 validation errors.
   - Fixed wrong linking to experimental version of filesystem for GCC version >= 8.


### [1.10.0]

### Freatures

   - Percussion kit - supports up the 16 percussions in the kit.
   - Multichannel support - supports up to 16 audio output channels.
   - Distortion drive envelope.
   - Distortion effect input limiter.
   - Play percussion button.
   - For build system check if Redkite is installed.
   - Enabled back the shortcut "k" key for triggering sound.
   - Documentation.
   - Backward compatibility, i.e. will load older presets.

### Improvements

   - Fmproved the checkbox for effects.
   - Resonance knob was reversed.

### Fixes

   - Fixed skipping notes for some hosts when notes are in a raw.
   - Fixed the crash for file dialog when going	to top file system '/' directory.
   - Fixed the label of FM checkbox.
