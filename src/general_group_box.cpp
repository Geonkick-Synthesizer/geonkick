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
        , kickAmplitudeKnob{nullptr}
        , kickLengthKnob{nullptr}
{
        setFixedSize(224, 250);
        createView();
        bindModel();
}

void GeneralGroupBox::createView()
{
        //createAplitudeEnvelopeHBox();
        //createEffects();
}

void GeneralGroupBox::updateView()
{
}

void GeneralGroupBox::createAplitudeEnvelopeHBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setPosition(0, 0);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(global_hboxbk_ampl_env)));
        amplitudeEnvelopeBox->show();

        kickAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        kickAmplitudeKnob->setDefaultValue(0.8);
        kickAmplitudeKnob->setFixedSize(80, 78);
        kickAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2 - 4);
        kickAmplitudeKnob->setBackgroundColor({0, 255, 0});
        kickAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        kickAmplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        kickAmplitudeKnob->setRange(0, 1.0);
        kickAmplitudeKnob->show();

        auto amplEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        amplEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::Amplitude
                                       && Envelope::Category::General == viewState()->getEnvelopeCategory());
        amplEnvelopeButton->setFixedSize(63, 21);
        amplEnvelopeButton->setPosition(kickAmplitudeKnob->x() + kickAmplitudeKnob->width() / 2
                                        - amplEnvelopeButton->width() / 2,
                                        kickAmplitudeKnob->y() + kickAmplitudeKnob->height() + 2);
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
                    viewState(), setEnvelope(Envelope::Category::General, Envelope::Type::Amplitude));
        RK_ACT_BIND(amplitudeEnvelopeBox->viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    amplEnvelopeButton, setPressed(envelope == Envelope::Type::Amplitude
                                                   && category == Envelope::Category::General));

        kickLengthKnob = new Knob(amplitudeEnvelopeBox);
        kickLengthKnob->setDefaultValue(300);
        kickLengthKnob->setFixedSize(80, 80);
        kickLengthKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 4);
        kickLengthKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        kickLengthKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        //        kickLengthKnob->setRange(50, geonkickApi->kickMaxLength());
        kickLengthKnob->show();
}

void GeneralGroupBox::createEffects()
{
        //globalEffects = new InstrumentGlobalEffects(this, );
        //globalEffects->setPosition(0, 210);
}

void GeneralGroupBox::bindModel()
{
        //auto instrumentModel = static_cast<InstrumentModel*>(getModel());
}

void GeneralGroupBox::unbindModel()
{
        //        unbindObject(getModel());
}
