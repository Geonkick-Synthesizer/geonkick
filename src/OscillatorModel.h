/**
 * File name: gkick_oscillator.h
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

#ifndef GEONKICK_OSCILLATOR_H
#define GEONKICK_OSCILLATOR_H

#include "geonkick_api.h"

class DistortionModel;
class FilterModel;
class GeonkickModel;

class OscillatorModel: public RkObject
{
 public:

  using Type = GeonkickApi::OscillatorType;
  using FunctionType = GeonkickApi::FunctionType;
  using EnvelopeType = GeonkickApi::EnvelopeType;
  using EnvelopeApplyType = GeonkickApi::EnvelopeApplyType;
  using FilterType   = GeonkickApi::FilterType;

  explicit OscillatorModel(GeonkickModel *model, OscillatorModel::Type type);
  OscillatorModel::FunctionType function() const;
  std::vector<EnvelopePoint> envelopePoints(EnvelopeType envelope) const;
  double amplitude(void) const;
  double frequency(void) const;
  double pitchShift(void) const;
  double noiseDensity(void) const;
  void setType(Type type);
  OscillatorModel::Type type(void) const;
  bool isFilterEnabled() const;
  OscillatorModel::FilterType filter() const;
  double filterFrequency(void) const;
  double filterQFactor() const;
  bool isEnabled() const;
  double envelopeLength() const;
  void enable(bool b);
  void setAsFm(bool b);
  bool isFm() const;
  bool setFunction(FunctionType func);
  void setPhase(gkick_real phase);
  gkick_real getPhase() const;
  void setSeed(int seed);
  int getSeed() const;
  void setAmplitude(double amp);
  void setFrequency(double freq);
  void setPitchShift(double semitones);
  void setNoiseDensity(double density);
  void addEnvelopePoint(EnvelopeType envelope,
                        const EnvelopePoint &point);
  void removeEnvelopePoint(EnvelopeType envelope,
                           int point_index);
  void updateEnvelopePoint(EnvelopeType envelope,
                           int point_index,
                           const EnvelopePoint &point);
  void setEnvelopeApplyType(EnvelopeType envelope,
			    EnvelopeApplyType apply);
  EnvelopeApplyType envelopeApplyType(EnvelopeType envelope) const;
  bool setSample(const std::string &file);
  int index() const;
  std::string samplesPath() const;
  FilterModel* getFilter() const;
  DistortionModel* getDistortion() const;
  GeonkickApi* api() const;

  RK_DECL_ACT(functionUpdated,
              functionUpdated(FunctionType func),
              RK_ARG_TYPE(FunctionType func),
              RK_ARG_VAL(func));
  RK_DECL_ACT(amplitudeUpdated,
              amplitudeUpdated(double v),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(v));
  RK_DECL_ACT(frequencyUpdated,
              frequencyUpdated(double v),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(v));
  RK_DECL_ACT(pitchShiftUpdated,
              pitchShiftUpdated(double semitones),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(semitones));
  RK_DECL_ACT(noiseDensityUpdated,
              noiseDensityUpdated(double density),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(density));
  RK_DECL_ACT(kickLengthUpdated,
              kickLengthUpdated(double len),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(len));

 protected:
          int envelopeIndex(EnvelopeType type) const;
 private:
	  GeonkickApi *geonkickApi;
          Type oscillatorType;
          FilterModel* filterModel;
          DistortionModel* distortionModel;
};

#endif // GEONKICK_OSCILLATOR_H
