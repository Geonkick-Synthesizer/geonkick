/**
 * File name: geonkick_state.cpp
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

#include "geonkick_state.h"
#include "base64.h"

#include <iomanip>

GeonkickState::GeonkickState() :
        limiterValue{0}
        , kickLength{50}
        , kickAmplitude{0.8}
        , kickFilterEnabled{0}
        , kickFilterFrequency{200}
        , kickFilterQFactor{1.0}
        , kickFilterType{GeonkickApi::FilterType::LowPass}
        , compressor{false, 0, 0, 0, 0, 0, 0}
        , distortion{false, 0, 0}
        , layers{false, false, false}
        , layersAmplitude{1.0, 1.0, 1.0}
        , currentLayer{GeonkickApi::Layer::Layer1}
        , tunedOutput{false}
{
        initOscillators();
}

void GeonkickState::loadData(const std::string &data)
{
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "kick" && m.value.IsObject())
                                parseKickObject(m.value);
                        for (decltype(layers.size()) i = 0; i < layers.size(); i++) {
                                setCurrentLayer(static_cast<GeonkickApi::Layer>(i));
                                if (m.name == ("osc" + std::to_string(0 + i * GKICK_OSC_GROUP_SIZE)).c_str())
                                        parseOscillatorObject(0, m.value);
                                if (m.name == ("osc" + std::to_string(1 + i * GKICK_OSC_GROUP_SIZE)).c_str())
                                        parseOscillatorObject(1, m.value);
                                if (m.name == ("osc" + std::to_string(2 + i * GKICK_OSC_GROUP_SIZE)).c_str())
                                        parseOscillatorObject(2, m.value);
                        }
                }
        }

}

void GeonkickState::initOscillators()
{
        for (decltype(layers.size()) i = 0; i < layers.size(); i++) {
                oscillators.insert({static_cast<int>(GeonkickApi::OscillatorType::Oscillator1)
                                        + GKICK_OSC_GROUP_SIZE * i,
                                        std::make_shared<OscillatorInfo>()});
                oscillators.insert({static_cast<int>(GeonkickApi::OscillatorType::Oscillator2)
                                        + GKICK_OSC_GROUP_SIZE * i,
                                        std::make_shared<OscillatorInfo>()});
                oscillators.insert({static_cast<int>(GeonkickApi::OscillatorType::Noise)
                                        + GKICK_OSC_GROUP_SIZE * i,
                                        std::make_shared<OscillatorInfo>()});
        }
}

void GeonkickState::parseKickObject(const rapidjson::Value &kick)
{
        if (kick.IsNull() || !kick.IsObject())
                return;

        for (const auto &m: kick.GetObject()) {
                if (m.name == "limiter" && m.value.IsDouble())
                        setLimiterValue(m.value.GetDouble());

                if (m.name == "tuned_output" && m.value.IsBool())
                        tuneOutput(m.value.GetBool());

                if (m.name == "layers" && m.value.IsArray()) {
                        layers = {false, false, false};
                        for (const auto &el: m.value.GetArray())
                                if (el.IsInt())
                                        setLayerEnabled(static_cast<GeonkickApi::Layer>(el.GetInt()), true);
                }

                if (m.name == "layers_amplitude" && m.value.IsArray()) {
                        layersAmplitude = {1.0, 1.0, 1.0};
                        int i = 0;
                        for (const auto &el: m.value.GetArray()) {
                                if (el.IsDouble())
                                        setLayerAmplitude(static_cast<GeonkickApi::Layer>(i), el.GetDouble());
                                i++;
                        }
                }


                if (m.name == "ampl_env" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "length" && el.value.IsDouble())
                                        setKickLength(el.value.GetDouble());
                                if (el.name == "amplitude" && el.value.IsDouble())
                                        setKickAmplitude(el.value.GetDouble());
                                if (el.name == "points" && el.value.IsArray()) {
                                        setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude,
                                                              parseEnvelopeArray(el.value));
                                }
                        }
                }

                if (m.name == "filter" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        enableKickFilter(el.value.GetBool());
                                if (el.name == "cutoff" && el.value.IsDouble())
                                        setKickFilterFrequency(el.value.GetDouble());
                                if (el.name == "factor" && el.value.IsDouble())
                                        setKickFilterQFactor(el.value.GetDouble());
                                if (el.name == "type" && el.value.IsInt())
                                        setKickFilterType(static_cast<GeonkickApi::FilterType>(el.value.GetInt()));
                                if (el.name == "cutoff_env" && el.value.IsArray()) {
                                        setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                                                              parseEnvelopeArray(el.value));
                                }
                        }
                }

                if (m.name == "compressor" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        enableCompressor(el.value.GetBool());
                                if (el.name == "attack" && el.value.IsDouble())
                                        setCompressorAttack(el.value.GetDouble());
                                if (el.name == "release" && el.value.IsDouble())
                                        setCompressorRelease(el.value.GetDouble());
                                if (el.name == "threshold" && el.value.IsDouble())
                                        setCompressorThreshold(el.value.GetDouble());
                                if (el.name == "ratio" && el.value.IsDouble())
                                        setCompressorRatio(el.value.GetDouble());
                                if (el.name == "knee" && el.value.IsDouble())
                                        setCompressorKnee(el.value.GetDouble());
                                if (el.name == "makeup" && el.value.IsDouble())
                                        setCompressorMakeup(el.value.GetDouble());

                        }
                }

                if (m.name == "distortion" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        enableDistortion(el.value.GetBool());
                                if (el.name == "volume" && el.value.IsDouble())
                                        setDistortionVolume(el.value.GetDouble());
                                if (el.name == "drive" && el.value.IsDouble())
                                        setDistortionDrive(el.value.GetDouble());
                        }
                }
        }
}

void GeonkickState::parseOscillatorObject(int index,  const rapidjson::Value &osc)
{
        if (osc.IsNull() || !osc.IsObject())
                return;

        for (const auto &m: osc.GetObject()) {
                if (m.name == "enabled" && m.value.IsBool())
                        setOscillatorEnabled(index, m.value.GetBool());
                if (m.name == "is_fm" && m.value.IsBool())
                        setOscillatorAsFm(index, m.value.GetBool());
                if (m.name == "sample" && m.value.IsString())
                        setOscillatorSample(index, fromBase64F(std::string(m.value.GetString())));
                if (m.name == "function" && m.value.IsInt())
                        setOscillatorFunction(index, static_cast<GeonkickApi::FunctionType>(m.value.GetInt()));
                if (m.name == "phase" && m.value.IsDouble())
                        setOscillatorPhase(index, m.value.GetDouble());
                if (m.name == "ampl_env" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "amplitude" && el.value.IsDouble())
                                        setOscillatorAmplitue(index, el.value.GetDouble());
                                if (el.name == "points" && el.value.IsArray())
                                        setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                    GeonkickApi::EnvelopeType::Amplitude);
                        }
                }

                if (static_cast<GeonkickApi::OscillatorType>(index) != GeonkickApi::OscillatorType::Noise) {
                        if (m.name == "freq_env" && m.value.IsObject()) {
                                for (const auto &el: m.value.GetObject()) {
                                        if (el.name == "amplitude" && el.value.IsDouble())
                                                setOscillatorFrequency(index, el.value.GetDouble());
                                        if (el.name == "points" && el.value.IsArray())
                                                setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                            GeonkickApi::EnvelopeType::Frequency);
                                }
                        }
                }

                if (m.name == "filter" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        setOscillatorFilterEnabled(index, el.value.GetBool());
                                if (el.name == "cutoff" && el.value.IsDouble())
                                        setOscillatorFilterCutOffFreq(index, el.value.GetDouble());
                                if (el.name == "factor" && el.value.IsDouble())
                                        setOscillatorFilterFactor(index, el.value.GetDouble());
                                if (el.name == "type" && el.value.IsInt())
                                        setOscillatorFilterType(index, static_cast<GeonkickApi::FilterType>(el.value.GetInt()));
                                if (el.name == "cutoff_env" && el.value.IsArray()) {
                                        setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                    GeonkickApi::EnvelopeType::FilterCutOff);
                                }

                        }
                }
        }
}

std::vector<RkRealPoint> GeonkickState::parseEnvelopeArray(const rapidjson::Value &envelopeArray)
{
        std::vector<RkRealPoint> points;
        for (const auto &el: envelopeArray.GetArray())
                if (el.IsArray() && el.GetArray().Size() == 2)
                        points.emplace_back(RkRealPoint(el.GetArray()[0].GetDouble(), el.GetArray()[1].GetDouble()));

        return points;
}

void GeonkickState::setLimiterValue(double val)
{
        limiterValue = val;
}

void GeonkickState::setKickLength(double val)
{
        kickLength = val;
}

void GeonkickState::setKickAmplitude(double val)
{
        kickAmplitude = val;
}

void GeonkickState::enableKickFilter(bool b)
{
        kickFilterEnabled = b;
}

void GeonkickState::setKickFilterFrequency(double val)
{
        kickFilterFrequency = val;
}

void GeonkickState::setKickFilterQFactor(double val)
{
        kickFilterQFactor = val;
}

void GeonkickState::setKickFilterType(GeonkickApi::FilterType type)
{
        kickFilterType = type;
}

void GeonkickState::setKickEnvelopePoints(GeonkickApi::EnvelopeType envelope, const std::vector<RkRealPoint> &points)
{
        if (envelope == GeonkickApi::EnvelopeType::Amplitude)
                kickEnvelopePoints = points;
        else if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
                kickFilterCutOffEnvelope = points;
}

double GeonkickState::getLimiterValue() const
{
        return limiterValue;
}

double GeonkickState::getKickLength() const
{
        return kickLength;
}

double GeonkickState::getKickAmplitude() const
{
        return kickAmplitude;
}

bool GeonkickState::isKickFilterEnabled() const
{
        return kickFilterEnabled;
}

double GeonkickState::getKickFilterFrequency() const
{
        return kickFilterFrequency;
}

double GeonkickState::getKickFilterQFactor() const
{
        return kickFilterQFactor;
}

GeonkickApi::FilterType GeonkickState::getKickFilterType() const
{
        return kickFilterType;
}

std::vector<RkRealPoint> GeonkickState::getKickEnvelopePoints(GeonkickApi::EnvelopeType envelope) const
{
        if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
                return kickFilterCutOffEnvelope;
        else
                return kickEnvelopePoints;
}

std::shared_ptr<GeonkickState::OscillatorInfo> GeonkickState::getOscillator(int index) const
{
        index += GKICK_OSC_GROUP_SIZE * static_cast<int>(currentLayer);
        auto it = oscillators.find(index);
        if (it != oscillators.end())
                return it->second;

        return nullptr;
}

void GeonkickState::setOscillatorEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->isEnabled = b;
}

void GeonkickState::setOscillatorFunction(int index, GeonkickApi::FunctionType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->function = type;
}

void GeonkickState::setOscillatorPhase(int index, double phase)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->phase = phase;
}

void GeonkickState::setOscillatorAmplitue(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->amplitude = val;
}

void GeonkickState::setOscillatorFrequency(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->frequency = val;
}

void GeonkickState::setOscillatorFilterEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->isFilterEnabled = b;
}

void GeonkickState::setOscillatorFilterType(int index, GeonkickApi::FilterType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterType = type;
}

void GeonkickState::setOscillatorFilterCutOffFreq(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterFrequency = val;
}

void GeonkickState::setOscillatorFilterFactor(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterFactor = val;
}

void GeonkickState::setOscillatorEnvelopePoints(int index,
                                                const std::vector<RkRealPoint> &points,
                                                GeonkickApi::EnvelopeType envelope)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                if (envelope == GeonkickApi::EnvelopeType::Amplitude)
                        oscillator->amplitudeEnvelope = points;
                else if (envelope == GeonkickApi::EnvelopeType::Frequency)
                        oscillator->frequencyEnvelope = points;
                else
                        oscillator->filterCutOffEnvelope = points;
        }
}

bool GeonkickState::isOscillatorAsFm(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->isFm;
        return false;
}

void GeonkickState::setOscillatorAsFm(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
               oscillator->isFm = b;
}

bool GeonkickState::isOscillatorEnabled(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->isEnabled;

        return false;
}

GeonkickApi::FunctionType GeonkickState::oscillatorFunction(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->function;

        return GeonkickApi::FunctionType::Sine;
}

double GeonkickState::oscillatorPhase(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->phase;

        return 0;
}

double GeonkickState::oscillatorAmplitue(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->amplitude;
        return 0;
}

double GeonkickState::oscillatorFrequency(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->frequency;
        return 0;
}

bool GeonkickState::isOscillatorFilterEnabled(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->isFilterEnabled;
        return false;
}

GeonkickApi::FilterType GeonkickState::oscillatorFilterType(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterType;
        return GeonkickApi::FilterType::LowPass;
}

double GeonkickState::oscillatorFilterCutOffFreq(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterFrequency;
        return 0;
}

double GeonkickState::oscillatorFilterFactor(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterFactor;
        return 0;
}

std::vector<RkRealPoint> GeonkickState::oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                if (type == GeonkickApi::EnvelopeType::Amplitude)
                        return oscillator->amplitudeEnvelope;
                else if (type == GeonkickApi::EnvelopeType::Frequency)
                        return oscillator->frequencyEnvelope;
                else
                        return oscillator->filterCutOffEnvelope;
        }

        return {};
}

void GeonkickState::enableCompressor(bool enable)
{
        compressor.enabled = enable;
}

bool GeonkickState::isCompressorEnabled() const
{
        return compressor.enabled;
}

void GeonkickState::setCompressorAttack(double attack)
{
        compressor.attack = attack;
}

void GeonkickState::setCompressorRelease(double release)
{
        compressor.release = release;
}

void GeonkickState::setCompressorThreshold(double threshold)
{
        compressor.threshold = threshold;
}

void GeonkickState::setCompressorRatio(double ratio)
{
        compressor.ratio = ratio;
}

void GeonkickState::setCompressorKnee(double knee)
{
        compressor.knee = knee;
}

void GeonkickState::setCompressorMakeup(double makeup)
{
        compressor.makeup = makeup;
}

double GeonkickState::getCompressorAttack() const
{
        return compressor.attack;
}

double GeonkickState::getCompressorRelease() const
{
        return compressor.release;
}

double GeonkickState::getCompressorThreshold() const
{
        return compressor.threshold;
}

double GeonkickState::getCompressorRatio() const
{
        return compressor.ratio;
}

double GeonkickState::getCompressorKnee() const
{
        return compressor.knee;
}

double GeonkickState::getCompressorMakeup() const
{
        return compressor.makeup;
}

void GeonkickState::enableDistortion(bool enable)
{
        distortion.enabled = enable;
}

bool GeonkickState::isDistortionEnabled() const
{
        return distortion.enabled;
}

void GeonkickState::setDistortionVolume(double volume)
{
        distortion.volume = volume;
}

void GeonkickState::setDistortionDrive(double drive)
{
        distortion.drive = drive;
}

double GeonkickState::getDistortionVolume() const
{
        return distortion.volume;
}

double GeonkickState::getDistortionDrive() const
{
        return distortion.drive;
}

std::string GeonkickState::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        oscJson(jsonStream);
        kickJson(jsonStream);
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

void GeonkickState::oscJson(std::ostringstream &jsonStream) const
{
        for (const auto& val: oscillators) {
                jsonStream << "\"osc" << val.first << "\": {" << std::endl;
                jsonStream << "\"enabled\": " << (val.second->isEnabled ? "true" : "false") << ", " << std::endl;
                jsonStream << "\"is_fm\": " << (val.second->isFm ? "true" : "false") << ", " << std::endl;
                if (val.second->function == GeonkickApi::FunctionType::Sample && !val.second->sample.empty())
                        jsonStream <<  "\"sample\": \"" << toBase64F(val.second->sample) << "\"," << std::endl;
                jsonStream <<  "\"function\": " << static_cast<int>(val.second->function) << "," << std::endl;
                jsonStream <<  "\"phase\": " << std::fixed << std::setprecision(5) << val.second->phase << ", " << std::endl;
                jsonStream << "\"ampl_env\": {" << std::endl;
                jsonStream << "\"amplitude\": "  << std::fixed << std::setprecision(5) << val.second->amplitude << ", " << std::endl;
                jsonStream << "\"points\": [" << std::endl;
                bool first = true;
                for (const auto &point: val.second->amplitudeEnvelope) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                                   << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
                }
                jsonStream << "]" << std::endl; // points
                jsonStream << "}," << std::endl; // ampl_env

                jsonStream << "\"freq_env\": {" << std::endl;
                jsonStream << "\"amplitude\": " << val.second->frequency << ", " << std::endl;
                jsonStream << "\"points\": [" << std::endl;
                first = true;
                for (const auto &point: val.second->frequencyEnvelope) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                                   << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
                }
                jsonStream << "]" << std::endl; // points
                jsonStream << "}," << std::endl; // freq_env
                jsonStream << "\"filter\": {" << std::endl;
                jsonStream << "\"enabled\": " << (val.second->isFilterEnabled ? "true" : "false");
                jsonStream << ", " << std::endl;
                jsonStream << "\"type\": " << static_cast<int>(val.second->filterType) << ", " << std::endl;
                jsonStream << "\"cutoff\": " << std::fixed << std::setprecision(5) << val.second->filterFrequency << ", " << std::endl;
                jsonStream << "\"cutoff_env\": [";
                first = true;
                for (const auto &point: val.second->filterCutOffEnvelope) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                                   << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
                }
                jsonStream << "], " << std::endl;
                jsonStream << "\"factor\": " << std::fixed << std::setprecision(5) << val.second->filterFactor << std::endl;
                jsonStream << "}" << std::endl;  // filter;
                jsonStream << "}" << std::endl;  // osc;
                jsonStream << "," << std::endl;
        }
}

void GeonkickState::kickJson(std::ostringstream &jsonStream) const
{
        jsonStream << "\"kick\": {" << std::endl;
        jsonStream << "\"layers\": [";
        bool first = true;
        for (decltype(layers.size()) i = 0; i < layers.size(); i++) {
                if (layers[i]) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << i;
                }
        }
        jsonStream << "]," << std::endl;

        jsonStream << "\"layers_amplitude\": [";
        first = true;
        for (decltype(layersAmplitude.size()) i = 0; i < layersAmplitude.size(); i++) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << std::fixed << std::setprecision(5) << layersAmplitude[i];
        }
        jsonStream << "]," << std::endl;

        jsonStream << "\"limiter\": " << std::fixed << std::setprecision(5) << getLimiterValue() << ", " << std::endl;
        jsonStream << "\"tuned_output\": " << (isOutputTuned() ? "true" : "false") << ", " << std::endl;
        jsonStream << "\"ampl_env\": {" << std::endl;
        jsonStream << "\"amplitude\": " << static_cast<double>(getKickAmplitude()) << ", " << std::endl;
        jsonStream << "\"length\": " << static_cast<double>(getKickLength()) << ", " << std::endl;
        auto points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude);
        jsonStream << "\"points\": [";
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                           << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
        }

        jsonStream << "]}, " << std::endl;

        jsonStream << "\"filter\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isKickFilterEnabled() ? "true" : "false");
        jsonStream << ", " << std::endl;
        jsonStream << "\"type\": " << static_cast<int>(getKickFilterType()) << ", " << std::endl;
        jsonStream << "\"cutoff\": " << std::fixed << std::setprecision(2) << getKickFilterFrequency() << ", " << std::endl;
        jsonStream << "\"factor\": " << std::fixed << std::setprecision(2) << getKickFilterQFactor() << ", " << std::endl;
        points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff);
        jsonStream << "\"cutoff_env\": [";
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                           << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
        }
        jsonStream << "]" << std::endl; // points
        jsonStream << "}, " << std::endl;  // filter;
        jsonStream << "\"compressor\": {" << std::endl;
        jsonStream << "\"enabled\": " << isCompressorEnabled() << ", " << std::endl;
        jsonStream << "\"attack\": " << std::fixed << std::setprecision(5) << getCompressorAttack() << ", " << std::endl;
        jsonStream << "\"release\": " << std::fixed << std::setprecision(5) << getCompressorRelease() << ", " << std::endl;
        jsonStream << "\"threshold\": " << std::fixed << std::setprecision(5) << getCompressorThreshold() << ", " << std::endl;
        jsonStream << "\"ratio\": " << std::fixed << std::setprecision(5) << getCompressorRatio() << ", " << std::endl;
        jsonStream << "\"knee\": " << std::fixed << std::setprecision(5) << getCompressorKnee() << ", " << std::endl;
        jsonStream << "\"makeup\": " << std::fixed << std::setprecision(5) << getCompressorMakeup() << std::endl;
        jsonStream << "}, " << std::endl;

        jsonStream << "\"distortion\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isDistortionEnabled() ? "true" : "false") << ", " << std::endl;
        jsonStream << "\"volume\": " << std::fixed << std::setprecision(5) << getDistortionVolume()  << ", " << std::endl;
        jsonStream << "\"drive\": " << std::fixed << std::setprecision(5) << getDistortionDrive() << std::endl;
        jsonStream << "}" << std::endl; // distortion
        jsonStream << "}" << std::endl; // kick
}

void GeonkickState::setLayerEnabled(GeonkickApi::Layer layer, bool b)
{
        auto index = static_cast<decltype(layers.size())>(layer);
        if (index >= 0 && index < layers.size())
                layers[index] = b;
}

bool GeonkickState::isLayerEnabled(GeonkickApi::Layer layer) const
{
        auto index = static_cast<decltype(layers.size())>(layer);
        if (index >= 0 && index < layers.size())
                return layers[index];
        else
                return false;
}

void GeonkickState::setCurrentLayer(GeonkickApi::Layer layer)
{
        currentLayer = layer;
}

void GeonkickState::setLayerAmplitude(GeonkickApi::Layer layer, double amplitude)
{
        auto index = static_cast<decltype(layersAmplitude.size())>(layer);
        if (index >= 0 && index < layersAmplitude.size())
                layersAmplitude[index] = amplitude;
}

double GeonkickState::getLayerAmplitude(GeonkickApi::Layer layer) const
{
        auto index = static_cast<decltype(layersAmplitude.size())>(layer);
        if (index >= 0 && index < layersAmplitude.size())
                return layersAmplitude[index];
        return 0;
}

void GeonkickState::tuneOutput(bool tune)
{
        tunedOutput = tune;
}

bool GeonkickState::isOutputTuned() const
{
        return tunedOutput;
}

std::vector<float> GeonkickState::fromBase64F(const std::string &str)
{
        size_t len;
        auto data_str = base64_decode(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), &len);
        if (data_str && len > sizeof(float)) {
                std::vector<float> data(reinterpret_cast<float*>(data_str),
                                        reinterpret_cast<float*>(data_str) + len / sizeof(float));
                free(data_str);
                return data;
        }
        return {};
}

std::string GeonkickState::toBase64F(const std::vector<float> &data)
{
        size_t len;
        auto base64 = base64_encode(reinterpret_cast<const unsigned char*>(data.data()),
                                    data.size() * sizeof(float), &len);
        if (base64  && len > 0) {
                auto str = std::move(std::string(reinterpret_cast<const char*>(base64), len));
                free(base64);
                str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
                return str;
        }
        return {};
}

void GeonkickState::setOscillatorSample(int oscillatorIndex, const std::vector<float> &sample)
{
        auto oscillator = getOscillator(oscillatorIndex);
        if (oscillator)
                oscillator->sample = sample;
}

std::vector<float> GeonkickState::getOscillatorSample(int oscillatorIndex) const
{
        auto oscillator = getOscillator(oscillatorIndex);
        return oscillator->sample;
}
