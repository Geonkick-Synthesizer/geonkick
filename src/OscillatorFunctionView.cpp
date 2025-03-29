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
#include "ViewState.h"

#include <RkSpinBox.h>
#include <RkLabel.h>

#include <numbers>

RK_DECLARE_IMAGE_RC(bk_knob_48x48);
RK_DECLARE_IMAGE_RC(knob_40x40);
RK_DECLARE_IMAGE_RC(noise_type_white);
RK_DECLARE_IMAGE_RC(noise_type_white_active);
RK_DECLARE_IMAGE_RC(noise_type_white_hover);
RK_DECLARE_IMAGE_RC(noise_type_brownian);
RK_DECLARE_IMAGE_RC(noise_type_brownian_active);
RK_DECLARE_IMAGE_RC(noise_type_brownian_hover);

OscillatorFunctionView::OscillatorFunctionView(GeonkickWidget *parent, OscillatorModel* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , phaseControl{nullptr}
        , phaseLabel{nullptr}
        , whiteNoiseButton{nullptr}
        , brownianNoiseButton{nullptr}
        , seedSpinBox{nullptr}
        , seedLabel{nullptr}
{
        setFixedSize({100, 62});

        RK_ACT_BIND(oscillatorModel,
                    functionUpdated,
                    RK_ACT_ARGS(OscillatorModel::FunctionType func),
                    this,
                    updateView());

        createView();
}

void OscillatorFunctionView::setModel(OscillatorModel *model)
{
        if (oscillatorModel)
                unbindObject(oscillatorModel);
        oscillatorModel = model;
        createView();
        updateView();
}

void OscillatorFunctionView::clearView()
{
        delete phaseControl;
        delete phaseLabel;
        delete whiteNoiseButton;
        delete brownianNoiseButton;
        delete seedSpinBox;
        delete seedLabel;
        phaseControl = nullptr;
        phaseLabel = nullptr;
        whiteNoiseButton = nullptr;
        brownianNoiseButton = nullptr;
        seedSpinBox = nullptr;
        seedLabel = nullptr;
}

void OscillatorFunctionView::createView()
{
        clearView();
        switch (oscillatorModel->function()) {
        case OscillatorModel::FunctionType::Sine:
        case OscillatorModel::FunctionType::Square:
        case OscillatorModel::FunctionType::Triangle:
        case OscillatorModel::FunctionType::Sawtooth:
                createPhaseControl();
                break;
        case OscillatorModel::FunctionType::Sample:
                createSampleControls();
                break;
        case OscillatorModel::FunctionType::NoiseWhite:
        case OscillatorModel::FunctionType::NoisePink:
        case OscillatorModel::FunctionType::NoiseBrownian:
                createNoiseControls();
                break;
        default:
                break;
        }
}

void OscillatorFunctionView::createPhaseControl()
{
        phaseControl = new Knob(this);
        phaseControl->setFixedSize(48, 48);
        phaseControl->setPosition((width() - phaseControl->width()) / 2, 0);
        phaseControl->setKnobBackgroundImage(RkImage(48, 48, RK_IMAGE_RC(bk_knob_48x48)));
        phaseControl->setKnobImage(RkImage(40, 40, RK_IMAGE_RC(knob_40x40)));
        phaseControl->setRange(0, 2 * std::numbers::pi);
        RK_ACT_BIND(phaseControl,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillatorModel,
                    setPhase(val));
        phaseLabel = new RkLabel(this, "Phase");
        phaseLabel->setFixedSize(30, 10);
        auto font = phaseLabel->font();
        font.setSize(9);
        font.setWeight(RkFont::Weight::Bold);
        phaseLabel->setFont(font);
        phaseLabel->setTextColor({210, 226, 226, 160});
        phaseLabel->setPosition((width() - phaseLabel->width()) / 2,
                               phaseControl->y() + phaseControl->height());
        phaseLabel->setBackgroundColor(background());
        phaseLabel->show();
}

void OscillatorFunctionView::createNoiseControls()
{
        whiteNoiseButton = new GeonkickButton(this);
        whiteNoiseButton->setPosition(30, 10);
        whiteNoiseButton->setFixedSize(18, 18);
        whiteNoiseButton->setUnpressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_white)));
        whiteNoiseButton->setImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_white_hover)), RkButton::State::UnpressedHover);
        whiteNoiseButton->setPressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_white_active)));
        RK_ACT_BIND(whiteNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    setNoiseView(OscillatorModel::FunctionType::NoiseWhite));

        brownianNoiseButton = new GeonkickButton(this);
        brownianNoiseButton->setPosition(60, 10);
        brownianNoiseButton->setFixedSize(18, 18);
        brownianNoiseButton->setUnpressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_brownian)));
        brownianNoiseButton->setImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_brownian_hover)), RkButton::State::UnpressedHover);
        brownianNoiseButton->setPressedImage(RkImage(18, 18, RK_IMAGE_RC(noise_type_brownian_active)));
        RK_ACT_BIND(brownianNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool val),
                    this,
                    setNoiseView(OscillatorModel::FunctionType::NoiseBrownian));

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

void OscillatorFunctionView::createSampleControls()
{
        createPhaseControl();
        phaseLabel->setText("Offset");
}

void OscillatorFunctionView::setNoiseView(OscillatorModel::FunctionType noiseType)
{
        oscillatorModel->setFunction(noiseType);
        updateView();
}

void OscillatorFunctionView::updateView()
{
        switch (oscillatorModel->function()) {
        case OscillatorModel::FunctionType::Sine:
        case OscillatorModel::FunctionType::Square:
        case OscillatorModel::FunctionType::Triangle:
        case OscillatorModel::FunctionType::Sawtooth:
        case OscillatorModel::FunctionType::Sample:
                phaseControl->setCurrentValue(oscillatorModel->getPhase());
                break;
        case OscillatorModel::FunctionType::NoiseWhite:
        case OscillatorModel::FunctionType::NoisePink:
        case OscillatorModel::FunctionType::NoiseBrownian:
                whiteNoiseButton->setPressed(OscillatorModel::FunctionType::NoiseWhite
                                             == oscillatorModel->function());
                brownianNoiseButton->setPressed(OscillatorModel::FunctionType::NoiseBrownian
                                             == oscillatorModel->function());
                seedSpinBox->setCurrentIndex(oscillatorModel->getSeed() / 10);
                break;
        default:
                break;
        }
}



