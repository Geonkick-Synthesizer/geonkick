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

#include <QGridLayout>

DistortionGroupBox::DistortionGroupBox(GeonkickWidget *parent)
        : GeonkickGroupBox(parent),
          volumeSlider(nullptr),
          driveSlider(nullptr)
{
        auto label = new GeonkickLabel(this);
        label->setImage("./themes/geontime/distortion_groupbox_label.png");
        setGroupBoxLabel(label);

        auto widget = new GeonkickWidget(this);
        addWidget(widget);
        auto gridLayout = new QGridLayout(widget);
        widget->setLayout(gridLayout);

        // Volume
        auto volumeLabel = new GeonkickLabel(widget);
        volumeLabel->setImage("./themes/geontime/distortion_volume_label.png");
        gridLayout->addWidget(volumeLabel, 0, 0, Qt::AlignRight);
        volumeSlider = new GeonkickSlider(widget);
        volumeSlider->setFixedSize(60, 12);
        volumeSlider->setValue(50);
        gridLayout->addWidget(volumeSlider, 0, 1, Qt::AlignLeft);

        // Drive
        auto driveLabel = new GeonkickLabel(widget);
        driveLabel->setImage("./themes/geontime/distortion_drive_label.png");
        gridLayout->addWidget(driveLabel, 1, 0, Qt::AlignRight);
        driveSlider = new GeonkickSlider(widget);
        driveSlider->setFixedSize(60, 12);
        gridLayout->addWidget(driveSlider, 1, 1, Qt::AlignLeft);
}

DistortionGroupBox::~DistortionGroupBox()
{
}
