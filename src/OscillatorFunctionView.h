/**
 * File name: OscillatorFunctionView.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef OSCILLATOR_FUNCTION_VIEW_H
#define OSCILLATOR_FUNCTION_VIEW_H

#include "geonkick_widget.h"
#include "OscillatorModel.h"

class Knob;
class GeonkickButton;
class RkSpinBox;
class RkLabel;

class OscillatorFunctionView: public GeonkickWidget {
 public:
        OscillatorFunctionView(GeonkickWidget *parent, OscillatorModel* model);
        void setModel(OscillatorModel *model);

protected:
        void clearView();
        void createView();
        void createPhaseControl();
        void createNoiseControls();
        void createSampleControls();
        void setNoiseView(OscillatorModel::FunctionType noiseType);
        void updateView();
        void browseSample();

 private:
        OscillatorModel *oscillatorModel;
        Knob *phaseControl;
        RkLabel *phaseLabel;
        GeonkickButton *whiteNoiseButton;
        GeonkickButton *brownianNoiseButton;
        RkSpinBox *seedSpinBox;
        RkLabel *seedLabel;
};

#endif // OSCILLATOR_FUNCTION_VIEW_H
