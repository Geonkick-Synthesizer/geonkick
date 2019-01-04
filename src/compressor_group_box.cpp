/**
 * File name: compressor_group_box.cpp
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

#include "compressor_group_box.h"
#include "geonkick_slider.h"
#include "geonkick_label.h"
#include "geonkick_checkbox.h"
#include "geonkick_api.h"

#include <QGridLayout>

CompressorGroupBox::CompressorGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent),
          geonkickApi(api),
          attackSlider(nullptr),
          thresholdSlider(nullptr),
          ratioSlider(nullptr),
          makeupSlider(nullptr),
          compressorCheckbox(new GeonkickCheckbox(this))
{
        compressorCheckbox->setCheckedImage(":/checkbox_checked_10x10.png");
        compressorCheckbox->setUncheckedImage(":/checkbox_unchecked_10x10.png");
        compressorCheckbox->setCheckboxLabelImage(":/compressor_groupbox_label.png");
        connect(compressorCheckbox, SIGNAL(stateUpdated(bool)), geonkickApi, SLOT(enableCompressor(bool)));
        setGroupBoxLabel(compressorCheckbox);

        auto widget = new GeonkickWidget(this);
        addWidget(widget);
        auto gridLayout = new QGridLayout(widget);
        widget->setLayout(gridLayout);

        // Attack
        auto attackLabel = new GeonkickLabel(widget);
        attackLabel->setImage(":/compressor_attack_label.png");
        gridLayout->addWidget(attackLabel, 0, 0, Qt::AlignRight);
        attackSlider = new GeonkickSlider(widget);
        connect(attackSlider, SIGNAL(valueUpdated(int)), this, SLOT(setAttack(int)));
        attackSlider->setFixedSize(60, 12);
        attackSlider->setValue(50);
        gridLayout->addWidget(attackSlider, 0, 1, Qt::AlignLeft);

        // Threshold
        auto thresholdLabel = new GeonkickLabel(widget);
        thresholdLabel->setImage(":/compressor_threshold_label.png");
        gridLayout->addWidget(thresholdLabel, 1, 0, Qt::AlignRight);
        thresholdSlider = new GeonkickSlider(widget);
        connect(thresholdSlider, SIGNAL(valueUpdated(int)), this, SLOT(setThreshold(int)));
        thresholdSlider->setFixedSize(60, 12);
        gridLayout->addWidget(thresholdSlider, 1, 1, Qt::AlignLeft);
        gridLayout->setColumnMinimumWidth(2, 10);

        // Ratio
        auto ratioLabel = new GeonkickLabel(widget);
        ratioLabel->setImage(":/compressor_ratio_label.png");
        gridLayout->addWidget(ratioLabel, 0, 3, Qt::AlignRight);
        ratioSlider = new GeonkickSlider(widget);
        connect(ratioSlider, SIGNAL(valueUpdated(int)), this, SLOT(setRatio(int)));
        ratioSlider->setFixedSize(60, 12);
        gridLayout->addWidget(ratioSlider, 0, 4, Qt::AlignLeft);

        // Makeup
        auto makeupLabel = new GeonkickLabel(widget);
        makeupLabel->setImage(":/compressor_makeup_label.png");
        gridLayout->addWidget(makeupLabel, 1, 3, Qt::AlignRight);
        makeupSlider = new GeonkickSlider(widget);
        connect(makeupSlider, SIGNAL(valueUpdated(int)), this, SLOT(setMakeup(int)));
        makeupSlider->setFixedSize(60, 12);
        gridLayout->addWidget(makeupSlider, 1, 4, Qt::AlignLeft);
        update();
}

void CompressorGroupBox::setAttack(int val)
{
        if (val == 0)
                geonkickApi->setCompressorAttack(0);
        else
                geonkickApi->setCompressorAttack(pow(10, (static_cast<double>(val) / 100) * log10(2000)) / 1000);
}

void CompressorGroupBox::setThreshold(int val)
{
        geonkickApi->setCompressorThreshold(-60 * (1 - (static_cast<double>(val) / 100)));
}

void CompressorGroupBox::setRatio(int val)
{
        geonkickApi->setCompressorRatio(20 * static_cast<double>(val) / 100);
}

void CompressorGroupBox::setMakeup(int val)
{
        geonkickApi->setCompressorMakeup(36 * (static_cast<double>(val) / 100));
}

void CompressorGroupBox::update()
{
        compressorCheckbox->setChecked(geonkickApi->isCompressorEnabled());
        attackSlider->setValue(100 * (log10(1000 * geonkickApi->getCompressorAttack()) / log10(2000)));
        thresholdSlider->setValue(100 * (1 - geonkickApi->getCompressorThreshold() / (-60)));
        ratioSlider->setValue(100 * geonkickApi->getCompressorRatio() / 20);
        makeupSlider->setValue(100 * geonkickApi->getCompressorMakeup() / 36);
}
