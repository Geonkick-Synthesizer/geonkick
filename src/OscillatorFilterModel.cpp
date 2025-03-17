/**
 * File name: OscillatorFilterModel.cpp
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

#include "OscillatorFilterModel.h"
#include "OscillatorModel.h"

OscillatorFilterModel::OscillatorFilterModel(OscillatorModel *parent)
        : FilterModel(parent)
        , oscillatorModel{parent}
{
}

void OscillatorFilterModel::enable(bool b)
{
        if (oscillatorModel->api()->enableOscillatorFilter(oscillatorModel->index(), b))
                action enabled(b);
}

bool OscillatorFilterModel::isEnabled() const
{
        return oscillatorModel->api()->isOscillatorFilterEnabled(oscillatorModel->index());
}

void OscillatorFilterModel::setCutOff(double val)
{
        if (oscillatorModel->api()->setOscillatorFilterCutOffFreq(oscillatorModel->index(), val))
                action cutOffChanged(val);
}

double OscillatorFilterModel::cutOff() const
{
        return oscillatorModel->api()->getOscillatorFilterCutOffFreq(oscillatorModel->index());
}

void OscillatorFilterModel::setResonance(double val)
{
        if (oscillatorModel->api()->setOscillatorFilterFactor(oscillatorModel->index(), val))
                action resonanceChanged(val);
}

double OscillatorFilterModel::resonance() const
{
        return oscillatorModel->api()->getOscillatorFilterFactor(oscillatorModel->index());
}

void OscillatorFilterModel::setType(FilterModel::FilterType type)
{
        if (oscillatorModel->api()->setOscillatorFilterType(oscillatorModel->index(), type))
                action typeChanged(type);
}

FilterModel::FilterType OscillatorFilterModel::type() const
{
        return oscillatorModel->api()->getOscillatorFilterType(oscillatorModel->index());
}

void OscillatorFilterModel::setCutOffRange(const std::pair<double, double>& range)
{
}

std::pair<double, double> OscillatorFilterModel::cutOffRange() const
{
        return {};
}

void OscillatorFilterModel::setResonanceRange(const std::pair<double, double>& range)
{
}

std::pair<double, double> OscillatorFilterModel::resonanceRange() const
{
        return {};
}

Envelope::Category OscillatorFilterModel::envelopeCategory() const
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
