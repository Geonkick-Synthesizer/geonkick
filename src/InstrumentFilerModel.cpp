/**
 * File name: InstrumentFilterModel.cpp
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

#include "InstrumentFilterModel.h"
#include "PercussionModel.h"

InstrumentFilterModel::InstrumentFilterModel(PercussionModel *parent)
: DistortionModel(parent)
,  percussionModel{parent}
{
}

void InstrumentFilterModel::enable(bool b)
{
        if (percussionModel->parentModel()->enableDistortion(percussionModel->index(), b))
                action enabled(b);
}

bool InstrumentFilterModel::isEnabled() const
{
        return percussionModel->isOscDistortionEnabled(percussionModel->index());
}

void InstrumentFilterModel::setDistortionType(DistortionType type)
{
        if (percussionModel->api()->setOscDistortionType(percussionModel->index(), type))
                action distortionTypeChanged(type);
}

DistortionModel::DistortionType
InstrumentFilterModel::getDistortionType() const
{
        return  percussionModel->api()->getOscDistortionType(percussionModel->index());
}

void InstrumentFilterModel::setInLimiter(double value)
{
        if (percussionModel->api()->setOscDistortionInLimiter(percussionModel->index(), value))
                action inLimiterChanged(value);
}

double InstrumentFilterModel::getInLimiter() const
{
        return  percussionModel->api()->getOscDistortionInLimiter(percussionModel->index());
}

void InstrumentFilterModel::setOutLimiter(double value)
{
        if (percussionModel->api()->setOscDistortionOutLimiter(percussionModel->index(), value))
                action outLimiterChanged(value);
}

double InstrumentFilterModel::getOutLimiter() const
{
        return  percussionModel->api()->getOscDistortionOutLimiter(percussionModel->index());
}

void InstrumentFilterModel::setDrive(double drive)
{
                if (percussionModel->api()->setOscDistortionDrive(percussionModel->index(), drive))
                action driveChanged(drive);
}

double InstrumentFilterModel::getDrive() const
{
        return  percussionModel->api()->getOscDistortionDrive(percussionModel->index());
}
