# Geonkick

Geonkick - a free software percussion synthesizer.

Details: Geonkick is a synthesizer that can synthesize elements
	 of percussion. The most basic examples are: kick drums,
	 snares, hit-hats, shakers, claps, steaks.

License: GPLv3

Version: 1.1

Author: Iurie Nistor

Source code repository: https://gitlab.com/iurie/geonkick

![Screenshot](data/screenshot.png)

#### Features

* Two oscillators
     - sine, square, triangle, saw-tooth
     - amplitude & frequency envelope
     - low & high pass filter
* Noise:
     - white & brownian
     - amplitude envelope
     - low & high pass filter
* General
     - amplitude envelope & kick length
     - low & high pass filter
     - limiter
     - compression
     - distortion
* Jack support:
     - 1 MIDI in, key velocity sensitive
     - 2 audio out
* Export
     - stereo & mono
     - WAV: 16, 24, 32 bit
     - FLAC: 16, 24 bit
     - Ogg Vorbis
* Open & Save preset in JSON format
* Standalone
* Platforms:
  - GNU/Linux
* Build system:
  - GPLv3 source code with CMake build system

#### Requirements

In order Geonkick to run and operate correctly there is a need for:

* GNU/Linux operating system
* Jack server installed and running at 48000 sample rate

#### Install

###### Install dependeces

I order to build Geonkick there is a need to install the following development packages:
* Qt5
* libsndfile
* JACK Audio Connection Kit

On Debian, Ubuntu, Ubuntu Studio install:

    apt-get install build-essential
    apt-get install cmake
    apt-get install qt5-default && qjackctl && libjack-dev && libsndfile-dev

###### Build & install Geonkick

Clone the code repository, compile and install

        git clone git@gitlab.com:iurie/geonkick.git
        mkdir geonkick/build
        cd geonkick/build
        cmake ../
        make
        make install

#### Tests
Here is a list of tests of the application on various GNU/Linux distributions
in order to see if it builds with dependences installed from repository,
installs, runs, and if there any issues.

| Distribution  | Dependences  | Build  | Install  | Run  | Comments  |
|:-:|:-:|:-:|:-:|:-:|:-:|
| Debian 9.x/64bit  | repository  | OK  | OK  | OK  |   |
| Ubuntu 18.10/64bit  | repository  | OK  | OK  | OK  |   |
| Fedora 29 / 64 bit  | repository | OK  | OK  | OK  |   |
| Ubuntu Studio  | repository  | OK  | OK  | OK  |   |

#### Packaging

* [Package](https://www.archlinux.org/packages/community/x86_64/geonkick/) for ArchLinux community repository by David Runge

#### Road map

Here are a list of planned milestones and issues opened for them,
dates when will be started and finished.

Milestones: https://gitlab.com/iurie/geonkick/milestones

#### Short user guide

###### Shortcut Keys

* Ctrl + r - reload default state, clean everything
* k - play kick
* Ctrl + h - hide envelope, only kick graph is shown

###### Working with envelopes
* Left double click to add a new point
* Left click on the point and move the point
* Right click on the point to remove it

###### Play the sound
Geonkick uses Jack and will create two audio outputs and one MIDI input.
If audio outputs are connected there are two ways to play the sound:
   * by MIDI input, for example, MIDI keyboard. This also will be key velocity sensitive, i.e. lower velocity corresponds to lower sound volume.
   * by pressing the key 'k'

###### Examples

Under the directory geonkick/examples there are some example presets that can be opened and play with.

