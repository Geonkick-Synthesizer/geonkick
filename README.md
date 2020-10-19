# Geonkick

Geonkick - a [free software](https://www.gnu.org/philosophy/free-sw.en.html) percussion synthesizer.

Geonkick is a synthesizer that can synthesize elements
of percussion. The most basic examples are: kicks,
snares, hit-hats, shakers, claps.

"Geon" in the name "Geonkick" refers to a hypothetical particle called "geon".

License: GPLv3

Version: 2.4.0 (not released yet)

If you have an idea about a feature or found an issue, please, submit it:

* on [GitLab](https://gitlab.com/iurie-sw/geonkick/issues)
* on [GitHub](https://github.com/iurie-sw/geonkick/issues)

If you want your created presets to be added in the list of Geonkick presets
, please, create a merge request with the preset archive on GitLab or GitHub.
See the folder "presets" how to create a preset archive.

![Screenshot](data/screenshot.png)

#### Download

Source code repository:

https://gitlab.com/iurie-sw/geonkick

Latest releases can be found on [tags](https://gitlab.com/iurie-sw/geonkick/-/tags).

#### Features

* Percussion kit
   - Supports up to 16 instruments.
   - Save / Load kit in JSON format
* Multitibral
   - 16 midi key range
   - user configurable key mapping
* Multichannel
   - 16 stereo output audio channels for plugin (1 stereo for standalone)
   - user configurable mapping of kit percussions to output channels
* 3 layers
* Layers Mixer
* 2 oscillators per layer
     - sine, square, triangle, saw-tooth, sample (wav, ogg, flac)
     - initial phase control
     - amplitude & frequency envelope
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
* Key velocity sensitive
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
* Plugin
     - LV2
* Platforms:
     - GNU/Linux

#### Requirements

In order Geonkick to run and operate correctly there is a need:

for standalone:

* GNU/Linux operating system
* Jack server running at 48000 sample rate

for plugin:

 * GNU/Linux operating system.
 * A host that supports LV2 plugin format running at 48000 sample rate.

#### Install

##### Install dependencies

In order to build Geonkick there is a need to install the following development packages:

* [Redkite](https://github.com/iurie-sw/redkite) GUI toolkit (it is recommented to use release version).
* libsndfile
* RapidJSON (version >= 1.1)
* JACK Audio Connection Kit (optional if building only for plugin)
* LV2 development library

On Debian, Ubuntu, Ubuntu Studio install:

    build and install first Redkite, than:
    apt-get install build-essential
    apt-get install cmake
    apt-get install qjackctl libjack-dev libsndfile-dev
    apt-get install rapidjson-dev
    apt-get install lv2-dev

##### Build & install Geonkick

Clone the Geonkick code repository, compile and install.

Note: It is recommended to use the release version (see repository tags).

        git clone https://gitlab.com/iurie-sw/geonkick.git
        mkdir geonkick/build
        cd geonkick/build
        cmake ../
        make
        make install

#### Packages

Geonkick can be found in the repository of ArchLinux, OpenSUSE, Fedora
, Manjaro, FreeBSD, KXStudio and Ubuntu Studio.

##### Packaging

Please, use only the release versions for packaging, and build against only the release version of Redkite.
If Redkite GUI toolkit is installed in a different location
than the standard ones there is a need to set GKICK_REDKITE_SDK_PATH
to point to the needed location.

#### Documentation

 [doc/Documentation.md](doc/Documentation.md)
