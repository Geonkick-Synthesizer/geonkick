/**
 * File name: general_envelope.h
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

#include "general_envelope.h"
#include "oscillator.h"

GeneralEnvelope::GeneralEnvelope(GeonkickApi *api, const RkRect &area)
        : Envelope(area)
        ,  geonkickApi{api}
{
        removeSupportedType(Envelope::Type::Frequency);
        removeSupportedType(Envelope::Type::PitchShift);
        RK_ACT_BIND(geonkickApi, kickLengthUpdated, RK_ACT_ARGS(double val), this, envelopeUpdated());
        RK_ACT_BIND(geonkickApi, kickAmplitudeUpdated, RK_ACT_ARGS(double val), this, envelopeUpdated());
        setType(Envelope::Type::Amplitude);
        setPoints(geonkickApi->getKickEnvelopePoints(type()));
}

void GeneralEnvelope::pointAddedEvent(double x, double y)
{
        geonkickApi->addKickEnvelopePoint(type(), x, y);
}

void GeneralEnvelope::pointUpdatedEvent(unsigned int index, double x, double y)
{
        geonkickApi->updateKickEnvelopePoint(type(), index, x, y);
}

void GeneralEnvelope::pointRemovedEvent(unsigned int index)
{
        geonkickApi->removeKickEnvelopePoint(type(), index);
}

double GeneralEnvelope::envelopeLength(void) const
{
        return geonkickApi->kickLength();
}

void GeneralEnvelope::setEnvelopeLengh(double len)
{
        geonkickApi->setKickLength(len);
}

double GeneralEnvelope::envelopeAmplitude(void) const
{
        if (type() == Envelope::Type::Amplitude)
                return geonkickApi->kickAmplitude();
        else if (type() == Envelope::Type::FilterCutOff)
                return geonkickApi->kickFilterFrequency();
	else if (type() == Envelope::Type::DistortionDrive)
		return geonkickApi->getDistortionDrive() / (pow(10, 36.0 / 20));
        else if (type() == Envelope::Type::DistortionVolume)
		return geonkickApi->getDistortionVolume();

        return 0;
}

void GeneralEnvelope::updateEnvelope()
{
	auto applyType = geonkickApi->getKickEnvelopeApplyType(type());
	setApplyType(applyType);
	updatePoints();
}

void GeneralEnvelope::updatePoints()
{
        setPoints(geonkickApi->getKickEnvelopePoints(type()));
}
