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

#include "OscillatorModel.h"
#include "OscillatorDistortionModel.h"
#include "OscillatorFilterModel.h"
#include "GeonkickModel.h"

OscillatorModel::OscillatorModel(GeonkickModel *model, OscillatorModel::Type type)
        : geonkickApi{model->api()}
        , oscillatorType{type}
        , filterModel{new OscillatorFilterModel(this)}
        , distortionModel{new OscillatorDistortionModel(this)}
{
        RK_ACT_BIND(geonkickApi,
                    kickLengthUpdated,
                    RK_ACT_ARGS(double val),
                    this, kickLengthUpdated(val));
}

void OscillatorModel::setAsFm(bool b)
{
        geonkickApi->setOscillatorAsFm(index(), b);
}

bool OscillatorModel::isFm() const
{
        return geonkickApi->isOscillatorAsFm(index());
}

bool OscillatorModel::setFunction(FunctionType func)
{
        auto functionType = static_cast<GeonkickApi::FunctionType>(func);
        if (functionType == geonkickApi->oscillatorFunction(index()))
                return false;

        geonkickApi->setOscillatorFunction(index(), functionType);
        action functionUpdated(func);
        return true;
}

void OscillatorModel::setPhase(gkick_real phase)
{
        geonkickApi->setOscillatorPhase(index(), phase);
}

gkick_real OscillatorModel::getPhase() const
{
        return geonkickApi->oscillatorPhase(index());
}

void OscillatorModel::setSeed(int seed)
{
        geonkickApi->setOscillatorSeed(index(), seed);
}

int OscillatorModel::getSeed() const
{
        return geonkickApi->oscillatorSeed(index());
}

OscillatorModel::FunctionType OscillatorModel::function() const
{
        return static_cast<FunctionType>(geonkickApi->oscillatorFunction(index()));
}

std::vector<EnvelopePoint>
OscillatorModel::envelopePoints(EnvelopeType envelope) const
{
        const auto env = geonkickApi->oscillatorEvelopePoints(index(), static_cast<GeonkickApi::EnvelopeType>(envelope));
        return geonkickApi->oscillatorEvelopePoints(index(), static_cast<GeonkickApi::EnvelopeType>(envelope));
}

void OscillatorModel::addEnvelopePoint(EnvelopeType envelope, const EnvelopePoint &point)
{
        geonkickApi->addOscillatorEnvelopePoint(index(),
                                                static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                point);
}

void OscillatorModel::removeEnvelopePoint(EnvelopeType envelope, int point_index)
{
        geonkickApi->removeOscillatorEvelopePoint(index(),
                                                  static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                  point_index);
}

void OscillatorModel::updateEnvelopePoint(EnvelopeType envelope,
                                     int point_index,
                                     const EnvelopePoint &point)
{
        geonkickApi->updateOscillatorEvelopePoint(index(),
                                                  static_cast<GeonkickApi::EnvelopeType>(envelope),
                                                  point_index,
                                                  point);
}

void OscillatorModel::setType(OscillatorModel::Type type)
{
        oscillatorType = type;
}

OscillatorModel::Type OscillatorModel::type() const
{
        return oscillatorType;
}

void OscillatorModel::setAmplitude(double amp)
{
        if (geonkickApi->setOscillatorAmplitude(index(), amp))
                action amplitudeUpdated(amp);
}

double OscillatorModel::amplitude(void) const
{
	return geonkickApi->oscillatorAmplitude(index());
}

void OscillatorModel::setFrequency(double freq)
{
	if (geonkickApi->setOscillatorFrequency(index(), freq))
                action frequencyUpdated(freq);
}

void OscillatorModel::setPitchShift(double semitones)
{
	if (geonkickApi->setOscillatorPitchShift(index(), semitones))
                action pitchShiftUpdated(semitones);
}

void OscillatorModel::setNoiseDensity(double density)
{
	if (geonkickApi->setOscillatorNoiseDensity(index(), density))
                action noiseDensityUpdated(density);
}

double OscillatorModel::frequency(void) const
{
        return geonkickApi->oscillatorFrequency(index());
}

double OscillatorModel::pitchShift(void) const
{
        return geonkickApi->oscillatorPitchShift(index());
}

double OscillatorModel::noiseDensity(void) const
{
        return geonkickApi->oscillatorNoiseDensity(index());
}

int OscillatorModel::index() const
{
        return static_cast<int>(oscillatorType);
}

void OscillatorModel::enable(bool b)
{
        geonkickApi->enableOscillator(index(), b);
}

bool OscillatorModel::isEnabled() const
{
        return geonkickApi->isOscillatorEnabled(index());
}

double OscillatorModel::envelopeLength() const
{
        return geonkickApi->kickLength();
}

bool OscillatorModel::setSample(const std::string &file)
{
        if (auto res = geonkickApi->setOscillatorSample(file, index()); !res)
                return false;
        auto path = std::filesystem::path(file);
        geonkickApi->setCurrentWorkingPath("Samples", path.has_parent_path() ? path.parent_path().string() : path.string());
        return true;
}

std::string OscillatorModel::samplesPath() const
{
        return geonkickApi->currentWorkingPath("Samples").string();
}

void OscillatorModel::setEnvelopeApplyType(OscillatorModel::EnvelopeType envelope,
				      OscillatorModel::EnvelopeApplyType apply)
{
	geonkickApi->setOscillatorEnvelopeApplyType(index(), envelope, apply);
}

OscillatorModel::EnvelopeApplyType
OscillatorModel::envelopeApplyType(OscillatorModel::EnvelopeType envelope) const
{
	return geonkickApi->getOscillatorEnvelopeApplyType(index(), envelope);
}

FilterModel* OscillatorModel::getFilter() const
{
        return filterModel;
}

DistortionModel* OscillatorModel::getDistortion() const
{
        return distortionModel;
}

GeonkickApi* OscillatorModel::api() const
{
        return geonkickApi;
}
