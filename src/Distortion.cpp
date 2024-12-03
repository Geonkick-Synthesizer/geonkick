/**
 * File name: Distortion.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#include "Distortion.h"

Distortion::Distortion(GeonkickWidget* parent, DistortionModel *model)
        : GeonkickWidget(parent)
        , distortionModel{model}
{
        createView();
        bindModel();
}

void Distortion::setdModel(DistortionModel *model)
{
        unbindObject(distortionModel);
        distortionModel = model;
        bindModel();
        updateView();
}

void Distortion::binModel()
{
        RK_ACT_BIND(distortionModel,
                    distortionTypeChanged,
                    RK_ACT_ARGS(enum DistortionType type),
                    this,
                    setDistortionType(type));
        RK_ACT_BIND(distortionModel,
                    inLimiterChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setInElimiter(value));
        RK_ACT_BIND(distortionModel,
                    outLimiterChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setOutElimiter(value));
        RK_ACT_BIND(distortionModel,
                    driveChanged,
                    RK_ACT_ARGS(double value),
                    this
                    setDrive(value));
}

void Distortion::createView()
{
        updateView();
}

void Distortion::updateView()
{
}

