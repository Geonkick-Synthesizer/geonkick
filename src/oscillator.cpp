/**
 * File name: oscillator.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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
        RK_ACT_BIND(geonkickApi,
                    kickLengthUpdated,
                    RK_ACT_ARGS(double val),
                    this, kickLengthUpdated(val));
}

void Oscillator::setAsFm(bool b)
{
        geonkickApi->setOscillatorAsFm(index(), b);
}

bool Oscillator::isFm() const
{
        return geonkickApi->isOscillatorAsFm(index());
}

void Oscillator::setFunction(FunctionType func)
{
        geonkickApi->setOscillatorFunction(index(), static_cast<GeonkickApi::FunctionType>(func));
}

void Oscillator::setPhase(gkick_real phase)
{
        geonkickApi->setOscillatorPhase(index(), phase);
}

gkick_real Oscillator::getPhase() const
{
        return geonkickApi->oscillatorPhase(index());
}

void Oscillator::setSeed(int seed)
{
        geonkickApi->setOscillatorSeed(index(), seed);
}

int Oscillator::getSeed() const
{
        return geonkickApi->oscillatorSeed(index());
}

Oscillator::FunctionType Oscillator::function() const
{
        return static_cast<FunctionType>(geonkickApi->oscillatorFunction(index()));
}

std::vector<RkRealPoint>
Oscillator::envelopePoints(EnvelopeType envelope) const
{
        return geonkickApi->oscillatorEvelopePoints(index(), static_cast<GeonkickApi::EnvelopeType>(envelope));
}

void Oscillator::addEnvelopePoint(EnvelopeType envelope, double x, double y)
{
        geonkickApi->addOscillatorEnvelopePoint(index(),
                                                static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                RkRealPoint(x, y));
}

void Oscillator::removeEnvelopePoint(EnvelopeType envelope, int point_index)
{
        geonkickApi->removeOscillatorEvelopePoint(index(),
                                                  static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                  point_index);
}

void Oscillator::updateEnvelopePoint(EnvelopeType envelope, int point_index, double x, double y)
{
        geonkickApi->updateOscillatorEvelopePoint(index(),
                                                  static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                  point_index,
                                                  RkRealPoint(x, y));
}

void Oscillator::setType(Oscillator::Type type)
{
        oscillatorType = type;
}

Oscillator::Type Oscillator::type() const
{
        return oscillatorType;
}

void Oscillator::setAmplitude(double amp)
{
        if (geonkickApi->setOscillatorAmplitude(index(), amp))
                action amplitudeUpdated(amp);
}

double Oscillator::amplitude(void) const
{
	return geonkickApi->oscillatorAmplitude(index());
}

void Oscillator::setFrequency(double freq)
{
	if (geonkickApi->setOscillatorFrequency(index(), freq))
                action frequencyUpdated(freq);
}

void Oscillator::setPitchShift(double semitones)
{
	if (geonkickApi->setOscillatorPitchShift(index(), semitones))
                action pitchShiftUpdated(semitones);
}

double Oscillator::frequency(void) const
{
        return geonkickApi->oscillatorFrequency(index());
}

double Oscillator::pitchShift(void) const
{
        return geonkickApi->oscillatorPitchShift(index());
}

int Oscillator::index() const
{
        return static_cast<int>(oscillatorType);
}

Oscillator::FilterType Oscillator::filter() const
{
        return static_cast<FilterType>(geonkickApi->getOscillatorFilterType(index()));
}

void Oscillator::setFilterType(FilterType filter)
{
        geonkickApi->setOscillatorFilterType(index(), static_cast<GeonkickApi::FilterType>(filter));
}

void Oscillator::enableFilter(bool b)
{
        geonkickApi->enableOscillatorFilter(index(), b);
}

bool Oscillator::isFilterEnabled() const
{
        return geonkickApi->isOscillatorFilterEnabled(index());
}

void Oscillator::setFilterFrequency(double f)
{
        geonkickApi->setOscillatorFilterCutOffFreq(index(), f);
}

double Oscillator::filterFrequency(void) const
{
        return geonkickApi->getOscillatorFilterCutOffFreq(index());
}

void Oscillator::setFilterQFactor(double factor)
{
        return geonkickApi->setOscillatorFilterFactor(index(), factor);
}

double Oscillator::filterQFactor() const
{
        return geonkickApi->getOscillatorFilterFactor(index());
}

void Oscillator::enable(bool b)
{
        geonkickApi->enableOscillator(index(), b);
}

bool Oscillator::isEnabled() const
{
        return geonkickApi->isOscillatorEnabled(index());
}

double Oscillator::envelopeLength() const
{
        return geonkickApi->kickLength();
}

void Oscillator::setSample(const std::string &file)
{
        geonkickApi->setOscillatorSample(file, index());
        auto path = std::filesystem::path(file);
        geonkickApi->setCurrentWorkingPath("Samples", path.has_parent_path() ? path.parent_path().string() : path.string());
}

std::string Oscillator::samplesPath() const
{
        return geonkickApi->currentWorkingPath("Samples").string();
}

void Oscillator::setEnvelopeApplyType(Oscillator::EnvelopeType envelope,
				      Oscillator::EnvelopeApplyType apply)
{
	geonkickApi->setOscillatorEnvelopeApplyType(index(), envelope, apply);
}

Oscillator::EnvelopeApplyType
Oscillator::envelopeApplyType(Oscillator::EnvelopeType envelope) const
{
	return geonkickApi->getOscillatorEnvelopeApplyType(index(), envelope);
}
