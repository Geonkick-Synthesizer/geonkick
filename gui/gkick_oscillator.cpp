/**
 * File name: gkick_oscillator.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include "gkick_oscillator.h"
#include <QDebug>

GKickOscillator::GKickOscillator(GKickApi *api, GKickOscillator::OscillatorType type) :
        kickApi(api),
        oscillatorType(type),
        filterType(GKickOscillator::FILTER_LP)
{
}

GKickOscillator::~GKickOscillator()
{
}

void GKickOscillator::setOscFunction(OscillatorFuncType type)
{
        if (kickApi) {
                kickApi->setOscFunction((int)oscillatorType, (enum geonkick_osc_func_type)type);
        }
}

GKickOscillator::OscillatorFuncType
GKickOscillator::getOscFunction()
{
        if (kickApi) {
                return static_cast<OscillatorFuncType>(kickApi->getOscFunction(oscillatorIndex()));
        }
}

QPolygonF GKickOscillator::getEnvelopePoints(GKickOscillator::OscillatorEnvelopeType type)
{
        QPolygonF points;

        if (kickApi) {
                points = kickApi->getOscEvelopePoints(oscillatorIndex(), envelopeIndex(type));
        }

        return points;
}

void GKickOscillator::addEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                       double x,
                                       double y)
{
        if (kickApi) {
                kickApi->addOscEnvelopePoint(oscillatorIndex(), envelopeIndex(type), QPointF(x, y));
        }
}

void GKickOscillator::removeEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                          int index)
{
        if (kickApi) {
                kickApi->removeOscEvelopePoint(oscillatorIndex(), envelopeIndex(type), index);
        }
}

void GKickOscillator::updateEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                          int index,
                                          double x,
                                          double y)
{
        if (kickApi) {
                kickApi->updateOscEvelopePoint(oscillatorIndex(),
                                               envelopeIndex(type),
                                               index,
                                               QPointF(x, y));
        }
}

void GKickOscillator::setOscillatorType(GKickOscillator::OscillatorType type)
{
        oscillatorType = type;
}

GKickOscillator::OscillatorType GKickOscillator::getType()
{
        return oscillatorType;
}

double GKickOscillator::getKickLength(void)
{
        if (!kickApi) {
                return 0.0;
        } else {
                return kickApi->getKickLength();
        }
}

void GKickOscillator::setOscAmplitudeValue(double v)
{
	if (!kickApi) {
		return;
	}

	bool b = kickApi->setOscAmplitudeValue(oscillatorIndex(), v);
	if (b) {
		emit oscAmplitudeValueUpdated(v);
	}
}

double GKickOscillator::getOscAmplitudeValue(void)
{
	if (!kickApi) {
		return 0.0;
	}

	double v = kickApi->getOscAmplitudeValue(oscillatorIndex());
        return v;
}

void GKickOscillator::setOscFrequencyValue(double v)
{
	if (!kickApi) {
		return;
	}

	bool b = kickApi->setOscFrequencyValue(oscillatorIndex(), v);
	if (b) {
		emit oscFrequencyValueUpdated(v);
	}
}

double GKickOscillator::getOscFrequencyValue(void)
{
	if (!kickApi) {
		return 0.0;
	}

        return kickApi->getOscFrequencyValue(oscillatorIndex());
}

int GKickOscillator::oscillatorIndex()
{
        return static_cast<int>(oscillatorType);
}

int GKickOscillator::envelopeIndex(GKickOscillator::OscillatorEnvelopeType type)
{
        return static_cast<int>(type);
}

void GKickOscillator::setFilterType(GKickOscillator::OscillatorFilterType type)
{
        filterType = type;
}

GKickOscillator::OscillatorFilterType GKickOscillator::getFilterType()
{
        return filterType;
}

void GKickOscillator::setFilterFrequency(double f)
{
}

double GKickOscillator::getFilterFrequency(void)
{
}

void GKickOscillator::setFilterQFactor(double v)
{
}

double GKickOscillator::getFilterQFactor()
{
}

QString GKickOscillator::name()
{
        if (oscillatorType == GKickOscillator::OSC_1) {
                return tr("OSC 1");
        } else if (oscillatorType == GKickOscillator::OSC_2) {
                return tr("OSC 2");
        } else {
                return tr("Noise");
        }
}

