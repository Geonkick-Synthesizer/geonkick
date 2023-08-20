# Geonkick

**Geonkick** - a [free software](https://www.gnu.org/philosophy/free-sw.en.html) percussion synthesizer.

Geonkick is a synthesizer capable of producing various percussion elements.
The most basic examples are: kicks, snares, hi-hats, shakers, claps.

The term **"Geon"** in the name "Geonkick" refers to a hypothetical particle called "geon".

**License:** GPLv3

**Version:** 2.9.2

If you have an idea about a feature or found an issue, please, submit it:

- [on GitLab](https://gitlab.com/iurie-sw/geonkick/issues)
- [on GitHub](https://github.com/iurie-sw/geonkick/issues)

If you want your created presets to be added to the list of Geonkick presets, please, create a pull request
with the preset archive on GitLab or GitHub.
See the folder "presets" for instructions on creating a preset archive.

![Screenshot](data/screenshot.png)

![Screenshot](data/screenshot_kit.png)

## Download

Source code repository:

https://gitlab.com/iurie-sw/geonkick

Latest releases can be found on [tags](https://gitlab.com/iurie-sw/geonkick/-/tags).

#### Features

* Kit
   - Supports up to 16 instruments.
   - Save / Load kit in JSON format
   - Export instrument kit to SFZ
* Multitibral
   - 16 midi key range
   - user configurable key mapping
* Multichannel
   - 16 stereo audio output channels for plugin (1 stereo for standalone)
   - user configurable mapping of kit instrument to output channels
* 3 layers
* Layers Mixer
* 2 oscillators per layer
     - sine, square, triangle, saw-tooth, sample (wav, ogg, flac)
     - initial phase control
     - amplitude, frequency and sample pitch shift envelope
     - low, band and high pass filter, cutoff envelope
* FM synthesis
     - OSC1->OSC2
* One noise generator per layer
     - white & brownian
     - random seed
     - amplitude envelope
     - low, band and high pass filter, cutoff envelope
* General
     - amplitude envelope & kick length
     - low & high pass filter, cutoff envelope
     - limiter
     - compression
     - distortion
     - distortion drive envelope
     - distortion volume envelope
     - tune percussion option (relative to key A4)
* Key velocity sensitive for instrument total output amplitude
* Jack support
* Export
     - mono, stereo
     - WAV: 16, 24, 32 bit
     - FLAC: 16, 24 bit
     - Ogg
* Open & Save presets in JSON format
* Preset browser
* Standalone
* Pitch to note
* LV2 Plugin
     - "Single" version that supports one
       instrument and one stereo channel per instance.
     - The default version that supports kits of 16 instruments
       and 16 output stereo channels per instance.
* Platforms:
     - GNU/Linux
* Manually scale the UI by factor of 2 (useful for HiDPI cases)

#### Requirements

In order Geonkick to run and operate correctly there is a need:

for standalone:

* GNU/Linux operating system
* Jack server running

for plugin:

 * GNU/Linux operating system.
 * A host that supports LV2 plugin format.

#### Install

##### Install dependencies

In order to build Geonkick there is a need to install
the following development packages:

* libsndfile
* RapidJSON (version >= 1.1)
* JACK Audio Connection Kit (optional if building only for plugin)
* LV2 development library

On Debian, Ubuntu, Ubuntu Studio install:

    apt-get install build-essential
    apt-get install cmake
    apt-get install qjackctl libjack-dev libsndfile-dev
    apt-get install rapidjson-dev
    apt-get install lv2-dev
    apt-get install libcairo2-dev

##### Build & install Geonkick

Clone the Geonkick code repository, compile and install.

Note: Use the release version (see tags).

        git clone https://gitlab.com/iurie-sw/geonkick.git
        mkdir geonkick/build
        cd geonkick/build
        cmake ../
        make
        make install

##### VST3

To build with VST3 support pass GKICK_VST3 to cmake

    cmake -DGKICK_VST3=ON ../

To build with VST3 there is a need for additional dependencies.
Check the VST3 SDK documentation.

#### Packages

Geonkick can be found in the repository of ArchLinux, OpenSUSE, Fedora
, Manjaro, FreeBSD, KXStudio and Ubuntu Studio.

##### Packaging

Please, use only release versions (see the repository tags).

#### Documentation

 [doc/Documentation.md](doc/Documentation.md)
