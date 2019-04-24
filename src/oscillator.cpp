/**
 * File name: oscillator.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include "oscillator.h"

Oscillator::Oscillator(GeonkickApi *api, Oscillator::Type type)
        : geonkickApi{api}
        , oscillatorType{type}
        , filterType{FilterType::LowPass}
{
        RK_ACT_BIND(geonkickApi, kickLengthUpdated, RK_ACT_ARGS(double val), this, kickLengthUpdated(val));
}

Oscillator::~Oscillator()
{
}

void Oscillator::setFunction(FunctionType func)
{
        geonkickApi->setOscillatorFunction(index(), static_cast<GeonkickApi::FunctionType>(func));
}

Oscillator::FunctionType Oscillator::function()
{
        return static_cast<FunctionType>(geonkickApi->oscillatorFunction(index()));
}

std::vector<RkRealPoint> Oscillator::envelopePoints(EnvelopeType envelope)
{
        std::vector<RkRealPoint> points;
        points = geonkickApi->oscillatorEvelopePoints(index(), static_cast<GeonkickApi::EnvelopeType>(envelope));
        return points;
}

void Oscillator::addEnvelopePoint(EnvelopeType envelope, double x, double y)
{
        geonkickApi->addOscillatorEnvelopePoint(index(), static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                RkRealPoint(x, y));
}

void Oscillator::removeEnvelopePoint(EnvelopeType envelope, int point_index)
{
        if (geonkickApi)
                geonkickApi->removeOscillatorEvelopePoint(index(), static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                          point_index);
}

void Oscillator::updateEnvelopePoint(EnvelopeType envelope, int point_index, double x, double y)
{
        geonkickApi->updateOscillatorEvelopePoint(index(), static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                  point_index, RkRealPoint(x, y));
}

void Oscillator::setType(Oscillator::Type type)
{
        oscillatorType = type;
}

Oscillator::Type Oscillator::type()
{
        return oscillatorType;
}

void Oscillator::setAmplitude(double amp)
{
        if (geonkickApi->setOscillatorAmplitude(index(), amp))
                amplitudeUpdated(amp);
}

double Oscillator::amplitude(void)
{
	return geonkickApi->oscillatorAmplitude(index());
}

void Oscillator::setFrequency(double freq)
{
	if (geonkickApi->setOscillatorFrequency(index(), freq))
                frequencyUpdated(freq);
}

double Oscillator::frequency(void)
{
        return geonkickApi->oscillatorFrequency(index());
}

int Oscillator::index()
{
        return static_cast<int>(oscillatorType);
}

Oscillator::FilterType Oscillator::filter()
{
        return static_cast<FilterType>(geonkickApi->getOscillatorFilterType(index()));
}

void Oscillator::setFilterType(FilterType type)
{
        geonkickApi->setOscillatorFilterType(index(), static_cast<GeonkickApi::FilterType>(type));
}

void Oscillator::enableFilter(bool b)
{
        geonkickApi->enableOscillatorFilter(index(), b);
}

bool Oscillator::isFilterEnabled()
{
        return geonkickApi->isOscillatorFilterEnabled(index());
}

void Oscillator::setFilterFrequency(double f)
{
        geonkickApi->setOscillatorFilterCutOffFreq(index(), f);
}

double Oscillator::filterFrequency(void)
{
        return geonkickApi->getOscillatorFilterCutOffFreq(index());
}

void Oscillator::setFilterQFactor(double v)
{
        return geonkickApi->setOscillatorFilterFactor(index(), v);
}

double Oscillator::filterQFactor()
{
        return geonkickApi->getOscillatorFilterFactor(index());
}

void Oscillator::enable(bool b)
{
        geonkickApi->enableOscillator(index(), b);
}

bool Oscillator::isEnabled()
{
        return geonkickApi->isOscillatorEnabled(index());
}

double Oscillator::envelopeLength()
{
        return geonkickApi->kickLength();
}
