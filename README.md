# Geonkick

Geonkick - a free software kick synthesizer.

Details: Geonkick is a synthesizer that can synthesize elements
	 of percussion. The most basic examples are: kick drums,
	 snares, hit-hats, shakers, claps, steaks.

License: GPLv3

Author: Iurie Nistor

Home page: http://geontime.com

The synthesizer runs as a standalone application and requires Jack server.

    Status: Active development. No release yet. Don't try this at home!

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
* Platforms:
     - GNU/Linux [i]
* Deploy:
  - GPLv3 source code with CMake build system [i]
  - Documentation
      - How to build and install from source
      - Some technical insights of how the synthesizer is working and is built
      * User guide
      * FAQ
      * Tutorial
  - Testing on Debian, Ubuntu, Fedora, KXStudio and AV Linux
* Logo design
  - Logo design to be used as an application icon [i]

Note: The release of version 1.0 will not provide install packages.
      This maybe added later after the release or if someone is
      willing to create one to add it here.
