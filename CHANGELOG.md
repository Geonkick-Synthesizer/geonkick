## [3.6.1]

### Fixes

- fix crash for single version of Geonkick when selecting some presets
- fix crash deleting and after selecting a kit instrument
- fix setting instrument channel after deleting a kit instrument

## [3.6.0]

### Features

- Sidebar browser for presets and samples
- Oscillator distortion effect
- Add 6 distortion types for distortion effect: hard clip, tan, atan, exp, log, polyomial
- Bézier curve for envelopes
- Add UI scale setting in "Settings" tab
- Add midi key numbers in midi key popup
- Edit instrument name in the TopBar
- Add instrument midi channel and note off constrols to TopBar

### Improvements

- Increase FM modulation coefficient
- Add gain to oscillator and instrument amplitude control
- Use a single preset format (now all presets are *.gkit)
- Setting as popup instead of tab widget
- Some improvements to UI (icons etc.)

### Fixes

- Fix "force midi channel" checkbox for settings


## [3.5.2]

### Fixes

- Fix crash when deleting an instrument from the kit
- Fix saving the plugin state in Cubase (reported by o-bardiuk )
- Donot export symbols from DSP and Redkite resources (reported by David Runge)
- Fix the artifact at the end of sound when tuning to a MIDI key.

### Other

- Remove a C++ files that were mistakenly added to the DSP folder

## [3.5.1]

### Fixes

- Fix showing the current envelope path
- Clear the midi channel spinbox when updating the GUI
- Add MIDI channel spinbox and note off button for a single version instrument in the top bar

## [3.5.0]

### Features

- Zoom along the time axis
- New oscillator UI: 3 oscillators with wave functions: sine, square, triangle, sawtooth, noise (white & brownian), and sample
- Preset navigator
- Noise density control
- Instruments tabs in the main UI
- Option to not ignore the NOTE OFF
- Add restore control to default value on double click
- Create new folder with file browser
- Add scroll to spinboxes

### Fixes

- Fixed shortcuts
- Fixed the name of the preset when saved and loaded
- Fixed some memory leaks in DSP
- Fixed some potential crashes on closing the UI

### Improvements

- Restore export button for Windows
- Moved to GUI graphics widgets (updated Redkite GUI toolkit)
- Improved the drawing of percussive sound waveform
- Better movement of envelope points for longer sounds
- Better UI performance due to moving to graphics widgets

## [3.4.0]

### Features

- UI & DSP: set instrument MIDI channel
- UI & DSP: Setting to force all instruments to MIDI a channel
- Presets: new kicks presets by Harald Revery
- Presets: new kicks presets by Oleh Bardiuk ( @o-bardiuk )
- UI: Add settings tab
- UI/File Browser: bookmark folder option
- UI/Preset Browser: Add custom preset folder

### Fixes

- Code: fix memory corruption when loading presets with samples
- Code: remove the usage of shared pointers from the Geonkick code
- DSP/Filter: fix the hardclip for high values ( bug found by @DerozerDSP )
- Core GUI/redkite: use list container for observers (fix potential crashes espacially for lib++, crash reported by @treapster )
- Core GUI/redkite: don't access the interface from the ~RkObjectImpl (fixes crashes in libc++ found by @treapster )
- UI: Fix -inf to int conversion for attack value in the compressor group box ( @treapster )
- DSP: Fix uninitialized gkick_envelope_apply_type ( @treapster )
- DSP & UI: Fix nan/inf values for leveler ( @treapster )
- Build system: use funsafe-math-optimizations -fno-math-errno -fno-trapping-math ( @treapster )
- Code: error checking on parsing presets (fix potential crashes)

### Improvements

- Presets: update key mapping for kit presets to follow closer the general MIDI percussion key map
- Code: move to C++20

## [3.3.2]

### Fixes

- Fix high CPU usage by DSP
- Fix limiter for the audition channel
	, use a separate audition channel for samples preview
- Fix instrument leveler value

## [3.3.1]

### Fixes

- Fix out of bound index in KitModel (by @treapster)
- Fix warning related to clang (by @treapster)
- Fix not running Geonkick standalone if can't lock memory (warn only)

## [3.3.0]

### Features

- Windows: Use ISS script for installer
- Windows: Add plugin custom paths for installer

### Improvements

- Core DSP: Mix the playing sounds instead cutting it on note off

## [3.2.0]

### Fixes

- Windows: Fixed installation of presets.
- Windows: Linked statically all dependencies.
- Windows: Removed installation of dependencies' DLLs and environment variable path from the installer.
- Fixed the path to the config folder.

### Features

- Windows: Added scaling for GUI.
- Windows: Added logical disks for file browser.

## [3.1.1]

### Fixes

- For Windows: Fix GUI getting slow after a time of working (fix release of DC after distroy of the Cairo win32 surface)
- Fix shading of effects group box by the global and noise group box

## [3.1.0]

### Features

- Add envelope for filter resonance
- Logarithmic apply for the entire line segment of OSC pitch and filter	cutoff	envelope

### Fixes

- Some fixes in	the filter DSP
- Correct the shading of the envelope area affected by the information box displaying point coordinates.
- Other	small fixes and improvements related to building

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
