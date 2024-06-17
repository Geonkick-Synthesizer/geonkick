/**
 * File name: oscillator_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GKICK_OSCILLATOR_GROUP_BOX_H
#define GKICK_OSCILLATOR_GROUP_BOX_H

#include "oscillator.h"
#include "geonkick_groupbox.h"

class GKickOscillator;
class GeonkickWidget;
class GeonkickButton;
class Knob;
class GeonkickSlider;
class Filter;

class OscillatorGroupBox: public GeonkickGroupBox
{
 public:
        OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc);
        void updateGui();
        RK_DECL_ACT(oscillatorFunctionChanged,
                    oscillatorFunctionChanged(int index),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(index));

 protected:
        void createWaveFunctionGroupBox();
        void createEvelopeGroupBox();
        void createFilterGroupBox();
        void setWaveFunction(Oscillator::FunctionType type);
        void setOscillatorPhase(int value);
        void setOscillatorSeed(int value);
        void setNoiseWhite(bool pressed);
        void setNoiseBrownian(bool pressed);
        void setSampleFunction(bool pressed);
        void groupBoxLabelUpdated(bool state);
        void browseSample();
        void updateAmpltudeEnvelopeBox();

 private:
        Oscillator *oscillator;
        GeonkickButton *oscillatorCheckbox;
        GeonkickButton *fmCheckbox;
        Filter *filterBox;
        GeonkickButton *sineButton;
        GeonkickButton *squareButton;
        GeonkickButton *triangleButton;
        GeonkickButton *sawtoothButton;
        GeonkickButton *noiseButton;
        GeonkickButton *sampleButton;
        GeonkickButton *sampleBrowseButton;
        GeonkickSlider *phaseSlider;
        GeonkickSlider *seedSlider;
        GeonkickButton *noiseWhiteButton;
        GeonkickButton *noiseBrownianButton;
        Knob *amplitudeKnob;
        Knob *frequencyKnob;
        Knob *pitchShiftKnob;
        bool filterTypeIsChecked;
        GeonkickWidget *amplitudeEnvelopeBox;
        GeonkickButton *oscFreqEnvelopeButton;
        GeonkickButton *pitchEnvelopeButton;
};

#endif // GKICK_OSCILLATOR_GROUP_BOX_H
