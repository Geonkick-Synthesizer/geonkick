/**
 * File name: distortion_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(distortion_enable);
RK_DECLARE_IMAGE_RC(distortion_enable_active);
RK_DECLARE_IMAGE_RC(distortion_in_limiter);
RK_DECLARE_IMAGE_RC(distortion_volume_label);
RK_DECLARE_IMAGE_RC(distortion_drive_label);

DistortionGroupBox::DistortionGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , volumeSlider{nullptr}
        , driveSlider{nullptr}
        , distortionCheckbox{nullptr}
        , volumeEnvelopeButton{nullptr}
        , driveEnvelopeButton{nullptr}
{
        setFixedSize(110, 63);
        distortionCheckbox = new GeonkickButton(this);
	distortionCheckbox->setSize(77, 12);
	distortionCheckbox->setCheckable(true);
        RK_ACT_BIND(distortionCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi, enableDistortion(b));
        distortionCheckbox->setPressedImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable_active)));
        distortionCheckbox->setUnpressedImage(RkImage(distortionCheckbox->size(), RK_IMAGE_RC(distortion_enable)));
        distortionCheckbox->setPosition((width() - distortionCheckbox->width()) / 2, 0);

        int sliderW = 60;
        int sliderH = 12;
        int yoffset = 2;
        int labelD  = 5;

        // In limiter
        inLimiterSlider = new GeonkickSlider(this);
        inLimiterSlider->setFixedSize(sliderW, sliderH);
        inLimiterSlider->setPosition(width() / 2 + labelD / 2 - 12, yoffset + (height() - sliderH) / 3);
        inLimiterSlider->onSetValue(50);
        RK_ACT_BIND(inLimiterSlider, valueUpdated, RK_ACT_ARGS(int val), this, setInLimiter(val));
        auto inLimiterLabel = new RkLabel(this);
        inLimiterLabel->show();
        inLimiterLabel->setImage(RkImage(24, 10, RK_IMAGE_RC(distortion_in_limiter)));
        inLimiterLabel->setFixedSize(24, 10);
        inLimiterLabel->setPosition(inLimiterSlider->x() - inLimiterLabel->width() - labelD, inLimiterSlider->y());

        // Volume
        volumeSlider = new GeonkickSlider(this);
        volumeSlider->setFixedSize(sliderW, sliderH);
        volumeSlider->setPosition(width() / 2 + labelD / 2 - 12, yoffset + (height() - sliderH) / 3 + sliderH + 4);
        volumeSlider->onSetValue(50);
        RK_ACT_BIND(volumeSlider, valueUpdated, RK_ACT_ARGS(int val), this, setVolume(val));
        auto volumeLabel = new RkLabel(this);
        volumeLabel->show();
        volumeLabel->setImage(RkImage(38, 8, RK_IMAGE_RC(distortion_volume_label)));
        volumeLabel->setFixedSize(38, 8);
        volumeLabel->setPosition(volumeSlider->x() - volumeLabel->width() - labelD, volumeSlider->y());

        // Volume envelope button
        volumeEnvelopeButton = new GeonkickButton();
        volumeEnvelopeButton->setSize(24, 24);
        volumeEnvelopeButton->setPosition({volumeSlider->x() + volumeSlider->width() + 3,
                                volumeSlider->y()});
        volumeEnvelopeButton->setImage(RkImage(volumeEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImageUnPressed);
        volumeEnvelopeButton->setImage(RkImage(volumeEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImageUnpressedHover);
        volumeEnvelopeButton->setImage(RkImage(volumeEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImagePressed);
        volumeEnvelopeButton->setImage(RkImage(volumeEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImagePressedHover);
        volumeEnvelopeButton->show();
        RK_ACT_BIND(driveEnvelopeButton, toggled(bool b),
                    RK_ACT_ARGS(bool pressed), this, showEnvelope());


        // Drive
        driveSlider = new GeonkickSlider(this);
        driveSlider->setFixedSize(sliderW, sliderH);
        driveSlider->setPosition(width() / 2 + labelD / 2 - 12, yoffset + (height() - sliderH) / 3 + 2 * sliderH + 8);
        RK_ACT_BIND(driveSlider, valueUpdated, RK_ACT_ARGS(int val), this, setDrive(val));
        auto driveLabel = new RkLabel(this);
        driveLabel->show();
        driveLabel->setImage(RkImage(24, 8, RK_IMAGE_RC(distortion_drive_label)));
        driveLabel->setFixedSize(24, 8);
        driveLabel->setPosition(driveSlider->x() - driveLabel->width() - labelD, driveSlider->y());

        // Drive envelope button
        driveEnvelopeButton = new GeonkickButton();
        driveEnvelopeButton->setSize(24, 24);
        driveEnvelopeButton->setPosition({driveSlider->x() + driveSlider->width() + 3,
                                driveSlider->y()});
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImageUnPressed);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImageUnpressedHover);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImagePressed);
        driveEnvelopeButton->setImage(RkImage(driveEnvelopeButton->size()),
                                      RkButton::ButtonImage::ImagePressedHover);
        driveEnvelopeButton->show();
        RK_ACT_BIND(driveEnvelopeButton, toggled(bool b),
                    RK_ACT_ARGS(bool pressed), this, showEnvelope());
        show();
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
        inLimiterSlider->onSetValue(100 * (60 - fabs(logVal)) / 60);


        // Volume
        double volume = geonkickApi->getDistortionVolume();
        if (volume > 0)
                logVal = 20 * log10(volume);
        else
                logVal = 60;
        volumeSlider->onSetValue(100 * (60 - fabs(logVal)) / 60);

        // Drive
        auto distortion = geonkickApi->getDistortionDrive();
        if (distortion < std::numeric_limits<decltype(distortion)>::min())
                distortion = 0;
        else
                distortion = 20 * log10(distortion);
        driveSlider->onSetValue(100 * distortion / 36);
}

void DistortionGroupBox::showEnvelope()
{
        if (driveEnvelopeButton->isPressed()) {
                envelopeWidget->showEnvelopeType(Envelope::Type::DistortionDrive);
                volumeEnvelopeButton->setPressed(false);
        } else if (driveEnvelopeButton->isPressed()) {
                envelopeWidget->showEnvelopeType(Envelope::Type::Amplitude);
                driveEnvelopeButton->setPressed(false);
        }
}
