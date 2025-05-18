/**
 * File name: percussion_state.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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

#ifndef GEONKICK_STATE_H
#define GEONKICK_STATE_H

#include "geonkick_api.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <unordered_map>

struct DistortionInfo {
        using DistortionType = GeonkickApi::DistortionType;
        bool toJson(std::ostringstream &jsonStream) const;
        bool fromObject(const auto& obj);
        bool enabled = false;
        DistortionType type = DistortionType::SoftClippingTan;
        double in_limiter = 1.0;
        double out_limiter = 1.0;
        double drive = 1.0;
        std::vector<EnvelopePoint> driveEnvelope = {{0, 1}, {1, 1}};
        std::vector<EnvelopePoint> outLimiterEnvelope = {{0, 1}, {1, 1}};
};

struct OscillatorInfo {
        OscillatorInfo()
                : type{GeonkickApi::OscillatorType::Oscillator1}
                , isEnabled{false}
                , isFm{false}
                , fmK{10.0}
                , function{GeonkickApi::FunctionType::Sine}
                , phase{0}
                , seed{0}
                , amplitude{0.8}
                , frequency{200}
                , pitchShift{0}
                , noiseDensity{1.0}
                , isFilterEnabled{false}
                , filterType{GeonkickApi::FilterType::LowPass}
                , filterFrequency{200}
                , filterFactor{1.0}
	        , frequencyEnvelopeApplyType{GeonkickApi::EnvelopeApplyType::Logarithmic}
	        , cutOffEnvelopeApplyType{GeonkickApi::EnvelopeApplyType::Logarithmic}
                , noiseDensityEnvelope{{0, 1}, {1, 1}} {}
        GeonkickApi::OscillatorType type;
        std::vector<float> sample;
        bool isEnabled;
        bool isFm;
        double fmK;
        GeonkickApi::FunctionType function;
        double phase;
        int seed;
        double amplitude;
        double frequency;
        double pitchShift;
        double noiseDensity;
        bool isFilterEnabled;
        GeonkickApi::FilterType filterType;
        double filterFrequency;
        double filterFactor;
        DistortionInfo distortion;
        std::vector<EnvelopePoint> amplitudeEnvelope;
        GeonkickApi::EnvelopeApplyType frequencyEnvelopeApplyType;
        std::vector<EnvelopePoint> frequencyEnvelope;
        GeonkickApi::EnvelopeApplyType cutOffEnvelopeApplyType;
        std::vector<EnvelopePoint> filterCutOffEnvelope;
        std::vector<EnvelopePoint> filterQFactorEnvelope;
        std::vector<EnvelopePoint> pitchShiftEnvelope;
        std::vector<EnvelopePoint> noiseDensityEnvelope;
};

class PercussionState
{
 public:
        using DistortionType = DistortionInfo::DistortionType;
        PercussionState();
        bool loadFile(const std::string &file);
        bool loadData(const std::string &data);
        bool loadObject(const rapidjson::Value &obj);
        size_t getId() const;
        void setId(size_t id);
        void setChannel(size_t channel);
        size_t getChannel() const;
        void setMidiChannel(signed char channel);
        signed char getMidiChannel() const;
        void setNoteOffEnabled(bool b = true);
        bool isNoteOffEnabled() const;
        void setMute(bool b);
        bool isMuted() const;
        void setSolo(bool b);
        bool isSolo() const;
        const std::string& getName() const;
        void setName(const std::string &name);
        signed char getPlayingKey() const;
        void setPlayingKey(signed char key);
        bool isEnabled() const;
        void enable(bool b);
        void setLimiterValue(double val);
        void setKickLength(double val);
        void setKickAmplitude(double val);
        void enableKickFilter(bool b);
        void setKickFilterFrequency(double val);
        void setKickFilterQFactor(double val);
        void setKickFilterType(GeonkickApi::FilterType type);
        void setKickEnvelopePoints(GeonkickApi::EnvelopeType envelope,
                                   const std::vector<EnvelopePoint> &points);
	void setKickEnvelopeApplyType(GeonkickApi::EnvelopeType envelope,
				      GeonkickApi::EnvelopeApplyType applyType);

        double getLimiterValue() const;
        double getKickLength() const;
        double getKickAmplitude() const;
        bool   isKickFilterEnabled() const;
        double getKickFilterFrequency() const;
        double getKickFilterQFactor() const;
        GeonkickApi::FilterType getKickFilterType() const;
        std::vector<EnvelopePoint> getKickEnvelopePoints(GeonkickApi::EnvelopeType envelope) const;
	GeonkickApi::EnvelopeApplyType getKickEnvelopeApplyType(GeonkickApi::EnvelopeType envelope) const;
        void setOscillatorEnabled(int index, bool b);
        void setOscillatorFunction(int index, GeonkickApi::FunctionType type);
        void setOscillatorPhase(int index, double phase);
        void setOscillatorSeed(int index, int seed);
        void setOscillatorAmplitue(int index, double val);
        void setOscillatorFrequency(int index, double val);
        void setOscillatorPitchShift(int index, double val);
        void setOscillatorNoiseDensity(int index, double val);
        void setOscillatorFilterEnabled(int index, bool b);
        void setOscillatorFilterType(int index, GeonkickApi::FilterType type);
        void setOscillatorFilterCutOffFreq(int index, double val);
        void setOscillatorFilterFactor(int index, double val);
	void setOscillatorEnvelopeApplyType(int index,
					    GeonkickApi::EnvelopeType envelopeType,
					    GeonkickApi::EnvelopeApplyType applyType);
	GeonkickApi::EnvelopeApplyType getOscillatorEnvelopeApplyType(int index,
								      GeonkickApi::EnvelopeType envelope) const;
        void setOscillatorEnvelopePoints(int index,
                                         const std::vector<EnvelopePoint> &points,
                                         GeonkickApi::EnvelopeType envelope);
        bool isOscillatorAsFm(int index) const;
        void setOscillatorAsFm(int index, bool b);
        double getOscillatorFmK(int index) const;
        bool isOscillatorEnabled(int index) const;
        GeonkickApi::FunctionType oscillatorFunction(int index) const;
        double oscillatorAmplitue(int index) const;
        double oscillatorPhase(int index) const;
        int oscillatorSeed(int index) const;
        double oscillatorFrequency(int index) const;
        double oscillatorPitchShift(int index) const;
        double oscillatorNoiseDensity(int index) const;
        bool isOscillatorFilterEnabled(int index) const;
        GeonkickApi::FilterType oscillatorFilterType(int index) const;
        double oscillatorFilterCutOffFreq(int index) const;
        double oscillatorFilterFactor(int index) const;
        std::vector<EnvelopePoint> oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const;
        void setOscillatorSample(int oscillatorIndex,
                                 const std::vector<float> &sample);
        std::vector<float> getOscillatorSample(int oscillatorIndex) const;
        void setOscDistortionEnabled(int index, bool b = true);
        void setOscDistortionType(int index, DistortionType type);
        void setOscDistortionInLimiter(int index, double val);
        void setOscDistortionOutLimiter(int index, double val);
        void setOscDistortionDrive(int index, double val);
        bool isOscDistortionEnabled(int index) const;
        DistortionType getOscDistortionType(int index) const;
        double getOscDistortionInLimiter(int index) const;
        double getOscDistortionOutLimiter(int index) const;
        double getOscDistortionDrive(int index) const;
        void enableDistortion(bool enable);
        bool isDistortionEnabled() const;
        void setDistortionType(DistortionType type);
        DistortionType getDistortionType() const;
        void setDistortionOutLimiter(double volume);
        void setDistortionInLimiter(double limit);
        void setDistortionDrive(double drive);
        double getDistortionInLimiter() const;
        double getDistortionOutLimiter() const;
        double getDistortionDrive() const;
        std::string toJson() const;
        void setLayerEnabled(GeonkickApi::Layer layer, bool b);
        bool isLayerEnabled(GeonkickApi::Layer layer) const;
        void setCurrentLayer(GeonkickApi::Layer layer);
        void setLayerAmplitude(GeonkickApi::Layer layer, double amplitude);
        double getLayerAmplitude(GeonkickApi::Layer layer) const;
        void tuneOutput(bool tune);
        bool isOutputTuned() const;
        static std::vector<float> fromBase64F(const std::string &str);
        static std::string toBase64F(const std::vector<float> &data);
        bool save(const std::string &fileName);
        static std::vector<EnvelopePoint> parseEnvelopeArray(const rapidjson::Value &envelopeArray);

 protected:
        void parseKickObject(const rapidjson::Value &kick);
        void parseOscillatorObject(int index,  const rapidjson::Value &osc);
        static void envelopeToJson(std::ostringstream &jsonStream,
                                   const std::string &envName,
                                   double amplitude,
                                   const std::vector<EnvelopePoint> &envelope,
				   GeonkickApi::EnvelopeApplyType applyType = GeonkickApi::EnvelopeApplyType::Linear);
        void oscJson(std::ostringstream &jsonStream) const;
        void kickJson(std::ostringstream &jsonStream) const;
	GeonkickApi::EnvelopeApplyType getApplyTypeFromObj(const rapidjson::Value &obj) const;
        void setOscillatorFmK(int index, double k);

private:
        void initOscillators();
        OscillatorInfo* getOscillator(int index);
        const OscillatorInfo* getConstOscillator(int index) const;

        int appVersion;
        size_t kickId;
        std::string kickName;
        signed char playingKey;
        size_t outputChannel;
        signed char outputMidiChannel;
        bool kickEnabled;
        bool percussionMuted;
        bool percussionSolo;
        bool noteOffEnabled;
        double limiterValue;
        double kickLength;
        double kickAmplitude;
        bool   kickFilterEnabled;
        double kickFilterFrequency;
        double kickFilterQFactor;
        GeonkickApi::FilterType kickFilterType;
	GeonkickApi::EnvelopeApplyType kickCutOffEnvelopeApplyType;
        std::vector<EnvelopePoint> kickFilterCutOffEnvelope;
	std::vector<EnvelopePoint> kickFilterQFactorEnvelope;
        std::vector<EnvelopePoint> kickEnvelopePoints;
        std::unordered_map<int, OscillatorInfo> oscillators;
        DistortionInfo instrumentDistortion;
        std::vector<bool> layers;
        std::vector<double> layersAmplitude;
        GeonkickApi::Layer currentLayer;
        bool tunedOutput;
};

#endif // GEONKICK_STATE_H
