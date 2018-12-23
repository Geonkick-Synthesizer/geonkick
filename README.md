# Geonkick

Geonkick - a free software kick synthesizer.

Details: Geonkick is a synthesizer that can synthesize elements
	 of percussion. The most basic examples are: kick drums,
	 snares, hit-hats, shakers, claps, steaks.

         The synthesizer can run as a standalone application and as
         a LV2 plugin. Standalone requires Jack server.

License: GPLv3

Author: Iurie Nistor

Home page: http://geontime.com

Status: Active development. No release yet.

![Screenshot](data/screenshot.png)

The first release will be version 1.0. There will not be any beta versions.
The application will be released only until all the points below are
finished, including documentation.

#### Features

* Two oscillators
     - sine, squre, triangle, sawtooth
     - amplitude & frequency envelope
     - low & hight pass filter
* Noise:
     - white & browninan
     - amplitude envelope
     - low & hight pass filter
* General
     - amplitude envelope & kick length
     - low & hight pass filter
     - limiter
     - compression - still in developemnt
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
* Plugin
     - LV2
* Platforms:
  - GNU/Linux
* Build system:
  - GPLv2 source code with CMake build system

* Documentation
      - How to build and install from source
      - Some technical insights of how the synthesizer is working and is built
      * User guide
      * FAQ
      * Tutorial
- Functional manual tests
- Build and test on: Debian, Ubuntu, Fedora and KXStudio, AV Linux
* Logo design
  - Logo design to be used as an application icon

Note: The realease version 1.0 will not provide install packages.
      This will be added later after the release.

#### Tests


##### Standalone

* Debian 9.x  OK
    - All depencecies were installed from repository. Build and running works.
* Ubuntu 18.10 OK
    - All depencecies were installed from repository. Build and running works.

##### LV2 Plugin

* Debian 9.x
    - Works in Ardour 5.12 built with Suil 0.10.x and Lilv that supports state change.
    - Works in QTracktor 0.9.3 built from source. The state of the plugin is not saved comparing to Ardour.

