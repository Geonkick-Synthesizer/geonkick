/**
 * File name: distortion_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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

#include "distortion_group_box.h"
#include "geonkick_slider.h"
#include "geonkick_button.h"
#include "geonkick_api.h"
#include "envelope_widget.h"
#include "ViewState.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(distortion_enable);
RK_DECLARE_IMAGE_RC(distortion_enable_hover);
RK_DECLARE_IMAGE_RC(distortion_enable_active);
RK_DECLARE_IMAGE_RC(distortion_in_limiter);
RK_DECLARE_IMAGE_RC(distortion_volume_env);
RK_DECLARE_IMAGE_RC(distortion_volume_env_hover);
RK_DECLARE_IMAGE_RC(distortion_volume_env_active);
RK_DECLARE_IMAGE_RC(distortion_drive_env);
RK_DECLARE_IMAGE_RC(distortion_drive_env_hover);
RK_DECLARE_IMAGE_RC(distortion_drive_env_active);
RK_DECLARE_IMAGE_RC(env_button);
RK_DECLARE_IMAGE_RC(env_button_hover);
RK_DECLARE_IMAGE_RC(env_button_on);

DistortionGroupBox::DistortionGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , volumeSlider{nullptr}
        , driveSlider{nullptr}
        , distortionCheckbox{nullptr}
        , volumeEnvButton{nullptr}
        , driveEnvButton{nullptr}
{
        setFixedSize(134, 75);
        distortionCheckbox = new GeonkickButton(this);
	distortionCheckbox->setSize(69, 21);
	distortionCheckbox->setCheckable(true);
        RK_ACT_BIND(distortionCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi, enableDistortion(b));
        distortionCheckbox->setImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable)),
                                     RkButton::State::Unpressed);
        distortionCheckbox->setImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable_active)),
                                     RkButton::State::Pressed);
        distortionCheckbox->setImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable_hover)),
                                     RkButton::State::UnpressedHover);
        distortionCheckbox->setImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable_hover)),
                                     RkButton::State::PressedHover);
        distortionCheckbox->setPosition((width() - distortionCheckbox->width()) / 2, 0);

        int sliderW = 60;
        int sliderH = 14;
        int yoffset = 8;
        int labelD  = 5;

        // In limiter
        inLimiterSlider = new GeonkickSlider(this);
        inLimiterSlider->setFixedSize(sliderW, sliderH);
        inLimiterSlider->setPosition(width() / 2 + labelD / 2, yoffset + (height() - sliderH) / 3);
        RK_ACT_BIND(inLimiterSlider, valueUpdated, RK_ACT_ARGS(int val), this, setInLimiter(val));
        auto inLimiterLabel = new RkLabel(this);
        inLimiterLabel->setBackgroundColor(background());
        inLimiterLabel->show();
        inLimiterLabel->setFixedSize(53, 14);
        inLimiterLabel->setImage(RkImage(inLimiterLabel->size(), RK_IMAGE_RC(distortion_in_limiter)));
        inLimiterLabel->setPosition(inLimiterSlider->x() - inLimiterLabel->width() - labelD, inLimiterSlider->y());

        // Volume
        volumeSlider = new GeonkickSlider(this);
        volumeSlider->setFixedSize(sliderW, sliderH);
        volumeSlider->setPosition(width() / 2 + labelD / 2, yoffset + (height() - sliderH) / 3 + sliderH + 2);
        volumeSlider->onSetValue(50);
        RK_ACT_BIND(volumeSlider, valueUpdated, RK_ACT_ARGS(int val), this, setVolume(val));
        volumeEnvButton = new GeonkickButton(this);
        volumeEnvButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::DistortionDrive
                                    && viewState()->getEnvelopeCategory() == Envelope::Category::General);
        volumeEnvButton->show();
        volumeEnvButton->setFixedSize(53, 14);
        volumeEnvButton->setImage(RkImage(volumeEnvButton->size(), RK_IMAGE_RC(distortion_volume_env)),
                                  RkButton::State::Unpressed);
        volumeEnvButton->setImage(RkImage(volumeEnvButton->size(), RK_IMAGE_RC(distortion_volume_env_active)),
                                  RkButton::State::Pressed);
        volumeEnvButton->setImage(RkImage(volumeEnvButton->size(), RK_IMAGE_RC(distortion_volume_env_hover)),
                                  RkButton::State::UnpressedHover);
        volumeEnvButton->setImage(RkImage(volumeEnvButton->size(), RK_IMAGE_RC(distortion_volume_env_hover)),
                                  RkButton::State::PressedHover);
        volumeEnvButton->setPosition(volumeSlider->x() - labelD - volumeEnvButton->width(), volumeSlider->y());
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    volumeEnvButton, setPressed(envelope == Envelope::Type::DistortionVolume
                                               && Envelope::Category::General == category));
        RK_ACT_BIND(volumeEnvButton, pressed,
                    RK_ACT_ARGS(), viewState(),
                    setEnvelope(Envelope::Category::General, Envelope::Type::DistortionVolume));

        // Drive
        driveSlider = new GeonkickSlider(this);
        driveSlider->setFixedSize(sliderW, sliderH);
        driveSlider->setPosition(width() / 2 + labelD / 2,
                                 yoffset + (height() - sliderH) / 3 + 2 * sliderH + 4);
        RK_ACT_BIND(driveSlider, valueUpdated, RK_ACT_ARGS(int val), this, setDrive(val));
        driveEnvButton = new GeonkickButton(this);
        driveEnvButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::DistortionDrive
                                   && viewState()->getEnvelopeCategory()== Envelope::Category::General);
        driveEnvButton->setFixedSize(53, 14);
        driveEnvButton->setImage(RkImage(driveEnvButton->size(), RK_IMAGE_RC(distortion_drive_env)),
                                 RkButton::State::Unpressed);
        driveEnvButton->setImage(RkImage(driveEnvButton->size(), RK_IMAGE_RC(distortion_drive_env_active)),
                                 RkButton::State::Pressed);
        driveEnvButton->setImage(RkImage(driveEnvButton->size(), RK_IMAGE_RC(distortion_drive_env_hover)),
                                 RkButton::State::UnpressedHover);
        driveEnvButton->setImage(RkImage(driveEnvButton->size(), RK_IMAGE_RC(distortion_drive_env_hover)),
                                 RkButton::State::PressedHover);
        driveEnvButton->setPosition(driveSlider->x() - driveEnvButton->width() - labelD,
                                    driveSlider->y());
        driveEnvButton->show();

        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    driveEnvButton, setPressed(envelope == Envelope::Type::DistortionDrive
                                               && Envelope::Category::General == category));
        RK_ACT_BIND(driveEnvButton, pressed,
                    RK_ACT_ARGS(), viewState(),
                    setEnvelope(Envelope::Category::General, Envelope::Type::DistortionDrive));
        show();
        updateGui();
}

void DistortionGroupBox::setInLimiter(int val)
{
        double logVal = -60 * (1.0 - (static_cast<double>(val) / 100));
        double limit = pow(10, logVal / 20);
        geonkickApi->setDistortionInLimiter(limit);
}

void DistortionGroupBox::setVolume(int val)
{
        double logVal = -60 * (1.0 - (static_cast<double>(val) / 100));
        double volume = pow(10, logVal / 20);
        geonkickApi->setDistortionVolume(volume);
}

void DistortionGroupBox::setDrive(int val)
{
        double db =  36.0 * static_cast<double>(val) / 100.0;
        geonkickApi->setDistortionDrive(pow(10, db / 20));
}

void DistortionGroupBox::updateGui()
{
        distortionCheckbox->setPressed(geonkickApi->isDistortionEnabled());

        // In Limiter
        double limit = geonkickApi->getDistortionInLimiter();
        double logVal;
        if (limit > 0)
                logVal = 20 * log10(limit);
        else
                logVal = 60;
        inLimiterSlider->onSetValue(100 * (60 - fabs(logVal)) / 60, 100);


        // Volume
        double volume = geonkickApi->getDistortionVolume();
        if (volume > 0)
                logVal = 20 * log10(volume);
        else
                logVal = 60;
        volumeSlider->onSetValue(100 * (60 - fabs(logVal)) / 60, 100 * (2.0 / 3));

        // Drive
        auto distortion = geonkickApi->getDistortionDrive();
        if (distortion < std::numeric_limits<decltype(distortion)>::min())
                distortion = 0;
        else
                distortion = 20 * log10(distortion);
        driveSlider->onSetValue(100 * distortion / 36, 0);
}
