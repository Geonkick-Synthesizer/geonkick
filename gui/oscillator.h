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

#include "geonkick_api.h"

class Oscillator: public QObject
{
  Q_OBJECT

 public:

  enum class Type: int {
          Oscillator1 = 0,
          Oscillator2 = 1,
          Noise       = 2
  };

  enum class FunctionType:int {
          Sine     = static_cast<int>(GeonkickApi::FunctionType::Sine),
          Square   = static_cast<int>(GeonkickApi::FunctionType::Square),
          Triangle = static_cast<int>(GeonkickApi::FunctionType::Triangle),
          Sawtooth = static_cast<int>(GeonkickApi::FunctionType::Sawtooth),
          Noise    = static_cast<int>(GeonkickApi::FunctionType::Noise)
  };

  enum class EnvelopeType:int {
          Amplitude  = static_cast<int>(GeonkickApi::EnvelopeType::Amplitude),
          Frequency  = static_cast<int>(GeonkickApi::EnvelopeType::Frequency)
  };

  enum class FilterType:int {
          LowPass    = static_cast<int>(GeonkickApi::FilterType::LowPass),
          HighPass = static_cast<int>(GeonkickApi::FilterType::HighPass)
  };

  Oscillator(GeonkickApi *api, Oscillator::Type type);
  ~Oscillator();
  Oscillator::FunctionType function();
  QPolygonF envelopePoints(EnvelopeType type);
  double amplitude(void);
  double frequency(void);
  void setType(Type type);
  Oscillator::Type type(void);
  Oscillator::FilterType filter();
  void setFilterFrequency(double f);
  bool isEnabled();
  double envelopeLength();

  public slots:
          void enable(bool b);
          void setFunction(FunctionType func);
          void setAmplitude(double amp);
          void setFrequency(double freq);
          void setFilterQFactor(double factor);
          double filterQFactor();

          void addEnvelopePoint(EnvelopeType envelope, double x, double y);
          void removeEnvelopePoint(EnvelopeType envelope, int point_index);

          void updateEnvelopePoint(EnvelopeType envelope, int point_index, double x, double y);
          void setFilterType(FilterType filter);
          double filterFrequency(void);

 signals:
	  void amplitudeUpdated(double v);
	  void frequencyUpdated(double v);

 protected:
          int index();
          int envelopeIndex(EnvelopeType type);
 private:
	  GeonkickApi *geonkickApi;
          Type oscillatorType;
          FilterType filterType;
};

#endif
