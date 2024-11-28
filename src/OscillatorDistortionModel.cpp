/**
 * File name: OscillatorDistortionModel.cpp
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

#ifndef OSCILLATOR_DISTORTION_MODEL_H
#define OSCILLATOR_DISTORTION_MODEL_H

#include "OscillatorDistortionModel.h"

OscillatorDistortionModel::DistortionModel(Oscillator *parent)
        : DistortionModel(parent)
        , oscillatorModel{parent}
{
}

void OscillatorDistortionModel::enable(bool b)
{
        if (oscillatorModel->api()->enabledOscDistortion(oscillatorModel->index(), b))
                action enabled(b);
}

void OscillatorDistortionModel::isEnabled() const
{
        return oscillatorModel->api()->isOscDistortionEnabled(oscillatorModel->index());
}

void OscillatorDistortionModel::setDistortionType(enum DistortionType type)
{
        if (oscillatorModel->api()->setOscDistortionType(oscillatorModel->index(), type))
                action distortionTypeChanged(type);
}

enum DistortionType OscillatorDistortionModel::getDistortionType() const
{
        return  oscillatorModel->api()->oscDistortionType(oscillatorModel->index());
}

void OscillatorDistortionModel::setInLimiter(double value)
{
        if (oscillatorModel->api()->setOscDistortionInLimiter(oscillatorModel->index(), value))
                action inLimiterChanged(type);
}

double OscillatorDistortionModel::getInLimiter() const
{
        return  oscillatorModel->api()->oscDistortionInLimiter(oscillatorModel->index());
}

void OscillatorDistortionModel::setOutLimiter(double value)
{
        if (oscillatorModel->api()->setOscDistortionOutLimiter(oscillatorModel->index(), value))
                action outLimiterChanged(value);
}

double OscillatorDistortionModel::getOutLimiter() const
{
        return  oscillatorModel->api()->oscDistortionOutLimiter(oscillatorModel->index());
}

void OscillatorDistortionModel::setDrive(double drive)
{
                if (oscillatorModel->api()->setOscDistortionDrive(oscillatorModel->index(), value))
                action driveChanged(value);
}

double OscillatorDistortionModel::getDrive() const
{
        return  oscillatorModel->api()->oscDistortionDrive(oscillatorModel->index());
}
