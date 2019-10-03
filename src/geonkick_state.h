/**
 * File name: geonkick_state.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

class GeonkickState
{
 public:

        GeonkickState();
        void loadData(const std::string &data);
        void setLimiterValue(double val);
        void setKickLength(double val);
        void setKickAmplitude(double val);
        void enableKickFilter(bool b);
        void setKickFilterFrequency(double val);
        void setKickFilterQFactor(double val);
        void setKickFilterType(GeonkickApi::FilterType type);
        void setKickEnvelopePoints(GeonkickApi::EnvelopeType envelope, const std::vector<RkRealPoint> &points);

        double getLimiterValue() const;
        double getKickLength() const;
        double getKickAmplitude() const;
        bool   isKickFilterEnabled() const;
        double getKickFilterFrequency() const;
        double getKickFilterQFactor() const;
        GeonkickApi::FilterType getKickFilterType() const;
        std::vector<RkRealPoint> getKickEnvelopePoints(GeonkickApi::EnvelopeType envelope) const;

        void setOscillatorEnabled(int index, bool b);
        void setOscillatorFunction(int index, GeonkickApi::FunctionType type);
        void setOscillatorPhase(int index, double phase);
        void setOscillatorAmplitue(int index, double val);
        void setOscillatorFrequency(int index, double val);
        void setOscillatorFilterEnabled(int index, bool b);
        void setOscillatorFilterType(int index, GeonkickApi::FilterType type);
        void setOscillatorFilterCutOffFreq(int index, double val);
        void setOscillatorFilterFactor(int index, double val);
        void setOscillatorEnvelopePoints(int index,
                                         const std::vector<RkRealPoint> &points,
                                         GeonkickApi::EnvelopeType envelope);

        bool isOscillatorAsFm(int index) const;
        void setOscillatorAsFm(int index, bool b);
        bool isOscillatorEnabled(int index) const;
        GeonkickApi::FunctionType oscillatorFunction(int index) const;
        double oscillatorAmplitue(int index) const;
        double oscillatorPhase(int index) const;
        double oscillatorFrequency(int index) const;
        bool isOscillatorFilterEnabled(int index) const;
        GeonkickApi::FilterType oscillatorFilterType(int index) const;
        double oscillatorFilterCutOffFreq(int index) const;
        double oscillatorFilterFactor(int index) const;
        std::vector<RkRealPoint> oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const;
        void setOscillatorSample(int oscillatorIndex, const std::vector<float> &sample);
        std::vector<float> getOscillatorSample(int oscillatorIndex) const;
        void enableCompressor(bool enable);
        bool isCompressorEnabled() const;
        void setCompressorAttack(double attack);
        void setCompressorRelease(double release);
        void setCompressorThreshold(double threshold);
        void setCompressorRatio(double ratio);
        void setCompressorKnee(double knee);
        void setCompressorMakeup(double makeup);
        double getCompressorAttack() const;
        double getCompressorRelease() const;
        double getCompressorThreshold() const;
        double getCompressorRatio() const;
        double getCompressorKnee() const;
        double getCompressorMakeup() const;

        void enableDistortion(bool enable);
        bool isDistortionEnabled() const;
        void setDistortionVolume(double volume);
        void setDistortionDrive(double drive);
        double getDistortionVolume() const;
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

 protected:
        void parseKickObject(const rapidjson::Value &kick);
        void parseOscillatorObject(int index,  const rapidjson::Value &osc);
        std::vector<RkRealPoint> parseEnvelopeArray(const rapidjson::Value &envelopeArray);
        void oscJson(std::ostringstream &jsonStream) const;
        void kickJson(std::ostringstream &jsonStream) const;

private:
        void initOscillators();
        struct OscillatorInfo {
              OscillatorInfo()
              : type{GeonkickApi::OscillatorType::Oscillator1}
                , isEnabled{false}
                , isFm{false}
                , function{GeonkickApi::FunctionType::Sine}
                , phase{0}
                , amplitude{0.8}
                , frequency{200}
                , isFilterEnabled{false}
                , filterType{GeonkickApi::FilterType::LowPass}
                , filterFrequency{200}
                , filterFactor{1.0} {}
                GeonkickApi::OscillatorType type;
                std::vector<float> sample;
                bool isEnabled;
                bool isFm;
                GeonkickApi::FunctionType function;
                double phase;
                double amplitude;
                double frequency;
                bool isFilterEnabled;
                GeonkickApi::FilterType filterType;
                double filterFrequency;
                double filterFactor;
                std::vector<RkRealPoint> amplitudeEnvelope;
                std::vector<RkRealPoint> frequencyEnvelope;
                std::vector<RkRealPoint> filterCutOffEnvelope;
        };

        std::shared_ptr<OscillatorInfo> getOscillator(int index) const;

        struct Compressor {
                bool enabled;
                double attack;
                double release;
                double threshold;
                double ratio;
                double knee;
                double makeup;
        };

        struct Distortion {
                bool enabled;
                double volume;
                double drive;
        };

        double limiterValue;
        double kickLength;
        double kickAmplitude;
        bool   kickFilterEnabled;
        double kickFilterFrequency;
        double kickFilterQFactor;
        GeonkickApi::FilterType kickFilterType;
        std::vector<RkRealPoint> kickFilterCutOffEnvelope;
        std::vector<RkRealPoint> kickEnvelopePoints;
        std::unordered_map<int, std::shared_ptr<OscillatorInfo>> oscillators;
        Compressor compressor;
        Distortion distortion;
        std::vector<bool> layers;
        std::vector<double> layersAmplitude;
        GeonkickApi::Layer currentLayer;
        bool tunedOutput;
};

#endif // GEONKICK_STATE_H
