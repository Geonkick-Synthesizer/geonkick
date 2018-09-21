/**
 * File name: gkick_oscillator.h
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

#ifndef GEONKICK_OSCILLATOR_H
#define GEONKICK_OSCILLATOR_H

#include <QPolygonF>
#include <QObject>

#include "gkickapi.h"
#include "geonkick.h"

class Oscillator: public QObject
{
  Q_OBJECT

 public:

  enum class OscillatorType: int {
          Oscialltor1 = GeonkickApi::OscillatorType::Oscillator1,
          Oscillator2 = GeonkickApi::OscillatorType::Oscillator2,
          Noise       = GeonkickApi::OscillatorType::Noise
  };

  enum class OscillatorFunctionType:int {
          Sine     = GeonkickApi::OscillatorFunctionType::Sine,
          Square   = GeonkickApi::OscillatorFunctionType::Sqaure,
          Triangle = GekickApi::OscillatorFunctionType::Triangle,
          Sawtooth = GekickApi::OscillatorFunctionType::Sawtooth,
          Noise    = GekickApi::OscillatorFunctionType::Noise
  };

  enum class OscillatorEnvelopeType:int {
          Amplitude  = GeokickApi::OscillatorEnvelopeType::Amplitude,
          Frequency  = GeokickApi::OscillatorEnvelopeType::Frequency
  };

  enum class OscillatorFilterType:char {
          LowPass    = GeokickApi::OscillatorFilterType::LowPass,
          HeightPass = GeokickApi::OscillatorFilterType::HightPass
  };

  Oscillator(GKickApi *api, GKickOscillator::OscillatorType type);
  ~Oscillator();
  double getKickLength(void);
  OscillatorFuncType getOscFunction();
  QPolygonF getEnvelopePoints(Oscillator::OscillatorEnvelopeType type);
  double getOscAmplitudeValue(void);
  double getOscFrequencyValue(void);
  void setOscillatorType(Oscillator::OscillatorType type);
  Oscillator::OscillatorType getType(void);
  void setFilterType(Oscillator::OscillatorFilterType type);
  Oscillator::OscillatorFilterType getFilterType();
  void setFilterFrequency(double f);
  double getFilterFrequency(void);
  bool isEnabled();

  public slots:
          void enable(bool b);
          void setOscFunction(OscillatorFuncType type);
          void setOscAmplitudeValue(double v);
          void setOscFrequencyValue(double v);
          void setFilterQFactor(double v);
          double getFilterQFactor();

          void addEnvelopePoint(Oscillator::OscillatorEnvelopeType type,
                                double x,
                                double y);
          void removeEnvelopePoint(Oscillator::OscillatorEnvelopeType type,
                                   int index);

          void updateEnvelopePoint(Oscillator::OscillatorEnvelopeType type,
                                   int index,
                                   double x,
                                   double y);
          void setFilterType(OscillatorFilterType type);

 signals:
	  void oscAmplitudeValueUpdated(double v);
	  void oscFrequencyValueUpdated(double v);

 protected:
          int oscillatorIndex();
          int envelopeIndex(Oscillator::OscillatorEnvelopeType type);
 private:
	  GeonkickApi *geonkickApi;
          OscillatorType oscillatorType;
          OscillatorFilterType filterType;
};

#endif
