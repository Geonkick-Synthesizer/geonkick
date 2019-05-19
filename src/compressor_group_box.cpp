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
#include "geonkick_button.h"
#include "geonkick_api.h"

#include <RkLabel.h>

extern const unsigned char rk_compressor_groupbox_label_png[];
extern const unsigned char rk_checkbox_checked_10x10_png[];
extern const unsigned char rk_checkbox_unchecked_10x10_png[];
extern const unsigned char rk_compressor_attack_label_png[];
extern const unsigned char rk_compressor_threshold_label_png[];
extern const unsigned char rk_compressor_ratio_label_png[];
extern const unsigned char rk_compressor_makeup_label_png[];

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
        compressorCheckbox->setPressedImage(RkImage(10, 10, rk_checkbox_checked_10x10_png));
        compressorCheckbox->setUnpressedImage(RkImage(10, 10, rk_checkbox_unchecked_10x10_png));
        compressorCheckbox->setFixedSize(10, 10);
        auto label = new RkLabel(this);
        label->show();
        label->setBackgroundColor(background());
        label->setFixedSize(65, 11);
        label->setImage(RkImage(65, 11, rk_compressor_groupbox_label_png));
        label->setPosition((width() - label->width()) / 2, 0);
        compressorCheckbox->setPosition(label->x() - 14, 0);

        int sliderW = 60;
        int sliderH = 12;
        int yoffset = 10;
        int labelD  = 5;

        attackSlider = new GeonkickSlider(this);
        attackSlider->setFixedSize(60, 12);
        attackSlider->onSetValue(50);
        attackSlider->setPosition((width() - 2 * sliderW) / 2 , yoffset + (height() - 2 * sliderH) / 3);
        RK_ACT_BIND(attackSlider, valueUpdated, RK_ACT_ARGS(int val), this, setAttack(val));
        auto attackLabel = new RkLabel(this);
        attackLabel->show();
        attackLabel->setImage(RkImage(33, 8, rk_compressor_attack_label_png));
        attackLabel->setFixedSize(33, 8);
        attackLabel->setPosition(attackSlider->x() - attackLabel->width() - labelD, attackSlider->y());

        thresholdSlider = new GeonkickSlider(this);
        thresholdSlider->setFixedSize(60, 12);
        thresholdSlider->setPosition((width() - 2 * sliderW) / 2 , yoffset + (height() - 2 * sliderH) / 3 + sliderH + 6);
        RK_ACT_BIND(thresholdSlider, valueUpdated, RK_ACT_ARGS(int value), this, setThreshold(value));
        auto thresholdLabel = new RkLabel(this);
        thresholdLabel->show();
        thresholdLabel->setImage(RkImage(45, 8, rk_compressor_threshold_label_png));
        thresholdLabel->setFixedSize(45, 8);
        thresholdLabel->setPosition(thresholdSlider->x() - thresholdLabel->width() - labelD, thresholdSlider->y());

        ratioSlider = new GeonkickSlider(this);
        ratioSlider->setFixedSize(60, 12);
        ratioSlider->setPosition((width() - 2 * sliderW) + sliderW, yoffset + (height() - 2 * sliderH) / 3);
        RK_ACT_BIND(ratioSlider, valueUpdated, RK_ACT_ARGS(int value), this, setRatio(value));
        auto ratioLabel = new RkLabel(this);
        ratioLabel->show();
        ratioLabel->setImage(RkImage(24, 8, rk_compressor_ratio_label_png));
        ratioLabel->setFixedSize(24, 8);
        ratioLabel->setPosition(ratioSlider->x() - ratioLabel->width() - labelD, ratioSlider->y());

        makeupSlider = new GeonkickSlider(this);
        makeupSlider->setFixedSize(60, 12);
        makeupSlider->setPosition((width() - 2 * sliderW) + sliderW, yoffset + (height() - 2 * sliderH) / 3 + sliderH + 6);
        RK_ACT_BIND(makeupSlider, valueUpdated, RK_ACT_ARGS(int value), this, setMakeup(value));
        auto makeupLabel = new RkLabel(this);
        makeupLabel->show();
        makeupLabel->setImage(RkImage(40, 10, rk_compressor_makeup_label_png));
        makeupLabel->setFixedSize(40, 10);
        makeupLabel->setPosition(makeupSlider->x() - makeupLabel->width() - labelD, makeupSlider->y());
        show();
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

void CompressorGroupBox::updateGui()
{
        compressorCheckbox->setPressed(geonkickApi->isCompressorEnabled());
        attackSlider->onSetValue(100 * (log10(1000 * geonkickApi->getCompressorAttack()) / log10(2000)));
        thresholdSlider->onSetValue(100 * (1 - geonkickApi->getCompressorThreshold() / (-60)));
        ratioSlider->onSetValue(100 * geonkickApi->getCompressorRatio() / 20);
        makeupSlider->onSetValue(100 * geonkickApi->getCompressorMakeup() / 36);
}
