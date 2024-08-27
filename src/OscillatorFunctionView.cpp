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
#include "knob.h"
#include "geonkick_button.h"

#include <RkSpinBox.h>
#include <RkLabel.h>

#include <numbers>

RK_DECLARE_IMAGE_RC(knob_bk_50x50);
RK_DECLARE_IMAGE_RC(knob_50x50);
RK_DECLARE_IMAGE_RC(noise_type_white);
RK_DECLARE_IMAGE_RC(noise_type_white_active);
RK_DECLARE_IMAGE_RC(noise_type_brownian);
RK_DECLARE_IMAGE_RC(noise_type_brownian_active);

OscillatorFunctionView::OscillatorFunctionView(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , phaseControl{nullptr}
        , whiteNoiseButton{nullptr}
        , brownianNoiseButton{nullptr}
        , seedSpinBox{nullptr}
        , seedLabel{nullptr}

{
        setFixedSize({100, 62});
        createView();
}

void OscillatorFunctionView::setModel(Oscillator *model)
{
        oscillatorModel = model;
        createView();
        updateView();
}

void OscillatorFunctionView::clearView()
{
        delete phaseControl;
        delete whiteNoiseButton;
        delete brownianNoiseButton;
        delete seedSpinBox;
        delete seedLabel;
        phaseControl = nullptr;
        whiteNoiseButton = nullptr;
        brownianNoiseButton = nullptr;
        seedSpinBox = nullptr;
        seedLabel = nullptr;
}

void OscillatorFunctionView::createView()
{
        clearView();
        switch (oscillatorModel->function()) {
        case Oscillator::FunctionType::Sine:
        case Oscillator::FunctionType::Square:
        case Oscillator::FunctionType::Triangle:
        case Oscillator::FunctionType::Sawtooth:
        case Oscillator::FunctionType::Sample:
                createPhaseControl();
                break;
        case Oscillator::FunctionType::NoiseWhite:
        case Oscillator::FunctionType::NoisePink:
        case Oscillator::FunctionType::NoiseBrownian:
                createNoiseControls();
                break;
        default:
                break;
        }
}

void OscillatorFunctionView::createPhaseControl()
{
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

void OscillatorFunctionView::createNoiseControls()
{
        whiteNoiseButton = new GeonkickButton(this);
        whiteNoiseButton->setPosition(30, 10);
        whiteNoiseButton->setFixedSize(18, 18);
        whiteNoiseButton->setUnpressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_white)));
        whiteNoiseButton->setPressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_white_active)));
        RK_ACT_BIND(whiteNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    setNoiseView(Oscillator::FunctionType::NoiseWhite));

        brownianNoiseButton = new GeonkickButton(this);
        brownianNoiseButton->setPosition(60, 10);
        brownianNoiseButton->setFixedSize(18, 18);
        brownianNoiseButton->setUnpressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_brownian)));
        brownianNoiseButton->setPressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_brownian_active)));
        RK_ACT_BIND(brownianNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool val),
                    this,
                    setNoiseView(Oscillator::FunctionType::NoiseBrownian));

        seedLabel = new RkLabel(this, "Seed");
        seedLabel->setFixedSize(30, 10);
        seedLabel->setTextColor({210, 226, 226, 160});
        seedLabel->setPosition(10,
                               brownianNoiseButton->y()
                               + brownianNoiseButton->height() + 10);
        seedLabel->setBackgroundColor(background());
        seedLabel->show();

        seedSpinBox = new RkSpinBox(this);
        seedSpinBox->setSize(48, 20);
        seedSpinBox->setPosition(seedLabel->x() + seedLabel->width() + 4,
                                 brownianNoiseButton->y()
                                 + brownianNoiseButton->height() + 6);
        seedSpinBox->setRange(0, 100);
        seedSpinBox->setTextColor({250, 250, 250});
        seedSpinBox->setBackgroundColor({60, 57, 57});
        seedSpinBox->upControl()->setBackgroundColor({50, 47, 47});
        seedSpinBox->upControl()->setTextColor({100, 100, 100});
        seedSpinBox->downControl()->setBackgroundColor({50, 47, 47});
        seedSpinBox->downControl()->setTextColor({100, 100, 100});
        seedSpinBox->show();
        RK_ACT_BIND(seedSpinBox,
                    currentIndexChanged,
                    RK_ACT_ARGS(int index),
                    oscillatorModel,
                    setSeed(10 * index));
}

void OscillatorFunctionView::setNoiseView(Oscillator::FunctionType noiseType)
{
        oscillatorModel->setFunction(noiseType);
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
                phaseControl->setCurrentValue(oscillatorModel->getPhase());
                break;
        case Oscillator::FunctionType::NoiseWhite:
        case Oscillator::FunctionType::NoisePink:
        case Oscillator::FunctionType::NoiseBrownian:
                whiteNoiseButton->setPressed(Oscillator::FunctionType::NoiseWhite
                                             == oscillatorModel->function());
                brownianNoiseButton->setPressed(Oscillator::FunctionType::NoiseBrownian
                                             == oscillatorModel->function());
                seedSpinBox->setCurrentIndex(oscillatorModel->getSeed() / 10);
                break;
        default:
                break;
        }
}



