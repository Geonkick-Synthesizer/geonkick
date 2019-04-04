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

#include "geonkick_api.h"

class Oscillator
{
 public:

  using Type = GeonkickApi::OscillatorType;
  using FunctionType = GeonkickApi::FunctionType;
  using EnvelopeType = GeonkickApi::EnvelopeType;
  using FilterType   = GeonkickApi::FilterType;

  explicit Oscillator(GeonkickApi *api, Oscillator::Type type);
  ~Oscillator();
  Oscillator::FunctionType function();
  std::vector<RkRealPoint> envelopePoints(EnvelopeType type);
  double amplitude(void);
  double frequency(void);
  void setType(Type type);
  Oscillator::Type type(void);
  bool isFilterEnabled();
  Oscillator::FilterType filter();
  double filterFrequency(void);
  double filterQFactor();
  bool isEnabled();
  double envelopeLength();

  // public slots:
  void enable(bool b);
  void setFunction(FunctionType func);
  void setAmplitude(double amp);
  void setFrequency(double freq);
  void enableFilter(bool b);
  void setFilterType(FilterType filter);
  void setFilterFrequency(double f);
  void setFilterQFactor(double factor);
  void addEnvelopePoint(EnvelopeType envelope, double x, double y);
  void removeEnvelopePoint(EnvelopeType envelope, int point_index);
  void updateEnvelopePoint(EnvelopeType envelope, int point_index, double x, double y);

  /* signals:
	  void amplitudeUpdated(double v);
	  void frequencyUpdated(double v);
          void kickLengthUpdated(double len);
  */
 protected:
          int index();
          int envelopeIndex(EnvelopeType type);
 private:
	  GeonkickApi *geonkickApi;
          Type oscillatorType;
          FilterType filterType;
};

#endif // GEONKICK_OSCILLATOR_H
