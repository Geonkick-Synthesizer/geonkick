/**
 * File name: OscillatorFunctionView.cpp
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

#include "OscillatorFunctionView.h"
#include "oscillator.h"
#include "knob.h"

#include <numbers>

RK_DECLARE_IMAGE_RC(knob_bk_50x50);
RK_DECLARE_IMAGE_RC(knob_50x50);

OscillatorFunctionView::OscillatorFunctionView(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , phaseControl{nullptr}
{
        setFixedSize({100, 62});
        updateView();
}

void OscillatorFunctionView::setModel(Oscillator *model)
{
        oscillatorModel = model;
        updateView();
}

void OscillatorFunctionView::updateView()
{
        switch (oscillatorModel->function()) {
        case Oscillator::FunctionType::Sine:
        case Oscillator::FunctionType::Square:
        case Oscillator::FunctionType::Triangle:
        case Oscillator::FunctionType::Sawtooth:
        case Oscillator::FunctionType::Sample:
                updatePhaseControl();
                break;
        default:
                break;
        }
}

void OscillatorFunctionView::updatePhaseControl()
{
        if (!phaseControl) {
                phaseControl = new Knob(this);
                phaseControl->setFixedSize(60, 60);
                phaseControl->setPosition((width() - phaseControl->width()) / 2, 0);
                phaseControl->setKnobBackgroundImage(RkImage(60, 60, RK_IMAGE_RC(knob_bk_50x50)));
                phaseControl->setKnobImage(RkImage(50, 50, RK_IMAGE_RC(knob_50x50)));
                phaseControl->setRange(0, 2 * std::numbers::pi);
                RK_ACT_BIND(phaseControl,
                            valueUpdated,
                            RK_ACT_ARGS(double val),
                            oscillatorModel,
                            setPhase(val));
        }
        phaseControl->setCurrentValue(oscillatorModel->getPhase());
}


