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

#include <iomanip>

GeonkickState::GeonkickState() :
        limiterValue{0}
        , kickLength{0}
        , kickAmplitude{0}
        , kickFilterEnabled{0}
        , kickFilterFrequency{0}
        , kickFilterQFactor{0}
        , kickFilterType{GeonkickApi::FilterType::LowPass}
        , oscillators{
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator1), std::make_shared<OscillatorInfo>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator2), std::make_shared<OscillatorInfo>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Noise), std::make_shared<OscillatorInfo>()}
        }
        , compressor{false, 0, 0, 0, 0, 0, 0}
        , distortion{false, 0, 0}
{
}

GeonkickState::GeonkickState(const std::string &data) :
        limiterValue{0}
        , kickLength{0}
        , kickAmplitude{0}
        , kickFilterEnabled{0}
        , kickFilterFrequency{0}
        , kickFilterQFactor{0}
        , kickFilterType{GeonkickApi::FilterType::LowPass}
        , oscillators{
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator1), std::make_shared<OscillatorInfo>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator2), std::make_shared<OscillatorInfo>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Noise), std::make_shared<OscillatorInfo>()}
        }
        , compressor{false, 0, 0, 0, 0, 0, 0}
        , distortion{false, 0, 0}
{
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "kick" && m.value.IsObject())
                                parseKickObject(m.value);
                        if (m.name == "osc1" || m.name == "osc2" || m.name == "osc3")
                                parseOscillatorObject(m.name.GetString(), m.value);
                }
        }
}

void GeonkickState::parseKickObject(const rapidjson::Value &kick)
{
        if (kick.IsNull() || !kick.IsObject())
                return;

        for (const auto &m: kick.GetObject()) {
                if (m.name == "limiter" && m.value.IsDouble())
                        setLimiterValue(m.value.GetDouble());

                if (m.name == "ampl_env" && m.value.IsObject()) {
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "length" && el.value.IsDouble())
                                        setKickLength(el.value.GetDouble());
                                if (el.name == "amplitude" && el.value.IsDouble())
                                        setKickAmplitude(el.value.GetDouble());
                                if (el.name == "points" && el.value.IsArray())
                                        setKickEnvelopePoints(parseEnvelopeArray(el.value));
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

                if (m.name == "compressor" && m.value.IsObject()) {
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

void GeonkickState::parseOscillatorObject(const char *name,  const rapidjson::Value &osc)
{
        if (name == nullptr || osc.IsNull() || !osc.IsObject())
                return;

        int index = 0;
        if (std::string(name) == std::string("osc1"))
                index = 0;
        else if(std::string(name) == std::string("osc2"))
                index = 1;
        else if (std::string(name) == std::string("osc3"))
                index = 2;
        else
                return;

        for (const auto &m: osc.GetObject()) {
                if (m.name == "enabled" && m.value.IsBool())
                        setOscillatorEnabled(index, m.value.GetBool());
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

void GeonkickState::setKickEnvelopePoints(const std::vector<RkRealPoint> &points)
{
        kickEnvelopePoints = points;
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

std::vector<RkRealPoint> GeonkickState::getKickEnvelopePoints() const
{
        return kickEnvelopePoints;
}

std::shared_ptr<GeonkickState::OscillatorInfo> GeonkickState::getOscillator(int index) const
{
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
                else
                        oscillator->frequencyEnvelope = points;
        }
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
                else
                        return oscillator->frequencyEnvelope;
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
        for (const auto& val: oscillators) {
                jsonStream << "\"osc" << val.first + 1 << "\": {" << std::endl;
                jsonStream << "\"enabled\": " << (val.second->isEnabled ? "true" : "false");
                jsonStream << "," << std::endl;
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
                jsonStream << "\"factor\": " << std::fixed << std::setprecision(5) << val.second->filterFactor << std::endl;
                jsonStream << "}" << std::endl;  // filter;
                jsonStream << "}" << std::endl;  // osc;
                jsonStream << "," << std::endl;
        }

        jsonStream << "\"kick\": {" << std::endl;
        jsonStream << "\"limiter\": " << std::fixed << std::setprecision(5) << getLimiterValue() << ", " << std::endl;
        jsonStream << "\"ampl_env\": {" << std::endl;
        jsonStream << "\"amplitude\": " << std::fixed << std::setprecision(5) << static_cast<double>(getKickAmplitude()) << ", " << std::endl;
        jsonStream << "\"length\": " << std::fixed << std::setprecision(5) << static_cast<double>(getKickLength()) << ", " << std::endl;
        auto points = getKickEnvelopePoints();
        jsonStream << "\"points\": [" << std::endl;
        bool first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ " << std::fixed << std::setprecision(5) << point.x()
                           << " , " << std::fixed << std::setprecision(5) << point.y() << "]";
        }
        jsonStream << "]" << std::endl; // points
        jsonStream << "}," << std::endl; // ampl_env

        jsonStream << "\"filter\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isKickFilterEnabled() ? "true" : "false");
        jsonStream << ", " << std::endl;
        jsonStream << "\"type\": " << static_cast<int>(getKickFilterType()) << ", " << std::endl;
        jsonStream << "\"cutoff\": " << std::fixed << std::setprecision(2) << getKickFilterFrequency() << ", " << std::endl;
        jsonStream << "\"factor\": " << std::fixed << std::setprecision(2) << getKickFilterQFactor() << std::endl;;
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
        jsonStream << "\"enabled\": " << isDistortionEnabled() << ", " << std::endl;
        jsonStream << "\"volume\": " << std::fixed << std::setprecision(5) << getDistortionVolume()  << ", " << std::endl;
        jsonStream << "\"drive\": " << std::fixed << std::setprecision(5) << getDistortionDrive() << std::endl;
        jsonStream << "}" << std::endl; // distortion
        jsonStream << "}" << std::endl; // kick
        jsonStream << "}" << std::endl; // json

        return jsonStream.str();
}
