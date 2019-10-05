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

extern const unsigned char rk_distortion_goupbox_label_png[];
extern const unsigned char rk_checkbox_checked_10x10_png[];
extern const unsigned char rk_checkbox_unchecked_10x10_png[];
extern const unsigned char rk_distortion_volume_label_png[];
extern const unsigned char rk_distortion_drive_label_png[];

DistortionGroupBox::DistortionGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , volumeSlider{nullptr}
        , driveSlider{nullptr}
        , distortionCheckbox{nullptr}
{
        setFixedSize(110, 63);
        distortionCheckbox = new GeonkickButton(this);
        RK_ACT_BIND(distortionCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi, enableDistortion(b));
        distortionCheckbox->setCheckable(true);
        distortionCheckbox->setPressedImage(RkImage(10, 10, rk_checkbox_checked_10x10_png));
        distortionCheckbox->setUnpressedImage(RkImage(10, 10, rk_checkbox_unchecked_10x10_png));
        distortionCheckbox->setFixedSize(10, 10);
        auto label = new RkLabel(this);
        label->show();
        label->setBackgroundColor(background());
        label->setImage(RkImage(50, 9, rk_distortion_goupbox_label_png));
        label->setFixedSize(50, 9);
        label->setPosition((width() - label->width()) / 2, 0);
        distortionCheckbox->setPosition(label->x() - 14, 0);

        int sliderW = 60;
        int sliderH = 12;
        int yoffset = -2;
        int labelD  = 5;

        // Volume
        volumeSlider = new GeonkickSlider(this);
        volumeSlider->setFixedSize(sliderW, sliderH);
        volumeSlider->setPosition(width() / 2 + labelD / 2 - 12, yoffset + (height() - sliderH) / 2);
        volumeSlider->onSetValue(50);
        RK_ACT_BIND(volumeSlider, valueUpdated, RK_ACT_ARGS(int val), this, setVolume(val));
        auto volumeLabel = new RkLabel(this);
        volumeLabel->show();
        volumeLabel->setImage(RkImage(38, 8, rk_distortion_volume_label_png));
        volumeLabel->setFixedSize(38, 8);
        volumeLabel->setPosition(volumeSlider->x() - volumeLabel->width() - labelD, volumeSlider->y());

        // Drive
        driveSlider = new GeonkickSlider(this);
        driveSlider->setFixedSize(sliderW, sliderH);
        driveSlider->setPosition(width() / 2 + labelD / 2 - 12, yoffset + (height() - sliderH) / 2 + sliderH + 6);
        RK_ACT_BIND(driveSlider, valueUpdated, RK_ACT_ARGS(int val), this, setDrive(val));
        auto driveLabel = new RkLabel(this);
        driveLabel->show();
        driveLabel->setImage(RkImage(24, 8, rk_distortion_drive_label_png));
        driveLabel->setFixedSize(24, 8);
        driveLabel->setPosition(driveSlider->x() - driveLabel->width() - labelD, driveSlider->y());
        show();
}

DistortionGroupBox::~DistortionGroupBox()
{
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
        double volume = geonkickApi->getDistortionVolume();
        double logVal;
        if (volume > 0)
                logVal = 20 * log10(volume);
        else
                logVal = 60;
        volumeSlider->onSetValue(100 * (60 - fabs(logVal)) / 60);
        auto distortion = geonkickApi->getDistortionDrive();
        if (distortion < std::numeric_limits<decltype(distortion)>::min())
                distortion = 0;
        else
                distortion = 20 * log10(distortion);
        driveSlider->onSetValue(100 * distortion / 36);
}
