/**
 * File name: FilterView.cpp
 * Project: Geonkick (A percussive synthesizer)
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

#include "FilterView.h"
#include "geonkick_button.h"
#include "knob.h"
#include "ViewState.h"

#include <RkEvent.h>

RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(effect_view_bk);
RK_DECLARE_IMAGE_RC(filter_enabled);
RK_DECLARE_IMAGE_RC(filter_enabled_hover);
RK_DECLARE_IMAGE_RC(filter_disabled);
RK_DECLARE_IMAGE_RC(knob_bk_60x60);
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
RK_DECLARE_IMAGE_RC(fl_qfactor_button_on);
RK_DECLARE_IMAGE_RC(fl_qfactor_button_hover);
RK_DECLARE_IMAGE_RC(fl_qfactor_button_off);

FilterView::FilterView(GeonkickWidget *parent, FilterModel *model)
        : AbstractView(parent, model)
        , filterModel{static_cast<FilterModel*>(model)}
        , cutOffKnob{nullptr}
        , resonanceKnob{nullptr}
        , lpFilterButton{nullptr}
        , hpFilterButton{nullptr}
        , bpFilterButton{nullptr}
{
        setFixedSize(224, 115);
        setBackgroundImage(RkImage(224, 115, RK_IMAGE_RC(effect_view_bk)));
        createView();
        bindModel();
}

void FilterView::createView()
{
        cutOffKnob = new Knob(this);
        cutOffKnob->setRangeType(Knob::RangeType::Logarithmic);
        cutOffKnob->setFixedSize(80, 78);
        cutOffKnob->setPosition((224 / 2 - 80) / 2, 10);
        cutOffKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        cutOffKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        cutOffKnob->setRange(20, 20000);

        auto cutoffEnvelopeButton = new GeonkickButton(this);
        cutoffEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::FilterCutOff
                                         && viewState()->getEnvelopeCategory() == filterModel->envelopeCategory());
        cutoffEnvelopeButton->setFixedSize(63, 21);
        cutoffEnvelopeButton->setPosition(cutOffKnob->x() + cutOffKnob->width() / 2 - cutoffEnvelopeButton->width() / 2,
                                          cutOffKnob->y() + cutOffKnob->height() + 2);
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
                    viewState(), setEnvelope(filterModel->envelopeCategory(), Envelope::Type::FilterCutOff));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    cutoffEnvelopeButton, setPressed(envelope == Envelope::Type::FilterCutOff
                                                     && category == filterModel->envelopeCategory()));
        resonanceKnob = new Knob(this);
	resonanceKnob->setRangeType(Knob::RangeType::Logarithmic);
        int w = 60;
        int h = 60;
        resonanceKnob->setPosition(224 / 2  + (224 / 2 - w) / 2, 7);
        resonanceKnob->setFixedSize(w, h);
        resonanceKnob->setKnobBackgroundImage(RkImage(w, h, RK_IMAGE_RC(knob_bk_60x60)));
        resonanceKnob->setKnobImage(RkImage(50, 50, RK_IMAGE_RC(knob_50x50)));
        resonanceKnob->setRange(1, 1000);

	auto qFactorEnvelopeButton = new GeonkickButton(this);
        qFactorEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::FilterQFactor
                                          && viewState()->getEnvelopeCategory() == filterModel->envelopeCategory());
        qFactorEnvelopeButton->setFixedSize(22, 18);
        qFactorEnvelopeButton->setPosition(resonanceKnob->x() + resonanceKnob->width() / 2 - qFactorEnvelopeButton->width() / 2,
                                          resonanceKnob->y() + resonanceKnob->height());
        qFactorEnvelopeButton->setImage(RkImage(qFactorEnvelopeButton->size(), RK_IMAGE_RC(fl_qfactor_button_off)),
                                       RkButton::State::Unpressed);
        qFactorEnvelopeButton->setImage(RkImage(qFactorEnvelopeButton->size(), RK_IMAGE_RC(fl_qfactor_button_on)),
                                       RkButton::State::Pressed);
        qFactorEnvelopeButton->setImage(RkImage(qFactorEnvelopeButton->size(), RK_IMAGE_RC(fl_qfactor_button_hover)),
                                       RkButton::State::PressedHover);
        qFactorEnvelopeButton->setImage(RkImage(qFactorEnvelopeButton->size(), RK_IMAGE_RC(fl_qfactor_button_hover)),
                                       RkButton::State::UnpressedHover);
        RK_ACT_BIND(qFactorEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(filterModel->envelopeCategory(), Envelope::Type::FilterQFactor));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    qFactorEnvelopeButton, setPressed(envelope == Envelope::Type::FilterQFactor
                                                     && category == filterModel->envelopeCategory()));

        int x = resonanceKnob->x() + resonanceKnob->width() / 2 - (3 * 25 + 8) / 2 ;
        int y = qFactorEnvelopeButton->y() + qFactorEnvelopeButton->height() + 6;
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
}

void FilterView::updateView()
{
        cutOffKnob->setCurrentValue(filterModel->cutOff());
        resonanceKnob->setCurrentValue(filterModel->resonance());
        updateTypeButtons();
}

void FilterView::updateTypeButtons()
{
        lpFilterButton->setPressed(filterModel->type() == GeonkickApi::FilterType::LowPass);
        hpFilterButton->setPressed(filterModel->type() == GeonkickApi::FilterType::HighPass);
        bpFilterButton->setPressed(filterModel->type() == GeonkickApi::FilterType::BandPass);
}

void FilterView::bindModel()
{
        filterModel = static_cast<FilterModel*>(getModel());
        RK_ACT_BIND(cutOffKnob, valueUpdated, RK_ACT_ARGS(double val), filterModel, setCutOff(val));
        RK_ACT_BIND(resonanceKnob, valueUpdated, RK_ACT_ARGS(double val), filterModel, setResonance(val));
        RK_ACT_BIND(lpFilterButton, toggled, RK_ACT_ARGS(bool b), this, onTypeChanged(GeonkickApi::FilterType::LowPass));
        RK_ACT_BIND(bpFilterButton, toggled, RK_ACT_ARGS(bool b), this, onTypeChanged(GeonkickApi::FilterType::BandPass));
        RK_ACT_BIND(hpFilterButton, toggled, RK_ACT_ARGS(bool b), this, onTypeChanged(GeonkickApi::FilterType::HighPass));
        RK_ACT_BIND(filterModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
}

void FilterView::unbindModel()
{
        unbindObject(getModel());
        cutOffKnob->unbindObject(getModel());
        resonanceKnob->unbindObject(getModel());
        lpFilterButton->unbindObject(getModel());
        bpFilterButton->unbindObject(getModel());
        hpFilterButton->unbindObject(getModel());
}

void FilterView::onTypeChanged(GeonkickApi::FilterType type)
{
        filterModel->setType(type);
        updateTypeButtons();
}
