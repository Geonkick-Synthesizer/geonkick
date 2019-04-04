/**
 * File name: oscillator_envelope.h
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

#ifndef GKICK_OSCILLLATOR_ENVELOPE_H
#define GKICK_OSCILLLATOR_ENVELOPE_H

#include "envelope.h"

#include <memory>

class Oscillator;

class OscillatorEnvelope: public Envelope
{
  Q_OBJECT

 public:

  OscillatorEnvelope(Oscillator* osc, const RkRect &area);
  ~OscillatorEnvelope();
  double envelopeLengh(void) const;

  protected slots:
          void updatePoints() final;

 protected:
  void pointAddedEvent(double x, double y) override;
  void pointUpdatedEvent(unsigned int index, double x, double y) override;
  void pointRemovedEvent(unsigned int index) override;
  double envelopeAmplitude() const final;

 private:
          Oscillator *oscillator;
};

#endif
