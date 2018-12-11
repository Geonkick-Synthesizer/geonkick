# Geonkick

Geonkick - a kick drum synthesizer.

Author: Iurie Nistor

License: GPLv3

Home page: http://geontime.com

![Screenshot](misc/Screenshot.png)


Status: Active development. No release yet.

What version 1.0 should include:

[i] - means implemented
[p] - means partial implemented

* Two oscillators:
     - since [i]
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
     - compression [p]
     - distortion [i]
* Jack support:
     - 1 MIDI in (key velocity sensitive) [i]
     - 2 audio out [i]
* Export to WAVE [i]
     - stereo [i]
     - mono [i]
     - WAV 16. 24. 23 bit [i]
     - FLAC 16, 24 bit [i]
* Save [i]
* Open [i]
* Standalone [i]
* Plugin
     - LV2 [i]
* Platforms:
     - GNU/Linux [i]

# Build and install

The application is no ready for use but if you like
to try it here are some steps:

--- Build API first ---

    cd geonkick/api

   If you want for standalone + jack, uncomment out the line
   #DEFINES += -DGEONKICK_AUDIO_JACK from the API Makefile.
   If you want as a LV2 plugin leave it commented out.

   make - it will build the API static library

--- Build gui ---

    cd geonkick

   mkdir build

   cd build

--- for standalone ---

    qmake ../gui/gui.pro

    make

run geonkick executable

--- for lv2 ---

    qmake ../gui/lv2.pro

    make

--- install LV2 ---

    cp generated library libgeonkick_lv2.so to geonkick/lv2/geonkick.lv2/geonkick_lv2.so
    than copy entire folder geonkick.lv2 to your LV2 plugins.
    For example to /usr/local/lib/lv2/




