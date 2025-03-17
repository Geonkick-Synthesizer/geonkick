/**
 * File name: oscillator_envelope.cpp
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

#include "oscillator_envelope.h"
#include "OscillatorModel.h"
#include "FilterModel.h"
#include "DistortionModel.h"

OscillatorEnvelope::OscillatorEnvelope(OscillatorModel* osc, const RkRect &area)
        : Envelope(area)
        , oscillator{osc}
{
        setType(Envelope::Type::Amplitude);
        setPoints(oscillator->envelopePoints(static_cast<OscillatorModel::EnvelopeType>(type())));
}

void OscillatorEnvelope::updatePoints()
{
        setPoints(oscillator->envelopePoints(static_cast<OscillatorModel::EnvelopeType>(type())));
}

void OscillatorEnvelope::updateEnvelope()
{
	auto applyType = oscillator->envelopeApplyType(static_cast<OscillatorModel::EnvelopeType>(type()));
	setApplyType(applyType);
	updatePoints();
}

void OscillatorEnvelope::pointAddedEvent(const EnvelopePoint &point)
{
        oscillator->addEnvelopePoint(static_cast<OscillatorModel::EnvelopeType>(type()), point);
}

void OscillatorEnvelope::pointUpdatedEvent(unsigned int index, const EnvelopePoint &point)
{
        oscillator->updateEnvelopePoint(static_cast<OscillatorModel::EnvelopeType>(type()), index, point);
}

void OscillatorEnvelope::pointRemovedEvent(unsigned int index)
{
        oscillator->removeEnvelopePoint(static_cast<OscillatorModel::EnvelopeType>(type()), index);
}

double OscillatorEnvelope::envelopeLength(void) const
{
        return oscillator->envelopeLength();
}

double OscillatorEnvelope::envelopeAmplitude() const
{
        switch (type()) {
        case Type::Amplitude:
                return oscillator->amplitude();
        case Type::Frequency:
                return oscillator->frequency();
        case Type::PitchShift:
                return oscillator->pitchShift();
        case Type::NoiseDensity:
                return oscillator->noiseDensity();
        case Type::FilterCutOff:
                return oscillator->getFilter()->cutOff();
	case Type::FilterQFactor:
                return oscillator->getFilter()->resonance();
        case Type::DistortionDrive:
                return oscillator->getDistortion()->getDrive() / pow(10, 36.0 / 20);
        default:
                return 0;
        }
}

OscillatorModel* OscillatorEnvelope::getOscillator() const
{
        return oscillator;
}
