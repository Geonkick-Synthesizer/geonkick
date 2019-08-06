/**
 * File name: filter.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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

#include "filter.h"
#include "geonkick_button.h"
#include "knob.h"

extern const unsigned char rk_knob_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_hboxbk_filter_png[];
extern const unsigned char rk_checkbox_checked_png[];
extern const unsigned char rk_checkbox_unchecked_png[];
extern const unsigned char rk_knob_bk_50x50_png[];
extern const unsigned char rk_knob_50x50_png[];

Filter::Filter(GeonkickWidget *parent)
        : filterCheckbox{nullptr}
        , cutOffKnob{nullptr}
        , resonanceKnob{nullptr}
{
        setBackgroundImage(RkImage(224, 125, rk_hboxbk_filter_png));
        setFixedSize(224, 125);

        filterCheckbox = new GeonkickButton(this);
        filterCheckbox->setCheckable(true);
        filterCheckbox->setBackgroundColor(68, 68, 70);
        filterCheckbox->setSize(10, 10);
        filterCheckbox->setPosition(10, 10);
        filterCheckbox->setPressedImage(RkImage(12, 12, rk_checkbox_checked_png));
        filterCheckbox->setUnpressedImage(RkImage(12, 12, rk_checkbox_unchecked_png));
        filterCheckbox->show();
        RK_ACT_BIND(filterCheckbox, toggled, RK_ACT_ARGS(bool b), this, enabled(b));

        cutOffKnob = new Knob(this);
        cutOffKnob->setRangeType(Knob::RangeType::Logarithmic);
        cutOffKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        cutOffKnob->setFixedSize(80, 80);
        cutOffKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        cutOffKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        cutOffKnob->setRange(20, 20000);
        cutOffKnob->show();
        RK_ACT_BIND(cutOffKnob, valueUpdated, RK_ACT_ARGS(double val), this, cutOffChanged(val));

        resonanceKnob = new Knob(filterEnvelopeBox);
        int w = 60;
        int h = 60;
        resonanceKnob->setPosition(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4 - 2);
        resonanceKnob->setFixedSize(w, h);
        resonanceKnob->setKnobBackgroundImage(RkImage(w, h, rk_knob_bk_50x50_png));
        resonanceKnob->setKnobImage(RkImage(50, 50, rk_knob_50x50_png));
        resonanceKnob->setRange(0.01, 10);
        resonanceKnob->show();
        RK_ACT_BIND(resonanceKnob, valueUpdated, RK_ACT_ARGS(double val), this, resonanceChanged(val));

        show();
}

void Filter::enable(bool b)
{
        filterCheckbox->setPressed(b);
}

bool Filter::isEnabled() const
{
        return filterCheckbox->isPressed();
}

void Filter::setCutOff(double val)
{
        frequencyKnob->setCurrentValue(val);
}

double Filter::cutOff() const
{
        return frequencyKnob->getValue();
}

void Filter::setResonance(double val)
{
        resonanceKnob->setCurrentValue(val);
}

double Filter::resonance() const
{
        resonanceKnob->getValue();
}

void Filter::setType(GeonkickApi::FilterType type)
{
}

GeonkickApi::FilterType Filter::type() const
{
}
