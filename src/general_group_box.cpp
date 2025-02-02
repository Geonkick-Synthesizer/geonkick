/**
 * File name: general_group_box.cpp
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


#include "general_group_box.h"
#include "percussion_model.h"
#include "knob.h"
#include "geonkick_button.h"
#include "FilterView.h"
#include "ViewState.h"
#include "InstrumentGlobalEffects.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(global_hboxbk_ampl_env);
RK_DECLARE_IMAGE_RC(hboxbk_filter);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(osc_ampl_button_off);
RK_DECLARE_IMAGE_RC(osc_ampl_button_on);
RK_DECLARE_IMAGE_RC(osc_ampl_button_hover);

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, PercussionModel *model)
        : AbstractView(parent, model)
        , instrumentAmplitudeKnob{nullptr}
        , instrumentLengthKnob{nullptr}
        , globalEffects{nullptr}
{
        setName("GeneralGroupBox");
        setFixedSize(224, 262);
        createView();
        bindModel();
}

void GeneralGroupBox::createView()
{
        createAplitudeEnvelopeHBox();
        createEffects();
}

void GeneralGroupBox::updateView()
{
        auto model = static_cast<PercussionModel*>(getModel());
        if (!model)
                return;
        instrumentAmplitudeKnob->setCurrentValue(model->getAmplitude());
        instrumentLengthKnob->setCurrentValue(model->getLength());
}

void GeneralGroupBox::createAplitudeEnvelopeHBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setPosition(0, 0);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(global_hboxbk_ampl_env)));
        amplitudeEnvelopeBox->show();

        instrumentAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        instrumentAmplitudeKnob->setDefaultValue(0.8);
        instrumentAmplitudeKnob->setFixedSize(80, 78);
        instrumentAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2 - 4);
        instrumentAmplitudeKnob->setBackgroundColor({0, 255, 0});
        instrumentAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        instrumentAmplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        instrumentAmplitudeKnob->setRange(0, 4.0);
        instrumentAmplitudeKnob->show();

        auto amplEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        amplEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::Amplitude
                                       && Envelope::Category::InstrumentGlobal == viewState()->getEnvelopeCategory());
        amplEnvelopeButton->setFixedSize(63, 21);
        amplEnvelopeButton->setPosition(instrumentAmplitudeKnob->x()
                                        + instrumentAmplitudeKnob->width() / 2
                                        - amplEnvelopeButton->width() / 2,
                                        instrumentAmplitudeKnob->y()
                                        + instrumentAmplitudeKnob->height() + 2);
        amplEnvelopeButton->setImage(RkImage(amplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_off)),
                                        RkButton::State::Unpressed);
        amplEnvelopeButton->setImage(RkImage(amplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_on)),
                                        RkButton::State::Pressed);
        amplEnvelopeButton->setImage(RkImage(amplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_hover)),
                                        RkButton::State::PressedHover);
        amplEnvelopeButton->setImage(RkImage(amplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_hover)),
                                        RkButton::State::UnpressedHover);
        amplEnvelopeButton->show();
        RK_ACT_BIND(amplEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(Envelope::Category::InstrumentGlobal, Envelope::Type::Amplitude));
        RK_ACT_BIND(amplitudeEnvelopeBox->viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    amplEnvelopeButton, setPressed(envelope == Envelope::Type::Amplitude
                                                   && category == Envelope::Category::InstrumentGlobal));

        instrumentLengthKnob = new Knob(amplitudeEnvelopeBox);
        instrumentLengthKnob->setDefaultValue(300);
        instrumentLengthKnob->setFixedSize(80, 80);
        instrumentLengthKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 4);
        instrumentLengthKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        instrumentLengthKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        instrumentLengthKnob->setRange(50, static_cast<PercussionModel*>(getModel())->getMaxLength());
        instrumentLengthKnob->show();
}

void GeneralGroupBox::createEffects()
{
        globalEffects = new InstrumentGlobalEffects(this, static_cast<PercussionModel*>(getModel()));
        globalEffects->setPosition(0, 125);
}

void GeneralGroupBox::bindModel()
{
        auto model = static_cast<PercussionModel*>(getModel());
        if (!model)
                return;
        RK_ACT_BIND(instrumentLengthKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    model,
                    setLength(val));
        RK_ACT_BIND(instrumentAmplitudeKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    model,
                    setAmplitude(val));
        globalEffects->setModel(model);
}

void GeneralGroupBox::unbindModel()
{
        instrumentLengthKnob->unbindObject(getModel());
        instrumentAmplitudeKnob->unbindObject(getModel());
}
