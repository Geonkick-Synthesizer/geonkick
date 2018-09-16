/**
 * File name: oscillator_envelope.cpp
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

#include "oscillator_envelope.h"
#include "gkick_oscillator.h"

OscillatorEnvelope::OscillatorEnvelope(std::shared_ptr<GKickOscillator> &osc)
        : oscillator(osc)
{
        if (oscillator->getType() == GKickOscillator::OSC_NOISE) {
                removeSupportedType(GKickEnvelope::ENV_TYPE_FREQUENCY);
        }
        setType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
        QPolygonF points = oscillator->getEnvelopePoints(static_cast<GKickOscillator::OscillatorEnvelopeType>(type()));
        setPoints(points);
}

OscillatorEnvelope::~OscillatorEnvelope()
{
}

void OscillatorEnvelope::updatePoints()
{
        QPolygonF points = oscillator->getEnvelopePoints(static_cast<GKickOscillator::OscillatorEnvelopeType>(type()));
        setPoints(points);
}

void OscillatorEnvelope::pointAddedEvent(double x, double y)
{
        oscillator->addEnvelopePoint(static_cast<GKickOscillator::OscillatorEnvelopeType>(type()), x, y);
}

void OscillatorEnvelope::pointUpdatedEvent(unsigned int index, double x, double y)
{
        oscillator->updateEnvelopePoint(static_cast<GKickOscillator::OscillatorEnvelopeType>(type()), index, x, y);
}

void OscillatorEnvelope::pointRemovedEvent(unsigned int index)
{
        oscillator->removeEnvelopePoint(static_cast<GKickOscillator::OscillatorEnvelopeType>(type()), index);

}

double OscillatorEnvelope::envelopeLengh(void) const
{
        return oscillator->getKickLength();
}
