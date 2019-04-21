/**
 * File name: oscillator_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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
class GeonkickCheckbox;
class Knob;

class OscillatorGroupBox: public GeonkickGroupBox
{
 public:
        OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc);
        ~OscillatorGroupBox();

        void update();
        RK_DECL_ACT(oscillatorFunctionChanged,
                    oscillatorFunctionChanged(int index),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(index));

 protected:
        void createWaveFunctionGroupBox();
        void createEvelopeGroupBox();
        void createFilterGroupBox();
        void setSineWave(bool pressed);
        void setSquareWave(bool pressed);
        void setTriangleWave(bool pressed);
        void setSawtoothWave(bool pressed);
        void setNoiseWhite(bool pressed);
        void setNoiseBrownian(bool pressed);
        void groupBoxLabelUpdated(bool state);
        void setFilterType(bool state);

 private:
        Oscillator *oscillator;
        GeonkickButton *oscillatorCheckbox;
        GeonkickButton *filterCheckbox;
        GeonkickButton *sineButton;
        GeonkickButton *squareButton;
        GeonkickButton *triangleButton;
        GeonkickButton *sawtoothButton;
        GeonkickButton *noiseWhiteButton;
        GeonkickButton *noiseBrownianButton;
        GeonkickButton *filterType;
        Knob *kickQFactorKnob;
        Knob *kickFrequencyKnob;
        Knob *envelopeAmplitudeKnob;
        Knob *frequencyAmplitudeKnob;
        bool filterTypeIsChecked;
};

#endif // GKICK_OSCILLATOR_GROUP_BOX_H
