/**
 * File name: Distortion.cpp
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

#include "DistortionView.h"
#include "DistortionModel.h"

#include "knob.h"
#include "geonkick_button.h"
#include "RkLabel.h"

RK_DECLARE_IMAGE_RC(effect_view_bk);
RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(bk_knob_48x48);
RK_DECLARE_IMAGE_RC(knob_40x40);
RK_DECLARE_IMAGE_RC(distortion_drive_env);
RK_DECLARE_IMAGE_RC(distortion_drive_env_active);
RK_DECLARE_IMAGE_RC(distortion_drive_env_hover);
RK_DECLARE_IMAGE_RC(distortion_hc_button);
RK_DECLARE_IMAGE_RC(distortion_hc_button_hover);
RK_DECLARE_IMAGE_RC(distortion_hc_button_active);
RK_DECLARE_IMAGE_RC(distortion_th_button);
RK_DECLARE_IMAGE_RC(distortion_th_button_hover);
RK_DECLARE_IMAGE_RC(distortion_th_button_active);
RK_DECLARE_IMAGE_RC(distortion_ath_button);
RK_DECLARE_IMAGE_RC(distortion_ath_button_hover);
RK_DECLARE_IMAGE_RC(distortion_ath_button_active);
RK_DECLARE_IMAGE_RC(distortion_exp_button);
RK_DECLARE_IMAGE_RC(distortion_exp_button_hover);
RK_DECLARE_IMAGE_RC(distortion_exp_button_active);
RK_DECLARE_IMAGE_RC(distortion_log_button);
RK_DECLARE_IMAGE_RC(distortion_log_button_hover);
RK_DECLARE_IMAGE_RC(distortion_log_button_active);
RK_DECLARE_IMAGE_RC(distortion_pl_button);
RK_DECLARE_IMAGE_RC(distortion_pl_button_hover);
RK_DECLARE_IMAGE_RC(distortion_pl_button_active);
RK_DECLARE_IMAGE_RC(distortion_in_label);
RK_DECLARE_IMAGE_RC(distortion_out_label);

DistortionView::DistortionView(GeonkickWidget* parent, DistortionModel *model)
        : AbstractView(parent, model)
{
        setFixedSize(224, 115);
        setBackgroundImage(RkImage(224, 115, RK_IMAGE_RC(effect_view_bk)));
        //setBackgroundColor(0, 255, 0);
        createView();
        bindModel();
}

void DistortionView::bindModel()
{
        /*        auto distortionModel = static_cast<DistortionModel*>(getModel());
        RK_ACT_BIND(distortionModel,
                    distortionTypeChanged,
                    RK_ACT_ARGS(enum DistortionType type),
                    this,
                    setDistortionType(type));
        RK_ACT_BIND(distortionModel,
                    inLimiterChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setInElimiter(value));
        RK_ACT_BIND(distortionModel,
                    outLimiterChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setOutElimiter(value));
        RK_ACT_BIND(distortionModel,
                    driveChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setDrive(value));*/
}

void DistortionView::unbindModel()
{
        //        unbindObject(getModel());
}

