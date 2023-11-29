/**
 * File name: percussion_state.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iuri Nistor 
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

#include "percussion_state.h"
#include "Base64EncoderDecoder.h"

#include <iomanip>

PercussionState::PercussionState()
        : appVersion{GEONKICK_VERSION}
        , kickId{0}
        , kickName{"Default"}
        , playingKey{-1}
        , outputChannel{0}
        , kickEnabled{true}
        , percussionMuted{false}
        , percussionSolo{false}
        , limiterValue{0}
        , kickLength{50}
        , kickAmplitude{0.8}
        , kickFilterEnabled{0}
        , kickFilterFrequency{200}
        , kickFilterQFactor{1.0}
        , kickFilterType{GeonkickApi::FilterType::LowPass}
	, kickCutOffEnvelopeApplyType{GeonkickApi::EnvelopeApplyType::Logarithmic}
        , compressor{false, 0, 0, 0, 0, 0, 0}
        , distortion{false, 1.0, 1.0, 1.0}
        , layers{false, false, false}
        , layersAmplitude{1.0, 1.0, 1.0}
        , currentLayer{GeonkickApi::Layer::Layer1}
        , tunedOutput{false}
{
        initOscillators();
}

bool PercussionState::loadFile(const std::string &file)
{
        if (file.size() < 7) {
                GEONKICK_LOG_ERROR("can't open preset.");
                return false;
        }

        std::filesystem::path filePath(file);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK")) {
                GEONKICK_LOG_ERROR("can't open preset. Wrong file format.");
                return false;
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(filePath));
        if (!sfile.is_open()) {
	        GEONKICK_LOG_ERROR("can't open preset file " << file);
                return false;
        }

        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));
        if (!loadData(fileData)) {
	        GEONKICK_LOG_ERROR("error on parsing preset file " << file);
	        return false;
        }
        return true;
}

bool PercussionState::loadData(const std::string &data)
{
        rapidjson::Document document;
        if (document.Parse(data.c_str()).HasParseError()) {
	        GEONKICK_LOG_ERROR("error on parsing JSON data");
	        return false;
        }
        loadObject(document);
	return true;
}

void PercussionState::loadObject(const rapidjson::Value &obj)
{
        if (!obj.IsObject())
                return;

        bool kickParsed = false;
        for (const auto &m: obj.GetObject()) {
                if (m.name == "kick" && m.value.IsObject()) {
                        parseKickObject(m.value);
                        kickParsed = true;
                        break;
                }
        }

        if (!kickParsed)
                return;

        for (const auto &m: obj.GetObject()) {
                for (decltype(layers.size()) i = 0; i < layers.size(); i++) {
                        setCurrentLayer(static_cast<GeonkickApi::Layer>(i));
                        if (m.name == ("osc" + std::to_string(0 + i * GKICK_OSC_GROUP_SIZE)).c_str()) {
                                parseOscillatorObject(0, m.value);
                                break;
                        } else if (m.name == ("osc" + std::to_string(1 + i * GKICK_OSC_GROUP_SIZE)).c_str()) {
                                parseOscillatorObject(1, m.value);
                                break;
                        } else if (m.name == ("osc" + std::to_string(2 + i * GKICK_OSC_GROUP_SIZE)).c_str()) {
                                parseOscillatorObject(2, m.value);
                                break;
                        }
                }
        }
}

size_t PercussionState::getId() const
{
        return kickId;
}

void PercussionState::setId(size_t id)
{
        kickId = id;
}

std::string PercussionState::getName() const
{
        return kickName;
}

void PercussionState::setName(const std::string &name)
{
        kickName = name;
}

signed char PercussionState::getPlayingKey() const
{
        return playingKey;
}

void PercussionState::setPlayingKey(signed char key)
{
        playingKey = key;
}

void PercussionState::setChannel(size_t channel)
{
        outputChannel = channel;
}

void PercussionState::setMute(bool b)
{
        percussionMuted = b;
}

bool PercussionState::isMuted() const
{
        return percussionMuted;
}

void PercussionState::setSolo(bool b)
{
        percussionSolo = b;
}

bool PercussionState::isSolo() const
{
        return percussionSolo;
}

size_t PercussionState::getChannel() const
{
        return outputChannel;
}

bool PercussionState::isEnabled() const
{
        return kickEnabled;
}

void PercussionState::enable(bool b)
{
        kickEnabled = b;
}

void PercussionState::initOscillators()
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

void PercussionState::parseKickObject(const rapidjson::Value &kick)
{
        if (kick.IsNull() || !kick.IsObject())
                return;

        for (const auto &m: kick.GetObject()) {
                if (m.name == "PercussionAppVersion" && m.value.IsInt())
                        appVersion = m.value.GetInt();
                if (m.name == "name" && m.value.IsString())
                        setName(m.value.GetString());
		if (m.name == "id" && m.value.IsInt())
                        setId(m.value.GetInt());
                if (m.name == "channel" && m.value.IsInt())
                        setChannel(m.value.GetInt());
                if (m.name == "mute" && m.value.IsBool())
                        setMute(m.value.GetBool());
                if (m.name == "solo" && m.value.IsBool())
                        setSolo(m.value.GetBool());
                if (m.name == "playing_key" && m.value.IsInt())
                        setPlayingKey(m.value.GetInt());
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
			auto applyType = GeonkickApi::EnvelopeApplyType::Linear;
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        enableKickFilter(el.value.GetBool());
                                if (el.name == "cutoff" && el.value.IsDouble())
                                        setKickFilterFrequency(el.value.GetDouble());
				if (el.name == "apply_type")
					applyType = getApplyTypeFromObj(el.value);
                                if (el.name == "factor" && el.value.IsDouble())
                                        setKickFilterQFactor(el.value.GetDouble());
                                if (el.name == "type" && el.value.IsInt())
                                        setKickFilterType(static_cast<GeonkickApi::FilterType>(el.value.GetInt()));
                                if (el.name == "cutoff_env" && el.value.IsArray()) {
                                        setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                                                              parseEnvelopeArray(el.value));
                                }
                        }
			setKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff, applyType);
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
                                if (el.name == "in_limiter" && el.value.IsDouble())
                                        setDistortionInLimiter(el.value.GetDouble());
                                if (el.name == "volume" && el.value.IsDouble())
                                        setDistortionVolume(el.value.GetDouble());
                                if (el.name == "drive" && el.value.IsDouble())
                                        setDistortionDrive(el.value.GetDouble());
				if (el.name == "drive_env" && el.value.IsArray())
					setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive,
							      parseEnvelopeArray(el.value));
                                if (el.name == "volume_env" && el.value.IsArray())
					setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume,
							      parseEnvelopeArray(el.value));

                        }
                }
        }
}

void PercussionState::parseOscillatorObject(int index,  const rapidjson::Value &osc)
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
                if (m.name == "seed" && m.value.IsInt())
                        setOscillatorSeed(index, m.value.GetInt());
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
				auto applyType = GeonkickApi::EnvelopeApplyType::Linear;
                                for (const auto &el: m.value.GetObject()) {
                                        if (el.name == "amplitude" && el.value.IsDouble())
                                                setOscillatorFrequency(index, el.value.GetDouble());
					if (el.name == "apply_type")
						applyType = getApplyTypeFromObj(el.value);
                                        if (el.name == "points" && el.value.IsArray())
                                                setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                            GeonkickApi::EnvelopeType::Frequency);
                                }
			        setOscillatorEnvelopeApplyType(index,
							       GeonkickApi::EnvelopeType::Frequency,
							       applyType);
                        }

                        if (m.name == "pitchshift_env" && m.value.IsObject()) {
                                for (const auto &el: m.value.GetObject()) {
                                        if (el.name == "amplitude" && el.value.IsDouble())
                                                setOscillatorPitchShift(index, el.value.GetDouble());
                                        if (el.name == "points" && el.value.IsArray())
                                                setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                            GeonkickApi::EnvelopeType::PitchShift);
                                }
                        }
                }

                if (m.name == "filter" && m.value.IsObject()) {
			auto applyType = GeonkickApi::EnvelopeApplyType::Linear;
                        for (const auto &el: m.value.GetObject()) {
                                if (el.name == "enabled" && el.value.IsBool())
                                        setOscillatorFilterEnabled(index, el.value.GetBool());
                                if (el.name == "cutoff" && el.value.IsDouble())
                                        setOscillatorFilterCutOffFreq(index, el.value.GetDouble());
                                if (el.name == "factor" && el.value.IsDouble())
                                        setOscillatorFilterFactor(index, el.value.GetDouble());
				if (el.name == "apply_type")
					applyType = getApplyTypeFromObj(el.value);
                                if (el.name == "type" && el.value.IsInt())
                                        setOscillatorFilterType(index, static_cast<GeonkickApi::FilterType>(el.value.GetInt()));
                                if (el.name == "cutoff_env" && el.value.IsArray()) {
                                        setOscillatorEnvelopePoints(index, parseEnvelopeArray(el.value),
                                                                    GeonkickApi::EnvelopeType::FilterCutOff);
                                }
                        }
			setOscillatorEnvelopeApplyType(index,
						       GeonkickApi::EnvelopeType::FilterCutOff,
						       applyType);
                }
        }
}

std::vector<RkRealPoint> PercussionState::parseEnvelopeArray(const rapidjson::Value &envelopeArray)
{
        std::vector<RkRealPoint> points;
        for (const auto &el: envelopeArray.GetArray())
                if (el.IsArray() && el.GetArray().Size() == 2)
                        points.emplace_back(RkRealPoint(el.GetArray()[0].GetDouble(),
                                                        el.GetArray()[1].GetDouble()));

        return points;
}

void PercussionState::setLimiterValue(double val)
{
        limiterValue = val;
}

void PercussionState::setKickLength(double val)
{
        kickLength = val;
}

void PercussionState::setKickAmplitude(double val)
{
        kickAmplitude = val;
}

void PercussionState::enableKickFilter(bool b)
{
        kickFilterEnabled = b;
}

void PercussionState::setKickFilterFrequency(double val)
{
        kickFilterFrequency = val;
}

void PercussionState::setKickFilterQFactor(double val)
{
        kickFilterQFactor = val;
}

void PercussionState::setKickFilterType(GeonkickApi::FilterType type)
{
        kickFilterType = type;
}

void PercussionState::setKickEnvelopePoints(GeonkickApi::EnvelopeType envelope,
                                            const std::vector<RkRealPoint> &points)
{
        if (envelope == GeonkickApi::EnvelopeType::Amplitude)
                kickEnvelopePoints = points;
        else if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
                kickFilterCutOffEnvelope = points;
	else if (envelope == GeonkickApi::EnvelopeType::DistortionDrive)
		kickDistortionDriveEnvelope = points;
        else if (envelope == GeonkickApi::EnvelopeType::DistortionVolume)
		kickDistortionVolumeEnvelope = points;
}

void PercussionState::setKickEnvelopeApplyType(GeonkickApi::EnvelopeType envelope,
					       GeonkickApi::EnvelopeApplyType applyType)
{

	if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
		kickCutOffEnvelopeApplyType = applyType;
}

GeonkickApi::EnvelopeApplyType
PercussionState::getKickEnvelopeApplyType(GeonkickApi::EnvelopeType envelope) const
{

	if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
		return kickCutOffEnvelopeApplyType;
	return GeonkickApi::EnvelopeApplyType::Linear;
}

double PercussionState::getLimiterValue() const
{
        return limiterValue;
}

double PercussionState::getKickLength() const
{
        return kickLength;
}

double PercussionState::getKickAmplitude() const
{
        return kickAmplitude;
}

bool PercussionState::isKickFilterEnabled() const
{
        return kickFilterEnabled;
}

double PercussionState::getKickFilterFrequency() const
{
        return kickFilterFrequency;
}

double PercussionState::getKickFilterQFactor() const
{
        return kickFilterQFactor;
}

GeonkickApi::FilterType PercussionState::getKickFilterType() const
{
        return kickFilterType;
}

std::vector<RkRealPoint>
PercussionState::getKickEnvelopePoints(GeonkickApi::EnvelopeType envelope) const
{
        if (envelope == GeonkickApi::EnvelopeType::Amplitude)
                return kickEnvelopePoints;
        else if (envelope == GeonkickApi::EnvelopeType::FilterCutOff)
                return kickFilterCutOffEnvelope;
	else if (envelope == GeonkickApi::EnvelopeType::DistortionDrive)
		return kickDistortionDriveEnvelope;
        else if (envelope == GeonkickApi::EnvelopeType::DistortionVolume)
		return kickDistortionVolumeEnvelope;
	else
		return {};
}

std::shared_ptr<PercussionState::OscillatorInfo>
PercussionState::getOscillator(int index) const
{
        index += GKICK_OSC_GROUP_SIZE * static_cast<int>(currentLayer);
        auto it = oscillators.find(index);
        if (it != oscillators.end())
                return it->second;

        return nullptr;
}

void PercussionState::setOscillatorEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->isEnabled = b;
}

void PercussionState::setOscillatorFunction(int index, GeonkickApi::FunctionType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->function = type;
}

void PercussionState::setOscillatorPhase(int index, double phase)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->phase = phase;
}

void PercussionState::setOscillatorSeed(int index, int seed)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->seed = seed;
}

void PercussionState::setOscillatorAmplitue(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->amplitude = val;
}

void PercussionState::setOscillatorFrequency(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->frequency = val;
}

void PercussionState::setOscillatorPitchShift(int index, double val)
{
        if (auto oscillator = getOscillator(index); oscillator)
                oscillator->pitchShift = val;
}

void PercussionState::setOscillatorFilterEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->isFilterEnabled = b;
}

void PercussionState::setOscillatorFilterType(int index, GeonkickApi::FilterType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterType = type;
}

void PercussionState::setOscillatorFilterCutOffFreq(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterFrequency = val;
}

void PercussionState::setOscillatorFilterFactor(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                oscillator->filterFactor = val;
}

void PercussionState::setOscillatorEnvelopePoints(int index,
                                                const std::vector<RkRealPoint> &points,
                                                GeonkickApi::EnvelopeType envelope)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                switch (envelope) {
                case GeonkickApi::EnvelopeType::Amplitude:
                        oscillator->amplitudeEnvelope = points;
                        break;
                case GeonkickApi::EnvelopeType::Frequency:
                        oscillator->frequencyEnvelope = points;
                        break;
                case GeonkickApi::EnvelopeType::PitchShift:
                        oscillator->pitchShiftEnvelope = points;
                        break;
                case GeonkickApi::EnvelopeType::FilterCutOff:
                        oscillator->filterCutOffEnvelope = points;
                        break;
                default:
                        return;
                }
        }
}

void PercussionState::setOscillatorEnvelopeApplyType(int index,
						     GeonkickApi::EnvelopeType envelopeType,
						     GeonkickApi::EnvelopeApplyType applyType)
{
        auto oscillator = getOscillator(index);
        if (!oscillator)
                return;

	switch (envelopeType) {
	case GeonkickApi::EnvelopeType::Frequency:
		oscillator->frequencyEnvelopeApplyType = applyType;
		break;
	case GeonkickApi::EnvelopeType::FilterCutOff:
		oscillator->cutOffEnvelopeApplyType = applyType;
		break;
	default:
		return;
	}
}

GeonkickApi::EnvelopeApplyType PercussionState::getOscillatorEnvelopeApplyType(int index,
									       GeonkickApi::EnvelopeType envelope) const
{
        auto oscillator = getOscillator(index);
        if (!oscillator)
                return GeonkickApi::EnvelopeApplyType::Linear;

	switch (envelope) {
	case GeonkickApi::EnvelopeType::Frequency:
		return oscillator->frequencyEnvelopeApplyType;
		break;
	case GeonkickApi::EnvelopeType::FilterCutOff:
		return oscillator->cutOffEnvelopeApplyType;
	default:
		return GeonkickApi::EnvelopeApplyType::Linear;
	}
}

bool PercussionState::isOscillatorAsFm(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->isFm;
        return false;
}

void PercussionState::setOscillatorAsFm(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator)
               oscillator->isFm = b;
}

bool PercussionState::isOscillatorEnabled(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->isEnabled;

        return false;
}

GeonkickApi::FunctionType PercussionState::oscillatorFunction(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->function;

        return GeonkickApi::FunctionType::Sine;
}

double PercussionState::oscillatorPhase(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->phase;

        return 0;
}

int PercussionState::oscillatorSeed(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->seed;

        return 0;
}

double PercussionState::oscillatorAmplitue(int index) const
{
        if (auto oscillator = getOscillator(index); oscillator)
                return oscillator->amplitude;
        return 0;
}

double PercussionState::oscillatorFrequency(int index) const
{
        if (auto oscillator = getOscillator(index); oscillator)
                return oscillator->frequency;
        return 0;
}

double PercussionState::oscillatorPitchShift(int index) const
{
        if (auto oscillator = getOscillator(index); oscillator)
                return oscillator->pitchShift;
        return 0;
}

bool PercussionState::isOscillatorFilterEnabled(int index) const
{
        if (auto oscillator = getOscillator(index); oscillator)
                return oscillator->isFilterEnabled;
        return false;
}

GeonkickApi::FilterType PercussionState::oscillatorFilterType(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterType;
        return GeonkickApi::FilterType::LowPass;
}

double PercussionState::oscillatorFilterCutOffFreq(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterFrequency;
        return 0;
}

double PercussionState::oscillatorFilterFactor(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator)
                return oscillator->filterFactor;
        return 0;
}

std::vector<RkRealPoint>
PercussionState::oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const
{
        if (auto oscillator = getOscillator(index); oscillator) {
                switch (type) {
                case GeonkickApi::EnvelopeType::Amplitude:
                        return oscillator->amplitudeEnvelope;
                case GeonkickApi::EnvelopeType::Frequency:
                        return oscillator->frequencyEnvelope;
                case GeonkickApi::EnvelopeType::PitchShift:
                        return oscillator->pitchShiftEnvelope;
                case GeonkickApi::EnvelopeType::FilterCutOff:
                        return oscillator->filterCutOffEnvelope;
                default:
                        return {};
                }
        }
        return {};
}

void PercussionState::enableCompressor(bool enable)
{
        compressor.enabled = enable;
}

bool PercussionState::isCompressorEnabled() const
{
        return compressor.enabled;
}

void PercussionState::setCompressorAttack(double attack)
{
        compressor.attack = attack;
}

void PercussionState::setCompressorRelease(double release)
{
        compressor.release = release;
}

void PercussionState::setCompressorThreshold(double threshold)
{
        compressor.threshold = threshold;
}

void PercussionState::setCompressorRatio(double ratio)
{
        compressor.ratio = ratio;
}

void PercussionState::setCompressorKnee(double knee)
{
        compressor.knee = knee;
}

void PercussionState::setCompressorMakeup(double makeup)
{
        compressor.makeup = makeup;
}

double PercussionState::getCompressorAttack() const
{
        return compressor.attack;
}

double PercussionState::getCompressorRelease() const
{
        return compressor.release;
}

double PercussionState::getCompressorThreshold() const
{
        return compressor.threshold;
}

double PercussionState::getCompressorRatio() const
{
        return compressor.ratio;
}

double PercussionState::getCompressorKnee() const
{
        return compressor.knee;
}

double PercussionState::getCompressorMakeup() const
{
        return compressor.makeup;
}

void PercussionState::enableDistortion(bool enable)
{
        distortion.enabled = enable;
}

bool PercussionState::isDistortionEnabled() const
{
        return distortion.enabled;
}

void PercussionState::setDistortionInLimiter(double limit)
{
        distortion.in_limiter = limit;
}

void PercussionState::setDistortionVolume(double volume)
{
        distortion.volume = volume;
}

void PercussionState::setDistortionDrive(double drive)
{
        distortion.drive = drive;
}

double PercussionState::getDistortionInLimiter() const
{
        return distortion.in_limiter;
}

double PercussionState::getDistortionVolume() const
{
        return distortion.volume;
}

double PercussionState::getDistortionDrive() const
{
        return distortion.drive;
}

std::string PercussionState::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << std::fixed << std::setprecision(7);
        jsonStream << "{" << std::endl;
        oscJson(jsonStream);
        kickJson(jsonStream);
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

void PercussionState::envelopeToJson(std::ostringstream &jsonStream,
                                     const std::string &envName,
                                     double amplitude,
                                     const std::vector<RkRealPoint> &envelope,
 				     GeonkickApi::EnvelopeApplyType applyType)
{
        jsonStream << "\"" << envName << "\": {" << std::endl;
        jsonStream << "\"amplitude\": " << amplitude << ", " << std::endl;
	jsonStream << "\"apply_type\": " << static_cast<int>(applyType) << ", " << std::endl;
        jsonStream << "\"points\": [" << std::endl;
        bool first = true;
        for (const auto &point: envelope) {
                if (!first)
                        jsonStream << ", ";
                jsonStream << "[ "  << point.x()
                           << " , "  << point.y() << "]";
                first = false;
        }
        jsonStream << "]" << std::endl;
        jsonStream << "}" << std::endl;
}

void PercussionState::oscJson(std::ostringstream &jsonStream) const
{
        for (const auto& val: oscillators) {
                jsonStream << "\"osc" << val.first << "\": {" << std::endl;
                jsonStream << "\"enabled\": " << (val.second->isEnabled ? "true" : "false") << ", " << std::endl;
                jsonStream << "\"is_fm\": " << (val.second->isFm ? "true" : "false") << ", " << std::endl;
                if (val.second->function == GeonkickApi::FunctionType::Sample && !val.second->sample.empty())
                        jsonStream <<  "\"sample\": \"" << toBase64F(val.second->sample) << "\"," << std::endl;
                jsonStream <<  "\"function\": " << static_cast<int>(val.second->function) << "," << std::endl;
                jsonStream <<  "\"phase\": " 
                           << val.second->phase << ", " << std::endl;
                jsonStream <<  "\"seed\": " << val.second->seed << ", " << std::endl;
                envelopeToJson(jsonStream,
                               "ampl_env",
                               val.second->amplitude,
                               val.second->amplitudeEnvelope);
                jsonStream << "," << std::endl;
                envelopeToJson(jsonStream,
                               "freq_env",
                               val.second->frequency,
                               val.second->frequencyEnvelope,
			       val.second->frequencyEnvelopeApplyType);
                jsonStream << "," << std::endl;
                envelopeToJson(jsonStream,
                               "pitchshift_env",
                               val.second->pitchShift,
                               val.second->pitchShiftEnvelope);
                jsonStream << "," << std::endl;
                jsonStream << "\"filter\": {" << std::endl;
                jsonStream << "\"enabled\": " << (val.second->isFilterEnabled ? "true" : "false");
                jsonStream << ", " << std::endl;
                jsonStream << "\"type\": " << static_cast<int>(val.second->filterType) << ", " << std::endl;
                jsonStream << "\"cutoff\": " 
                           << val.second->filterFrequency << ", " << std::endl;
		jsonStream << "\"apply_type\": " 
                           << static_cast<int>(val.second->cutOffEnvelopeApplyType) << ", " << std::endl;
                jsonStream << "\"cutoff_env\": [";
                bool first = true;
                for (const auto &point: val.second->filterCutOffEnvelope) {
                        if (first)
                                first = false;
                        else
                                jsonStream << ", ";
                        jsonStream << "[ "  << point.x()
                                   << " , "  << point.y() << "]";
                }
                jsonStream << "], " << std::endl;
                jsonStream << "\"factor\": " 
                           << val.second->filterFactor << std::endl;
                jsonStream << "}" << std::endl;  // filter;
                jsonStream << "}" << std::endl;  // osc;
                jsonStream << "," << std::endl;
        }
}

void PercussionState::kickJson(std::ostringstream &jsonStream) const
{
        jsonStream << "\"kick\": {" << std::endl;
        jsonStream << "\"PercussionAppVersion\": " << GEONKICK_VERSION << "," << std::endl;
	jsonStream << "\"id\": " << getId() << "," << std::endl;
        jsonStream << "\"channel\": " << getChannel() << "," << std::endl;
        jsonStream << "\"mute\": " << (isMuted() ? "true" : "false") << "," << std::endl;
        jsonStream << "\"solo\": " << (isSolo() ? "true" : "false") << "," << std::endl;
        jsonStream << "\"name\": \"" << getName() << "\"," << std::endl;
        jsonStream << "\"playing_key\": " << static_cast<int>(getPlayingKey()) << "," << std::endl;
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
                        jsonStream  << layersAmplitude[i];
        }
        jsonStream << "]," << std::endl;
        jsonStream << "\"limiter\": " 
                   << getLimiterValue() << ", " << std::endl;
        jsonStream << "\"tuned_output\": " << (isOutputTuned() ? "true" : "false") << ", " << std::endl;
        jsonStream << "\"ampl_env\": {" << std::endl;
        jsonStream << "\"amplitude\": " << getKickAmplitude() << ", " << std::endl;
        jsonStream << "\"length\": " << getKickLength() << ", " << std::endl;
        auto points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude);
        jsonStream << "\"points\": [";
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ "  << point.x()
                           << " , "  << point.y() << "]";
        }

        jsonStream << "]}, " << std::endl;

        jsonStream << "\"filter\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isKickFilterEnabled() ? "true" : "false");
        jsonStream << ", " << std::endl;
        jsonStream << "\"type\": " << static_cast<int>(getKickFilterType()) << ", " << std::endl;
        jsonStream << "\"cutoff\": "
                   << getKickFilterFrequency() << ", " << std::endl;
	jsonStream << "\"apply_type\": "
                   << static_cast<int>(getKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff)) << ", " << std::endl;
        jsonStream << "\"factor\": "
                   << getKickFilterQFactor() << ", " << std::endl;
        points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff);
        jsonStream << "\"cutoff_env\": [";
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ "  << point.x()
                           << " , "  << point.y() << "]";
        }
        jsonStream << "]" << std::endl; // points
        jsonStream << "}, " << std::endl;  // filter;
        jsonStream << "\"compressor\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isCompressorEnabled() ? "true" : "false") << ", " << std::endl;
        jsonStream << "\"attack\": " 
                   << getCompressorAttack() << ", " << std::endl;
        jsonStream << "\"release\": " 
                   << getCompressorRelease() << ", " << std::endl;
        jsonStream << "\"threshold\": " 
                   << getCompressorThreshold() << ", " << std::endl;
        jsonStream << "\"ratio\": " 
                   << getCompressorRatio() << ", " << std::endl;
        jsonStream << "\"knee\": " 
                   << getCompressorKnee() << ", " << std::endl;
        jsonStream << "\"makeup\": " 
                   << getCompressorMakeup() << std::endl;
        jsonStream << "}, " << std::endl;

        jsonStream << "\"distortion\": {" << std::endl;
        jsonStream << "\"enabled\": " << (isDistortionEnabled() ? "true" : "false") << ", " << std::endl;
        jsonStream << "\"in_limiter\": " 
                   << getDistortionInLimiter()  << ", " << std::endl;
        jsonStream << "\"volume\": " 
                   << getDistortionVolume()  << ", " << std::endl;
        jsonStream << "\"drive\": " 
                   << getDistortionDrive() << ", " << std::endl;
	jsonStream << "\"drive_env\": [" << std::endl;
	points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive);
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ "  << point.x()
                           << " , "  << point.y() << "]";
        }
        jsonStream << "], " << std::endl;
        jsonStream << "\"volume_env\": [" << std::endl;
	points = getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume);
        first = true;
        for (const auto &point: points) {
                if (first)
                        first = false;
                else
                        jsonStream << ", ";
                jsonStream << "[ "  << point.x()
                           << " , " << point.y() << "]";
        }
	jsonStream << "]" << std::endl;
        jsonStream << "}" << std::endl; // distortion
        jsonStream << "}" << std::endl; // kick
}

void PercussionState::setLayerEnabled(GeonkickApi::Layer layer, bool b)
{
        auto index = static_cast<decltype(layers.size())>(layer);
        if (index >= 0 && index < layers.size())
                layers[index] = b;
}

bool PercussionState::isLayerEnabled(GeonkickApi::Layer layer) const
{
        auto index = static_cast<decltype(layers.size())>(layer);
        if (index >= 0 && index < layers.size())
                return layers[index];
        else
                return false;
}

void PercussionState::setCurrentLayer(GeonkickApi::Layer layer)
{
        currentLayer = layer;
}

void PercussionState::setLayerAmplitude(GeonkickApi::Layer layer, double amplitude)
{
        auto index = static_cast<decltype(layersAmplitude.size())>(layer);
        if (index >= 0 && index < layersAmplitude.size())
                layersAmplitude[index] = amplitude;
}

double PercussionState::getLayerAmplitude(GeonkickApi::Layer layer) const
{
        auto index = static_cast<decltype(layersAmplitude.size())>(layer);
        if (index >= 0 && index < layersAmplitude.size())
                return layersAmplitude[index];
        return 0;
}

void PercussionState::tuneOutput(bool tune)
{
        tunedOutput = tune;
}

bool PercussionState::isOutputTuned() const
{
        return tunedOutput;
}

std::vector<float> PercussionState::fromBase64F(const std::string &str)
{
        return Base64EncoderDecoder::decode(str);
}

std::string PercussionState::toBase64F(const std::vector<float> &data)
{
        return Base64EncoderDecoder::encode(data);
}

bool PercussionState::save(const std::string &fileName)
{
        if (fileName.size() < 7) {
                GEONKICK_LOG_ERROR("file name is wrong");
                return false;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK"))
                filePath.replace_extension(".gkick");

        std::ofstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                GEONKICK_LOG_ERROR("can't open file for saving: " << filePath);
                return false;
        }
        file << toJson();
        file.close();
        return true;
}

void PercussionState::setOscillatorSample(int oscillatorIndex, const std::vector<float> &sample)
{
        auto oscillator = getOscillator(oscillatorIndex);
        if (oscillator)
                oscillator->sample = sample;
}

std::vector<float> PercussionState::getOscillatorSample(int oscillatorIndex) const
{
        auto oscillator = getOscillator(oscillatorIndex);
        return oscillator->sample;
}

GeonkickApi::EnvelopeApplyType PercussionState::getApplyTypeFromObj(const rapidjson::Value &value) const
{
	if (value.IsInt())
		return static_cast<GeonkickApi::EnvelopeApplyType>(value.GetInt());
	return GeonkickApi::EnvelopeApplyType::Linear;
}
