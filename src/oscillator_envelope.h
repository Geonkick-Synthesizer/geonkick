/**
 * File name: oscillator_envelope.h
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

#ifndef GKICK_OSCILLLATOR_ENVELOPE_H
#define GKICK_OSCILLLATOR_ENVELOPE_H

#include "envelope.h"

class OscillatorModel;

class OscillatorEnvelope: public Envelope
{
public:

        OscillatorEnvelope(OscillatorModel* osc, const RkRect &area);
        double envelopeLength(void) const override;
        OscillatorModel* getOscillator() const;

protected:
        void updateEnvelope() override;
        void updatePoints() override;
        void pointAddedEvent(const EnvelopePoint &point) override;
        void pointUpdatedEvent(unsigned int index, const EnvelopePoint &point) override;
        void pointRemovedEvent(unsigned int index) override;
        double envelopeAmplitude() const override;

private:
        OscillatorModel *oscillator;
};

#endif
