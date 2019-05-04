/**
 * File name: geonkick_api.h
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

#ifndef GEONKICK_API_H
#define GEONKICK_API_H

#include "globals.h"

#include <RkRealPoint.h>

class Oscillator;
class GeonkickState;
class RkEventQueue;

class GeonkickApi {

 public:

  enum class OscillatorType: int {
         Oscillator1 = 0,
         Oscillator2 = 1,
         Noise       = 2
  };

  enum class FunctionType:int {
          Sine          = GEONKICK_OSC_FUNC_SINE,
          Square        = GEONKICK_OSC_FUNC_SQUARE,
          Triangle      = GEONKICK_OSC_FUNC_TRIANGLE,
          Sawtooth      = GEONKICK_OSC_FUNC_SAWTOOTH,
          NoiseWhite    = GEONKICK_OSC_FUNC_NOISE_WHITE,
          NoisePink     = GEONKICK_OSC_FUNC_NOISE_PINK,
          NoiseBrownian = GEONKICK_OSC_FUNC_NOISE_BROWNIAN,
          Unknown       = GEONKICK_OSC_FUNC_UNKNOWN
  };

  enum class EnvelopeType:int {
          Amplitude = GEONKICK_AMPLITUDE_ENVELOPE,
          Frequency = GEONKICK_FREQUENCY_ENVELOPE
  };

  enum class FilterType:int {
          LowPass  = GEONKICK_FILTER_LOW_PASS,
          HighPass = GEONKICK_FILTER_HIGH_PASS
  };

  GeonkickApi();
  ~GeonkickApi();
  void setEventQueue(const std::shared_ptr<RkEventQueue> &evq);
  bool init();
  void registerCallbacks(bool b);
  std::vector<std::unique_ptr<Oscillator>> oscillators(void);
  bool isOscillatorEnabled(int index);
  std::vector<RkRealPoint> oscillatorEvelopePoints(int oscillatorIndex, EnvelopeType envelope) const;
  void addOscillatorEnvelopePoint(int oscillatorIndex, EnvelopeType envelope, const RkRealPoint &point);
  void removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex);
  void updateOscillatorEvelopePoint(int oscillatorIndex,
                                    EnvelopeType envelope,
                                    int pointIndex,
                                    const RkRealPoint &point);
  GeonkickApi::FunctionType oscillatorFunction(int oscillatorIndex) const;
  double kickMaxLength(void) const;
  double kickLength(void) const;
  double kickAmplitude() const;
  bool isKickFilterEnabled() const;
  double kickFilterFrequency(void) const;
  double kickFilterQFactor() const;
  FilterType kickFilterType() const;
  std::vector<RkRealPoint> getKickEnvelopePoints() const;
  bool setOscillatorFrequency(int oscillatorIndex, double frequency);
  double oscillatorAmplitude(int oscillatorIndex) const;
  double oscillatorFrequency(int oscillatorIndex) const;
  void addKickEnvelopePoint(double x, double y);
  void removeKickEnvelopePoint(int pointIndex);
  void updateKickEnvelopePoint(int index, double x, double y);
  void setOscillatorEvelopePoints(int index, EnvelopeType envelope, const std::vector<RkRealPoint> &points);
  void setOscillatorFunction(int oscillatorIndex, FunctionType function);
  void enableOscillator(int oscillatorIndex, bool enable);
  void enableOscillatorFilter(int oscillatorIndex, bool enable);
  bool isOscillatorFilterEnabled(int oscillatorIndex);
  void setOscillatorFilterType(int oscillatorIndex, FilterType filter);
  FilterType getOscillatorFilterType(int oscillatorIndex);
  void setOscillatorFilterCutOffFreq(int oscillatorIndex, double frequency);
  double getOscillatorFilterCutOffFreq(int oscillatorIndex);
  void setOscillatorFilterFactor(int oscillatorIndex, double factor);
  double getOscillatorFilterFactor(int oscillatorIndex);
  bool setOscillatorAmplitude(int oscillatorIndex, double amplitude);
  double limiterValue();
  void getKickBuffer(std::vector<gkick_real> &buffer);
  std::vector<gkick_real> getKickBuffer();
  int getSampleRate();
  static std::shared_ptr<GeonkickState> getDefaultState();
  gkick_real getAudioFrame();
  std::shared_ptr<GeonkickState> getState();

  bool isCompressorEnabled() const;
  double getCompressorAttack() const;
  double getCompressorRelease() const;
  double getCompressorThreshold() const;
  double getCompressorRatio() const;
  double getCompressorKnee() const;
  double getCompressorMakeup() const;

  bool isDistortionEnabled() const;
  double getDistortionVolume() const;
  double getDistortionDrive() const;

  bool isJackEnabled() const;

  void setStandalone(bool b);
  bool isStandalone() const;

  void setKickAmplitude(double amplitude);
  void setKickLength(double length);
  void setLimiterValue(double value);
  void setKickFilterFrequency(double frequency);
  void setKickFilterQFactor(double factor);
  void enableKickFilter(bool b);
  void setKickFilterType(FilterType type);
  void setState(const std::shared_ptr<GeonkickState> &state);
  void setState(const std::string &data);
  void setKickEnvelopePoints(const std::vector<RkRealPoint> &points);
  void setKeyPressed(bool b, int velocity);
  void enableCompressor(bool enable);
  void setCompressorAttack(double attack);
  void setCompressorRelease(double release);
  void setCompressorThreshold(double threshold);
  void setCompressorRatio(double ratio);
  void setCompressorKnee(double knee);
  void setCompressorMakeup(double makeup);
  void enableDistortion(bool enable);
  void setDistortionVolume(double volume);
  void setDistortionDrive(double drive);
  std::shared_ptr<RkEventQueue> eventQueue;

  RK_DECL_ACT(kickLengthUpdated, kickLengthUpdated(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
  RK_DECL_ACT(kickAmplitudeUpdated, kickAmplitudeUpdated(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
  RK_DECL_ACT(kickUpdated, kickUpdated(), RK_ARG_TYPE(), RK_ARG_VAL());
  RK_DECL_ACT(currentPlayingFrameVal,
              currentPlayingFrameVal(double val),
              RK_ARG_TYPE(double), RK_ARG_VAL(val));

protected:
  static void kickUpdatedCallback(void *arg);
  static void limiterCallback(void *arg, gkick_real val);
  void emitKickUpdated();
  void setOscillatorState(OscillatorType oscillator, const std::shared_ptr<GeonkickState> &state);
  void getOscillatorState(OscillatorType osc, const std::shared_ptr<GeonkickState> &state);
  void setLimiterVal(double val);

private:
  struct geonkick *geonkickApi;
  std::atomic<bool> updateLimiterLeveler;
  std::atomic<double> limiterLevelerVal;
  bool jackEnabled;
  bool standaloneInstance;
};

#endif // GEONKICK_API_H
