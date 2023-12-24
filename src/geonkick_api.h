/**
 * File name: geonkick_api.h
 * Project: Geonkick (A percussion synthesizer)
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

#ifndef GEONKICK_API_H
#define GEONKICK_API_H

#include "globals.h"

#include <RkRealPoint.h>

class Oscillator;
class PercussionState;
class KitState;
class RkEventQueue;
class PresetFolder;
class UiSettings;

class GeonkickApi : public RkObject {

 public:

  enum class InstanceType: int {
                             Standalone = 0,
                             Lv2        = 1,
                             Vst3       = 2
  };

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

  enum class FunctionType: int {
          Sine          = GEONKICK_OSC_FUNC_SINE,
          Square        = GEONKICK_OSC_FUNC_SQUARE,
          Triangle      = GEONKICK_OSC_FUNC_TRIANGLE,
          Sawtooth      = GEONKICK_OSC_FUNC_SAWTOOTH,
          NoiseWhite    = GEONKICK_OSC_FUNC_NOISE_WHITE,
          NoisePink     = GEONKICK_OSC_FUNC_NOISE_PINK,
          NoiseBrownian = GEONKICK_OSC_FUNC_NOISE_BROWNIAN,
          Sample        = GEONKICK_OSC_FUNC_SAMPLE,
  };

  enum class EnvelopeType: int {
          Amplitude = GEONKICK_AMPLITUDE_ENVELOPE,
          Frequency = GEONKICK_FREQUENCY_ENVELOPE,
	  FilterCutOff = GEONKICK_FILTER_CUTOFF_ENVELOPE,
	  FilterQFactor = GEONKICK_FILTER_Q_ENVELOPE,
	  DistortionDrive = GEONKICK_DISTORTION_DRIVE_ENVELOPE,
          DistortionVolume = GEONKICK_DISTORTION_VOLUME_ENVELOPE,
          PitchShift = GEONKICK_PITCH_SHIFT_ENVELOPE
  };

  enum class EnvelopeApplyType: int {
	  Linear = GEONKICK_ENVELOPE_APPLY_LINEAR,
	  Logarithmic = GEONKICK_ENVELOPE_APPLY_LOGARITHMIC
  };

  enum class FilterType: int {
          LowPass  = GEONKICK_FILTER_LOW_PASS,
          HighPass = GEONKICK_FILTER_HIGH_PASS,
          BandPass = GEONKICK_FILTER_BAND_PASS
  };

        GeonkickApi(int sample_rate = Geonkick::defaultSampleRate,
                    InstanceType instance = InstanceType::Standalone,
                    geonkick *dsp = nullptr);
  ~GeonkickApi();
  void setInstanceType(InstanceType type);
  InstanceType getInstanceType() const;
  static unsigned int getVersion();
  size_t static numberOfChannels();
  void setEventQueue(RkEventQueue *queue);
  bool initDSP();
  bool init();
  void registerCallbacks(bool b);
  std::vector<std::unique_ptr<Oscillator>> oscillators(void);
  bool isOscillatorEnabled(int oscillatorIndex) const;
  std::vector<RkRealPoint> oscillatorEvelopePoints(int oscillatorIndex,
                                                   EnvelopeType envelope) const;
  EnvelopeApplyType getOscillatorEnvelopeApplyType(int index,
						   EnvelopeType envelope) const;
  void addOscillatorEnvelopePoint(int oscillatorIndex,
                                  EnvelopeType envelope,
                                  const RkRealPoint &point);
  void removeOscillatorEvelopePoint(int oscillatorIndex,
                                    EnvelopeType envelope,
                                    int pointIndex);
  void updateOscillatorEvelopePoint(int oscillatorIndex,
                                    EnvelopeType envelope,
                                    int pointIndex,
                                    const RkRealPoint &point);
  FunctionType oscillatorFunction(int oscillatorIndex) const;
  gkick_real oscillatorPhase(int oscillatorIndex) const;
  int oscillatorSeed(int oscillatorIndex) const;
  void setOscillatorSample(const std::string &file,
                           int oscillatorIndex);
  void setOscillatorSample(const std::vector<float> &sample,
                           int oscillatorIndex);
  std::vector<float> getOscillatorSample(int oscillatorIndex) const;
  double kickMaxLength(void) const;
  double kickLength(void) const;
  double kickAmplitude() const;
  bool isKickFilterEnabled() const;
  double kickFilterFrequency(void) const;
  double kickFilterQFactor() const;
  FilterType kickFilterType() const;
  std::vector<RkRealPoint> getKickEnvelopePoints(EnvelopeType envelope) const;
  EnvelopeApplyType getKickEnvelopeApplyType(EnvelopeType envelope) const;
  bool setOscillatorFrequency(int oscillatorIndex,
                              double frequency);
  bool setOscillatorPitchShift(int oscillatorIndex,
                               double semitones);

  void setOscillatorAsFm(int oscillatorIndex, bool b);
  bool isOscillatorAsFm(int oscillatorIndex) const;
  double oscillatorAmplitude(int oscillatorIndex) const;
  double oscillatorFrequency(int oscillatorIndex) const;
  double oscillatorPitchShift(int oscillatorIndex) const;
  void addKickEnvelopePoint(EnvelopeType envelope,
                            double x,
                            double y);
  void removeKickEnvelopePoint(EnvelopeType envelope,
                               int pointIndex);
  void updateKickEnvelopePoint(EnvelopeType envelope,
                               int index,
                               double x,
                               double y);
  void setOscillatorEvelopePoints(int index,
                                  EnvelopeType envelope,
                                  const std::vector<RkRealPoint> &points);
  void setOscillatorEnvelopeApplyType(int index,
				      EnvelopeType envelope,
				      EnvelopeApplyType applyType);
  void setOscillatorFunction(int oscillatorIndex,
                             FunctionType function);
  void setOscillatorPhase(int oscillatorIndex,
                          gkick_real phase);
  void setOscillatorSeed(int oscillatorIndex,
                         int seed);
  void enableOscillator(int oscillatorIndex,
                        bool enable);
  void enableOscillatorFilter(int oscillatorIndex,
                              bool enable);
  bool isOscillatorFilterEnabled(int oscillatorIndex) const;
  void setOscillatorFilterType(int oscillatorIndex,
                               FilterType filter);
  FilterType getOscillatorFilterType(int oscillatorIndex) const;
  void setOscillatorFilterCutOffFreq(int oscillatorIndex,
                                     double frequency);
  double getOscillatorFilterCutOffFreq(int oscillatorIndex) const;
  void setOscillatorFilterFactor(int oscillatorIndex,
                                 double factor);
  double getOscillatorFilterFactor(int oscillatorIndex) const;
  bool setOscillatorAmplitude(int oscillatorIndex,
                              double amplitude);
  double limiterValue() const;
  int getSampleRate() const;
  static std::unique_ptr<KitState> getDefaultKitState();
  static std::shared_ptr<PercussionState> getDefaultPercussionState();
  // This function is called only from the audio thread.
  void setKeyPressed(bool b, int note, int velocity);

  // This function is called only from the audio thread.
  void process(float** out, size_t channel, size_t size);

  std::shared_ptr<PercussionState> getPercussionState(size_t id) const;
  std::shared_ptr<PercussionState> getPercussionState() const;
  bool isCompressorEnabled() const;
  double getCompressorAttack() const;
  double getCompressorRelease() const;
  double getCompressorThreshold() const;
  double getCompressorRatio() const;
  double getCompressorKnee() const;
  double getCompressorMakeup() const;
  bool isDistortionEnabled() const;
  double getDistortionInLimiter() const;
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
  void setPercussionState(const std::string &data);
  void setPercussionState(const std::shared_ptr<PercussionState> &state);
  std::unique_ptr<KitState> getKitState() const;
  bool setKitState(const std::string &data);
  bool setKitState(const std::unique_ptr<KitState> &state);
  void setKitName(const std::string &name);
  std::string getKitName() const;
  void setKitAuthor(const std::string &author);
  std::string getKitAuthor() const;
  void setKitUrl(const std::string &url);
  std::string getKitUrl() const;
  void setKickEnvelopePoints(EnvelopeType envelope,
                             const std::vector<RkRealPoint> &points);
  void setKickEnvelopeApplyType(EnvelopeType envelope,
				EnvelopeApplyType applyType);
  void playKick(int id = -1);
  void enableCompressor(bool enable);
  void setCompressorAttack(double attack);
  void setCompressorRelease(double release);
  void setCompressorThreshold(double threshold);
  void setCompressorRatio(double ratio);
  void setCompressorKnee(double knee);
  void setCompressorMakeup(double makeup);
  void enableDistortion(bool enable);
  void setDistortionVolume(double volume);
  void setDistortionInLimiter(double limit);
  void setDistortionDrive(double drive);
  std::vector<gkick_real> getKickBuffer() const;
  std::vector<gkick_real> getInstrumentBuffer(int id) const;
  void triggerSynthesis();
  void setLayer(Layer layer);
  Layer layer() const;
  void setLayerAmplitude(Layer layer, double amplitude);
  double getLayerAmplitude(Layer layer) const;
  void enbaleLayer(Layer layer, bool enable = true);
  bool isLayerEnabled(Layer layer) const;
  int getOscIndex(int index) const;
  double getLimiterLevelerValue(size_t index = -1) const;
  std::filesystem::path currentWorkingPath(const std::string &key) const;
  void setCurrentWorkingPath(const std::string &key,
                             const std::filesystem::path &path);
  void tuneAudioOutput(int id, bool tune);
  bool isAudioOutputTuned(int id) const;
  size_t getPercussionsNumber() const;
  bool setCurrentPercussion(int index);
  size_t currentPercussion() const;
  int getUnusedPercussion() const;
  bool enablePercussion(int index, bool enable = true);
  bool isPercussionEnabled(int index) const;
  size_t enabledPercussions() const;
  bool setPercussionPlayingKey(int index, int key);
  int getPercussionPlayingKey(int index) const;
  int percussionsReferenceKey() const;
  bool setPercussionChannel(int index, size_t channel);
  int getPercussionChannel(int index) const;
  bool setPercussionName(int index, const std::string &name);
  bool setPercussionLimiter(size_t id, double val);
  double percussionLimiter(size_t id) const;
  bool mutePercussion(size_t id, bool b);
  bool isPercussionMuted(size_t id) const;
  bool soloPercussion(size_t id, bool b);
  bool isPercussionSolo(size_t id) const;
  std::string getPercussionName(int index) const;
  void copyToClipboard();
  void pasteFromClipboard();
  void setScaleFactor(double factor);
  double getScaleFactor() const;

  RK_DECL_ACT(kickLengthUpdated,
              kickLengthUpdated(double val),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(val));
  RK_DECL_ACT(kickAmplitudeUpdated,
              kickAmplitudeUpdated(double val),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(val));
  RK_DECL_ACT(kickUpdated,
              kickUpdated(),
              RK_ARG_TYPE(),
              RK_ARG_VAL());
  RK_DECL_ACT(newKickBuffer,
              newKickBuffer(std::vector<gkick_real> buffer),
              RK_ARG_TYPE(std::vector<gkick_real>),
              RK_ARG_VAL(std::move(buffer)));
  RK_DECL_ACT(currentPlayingFrameVal,
              currentPlayingFrameVal(double val),
              RK_ARG_TYPE(double),
              RK_ARG_VAL(val));
  RK_DECL_ACT(stateChanged,
              stateChanged(),
              RK_ARG_TYPE(),
              RK_ARG_VAL());
  RK_DECL_ACT(kitUpdated,
              kitUpdated(),
              RK_ARG_TYPE(),
              RK_ARG_VAL());
  RK_DECL_ACT(percussionUpdated,
              percussionUpdated(int id),
              RK_ARG_TYPE(int),
              RK_ARG_VAL(id));

  void setSettings(const std::string &key, const std::string &value);
  std::string getSettings(const std::string &key) const;
  void notifyUpdateGraph();
  void notifyUpdateParameters();
  void notifyUpdateGui();
  void notifyPercussionUpdated(int id);
  void notifyKitUpdated();
  std::vector<int> ordredPercussionIds() const;
  void removeOrderedPercussionId(int id);
  void addOrderedPercussionId(int id);
  void clearOrderedPercussionIds();
  bool moveOrdrepedPercussionId(int index, int n);
  PresetFolder* getPresetFolder(size_t index) const;
  size_t numberOfPresetFolders() const;
  UiSettings* getUiSettings() const;
  void setState(const std::string &data);
  std::string getState() const;
  std::vector<gkick_real> setPreviewSample(const std::string &file);
  void playSamplePreview();
  void setSamplePreviewLimiter(double val);
  double samplePreviewLimiter() const;
  bool exportInstrumentBuffer(int id,
                              int sampleRate,
                              size_t channels,
                              const std::string &filePath);

protected:
  void setupPaths();
  void loadPresets();
  void loadPresetsFolders(const std::filesystem::path &path);
  static void kickUpdatedCallback(void *arg,
                                  gkick_real *buff,
                                  size_t size,
                                  size_t id);
  static void limiterCallback(void *arg, size_t index, gkick_real val);
  void updateKickBuffer(const std::vector<gkick_real> &&buffer, size_t id);
  void setOscillatorState(Layer layer,
                          OscillatorType oscillator,
                          const std::shared_ptr<PercussionState> &state);
  void getOscillatorState(Layer layer,
                          OscillatorType osc,
                          const std::shared_ptr<PercussionState> &state) const;
  void setLimiterLevelerValue(size_t index, double val);
  static std::vector<gkick_real> loadSample(const std::string &file,
                                            double length = 4.0,
                                            int sampleRate = 48000,
                                            int channels = 1);

private:
  mutable struct geonkick *geonkickApi;
  InstanceType instanceType;
  std::array<std::atomic<double>, GEONKICK_MAX_INSTRUMENTS> limiterLevelers;
  bool jackEnabled;
  bool standaloneInstance;
  mutable std::mutex apiMutex;
  RkEventQueue *eventQueue;
  std::vector<std::vector<gkick_real>> kickBuffers;
  mutable Layer currentLayer;
  std::string kitName;
  std::string kitAuthor;
  std::string kitUrl;
  std::shared_ptr<PercussionState> clipboardPercussion;

  /**
   * Current working paths for entire application.
   * Since on plugins the GUI is closed, there is a need to
   * store this path in the API instance.
   */
  std::unordered_map<std::string, std::filesystem::path> workingPaths;
  std::unordered_map<std::string, std::string> apiSettings;
  std::vector<int> percussionIdList;
  std::vector<std::unique_ptr<PresetFolder>> presetsFoldersList;
  std::unique_ptr<UiSettings> uiSettings;
  int sampleRate;
  double scaleFactor;
};

#endif // GEONKICK_API_H
