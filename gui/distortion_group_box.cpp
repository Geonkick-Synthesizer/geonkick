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
#include "geonkick_label.h"
#include "geonkick_checkbox.h"
#include "geonkick_api.h"

#include <QGridLayout>

DistortionGroupBox::DistortionGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent),
          geonkickApi(api),
          volumeSlider(nullptr),
          driveSlider(nullptr),
          distortionCheckbox(new GeonkickCheckbox(this))
{
        distortionCheckbox->setCheckedImage("./themes/geontime/checkbox_checked_10x10.png");
        distortionCheckbox->setUncheckedImage("./themes/geontime/checkbox_unchecked_10x10.png");
        distortionCheckbox->setCheckboxLabelImage("./themes/geontime/distortion_groupbox_label.png");
        connect(distortionCheckbox, SIGNAL(stateUpdated(bool)), geonkickApi, SLOT(enableDistortion(bool)));
        setGroupBoxLabel(distortionCheckbox);

        auto widget = new GeonkickWidget(this);
        addWidget(widget);
        auto gridLayout = new QGridLayout(widget);
        widget->setLayout(gridLayout);

        // Volume
        auto volumeLabel = new GeonkickLabel(widget);
        volumeLabel->setImage("./themes/geontime/distortion_volume_label.png");
        gridLayout->addWidget(volumeLabel, 0, 0, Qt::AlignRight);
        volumeSlider = new GeonkickSlider(widget);
        connect(volumeSlider, SIGNAL(valueUpdated(int)), this, SLOT(setVolume(int)));
        volumeSlider->setFixedSize(60, 12);
        volumeSlider->setValue(50);
        gridLayout->addWidget(volumeSlider, 0, 1, Qt::AlignLeft);

        // Drive
        auto driveLabel = new GeonkickLabel(widget);
        driveLabel->setImage("./themes/geontime/distortion_drive_label.png");
        gridLayout->addWidget(driveLabel, 1, 0, Qt::AlignRight);
        driveSlider = new GeonkickSlider(widget);
        connect(driveSlider, SIGNAL(valueUpdated(int)), this, SLOT(setDrive(int)));
        driveSlider->setFixedSize(60, 12);
        gridLayout->addWidget(driveSlider, 1, 1, Qt::AlignLeft);
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
        geonkickApi->setDistortionDrive(val);
}

void DistortionGroupBox::update()
{
        distortionCheckbox->setChecked(geonkickApi->isDistortionEnabled());
        double volume = geonkickApi->getDistortionVolume();
        double logVal;
        if (volume > 0)
                logVal = 20 * log10(volume);
        else
                logVal = 60;
        volumeSlider->setValue(100 * (60 - fabs(logVal)) / 60);
        driveSlider->setValue(geonkickApi->getDistortionDrive());
}