void DistortionView::createView()
{
        auto inLimiter = new Knob(this);
        inLimiter->setFixedSize(48, 48);
        inLimiter->setPosition(width() / 2, 2);
        inLimiter->setKnobBackgroundImage(RkImage(48, 48, RK_IMAGE_RC(bk_knob_48x48)));
        inLimiter->setKnobImage(RkImage(40, 40, RK_IMAGE_RC(knob_40x40)));
        inLimiter->setRange(0.01, 1);

        auto inLimiterLabel = new RkLabel(this,  "Input");
        inLimiterLabel->setFixedSize(30, 10);
        {
                auto font = inLimiterLabel->font();
                font.setSize(9);
                font.setWeight(RkFont::Weight::Bold);
                inLimiterLabel->setFont(font);
        }
        inLimiterLabel->setTextColor({210, 226, 226, 160});
        inLimiterLabel->setBackgroundColor(background());
        inLimiterLabel->setPosition(inLimiter->x() + 9,
                                    inLimiter->y() + inLimiter->height());
        inLimiterLabel->show();

        auto outLimiter = new Knob(this);
        outLimiter->setFixedSize(48, 48);
        outLimiter->setPosition(width() / 2 + inLimiter->width() + 6, 2);
        outLimiter->setKnobBackgroundImage(RkImage(48, 48, RK_IMAGE_RC(bk_knob_48x48)));
        outLimiter->setKnobImage(RkImage(40, 40, RK_IMAGE_RC(knob_40x40)));
        outLimiter->setRange(0.01, 1);

        auto outLimiterLabel = new RkLabel(this,  "Output");
        outLimiterLabel->setFixedSize(30, 10);
        {
                auto font = outLimiterLabel->font();
                font.setSize(9);
                font.setWeight(RkFont::Weight::Bold);
                outLimiterLabel->setFont(font);
        }
        outLimiterLabel->setTextColor({210, 226, 226, 160});
        outLimiterLabel->setBackgroundColor(background());
        outLimiterLabel->setPosition(outLimiter->x() + 8,
                                    outLimiter->y() + outLimiter->height());
        outLimiterLabel->show();

        auto hcDistotionButton = new GeonkickButton(this);
        hcDistotionButton->setBackgroundColor(background());
        hcDistotionButton->setFixedSize(25, 18);
        hcDistotionButton->setPosition(width() / 2 + 10,  69);
        hcDistotionButton->setImage(RkImage(hcDistotionButton->size(), RK_IMAGE_RC(distortion_hc_button)),
                                 RkButton::State::Unpressed);
        hcDistotionButton->setImage(RkImage(hcDistotionButton->size(), RK_IMAGE_RC(distortion_hc_button_active)),
                                 RkButton::State::Pressed);
        hcDistotionButton->setImage(RkImage(hcDistotionButton->size(), RK_IMAGE_RC(distortion_hc_button_hover)),
                                 RkButton::State::UnpressedHover);
        auto thDistortionButton = new GeonkickButton(this);
        thDistortionButton->setBackgroundColor(background());
        thDistortionButton->setFixedSize(25, 18);
        thDistortionButton->setPosition(hcDistotionButton->x() + hcDistotionButton->width() + 4,
                                    hcDistotionButton->y());
        thDistortionButton->setImage(RkImage(thDistortionButton->size(), RK_IMAGE_RC(distortion_th_button)),
                                 RkButton::State::Unpressed);
        thDistortionButton->setImage(RkImage(thDistortionButton->size(), RK_IMAGE_RC(distortion_th_button_active)),
                                 RkButton::State::Pressed);
        thDistortionButton->setImage(RkImage(thDistortionButton->size(), RK_IMAGE_RC(distortion_th_button)),
                                 RkButton::State::UnpressedHover);
        auto athDistortionButton = new GeonkickButton(this);
        athDistortionButton->setBackgroundColor(background());
        athDistortionButton->setFixedSize(25, 18);
        athDistortionButton->setPosition(thDistortionButton->x() + thDistortionButton->width() + 4,
                                    thDistortionButton->y());
        athDistortionButton->setImage(RkImage(athDistortionButton->size(), RK_IMAGE_RC(distortion_ath_button)),
                                 RkButton::State::Unpressed);
        athDistortionButton->setImage(RkImage(athDistortionButton->size(), RK_IMAGE_RC(distortion_ath_button_active)),
                                 RkButton::State::Pressed);
        athDistortionButton->setImage(RkImage(athDistortionButton->size(), RK_IMAGE_RC(distortion_ath_button_hover)),
                                 RkButton::State::UnpressedHover);

        auto expDistortionButton = new GeonkickButton(this);
        expDistortionButton->setBackgroundColor(background());
        expDistortionButton->setFixedSize(25, 18);
        expDistortionButton->setPosition(width() / 2 + 10, 70 + 21);
        expDistortionButton->setImage(RkImage(expDistortionButton->size(), RK_IMAGE_RC(distortion_exp_button)),
                                 RkButton::State::Unpressed);
        expDistortionButton->setImage(RkImage(expDistortionButton->size(), RK_IMAGE_RC(distortion_exp_button_active)),
                                 RkButton::State::Pressed);
        expDistortionButton->setImage(RkImage(expDistortionButton->size(), RK_IMAGE_RC(distortion_exp_button_hover)),
                                 RkButton::State::UnpressedHover);
        auto logDistortionButton = new GeonkickButton(this);
        logDistortionButton->setBackgroundColor(background());
        logDistortionButton->setFixedSize(25, 18);
        logDistortionButton->setPosition(expDistortionButton->x() + expDistortionButton->width() + 4,
                                    expDistortionButton->y());
        logDistortionButton->setImage(RkImage(logDistortionButton->size(), RK_IMAGE_RC(distortion_log_button)),
                                 RkButton::State::Unpressed);
        logDistortionButton->setImage(RkImage(logDistortionButton->size(), RK_IMAGE_RC(distortion_log_button_active)),
                                 RkButton::State::Pressed);
        logDistortionButton->setImage(RkImage(logDistortionButton->size(), RK_IMAGE_RC(distortion_log_button_hover)),
                                 RkButton::State::UnpressedHover);
        auto plDistortionButton = new GeonkickButton(this);
        plDistortionButton->setBackgroundColor(background());
        plDistortionButton->setFixedSize(25, 18);
        plDistortionButton->setPosition(logDistortionButton->x() + logDistortionButton->width() + 4,
                                    logDistortionButton->y());
        plDistortionButton->setImage(RkImage(plDistortionButton->size(), RK_IMAGE_RC(distortion_pl_button)),
                                 RkButton::State::Unpressed);
        plDistortionButton->setImage(RkImage(plDistortionButton->size(), RK_IMAGE_RC(distortion_pl_button_active)),
                                 RkButton::State::Pressed);
        plDistortionButton->setImage(RkImage(plDistortionButton->size(), RK_IMAGE_RC(distortion_pl_button_hover)),
                                 RkButton::State::UnpressedHover);

        auto driveKnob = new Knob(this);
        driveKnob->setRangeType(Knob::RangeType::Logarithmic);
        driveKnob->setFixedSize(80, 78);
        driveKnob->setPosition(16, 10);
        driveKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        driveKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        driveKnob->setRange(1.0, 63);

        auto driveEnvelopeButton = new GeonkickButton(this);
        driveEnvelopeButton->setFixedSize(63, 21);
        driveEnvelopeButton->setPosition(driveKnob->x() + driveKnob->width() / 2 - driveEnvelopeButton->width() / 2,
                                           driveKnob->y() + driveKnob->height() + 2);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size(), RK_IMAGE_RC(distortion_drive_env)),
                                        RkButton::State::Unpressed);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size(), RK_IMAGE_RC(distortion_drive_env_active)),
                                        RkButton::State::Pressed);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size(), RK_IMAGE_RC(distortion_drive_env_hover)),
                                        RkButton::State::PressedHover);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size(), RK_IMAGE_RC(distortion_drive_env_hover)),
        RkButton::State::UnpressedHover);
}

void DistortionView::updateView()
{
}

