/**
 * File name: effects_group_box.cpp
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

#include "effects_group_box.h"
#include "distortion_group_box.h"
#include "layers_group_box.h"
#include "geonkick_api.h"

#include <RkLabel.h>

EffectsGroupBox::EffectsGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent, Orientation::Horizontal)
        , geonkickApi(api)
        , distortionGroupBox{nullptr}
{
        distortionGroupBox = new DistortionGroupBox(geonkickApi, this);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), distortionGroupBox, updateGui());
#ifndef GEONKICK_LIMITED_VERSION
        auto layers = new LayersGroupBox(geonkickApi, this);
        layers->setBackgroundColor({100, 100, 100});
        layers->setPosition(distortionGroupBox->x() + distortionGroupBox->width(), 4);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), layers, updateGui());
#endif // GEONKICK_LIMITED_VERSION
        RkString geonkickInfo{"Standalone"};
        if (geonkickApi->getInstanceType() == GeonkickApi::InstanceType::Lv2)
                geonkickInfo = "LV2";
        else if (geonkickApi->getInstanceType() == GeonkickApi::InstanceType::Vst3)
                geonkickInfo = "VST3";
        geonkickInfo += std::string(" ") + Geonkick::applicationVersionStr;
}


