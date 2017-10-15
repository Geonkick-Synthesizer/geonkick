#ifndef GKICK_OSCILLATOR_H
#define GKICK_OSCILLATOR_H

/**
 * File name: gkickoscillator.h
 * Project: GeonKick
 *
 * Copyright (C) 2015 Iurie Nistor
 * Email: nistor@iurie.org
 *
 * This file is part of GeonKick.
 *
 * waveToMp3 is free software; you can redistribute it and/or modify
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

#include <QPolygonF>
#include <QObject>

#include "gkickapi.h"
#include "geonkick.h"

class GKickOscillator: public QObject
{
  Q_OBJECT

 public:

  enum OscillatorType {
          OSC_1 = 0,
          OSC_2 = 1,
          OSC_NOISE = 2
  };

  enum OscillatorFuncType {
          OSC_FUNC_SINE     = GEONKICK_OSC_FUNC_SINE,
          OSC_FUNC_SQARE    = GEONKICK_OSC_FUNC_SQARE,
          OSC_FUNC_TRIANGLE = GEONKICK_OSC_FUNC_TRIANGLE,
          OSC_FUNC_SAWTOOTH = GEONKICK_OSC_FUNC_SAWTOOTH,
          OSC_FUNC_NOISE    = GEONKICK_OSC_FUNC_NOISE
  };

  enum OscillatorEnvelopeType {
          OSC_ENV_TYPE_APLITUDE  = GEONKICK_AMPLITUDE_ENVELOPE,
          OSC_ENV_TYPE_FREQUENCY = GEONKICK_FREQUENCY_ENVELOPE
  };

  enum OscillatorFilterType {
          FILTER_LP = 0,
          FILTER_HP = 1
  };

  GKickOscillator(GKickApi *api, GKickOscillator::OscillatorType type);
  ~GKickOscillator();
  double getKickLength(void);
  QPolygonF getEnvelopePoints(GKickOscillator::OscillatorEnvelopeType type);
  void setOscFunction(OscillatorFuncType type);
  double getOscAmplitudeValue(void);
  double getOscFrequencyValue(void);
  void setOscillatorType(GKickOscillator::OscillatorType type);
  GKickOscillator::OscillatorType getType(void);
  void setFilterType(GKickOscillator::OscillatorFilterType type);
  GKickOscillator::OscillatorFilterType getFilterType();
  void setFilterFrequency(double f);
  double getFilterFrequency(void);
  QString name();

  public slots:
          void setOscAmplitudeValue(double v);
          void setOscFrequencyValue(double v);
          void setFilterQFactor(double v);
          double getFilterQFactor();

          void addEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                double x,
                                double y);
          void removeEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                   int index);

          void updateEnvelopePoint(GKickOscillator::OscillatorEnvelopeType type,
                                   int index,
                                   double x,
                                   double y);

 signals:
	  void oscAmplitudeValueUpdated(double v);
	  void oscFrequencyValueUpdated(double v);

 protected:
          int oscillatorIndex();
          int envelopeIndex(GKickOscillator::OscillatorEnvelopeType type);
 private:
	  GKickApi *kickApi;
          OscillatorType oscillatorType;
          OscillatorFilterType filterType;
};

#endif
