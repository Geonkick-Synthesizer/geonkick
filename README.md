# Geonkick

**Geonkick** - a [free software](https://www.gnu.org/philosophy/free-sw.en.html) percussion synthesizer.

**Website:** [https://geonkick.org](https://geonkick.org)

Geonkick is a free software synthesizer capable of generating
a wide range of percussive sounds, including kicks, snares,
claps, hi-hats, shakers, and unique effect sounds.

**License:** GNU General Public License, Version 3

**Author and maintainer:** Iurie Nistor

**Version:** 3.0.1

If you have an idea about a feature or found an issue, please, submit it:

- [on GitLab](https://gitlab.com/Geonkick-Synthesizer/geonkick/issues)
- [on Codeberg](https://codeberg.org/Geonkick-Synthesizer/geonkick/issues)
- [on GitHub](https://github.com/Geonkick-Synthesizer/geonkick/issues)

![Screenshot](data/screenshot.png)

![Screenshot](data/screenshot_kit.png)

## Download

Source code repository:

- https://gitlab.com/Geonkick-Synthesizer/geonkick
- https://codeberg.org/Geonkick-Synthesizer/geonkick
- https://github.com/Geonkick-Synthesizer/geonkick

## Features

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
* LV2 & VST3 Plugin
     - "Single" version that supports one
       instrument and one stereo channel per instance.
     - The default version that supports kits of 16 instruments
       and 16 output stereo channels per instance.
* Operating System:
     - GNU/Linux (LV2, VST3, Standalone)
     - Windows (LV2, VST3)
* Only on GNU/Linux : manually scale the UI by factor (useful for HiDPI cases)

## Requirements

In order Geonkick to run and operate correctly there is a need:

for standalone:

* GNU/Linux operating system
* Jack server running

for plugin:

 * GNU/Linux or Windows operating system.
 * A host that supports LV2 or VST3 plugin format.

## Build & Install

##### Install dependencies

In order to build Geonkick there is a need to install
the following development packages:

* libsndfile
* OpenSSL
* RapidJSON
* JACK Audio Connection Kit
* LV2 development library

On Debian, Ubuntu, Ubuntu Studio install:

    apt-get install build-essential
    apt-get install cmake
    apt-get install libjack-dev libsndfile-dev
    apt-get install rapidjson-dev
    apt-get install lv2-dev
    apt-get install libcairo2-dev

##### Build & install Geonkick

Clone the Geonkick code repository, compile and install.

        copy the source code of the latest release
        mkdir geonkick/build
        cd geonkick/build
        cmake ../
        make
        make install

##### Build Geonkick with VST3 Plugin

Install additional dependences:

    apt-get install libx11-xcb-dev libxcb-util-dev libxcb-cursor-dev libxcb-xkb-dev libxkbcommon-dev libxkbcommon-x11-dev libfontconfig1-dev libcairo2-dev libgtkmm-3.0-dev libsqlite3-dev libxcb-keysyms1-dev


Get VST3 SDK than build Geonkick VST3 plugin:

        mkdir geonkick/build
        cd geonkick/build
        cmake -DVST3_SDK_PATH=<full system path to VST3 SDK folder> -DSMTG_ENABLE_VST3_PLUGIN_EXAMPLES=OFF -DSMTG_ENABLE_VST3_HOSTING_EXAMPLES=OFF -DSMTG_ENABLE_VSTGUI_SUPPORT=OFF ../
        make
        make install

##### Building on Windows

To build on Windows, you need to install MSYS2/UCRT64 and follow the same steps as mentioned above. Alternatively, you can refer to the 'Building' step in the file .github/workflows/windows.yml of Geonkick repository.

## Packages & Binaries

Geonkick can be found in the repositories of Arch Linux, openSUSE, Fedora, Manjaro, FreeBSD, KXStudio, and Ubuntu Studio.

Additionally, binareis/installer are attached as artifacts to the repository release notes or pipelines that generate builds from the main branch. It is recommended to use release tag artifacts only.

##### Packaging

Please use only release versions ([see repository releases](https://gitlab.com/Geonkick-Synthesizer/geonkick/-/releases)).

## Documentation

 [doc/Geonkick_User_Guide.md](doc/Geonkick_User_Guide.md)
