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

Oscillator::Oscillator(GeonkickApi *api, Oscillator::OscillatorType type) :
        geonkickApi(api),
        oscillatorType(type),
        filterType(FilteType::LowPass)
{
}

Oscillator::~Oscillator()
{
}

void Oscillator::setOscFunction(OscillatorFunctionType type)
{
        geonkickApi->setOscillatorFunction(oscillatorType, type);
}

Oscillator::OscillatorFunctionType Oscillator::getOscFunction()
{
        return static_cast<OscillatorFunctionType>(geonkickApi->getOscFunction(oscillatorIndex()));
}

QPolygonF Oscillator::getEnvelopePoints(Oscillator::OscillatorEnvelopeType type)
{
        QPolygonF points;
        points = geonkickApi->getOscEvelopePoints(oscillatorIndex(), envelopeIndex(type));
        return points;
}

void Oscillator::addEnvelopePoint(Oscillator::OscillatorEnvelopeType type, double x, double y)
{
        geonkickApi->addOscEnvelopePoint(oscillatorIndex(), envelopeIndex(type), QPointF(x, y));
}

void Oscillator::removeEnvelopePoint(Oscillator::OscillatorEnvelopeType type, int index)
{
        if (geonkickApi) {
                geonkickApi->removeOscEvelopePoint(oscillatorIndex(), envelopeIndex(type), index);
        }
}

void Oscillator::updateEnvelopePoint(Oscillator::OscillatorEnvelopeType type,
                                     int index,
                                     double x,
                                     double y)
{
        geonkickApi->updateOscEvelopePoint(oscillatorIndex(), envelopeIndex(type),
                                           index, QPointF(x, y));
}

void Oscillator::setOscillatorType(Oscillator::OscillatorType type)
{
        oscillatorType = type;
}

Oscillator::OscillatorType Oscillator::getType()
{
        return oscillatorType;
}

double Oscillator::getKickLength(void)
{
        return geonkickApi->getKickLength();
}

void Oscillator::setOscAmplitudeValue(double v)
{
	if (geonkickApi->setOscAmplitudeValue(oscillatorIndex(), v)) {
		emit oscAmplitudeValueUpdated(v);
	}
}

double Oscillator::getOscAmplitudeValue(void)
{
	return geonkickApi->getOscAmplitudeValue(oscillatorIndex());
}

void Oscillator::setOscFrequencyValue(double v)
{
	if (geonkickApi->setOscFrequencyValue(oscillatorIndex(), v)) {
		emit oscFrequencyValueUpdated(v);
	}
}

double Oscillator::getOscFrequencyValue(void)
{
        return geonkickApi->getOscFrequencyValue(oscillatorIndex());
}

int Oscillator::oscillatorIndex()
{
        return static_cast<int>(oscillatorType);
}

int Oscillator::envelopeIndex(Oscillator::OscillatorEnvelopeType type)
{
        return static_cast<int>(type);
}

void Oscillator::setFilterType(Oscillator::OscillatorFilterType type)
{
        filterType = type;
}

Oscillator::OscillatorFilterType Oscillator::getFilterType()
{
        return filterType;
}

void Oscillator::setFilterFrequency(double f)
{
}

double Oscillator::getFilterFrequency(void)
{
}

void Oscillator::setFilterQFactor(double v)
{
}

double Oscillator::getFilterQFactor()
{
}

void Oscillator::enable(bool b)
{
        geonkickApi->enableOscillator(oscillatorIndex(), b);
}

bool Oscillator::isEnabled()
{
        return geonkickApi->isOscillatorEnabled(oscillatorIndex());
}

void Oscillator::setFilterType(OscillatorFilterType type)
{
        geonkickApi->setOscillatorFilterType(oscillatorIndex(), static_cast<GeonkickApi::FilterType>(type));
}
