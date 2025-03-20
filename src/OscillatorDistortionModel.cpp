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

#include "OscillatorDistortionModel.h"
#include "OscillatorModel.h"

OscillatorDistortionModel::OscillatorDistortionModel(OscillatorModel *parent)
        : DistortionModel(parent)
        , oscillatorModel{parent}
{
}

void OscillatorDistortionModel::enable(bool b)
{
        if (oscillatorModel->api()->oscEnableDistortion(oscillatorModel->index(), b))
                action enabled(b);
}

bool OscillatorDistortionModel::isEnabled() const
{
        return oscillatorModel->api()->isOscDistortionEnabled(oscillatorModel->index());
}

void OscillatorDistortionModel::setDistortionType(DistortionType type)
{
        if (oscillatorModel->api()->setOscDistortionType(oscillatorModel->index(), type))
                action distortionTypeChanged(type);
}

DistortionModel::DistortionType
OscillatorDistortionModel::getDistortionType() const
{
        return  oscillatorModel->api()->getOscDistortionType(oscillatorModel->index());
}

void OscillatorDistortionModel::setInLimiter(double value)
{
        if (oscillatorModel->api()->setOscDistortionInLimiter(oscillatorModel->index(), value))
                action inLimiterChanged(value);
}

double OscillatorDistortionModel::getInLimiter() const
{
        return  oscillatorModel->api()->getOscDistortionInLimiter(oscillatorModel->index());
}

void OscillatorDistortionModel::setOutLimiter(double value)
{
        if (oscillatorModel->api()->setOscDistortionOutLimiter(oscillatorModel->index(), value))
                action outLimiterChanged(value);
}

double OscillatorDistortionModel::getOutLimiter() const
{
        return  oscillatorModel->api()->getOscDistortionOutLimiter(oscillatorModel->index());
}

void OscillatorDistortionModel::setDrive(double drive)
{
                if (oscillatorModel->api()->setOscDistortionDrive(oscillatorModel->index(), drive))
                action driveChanged(drive);
}

double OscillatorDistortionModel::getDrive() const
{
        return  oscillatorModel->api()->getOscDistortionDrive(oscillatorModel->index());
}

Envelope::Category OscillatorDistortionModel::envelopeCategory() const
{
        switch(oscillatorModel->type()) {
        case OscillatorModel::Type::Oscillator1:
                return Envelope::Category::Oscillator1;
                break;
        case OscillatorModel::Type::Oscillator2:
                return Envelope::Category::Oscillator2;
                break;
        case OscillatorModel::Type::Oscillator3:
                return Envelope::Category::Oscillator3;
                break;
        default:
                return Envelope::Category::Oscillator1;
        }
}
