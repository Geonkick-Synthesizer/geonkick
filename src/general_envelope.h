/**
 * File name: general_envelope.h
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

#ifndef GKICK_GENERAL_ENVELOPE_H
#define GKICK_GENERAL_ENVELOPE_H

#include "envelope.h"

class Oscillator;
class GeonkickApi;

class GeneralEnvelope: public Envelope
{
  Q_OBJECT

 public:

  GeneralEnvelope(GeonkickApi *api, const RkRect &area);
  ~GeneralEnvelope();
  double envelopeLengh(void) const;

public slots:
  void setEnvelopeLengh(double len) final;

protected slots:
  void updatePoints() final;

 protected:
  void pointAddedEvent(double x, double y) override;
  void pointUpdatedEvent(unsigned int index, double x, double y) override;
  void pointRemovedEvent(unsigned int index) override;
  double envelopeAmplitude(void) const final;

 private:
          GeonkickApi *geonkickApi;
};

#endif
