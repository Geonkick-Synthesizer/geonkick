/**
 * File name: layers_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor 
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

#include "layers_group_box.h"
#include "geonkick_slider.h"
#include "geonkick_button.h"
#include "geonkick_api.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(layers_mixer);

LayersGroupBox::LayersGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent)
        , geonkickApi{api}
        , layerSliders{nullptr, nullptr, nullptr}
{
        setFixedSize(110, 65);
        setBackgroundImage(RkImage(size(), RK_IMAGE_RC(layers_mixer)));

        int y = 23;
        for (auto i = 0; i < 3; i++) {
                layerSliders[i] = new GeonkickSlider(this);
                layerSliders[i]->setFixedSize(width() - 38, 10);
                layerSliders[i]->setPosition(18, y);
                y += layerSliders[i]->height() + 6;
                layerSliders[i]->show();
                RK_ACT_BIND(layerSliders[i],
                            valueUpdated,
                            RK_ACT_ARGS(int val),
                            this,
                            setLayerAmplitude(i, val));
        }
        show();
        updateGui();
}

void LayersGroupBox::setLayerAmplitude(int layer, int val)
{
        double logVal = -60 * (1.0 - (static_cast<double>(val) / 100));
        double amplitude = pow(10, logVal / 20);
        geonkickApi->setLayerAmplitude(static_cast<GeonkickApi::Layer>(layer), amplitude);
}

void LayersGroupBox::updateGui()
{
        for (auto i = 0; i < 3; i++) {
                double amplitude = geonkickApi->getLayerAmplitude(static_cast<GeonkickApi::Layer>(i));
                double logVal;
                if (amplitude > 0)
                        logVal = 20 * log10(amplitude);
                else
                        logVal = 60;
                layerSliders[i]->onSetValue(100 * (60 - fabs(logVal)) / 60, 100);
        }
}
