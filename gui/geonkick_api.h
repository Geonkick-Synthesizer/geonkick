/**
 * File name: gkickapi.h
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

#ifndef GEONKICK_API_H
#define GEONKICK_API_H

#include <memory>
#include <vector>
#include <QPolygonF>
#include <QObject>

#include "geonkick.h"

class Oscillator;

class GeonkickApi: public QObject {
 Q_OBJECT

 public:

  enum class FunctionType:int {
          Sine     = GEONKICK_OSC_FUNC_SINE,
          Square   = GEONKICK_OSC_FUNC_SQUARE,
          Triangle = GEONKICK_OSC_FUNC_TRIANGLE,
          Sawtooth = GEONKICK_OSC_FUNC_SAWTOOTH,
          Noise    = GEONKICK_OSC_FUNC_NOISE,
          Unknown  = GEONKICK_OSC_FUNC_UNKNOWN
  };

  enum class EnvelopeType:int {
          Amplitude = GEONKICK_AMPLITUDE_ENVELOPE,
          Frequency = GEONKICK_FREQUENCY_ENVELOPE
  };

  enum class FilterType:int {
          LowPass  = GEONKICK_FILTER_LOW_PASS,
          HighPass = GEONKICK_FREQUENCY_HIGH_PASS
  };

  GeonkickApi(QObject *parent = nullptr);
  ~GeonkickApi();
  bool init();
  std::vector<Oscillator*> oscillators(void);
  bool isOscillatorEnabled(int index);
  QPolygonF oscillatorEvelopePoints(int oscillatorIndex, EnvelopeType envelope) const;
  void addOscillatorEnvelopePoint(int oscillatorIndex, EnvelopeType envelope, const QPointF &point);
  void removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex);
  void updateOscillatorEvelopePoint(int oscillatorIndex,
                                     EnvelopeType envelope,
                                     int pointIndex,
                                     const QPointF &point);
  GeonkickApi::FunctionType oscillatorFunction(int oscillatorIndex) const;
  double kickMaxLength(void) const;
  double kickLength(void) const;
  double kickAmplitude() const;
  double kickFilterFrequency(void) const;
  bool setOscillatorFrequency(int oscillatorIndex, double frequency);
  double oscillatorAmplitude(int oscillatorIndex) const;
  double oscillatorFrequency(int oscillatorIndex) const;
  double kickFilterQFactor() const;
  void setKickFilterFrequency(double frequency);
  void setKickFilterQFactor(double factor);
  void addKickEnvelopePoint(double x, double y);
  void removeKickEnvelopePoint(int pointIndex);
  void updateKickEnvelopePoint(double x, double y);
  void setOscillatorFunction(int oscillatorIndex, FunctionType function);
  void enableOscillator(int oscillatorIndex, bool enable);
  void setOscillatorFilterType(int oscillatorIndex, FilterType filter);
  bool setOscillatorAmplitude(int oscillatorIndex, double amplitude);
  double limiterValue();

public slots:
  void setKickAmplitude(double amplitude);
  void setKickLength(double length);
  void setLimiterValue(double value);

signals:
  void kickLengthUpdated(double length);
  void kickAmplitudeUpdated(double amplitude);

private:
  struct geonkick *geonkickApi;
};

#endif
