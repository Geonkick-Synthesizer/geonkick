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

What version 1.0 should include:

Note: [i] - implemented

* Two oscillators:
     - sine [i]
     - square [i]
     - triangle [i]
     - sawtooth [i]
     - amplitude envelope [i]
     - frequency envelope [i]
     - low & hight pass filter [i]
* Noise:
     - white [i]
     - browninan [i]
     - amplitude envelope [i]
     - low & hight pass filter [i]
* Kick
     - amplitude envelope [i]
     - low & hight pass filter [i]
     - kick length [i]
     - kick limiter [i]
     - compression
     - distortion [i]
* Jack support:
     - 1 MIDI in (key velocity sensitive) [i]
     - 2 audio out [i]
* Export to WAVE [i]
     - stereo [i]
     - mono [i]
     - WAV 16, 24, 32 bit [i]
     - FLAC 16, 24 bit [i]
* Save preset in JSON format [i]
* Open preset from JSON format [i]
* Standalone [i]
* Plugin
     - LV2 [i] - works only with hosts that provide Qt5.
                 It works in Ardour but there is a need to compile
                 Ardour with libsuil 0.1.x.
* Platforms:
     - GNU/Linux [i]
* Deploy:
  - GPLv2 source code with CMake build system [i]
  - Documentation
      - How to build and install from source
      - Some technical insights of how the synthesizer is working and is built
      * User guide
      * FAQ
      * Tutorial
  - Functional manual tests
  - Build and test on: Debian, Ubuntu, Fedora and KXStudio
* Logo design
  - Logo design to be used as an application icon [i]

Note: The realease of version 1.0 will not provide install packages.
      This maybe added later after the release.
