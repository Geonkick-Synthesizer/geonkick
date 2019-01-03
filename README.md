# Geonkick

Geonkick - a free software percussion synthesizer.

Details: Geonkick is a synthesizer that can synthesize elements
	 of percussion. The most basic examples are: kick drums,
	 snares, hit-hats, shakers, claps, steaks.

License: GPLv3

Author: Iurie Nistor

Source code repository: https://gitlab.com/iurie/geonkick

Status: Active development. No release yet.

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
     - compression - still in development
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

In order Geonkick to run there is a need for:

* GNU/Linux operating system
* Jack server installed and running

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

