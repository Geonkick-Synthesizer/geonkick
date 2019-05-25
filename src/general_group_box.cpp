/**
 * File name: general_group_box.cpp
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

#include "geonkick_api.h"
#include "general_group_box.h"
#include "knob.h"
#include "geonkick_button.h"

#include <RkLabel.h>

extern const unsigned char rk_hboxbk_ampl_env_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_hboxbk_filter_png[];
extern const unsigned char rk_checkbox_checked_png[];
extern const unsigned char rk_checkbox_unchecked_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_knob_bk_50x50_png[];
extern const unsigned char rk_knob_50x50_png[];
extern const unsigned char rk_filter_type_hp_png[];
extern const unsigned char rk_filter_type_lp_png[];
extern const unsigned char rk_general_groupbox_label_png[];

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , filterCheckbox{nullptr}
        , kickAmplitudeKnob{nullptr}
        , kickLengthKnob{nullptr}
        , kickFrequencyKnob{nullptr}
        , kickQFactorKnob{nullptr}
        , filterType{nullptr}
{
        setFixedSize(224, 380);
        auto label = new RkLabel(this);
        label->setPosition(5, 0);
        label->setSize(64, 11);
        label->setImage(RkImage(64, 11, rk_general_groupbox_label_png));
        label->show();
        createAplitudeEnvelopeHBox();
        createFilterHBox();
        updateGui();
}

GeneralGroupBox::~GeneralGroupBox()
{
}

void GeneralGroupBox::createAplitudeEnvelopeHBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setPosition(0, 18);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_ampl_env_png));
        amplitudeEnvelopeBox->show();

        kickAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        kickAmplitudeKnob->setFixedSize(80, 80);
        kickAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        kickAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        kickAmplitudeKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        kickAmplitudeKnob->setRange(0, 1.0);
        kickAmplitudeKnob->show();
        RK_ACT_BIND(kickAmplitudeKnob, valueUpdated, RK_ACT_ARGS(double val), geonkickApi, setKickAmplitude(val));

        kickLengthKnob = new Knob(amplitudeEnvelopeBox);
        kickLengthKnob->setFixedSize(80, 80);
        kickLengthKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2);
        kickLengthKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        kickLengthKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        kickLengthKnob->setRange(50, geonkickApi->kickMaxLength());
        kickLengthKnob->show();
        RK_ACT_BIND(kickLengthKnob, valueUpdated, RK_ACT_ARGS(double val), geonkickApi, setKickLength(val));
}

void GeneralGroupBox::createFilterHBox()
{
        auto filterEnvelopeBox = new GeonkickWidget(this);
        filterEnvelopeBox->setPosition(0, 151);
        filterEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_filter_png));
        filterEnvelopeBox->setFixedSize(224, 125);
        filterEnvelopeBox->show();

        filterCheckbox = new GeonkickButton(filterEnvelopeBox);
        filterCheckbox->setCheckable(true);
        filterCheckbox->setBackgroundColor(68, 68, 70);
        filterCheckbox->setSize(10, 10);
        filterCheckbox->setPosition(10, 10);
        filterCheckbox->setPressedImage(RkImage(12, 12, rk_checkbox_checked_png));
        filterCheckbox->setUnpressedImage(RkImage(12, 12, rk_checkbox_unchecked_png));
        filterCheckbox->show();
        RK_ACT_BIND(filterCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi, enableKickFilter(b));

        kickFrequencyKnob = new Knob(filterEnvelopeBox);
        kickFrequencyKnob->setRangeType(Knob::RangeType::Logarithmic);
        kickFrequencyKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        kickFrequencyKnob->setFixedSize(80, 80);
        kickFrequencyKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        kickFrequencyKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        kickFrequencyKnob->setRange(200, 20000);
        kickFrequencyKnob->show();
        RK_ACT_BIND(kickFrequencyKnob, valueUpdated, RK_ACT_ARGS(double val), geonkickApi, setKickFilterFrequency(val));

        kickQFactorKnob = new Knob(filterEnvelopeBox);
        int w = 60;
        int h = 60;
        kickQFactorKnob->setPosition(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4 - 2);
        kickQFactorKnob->setFixedSize(w, h);
        kickQFactorKnob->setKnobBackgroundImage(RkImage(w, h, rk_knob_bk_50x50_png));
        kickQFactorKnob->setKnobImage(RkImage(50, 50, rk_knob_50x50_png));
        kickQFactorKnob->setRange(0.01, 10);
        kickQFactorKnob->show();
        RK_ACT_BIND(kickQFactorKnob, valueUpdated, RK_ACT_ARGS(double val), geonkickApi, setKickFilterQFactor(val));

        filterType = new GeonkickButton(filterEnvelopeBox);
        filterType->setCheckable(true);
        RK_ACT_BIND(filterType, toggled, RK_ACT_ARGS(bool state), this, setFilterType(state));
        w = 80;
        h = 25;
        filterType->setPosition(224 / 2 + (224 / 2 - w) / 2, kickQFactorKnob->y() + kickQFactorKnob->height() + 15);
        filterType->setFixedSize(w, h);
        filterType->setPressedImage(RkImage(80, 25, rk_filter_type_hp_png));
        filterType->setUnpressedImage(RkImage(80, 25, rk_filter_type_lp_png));
        filterType->show();
}

void GeneralGroupBox::setFilterType(bool state)
{
        if (state)
                geonkickApi->setKickFilterType(Oscillator::FilterType::HighPass);
        else
                geonkickApi->setKickFilterType(Oscillator::FilterType::LowPass);
}

void GeneralGroupBox::updateGui()
{
        kickAmplitudeKnob->setCurrentValue(geonkickApi->kickAmplitude());
        kickLengthKnob->setCurrentValue(geonkickApi->kickLength());
        filterCheckbox->setPressed(geonkickApi->isKickFilterEnabled());
        kickFrequencyKnob->setCurrentValue(geonkickApi->kickFilterFrequency());
        kickQFactorKnob->setCurrentValue(geonkickApi->kickFilterQFactor());
        if (geonkickApi->kickFilterType() == Oscillator::FilterType::LowPass)
                filterType->setPressed(false);
        else
                filterType->setPressed(true);

}

