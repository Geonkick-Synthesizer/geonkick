/**
 * File name: filter.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor 
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
#include "ViewState.h"

#include <RkEvent.h>

RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(hboxbk_filter);
RK_DECLARE_IMAGE_RC(filter_enabled);
RK_DECLARE_IMAGE_RC(filter_enabled_hover);
RK_DECLARE_IMAGE_RC(filter_disabled);
RK_DECLARE_IMAGE_RC(knob_bk_50x50);
RK_DECLARE_IMAGE_RC(knob_50x50);
RK_DECLARE_IMAGE_RC(filter_type_lp);
RK_DECLARE_IMAGE_RC(filter_type_hp);
RK_DECLARE_IMAGE_RC(filter_type_bp);
RK_DECLARE_IMAGE_RC(filter_type_lp_checked);
RK_DECLARE_IMAGE_RC(filter_type_hp_checked);
RK_DECLARE_IMAGE_RC(filter_type_bp_checked);
RK_DECLARE_IMAGE_RC(filter_type_lp_hover);
RK_DECLARE_IMAGE_RC(filter_type_hp_hover);
RK_DECLARE_IMAGE_RC(filter_type_bp_hover);
RK_DECLARE_IMAGE_RC(filter_cutoff_button_on);
RK_DECLARE_IMAGE_RC(fl_cutoff_button_on);
RK_DECLARE_IMAGE_RC(fl_cutoff_button_hover);
RK_DECLARE_IMAGE_RC(fl_cutoff_button_off);

Filter::Filter(GeonkickWidget *parent, Envelope::Category category)
        : GeonkickWidget(parent)
        , filterCheckbox{nullptr}
        , cutOffKnob{nullptr}
        , resonanceKnob{nullptr}
        , lpFilterButton{nullptr}
        , hpFilterButton{nullptr}
        , bpFilterButton{nullptr}
        , envelopeCategory{category}
{
        setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_filter)));
        setFixedSize(224, 125);

        filterCheckbox = new GeonkickButton(this);
        filterCheckbox->setCheckable(true);
        filterCheckbox->setBackgroundColor(68, 68, 70);
        filterCheckbox->setSize(32, 21);
        filterCheckbox->setPosition(width() / 2 - filterCheckbox->width() / 2, 10);
        filterCheckbox->setImage(RkImage(filterCheckbox->size(), RK_IMAGE_RC(filter_enabled)),
                                       RkButton::State::Pressed);
        filterCheckbox->setImage(RkImage(filterCheckbox->size(), RK_IMAGE_RC(filter_disabled)),
                                       RkButton::State::Unpressed);
        filterCheckbox->setImage(RkImage(filterCheckbox->size(), RK_IMAGE_RC(filter_enabled_hover)),
                                       RkButton::State::PressedHover);
        filterCheckbox->setImage(RkImage(filterCheckbox->size(), RK_IMAGE_RC(filter_enabled_hover)),
                                       RkButton::State::UnpressedHover);
        RK_ACT_BIND(filterCheckbox, toggled, RK_ACT_ARGS(bool b), this, enabled(b));

        cutOffKnob = new Knob(this);
        cutOffKnob->setRangeType(Knob::RangeType::Logarithmic);
        cutOffKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        cutOffKnob->setFixedSize(80, 78);
        cutOffKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        cutOffKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        cutOffKnob->setRange(20, 20000);
        RK_ACT_BIND(cutOffKnob, valueUpdated, RK_ACT_ARGS(double val), this, cutOffChanged(val));

        auto cutoffEnvelopeButton = new GeonkickButton(this);
        cutoffEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::FilterCutOff
                                         && viewState()->getEnvelopeCategory() == envelopeCategory);
        cutoffEnvelopeButton->setFixedSize(63, 21);
        cutoffEnvelopeButton->setPosition(cutOffKnob->x() + cutOffKnob->width() / 2 - cutoffEnvelopeButton->width() / 2,
                                          cutOffKnob->y() + cutOffKnob->height());
        cutoffEnvelopeButton->setImage(RkImage(cutoffEnvelopeButton->size(), RK_IMAGE_RC(fl_cutoff_button_off)),
                                       RkButton::State::Unpressed);
        cutoffEnvelopeButton->setImage(RkImage(cutoffEnvelopeButton->size(), RK_IMAGE_RC(fl_cutoff_button_on)),
                                       RkButton::State::Pressed);
        cutoffEnvelopeButton->setImage(RkImage(cutoffEnvelopeButton->size(), RK_IMAGE_RC(fl_cutoff_button_hover)),
                                       RkButton::State::PressedHover);
        cutoffEnvelopeButton->setImage(RkImage(cutoffEnvelopeButton->size(), RK_IMAGE_RC(fl_cutoff_button_hover)),
                                       RkButton::State::UnpressedHover);
        RK_ACT_BIND(cutoffEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(envelopeCategory, Envelope::Type::FilterCutOff));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    cutoffEnvelopeButton, setPressed(envelope == Envelope::Type::FilterCutOff
                                                     && category == envelopeCategory));

        resonanceKnob = new Knob(this);
	resonanceKnob->setRangeType(Knob::RangeType::Logarithmic);
        int w = 60;
        int h = 60;
        resonanceKnob->setPosition(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4 - 2);
        resonanceKnob->setFixedSize(w, h);
        resonanceKnob->setKnobBackgroundImage(RkImage(w, h, RK_IMAGE_RC(knob_bk_50x50)));
        resonanceKnob->setKnobImage(RkImage(50, 50, RK_IMAGE_RC(knob_50x50)));
        resonanceKnob->setRange(0.01, 10);
        RK_ACT_BIND(resonanceKnob, valueUpdated, RK_ACT_ARGS(double val), this, resonanceChanged(val));

        int x = resonanceKnob->x() + resonanceKnob->width() / 2 - (3 * 25 + 8) / 2 ;
        int y = height() - 30;
        lpFilterButton = new GeonkickButton(this);
        lpFilterButton->setBackgroundColor(background());
        lpFilterButton->setFixedSize(25, 18);
        lpFilterButton->setPosition(x,  y);
        lpFilterButton->setImage(RkImage(lpFilterButton->size(), RK_IMAGE_RC(filter_type_lp)),
                                 RkButton::State::Unpressed);
        lpFilterButton->setImage(RkImage(lpFilterButton->size(), RK_IMAGE_RC(filter_type_lp_checked)),
                                 RkButton::State::Pressed);
        lpFilterButton->setImage(RkImage(lpFilterButton->size(), RK_IMAGE_RC(filter_type_lp_hover)),
                                 RkButton::State::UnpressedHover);
        RK_ACT_BIND(lpFilterButton, toggled, RK_ACT_ARGS(bool b), this,
                    setFilterType(GeonkickApi::FilterType::LowPass, b));

        bpFilterButton = new GeonkickButton(this);
        bpFilterButton->setBackgroundColor(background());
        bpFilterButton->setFixedSize(25, 18);
        bpFilterButton->setPosition(lpFilterButton->x() + lpFilterButton->width() + 4,
                                    lpFilterButton->y());
        bpFilterButton->setImage(RkImage(bpFilterButton->size(), RK_IMAGE_RC(filter_type_bp)),
                                 RkButton::State::Unpressed);
        bpFilterButton->setImage(RkImage(bpFilterButton->size(), RK_IMAGE_RC(filter_type_bp_checked)),
                                 RkButton::State::Pressed);
        bpFilterButton->setImage(RkImage(bpFilterButton->size(), RK_IMAGE_RC(filter_type_bp_hover)),
                                 RkButton::State::UnpressedHover);
        RK_ACT_BIND(bpFilterButton, toggled, RK_ACT_ARGS(bool b), this,
                    setFilterType(GeonkickApi::FilterType::BandPass, b));

        hpFilterButton = new GeonkickButton(this);
        hpFilterButton->setBackgroundColor(background());
        hpFilterButton->setFixedSize(25, 18);
        hpFilterButton->setPosition(bpFilterButton->x() + bpFilterButton->width() + 4,
                                    bpFilterButton->y());
        hpFilterButton->setImage(RkImage(hpFilterButton->size(), RK_IMAGE_RC(filter_type_hp)),
                                 RkButton::State::Unpressed);
        hpFilterButton->setImage(RkImage(hpFilterButton->size(), RK_IMAGE_RC(filter_type_hp_checked)),
                                 RkButton::State::Pressed);
        hpFilterButton->setImage(RkImage(hpFilterButton->size(), RK_IMAGE_RC(filter_type_hp_hover)),
                                 RkButton::State::UnpressedHover);
        RK_ACT_BIND(hpFilterButton, toggled, RK_ACT_ARGS(bool b), this,
                    setFilterType(GeonkickApi::FilterType::HighPass, b));

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
        cutOffKnob->setCurrentValue(val);
}

double Filter::cutOff() const
{
        return cutOffKnob->getValue();
}

void Filter::setResonance(double val)
{
        resonanceKnob->setCurrentValue(val);
}

double Filter::resonance() const
{
        return resonanceKnob->getValue();
}

void Filter::setType(GeonkickApi::FilterType type)
{
        lpFilterButton->setPressed(type == GeonkickApi::FilterType::LowPass);
        hpFilterButton->setPressed(type == GeonkickApi::FilterType::HighPass);
        bpFilterButton->setPressed(type == GeonkickApi::FilterType::BandPass);
}

void Filter::setFilterType(GeonkickApi::FilterType type, bool b)
{
        if (b) {
                setType(type);
                typeChanged(type);
        }
}

GeonkickApi::FilterType Filter::type() const
{
        if (lpFilterButton->isPressed())
                return GeonkickApi::FilterType::LowPass;
        else if (hpFilterButton->isPressed())
                return GeonkickApi::FilterType::HighPass;
        else
                return GeonkickApi::FilterType::BandPass;
}

void Filter::setCutOffRange(double from, double to)
{
        cutOffKnob->setRange(from, to);
}

void Filter::setResonanceRange(double from, double to)
{
        resonanceKnob->setRange(from, to);
}

void Filter::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Right
            || event->button() == RkMouseEvent::ButtonType::Left) {
                bool b = !isEnabled();
                enable(b);
                enabled(b);
        }
}
