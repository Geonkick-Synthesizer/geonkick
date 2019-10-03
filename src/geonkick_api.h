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

  // Adding more layers there is a need to change
  // the GKICK_OSC_GROUP_NUMBER in the API.
  // Currently is set to 3, i.e. index = 0, 1, and 2.
  enum class Layer: int {
                Layer1 = 0,
                Layer2 = 1,
                Layer3 = 2
  };

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
          Sample        = GEONKICK_OSC_FUNC_SAMPLE,
  };

  enum class EnvelopeType:int {
          Amplitude = GEONKICK_AMPLITUDE_ENVELOPE,
          Frequency = GEONKICK_FREQUENCY_ENVELOPE,
          FilterCutOff = GEONKICK_FILTER_CUTOFF_ENVELOPE
  };

  enum class FilterType:int {
          LowPass  = GEONKICK_FILTER_LOW_PASS,
          HighPass = GEONKICK_FILTER_HIGH_PASS,
          BandPass = GEONKICK_FILTER_BAND_PASS
  };

  GeonkickApi();
  ~GeonkickApi();
  void setEventQueue(RkEventQueue *queue);
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
  FunctionType oscillatorFunction(int oscillatorIndex) const;
  gkick_real oscillatorPhase(int oscillatorIndex) const;
  void setOscillatorSample(const std::string &file, int oscillatorIndex);
  void setOscillatorSample(const std::vector<float> &sample, int oscillatorIndex);
  std::vector<float> getOscillatorSample(int oscillatorIndex) const;
  double kickMaxLength(void) const;
  double kickLength(void) const;
  double kickAmplitude() const;
  bool isKickFilterEnabled() const;
  double kickFilterFrequency(void) const;
  double kickFilterQFactor() const;
  FilterType kickFilterType() const;
  std::vector<RkRealPoint> getKickEnvelopePoints(EnvelopeType envelope) const;
  bool setOscillatorFrequency(int oscillatorIndex, double frequency);
  void setOscillatorAsFm(int oscillatorIndex, bool b);
  bool isOscillatorAsFm(int oscillatorIndex) const;
  double oscillatorAmplitude(int oscillatorIndex) const;
  double oscillatorFrequency(int oscillatorIndex) const;
  void addKickEnvelopePoint(EnvelopeType envelope, double x, double y);
  void removeKickEnvelopePoint(EnvelopeType envelope, int pointIndex);
  void updateKickEnvelopePoint(EnvelopeType envelope, int index, double x, double y);
  void setOscillatorEvelopePoints(int index, EnvelopeType envelope, const std::vector<RkRealPoint> &points);
  void setOscillatorFunction(int oscillatorIndex, FunctionType function);
  void setOscillatorPhase(int oscillatorIndex, gkick_real phase);
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
  int getSampleRate() const;
  static std::shared_ptr<GeonkickState> getDefaultState();
  // This function is called only from the audio thread.
  gkick_real getAudioFrame() const;
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
  void setKickEnvelopePoints(EnvelopeType envelope, const std::vector<RkRealPoint> &points);
  void setKeyPressed(bool b, int note, int velocity);
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
  std::vector<gkick_real> getKickBuffer() const;
  void triggerSynthesis();
  void setLayer(Layer layer);
  Layer layer() const;
  void setLayerAmplitude(Layer layer, double amplitude);
  double getLayerAmplitude(Layer layer) const;
  void enbaleLayer(Layer layer, bool enable = true);
  bool isLayerEnabled(Layer layer) const;
  int getOscIndex(int index) const;
  double getLimiterLevelerValue() const;
  std::filesystem::path currentWorkingPath(const std::string &key) const;
  void setCurrentWorkingPath(const std::string &key, const std::filesystem::path &path);
  void tuneAudioOutput(bool tune);
  bool isAudioOutputTuned() const;

  RK_DECL_ACT(kickLengthUpdated, kickLengthUpdated(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
  RK_DECL_ACT(kickAmplitudeUpdated, kickAmplitudeUpdated(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
  RK_DECL_ACT(kickUpdated, kickUpdated(), RK_ARG_TYPE(), RK_ARG_VAL());
  RK_DECL_ACT(newKickBuffer,
              newKickBuffer(std::vector<gkick_real> buffer),
              RK_ARG_TYPE(std::vector<gkick_real>),
              RK_ARG_VAL(std::move(buffer)));
  RK_DECL_ACT(currentPlayingFrameVal,
              currentPlayingFrameVal(double val),
              RK_ARG_TYPE(double), RK_ARG_VAL(val));
  RK_DECL_ACT(stateChanged, stateChanged(), RK_ARG_TYPE(), RK_ARG_VAL());

  void setSettings(const std::string &key, const std::string &value);
  std::string getSettings(const std::string &key) const;

protected:
  static void kickUpdatedCallback(void *arg, gkick_real *buff, size_t size);
  static void limiterCallback(void *arg, gkick_real val);
  void updateKickBuffer(const std::vector<gkick_real> &&buffer);
  void setOscillatorState(Layer layer,
                          OscillatorType oscillator,
                          const std::shared_ptr<GeonkickState> &state);
  void getOscillatorState(Layer layer,
                          OscillatorType osc,
                          const std::shared_ptr<GeonkickState> &state);
  void setLimiterVal(double val);
  static std::vector<gkick_real> loadSample(const std::string &file,
                                            double length = 4.0,
                                            int smapleRate = 48000,
                                            int channels = 1);

private:
  struct geonkick *geonkickApi;
  std::atomic<bool> updateLimiterLeveler;
  std::atomic<double> limiterLevelerVal;
  bool jackEnabled;
  bool standaloneInstance;
  mutable std::mutex apiMutex;
  RkEventQueue *eventQueue;
  std::vector<gkick_real> kickBuffer;
  Layer currentLayer;

  /**
   * Current working paths for entire application.
   * Since on plugins the GUI is closed, there is a need to
   * store this path in API instance.
   */
  std::unordered_map<std::string, std::filesystem::path> workingPaths;
  std::unordered_map<std::string, std::string> apiSettings;
};

#endif // GEONKICK_API_H
