/**
 * File name: general_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "geonkick_api.h"
#include "general_group_box.h"
#include "knob.h"
#include "geonkick_button.h"
#include "filter.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(hboxbk_ampl_env);
RK_DECLARE_IMAGE_RC(hboxbk_filter);
RK_DECLARE_IMAGE_RC(checkbox_checked);
RK_DECLARE_IMAGE_RC(checkbox_unchecked);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(general_groupbox_label);

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , filterBox{nullptr}
        , kickAmplitudeKnob{nullptr}
        , kickLengthKnob{nullptr}
{
        setFixedSize(224, 380);
        auto label = new RkLabel(this);
        label->setBackgroundColor(background());
        label->setPosition(5, 0);
        label->setSize(64, 11);
        label->setImage(RkImage(64, 11, RK_IMAGE_RC(general_groupbox_label)));
        label->show();
        createAplitudeEnvelopeHBox();
        createFilterHBox();
        updateGui();
}

void GeneralGroupBox::createAplitudeEnvelopeHBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setPosition(0, 18);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_ampl_env)));
        amplitudeEnvelopeBox->show();

        kickAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        kickAmplitudeKnob->setFixedSize(80, 80);
        kickAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        kickAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        kickAmplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        kickAmplitudeKnob->setRange(0, 1.0);
        kickAmplitudeKnob->show();
        RK_ACT_BIND(kickAmplitudeKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    geonkickApi,
                    setKickAmplitude(val));

        kickLengthKnob = new Knob(amplitudeEnvelopeBox);
        kickLengthKnob->setFixedSize(80, 80);
        kickLengthKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2);
        kickLengthKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        kickLengthKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        kickLengthKnob->setRange(50, geonkickApi->kickMaxLength());
        kickLengthKnob->show();
        RK_ACT_BIND(kickLengthKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    geonkickApi,
                    setKickLength(val));
}

void GeneralGroupBox::createFilterHBox()
{
        filterBox = new Filter(this);
        filterBox->setCutOffRange(20, 20000);
        filterBox->setResonanceRange(1, 1000);
        filterBox->setPosition(0, 151);
        RK_ACT_BIND(filterBox, enabled, RK_ACT_ARGS(bool b),
                    geonkickApi, enableKickFilter(b));
        RK_ACT_BIND(filterBox, cutOffChanged, RK_ACT_ARGS(double val),
                    geonkickApi, setKickFilterFrequency(val));
        RK_ACT_BIND(filterBox, resonanceChanged, RK_ACT_ARGS(double val),
                    geonkickApi, setKickFilterQFactor(val));
        RK_ACT_BIND(filterBox, typeChanged,
                    RK_ACT_ARGS(GeonkickApi::FilterType type),
                    geonkickApi, setKickFilterType(type));
}

void GeneralGroupBox::updateGui()
{
        kickAmplitudeKnob->setCurrentValue(geonkickApi->kickAmplitude());
        kickLengthKnob->setCurrentValue(geonkickApi->kickLength());
        filterBox->enable(geonkickApi->isKickFilterEnabled());
        filterBox->setCutOff(geonkickApi->kickFilterFrequency());
        filterBox->setResonance(geonkickApi->kickFilterQFactor());
        filterBox->setType(geonkickApi->kickFilterType());
}

