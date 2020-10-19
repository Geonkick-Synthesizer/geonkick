/**
 * File name: compressor_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://iuriepage.wordpress.com>
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
#include "geonkick_button.h"
#include "geonkick_api.h"

#include <RkContainer.h>
#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(compressor_enable);
RK_DECLARE_IMAGE_RC(compressor_enable_hover);
RK_DECLARE_IMAGE_RC(compressor_enable_active);
RK_DECLARE_IMAGE_RC(compressor_attack_label);
RK_DECLARE_IMAGE_RC(compressor_threshold_label);
RK_DECLARE_IMAGE_RC(compressor_ratio_label);
RK_DECLARE_IMAGE_RC(compressor_makeup_label);

CompressorGroupBox::CompressorGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , attackSlider{nullptr}
        , thresholdSlider{nullptr}
        , ratioSlider{nullptr}
        , makeupSlider{nullptr}
        , compressorCheckbox{nullptr}
{
        setFixedSize(224, 63);
        compressorCheckbox = new GeonkickButton(this);
        RK_ACT_BIND(compressorCheckbox,  toggled, RK_ACT_ARGS(bool b), geonkickApi, enableCompressor(b));
        compressorCheckbox->setCheckable(true);
	compressorCheckbox->setFixedSize(78, 21);
        compressorCheckbox->setImage(RkImage(compressorCheckbox->size(), RK_IMAGE_RC(compressor_enable)),
                                     RkButton::ButtonImage::ImageUnpressed);
        compressorCheckbox->setImage(RkImage(compressorCheckbox->size(), RK_IMAGE_RC(compressor_enable_active)),
                                     RkButton::ButtonImage::ImagePressed);
        compressorCheckbox->setImage(RkImage(compressorCheckbox->size(), RK_IMAGE_RC(compressor_enable_hover)),
                                     RkButton::ButtonImage::ImageUnpressedHover);
        compressorCheckbox->setImage(RkImage(compressorCheckbox->size(), RK_IMAGE_RC(compressor_enable_hover)),
                                     RkButton::ButtonImage::ImagePressedHover);
	compressorCheckbox->setPosition((width() - compressorCheckbox->width()) / 2, 0);

        int sliderW = 56;
        int sliderH = 14;
        auto vContainer = new RkContainer(this, Rk::Orientation::Vertical);
        vContainer->setSize(size());
        vContainer->addSpace(32);

        auto hContainer = new RkContainer(this);
        hContainer->setSize({vContainer->width(), sliderH});
        auto attackLabel = new RkLabel(this);
        attackLabel->setBackgroundColor(background());
        attackLabel->setFixedSize(53, 14);
        attackLabel->setImage(RkImage(attackLabel->size(), RK_IMAGE_RC(compressor_attack_label)));
        attackLabel->show();
        hContainer->addWidget(attackLabel);
        attackSlider = new GeonkickSlider(this);
        attackSlider->setFixedSize(sliderW, 12);
        attackSlider->onSetValue(50);
        RK_ACT_BIND(attackSlider, valueUpdated, RK_ACT_ARGS(int val), this, setAttack(val));
        hContainer->addSpace(3);
        hContainer->addWidget(attackSlider);

        auto ratioLabel = new RkLabel(this);
        ratioLabel->setBackgroundColor(background());
        ratioLabel->setFixedSize(53, 14);
        ratioLabel->setImage(RkImage(ratioLabel->size(), RK_IMAGE_RC(compressor_ratio_label)));
        ratioLabel->show();
        hContainer->addWidget(ratioLabel);
        ratioSlider = new GeonkickSlider(this);
        ratioSlider->setFixedSize(sliderW, 12);
        RK_ACT_BIND(ratioSlider, valueUpdated, RK_ACT_ARGS(int value), this, setRatio(value));
        hContainer->addWidget(ratioSlider);
        vContainer->addContainer(hContainer);
        vContainer->addSpace(3);

        hContainer = new RkContainer(this);
        hContainer->setSize({vContainer->width(), sliderH});
        auto thresholdLabel = new RkLabel(this);
        thresholdLabel->setBackgroundColor(background());
        thresholdLabel->setFixedSize(53, 14);
        thresholdLabel->setImage(RkImage(thresholdLabel->size(), RK_IMAGE_RC(compressor_threshold_label)));
        thresholdLabel->show();
        hContainer->addWidget(thresholdLabel);
        thresholdSlider = new GeonkickSlider(this);
        thresholdSlider->setFixedSize(sliderW, 12);
        RK_ACT_BIND(thresholdSlider, valueUpdated, RK_ACT_ARGS(int value), this, setThreshold(value));
        hContainer->addSpace(3);
        hContainer->addWidget(thresholdSlider);

        auto makeupLabel = new RkLabel(this);
        makeupLabel->setFixedSize(53, 14);
        makeupLabel->setBackgroundColor(background());
        makeupLabel->setImage(RkImage(makeupLabel->size(), RK_IMAGE_RC(compressor_makeup_label)));
        makeupLabel->show();
        hContainer->addWidget(makeupLabel);
        makeupSlider = new GeonkickSlider(this);
        makeupSlider->setFixedSize(sliderW, 12);
        RK_ACT_BIND(makeupSlider, valueUpdated, RK_ACT_ARGS(int value), this, setMakeup(value));
        hContainer->addWidget(makeupSlider);
        vContainer->addContainer(hContainer);
        vContainer->update();
        show();
        updateGui();
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
        double db = -60.0 * static_cast<double>(100 - val) / 100;
        geonkickApi->setCompressorThreshold(pow(10, db / 20));
}

void CompressorGroupBox::setRatio(int val)
{
        double v = log2(19) * static_cast<double>(val) / 100;
        geonkickApi->setCompressorRatio(pow(2, v));
}

void CompressorGroupBox::setMakeup(int val)
{
        double db = 36.0 * static_cast<double>(val) / 100;
        geonkickApi->setCompressorMakeup(pow(10, db / 20));
}

void CompressorGroupBox::updateGui()
{
        compressorCheckbox->setPressed(geonkickApi->isCompressorEnabled());

        // Attack
        attackSlider->onSetValue(100 * (log10(1000 * geonkickApi->getCompressorAttack()) / log10(2000)));

        // Threshold
        auto threshold = geonkickApi->getCompressorThreshold();
        double db = 0;
        if (threshold < std::numeric_limits<decltype(geonkickApi->getCompressorThreshold())>::min())
                db = -60;
        else
                db = 20.0 * log10(threshold);
        thresholdSlider->onSetValue(100 - 100 * (db / -60));

        // Ratio
        double ratio = geonkickApi->getCompressorRatio();
        if (ratio < 1.0)
                ratio = 0;
        else
                ratio = log2(ratio);
        ratioSlider->onSetValue(100 * ratio / log2(19));

        // Makeup
        double makeup = geonkickApi->getCompressorMakeup();
        if (makeup < 1.0)
                makeup = 1.0;
        db = 20 * log10(makeup);
        makeupSlider->onSetValue(100 * (db / 36));
}
