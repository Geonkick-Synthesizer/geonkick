/**
 * File name: effects_group_box.cpp
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

#include "effects_group_box.h"
#include "compressor_group_box.h"
#include "distortion_group_box.h"
#include "geonkick_api.h"

EffectsGroupBox::EffectsGroupBox(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickGroupBox(parent, Orientation::Horizontal)
        , geonkickApi(api)
{
        auto compressor = new CompressorGroupBox(geonkickApi, this);
        compressor->setPosition(0, 10);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), compressor, updateGui());
        auto distortion = new DistortionGroupBox(geonkickApi, this);
        distortion->setPosition(compressor->width() + 10, 10);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), distortion, updateGui());
}

EffectsGroupBox::~EffectsGroupBox()
{
}

