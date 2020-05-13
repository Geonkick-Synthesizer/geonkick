# Geonkick

Geonkick - a [free software](https://www.gnu.org/philosophy/free-sw.en.html) percussion synthesizer.

Geonkick is a synthesizer that can synthesize elements
of percussion. The most basic examples are: kicks,
snares, hit-hats, shakers, claps.

License: GPLv3

Version: 2.1.1

If you have an idea about a feature or found an issue, please, submit it:

* on [GitLab](https://gitlab.com/iurie-sw/geonkick/issues)
* on [GitHub](https://github.com/iurie-sw/geonkick/issues)

If you are using Geonkick, have created some presets and want to be included
in the factory presets in order Geonkick users to use them, please, attach
the archive of presets at:

* https://gitlab.com/iurie-sw/geonkick/-/issues/149
* https://github.com/iurie-sw/geonkick/issues/65

![Screenshot](data/screenshot.png)

#### Download

Source code repository:

https://gitlab.com/iurie-sw/geonkick

Latest releases can be found on [tags](https://gitlab.com/iurie-sw/geonkick/-/tags).

#### Features

* Percussion kit
   - Supports up to 16 percussions.
   - Save / Load kit in JSON format
* Multitibral
   - 16 midi keys range
   - user configurable key mapping
* Multichannel
   - for plugin up to 16 output audio channels
   - for standalone 1 output channel
   - user configurable mapping of kit percussions to channels
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
     - tune percussion option (relative to key A4)
* Key velocity sensitive
* Jack support
* Export
     - mono, stereo
     - WAV: 16, 24, 32 bit
     - FLAC: 16, 24 bit
     - Ogg
* Open & Save presets in JSON format
* Standalone
* Pitch to note
* Plugin
  - LV2
  - VST3
* Platforms:
  - GNU/Linux

#### Demo & Examples

##### Tutorials

Here is a [list of videos and tutorial](https://www.youtube.com/playlist?list=PL9Z4qz_xHZ-JfNARCWeR1Jx8Cf1upcWwY) about Geonkick.

##### Examples

In the directory doc/examples can be found some example presets.

#### Requirements

In order Geonkick to run and operate correctly there is a need:

for standalone:

* GNU/Linux operating system
* Jack server running at 48000 sample rate

for plugin:

 * GNU/Linux operating system.
 * A host that supports LV2 or VST3 plugin format running at 48000 sample rate.

#### Install

##### Install dependencies

In order to build Geonkick there is a need to install the following development packages:

<<<<<<< HEAD
* [Redkite](https://github.com/geontime/redkite) GUI toolkit (it is recommented to use release versions).
=======
* [Redkite](https://github.com/iurie-sw/redkite) GUI toolkit.
>>>>>>> master
* libsndfile
* RapidJSON (version >= 1.1)
* JACK Audio Connection Kit (optional if building only for plugin)
* LV2 development library
* For VST3 there is a need to install VST3 SDK (optional if you don't want VST3 plugin)

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

##### VST

VST build is turned off by default. In order to enable it there is a need to pass
GKICK_VST_SDK_PATH to cmake:

    cmake -DGKICK_VST_SDK_PATH=/full/path/to/vst3sdk ../

Note: at least Geonkick builds and works with VST3 SDK version [vstsdk3614_03_12_2019_build_24](https://github.com/steinbergmedia/vst3sdk/commit/0908f475f52af56682321192d800ef25d1823dd2)

#### Packages

Geonkick can be found in the repository of ArchLinux, OpenSUSE, Fedora, Manjaro, FreeBSD, KXStudio and others.

##### Packaging

Please, use only the release versions for packaging, and build against only the release version of Redkite.
If Redkite GUI toolkit is installed in a different location
than the standard ones there is a need to set GKICK_REDKITE_SDK_PATH
to point to the needed location.

#### Documentation

 [doc/Documentation.md](doc/Documentation.md)
