# GeonKick
Kick drum synthesizer (standalone)

Version 0.3.1

Features:

Working with Jack server:

- 2 audio outputs
- 1 MIDI in

Two oscillators:
 - base
      wave form: sine, sqare, sawtooth, triangle)
      envelopes: amplitude, frequency
 - noise
      envelope: amplitude

Settings:
  - kick length

Build:

From the top directory:
cd api
make
cd ../gui
qmake
make

Use:
Run the geonkick from the gui directory
