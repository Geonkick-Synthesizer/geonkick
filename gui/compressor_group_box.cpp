/**
 * File name: compressor_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 20112 Iurie Nistor (http://geontime.com)
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

#include "compressor_group_box.h"
#include "geonkick_slider.h"
#include "geonkick_label.h"
#include "geonkick_checkbox.h"

#include <QGridLayout>

CompressorGroupBox::CompressorGroupBox(GeonkickWidget *parent)
        : GeonkickGroupBox(parent),
          attackSlider(nullptr),
          releaseSlider(nullptr),
          thresholdSlider(nullptr),
          ratioSlider(nullptr),
          kneeSlider(nullptr),
          makeupSlider(nullptr),
          compressorCheckbox(new GeonkickCheckbox(this))
{
        compressorCheckbox->setCheckedImage("./themes/geontime/checkbox_checked_10x10.png");
        compressorCheckbox->setUncheckedImage("./themes/geontime/checkbox_unchecked_10x10.png");
        compressorCheckbox->setCheckboxLabelImage("./themes/geontime/compressor_groupbox_label.png");
        setGroupBoxLabel(compressorCheckbox);

        auto widget = new GeonkickWidget(this);
        addWidget(widget);
        auto gridLayout = new QGridLayout(widget);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(gridLayout);

        // Attack
        auto attackLabel = new GeonkickLabel(widget);
        attackLabel->setImage("./themes/geontime/compressor_attack_label.png");
        gridLayout->addWidget(attackLabel, 0, 0, Qt::AlignRight);
        attackSlider = new GeonkickSlider(widget);
        attackSlider->setFixedSize(60, 12);
        attackSlider->setValue(50);
        gridLayout->addWidget(attackSlider, 0, 1, Qt::AlignLeft);

        // Relsease
        auto releaseLabel = new GeonkickLabel(widget);
        releaseLabel->setImage("./themes/geontime/compressor_release_label.png");
        gridLayout->addWidget(releaseLabel, 1, 0, Qt::AlignRight);
        releaseSlider = new GeonkickSlider(widget);
        releaseSlider->setFixedSize(60, 12);
        gridLayout->addWidget(releaseSlider, 1, 1, Qt::AlignLeft);

        // Threshold
        auto thresholdLabel = new GeonkickLabel(widget);
        thresholdLabel->setImage("./themes/geontime/compressor_threshold_label.png");
        gridLayout->addWidget(thresholdLabel, 2, 0, Qt::AlignRight);
        thresholdSlider = new GeonkickSlider(widget);
        thresholdSlider->setFixedSize(60, 12);
        gridLayout->addWidget(thresholdSlider, 2, 1, Qt::AlignLeft);

        gridLayout->setColumnMinimumWidth(2, 10);

        // Ratio
        auto ratioLabel = new GeonkickLabel(widget);
        ratioLabel->setImage("./themes/geontime/compressor_ratio_label.png");
        gridLayout->addWidget(ratioLabel, 0, 3, Qt::AlignRight);
        ratioSlider = new GeonkickSlider(widget);
        ratioSlider->setFixedSize(60, 12);
        gridLayout->addWidget(ratioSlider, 0, 4, Qt::AlignLeft);

        // Knee
        auto kneeLabel = new GeonkickLabel(widget);
        kneeLabel->setImage("./themes/geontime/compressor_knee_label.png");
        gridLayout->addWidget(kneeLabel, 1, 3, Qt::AlignRight);
        kneeSlider = new GeonkickSlider(widget);
        kneeSlider->setFixedSize(60, 12);
        gridLayout->addWidget(kneeSlider, 1, 4, Qt::AlignLeft);

        // Makeup
        auto makeupLabel = new GeonkickLabel(widget);
        makeupLabel->setImage("./themes/geontime/compressor_makeup_label.png");
        gridLayout->addWidget(makeupLabel, 2, 3, Qt::AlignRight);
        makeupSlider = new GeonkickSlider(widget);
        makeupSlider->setFixedSize(60, 12);
        gridLayout->addWidget(makeupSlider, 2, 4, Qt::AlignLeft);
}

CompressorGroupBox::~CompressorGroupBox()
{
}
