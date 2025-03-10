/**
 * File name: geonkick_api.cpp
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

#include "geonkick_api.h"
#include "DesktopPaths.h"
#include "oscillator.h"
#include "globals.h"
#include "percussion_state.h"
#include "kit_state.h"
#include "preset.h"
#include "preset_folder.h"
#include "UiSettings.h"
#include "GeonkickConfig.h"

#include <RkEventQueue.h>
#include <RkAction.h>

#include <sndfile.h>

GeonkickApi::GeonkickApi(int sample_rate, InstanceType instance, geonkick *dsp)
        : geonkickApi{dsp}
        , instanceType{instance}
        , limiterLevelers{}
        , jackEnabled{false}
        , standaloneInstance{false}
        , eventQueue{nullptr}
        , currentLayer{Layer::Layer1}
        , kitName{"Unknown"}
        , kitAuthor{"Author"}
        , clipboardPercussion{nullptr}
        , uiSettings{std::make_unique<UiSettings>()}
	, sampleRate{sample_rate}
        , scaleFactor{1.0}
{
        setupPaths();
        uiSettings->setSamplesBrowserPath(getSettings("GEONKICK_CONFIG/HOME_PATH"));
        GeonkickConfig cfg;
        scaleFactor = cfg.getScaleFactor();
        forceMidiChannel(cfg.getMidiChannel(), cfg.isMidiChannelForced());
}

GeonkickApi::~GeonkickApi()
{
  	if (geonkickApi)
                geonkick_free(&geonkickApi);
}

void GeonkickApi::setInstanceType(InstanceType type)
{
        instanceType = type;
}

GeonkickApi::InstanceType GeonkickApi::getInstanceType() const
{
        return instanceType;
}

unsigned int GeonkickApi::getVersion()
{
        return GEONKICK_VERSION;
}

void GeonkickApi::setEventQueue(RkEventQueue *queue)
{
        std::lock_guard<std::mutex> lock(apiMutex);
        eventQueue = queue;
}

bool GeonkickApi::initDSP()
{
        if (!geonkickApi) {
                if (geonkick_create(&geonkickApi, sampleRate) != GEONKICK_OK) {
                        GEONKICK_LOG_ERROR("can't create geonkick API");
                        return false;
                }
        }
        return true;
}

bool GeonkickApi::init()
{
        if (!initDSP())
	        return false;

	loadPresets();

        jackEnabled = geonkick_is_module_enabed(geonkickApi, GEONKICK_MODULE_JACK);
	geonkick_enable_synthesis(geonkickApi, false);

	auto nInstruments = numberOfInstruments();
        auto nChannels = numberOfChannels();
        kickBuffers = std::vector<std::vector<gkick_real>>(nInstruments);
	for (decltype(nInstruments) i = 0; i < nInstruments; i++) {
                auto state = getDefaultPercussionState();
                state->setId(i);
                state->setChannel(i % nChannels);
		setPercussionState(state);
        }

        setKitState(std::move(getDefaultKitState()));
        enablePercussion(0, true);
        addOrderedPercussionId(0);

        // Set the first the percussion by default to be controllable.
	geonkick_set_current_percussion(geonkickApi, 0);
	geonkick_enable_synthesis(geonkickApi, true);
        return true;
}

size_t GeonkickApi::numberOfInstruments()
{
        return geonkick_instruments_number();
}

size_t GeonkickApi::numberOfChannels()
{
        return geonkick_channels_number();
}

size_t GeonkickApi::numberOfMidiChannels()
{
        return geonkick_midi_channels_number();
}

size_t GeonkickApi::numberOfLayers()
{
        return geonkick_layers_number();
}

std::unique_ptr<KitState> GeonkickApi::getDefaultKitState()
{
        return std::make_unique<KitState>();
}

std::unique_ptr<PercussionState> GeonkickApi::getDefaultPercussionState()
{
        auto state = std::make_unique<PercussionState>();
        state->setName("Default");
        state->setId(0);
        state->setPlayingKey(-1);
        state->setChannel(0);
        state->setLimiterValue(1.0);
        state->tuneOutput(false);
        state->setKickLength(300);
        state->setKickAmplitude(0.8);
        state->enableKickFilter(false);
        state->setKickFilterFrequency(200);
        state->setKickFilterQFactor(10);
        state->setKickFilterType(GeonkickApi::FilterType::LowPass);
        std::vector<EnvelopePoint> envelope;
        envelope.push_back({0, 1});
        envelope.push_back({1, 1});
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude, envelope);
        state->setKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff,
					EnvelopeApplyType::Logarithmic);
	state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff, envelope);
	state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterQFactor, envelope);
	state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive, envelope);
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume, envelope);
        state->enableDistortion(false);
        state->setDistortionType(DistortionType::SoftClippingTan);
        state->setDistortionOutLimiter(0.1);
        state->setDistortionInLimiter(1.0);
        state->setDistortionDrive(1.0);

        std::vector<GeonkickApi::OscillatorType> oscillators = {
                GeonkickApi::OscillatorType::Oscillator1,
                GeonkickApi::OscillatorType::Oscillator2,
                GeonkickApi::OscillatorType::Oscillator3
        };

        auto nLayers = numberOfLayers();
        for (size_t l = 0; l < nLayers; l++) {
                auto layer = static_cast<Layer>(l);
                state->setLayerEnabled(layer, layer == Layer::Layer1);
                state->setLayerAmplitude(layer, 1.0);
                for (auto const &osc: oscillators) {
                        int index = static_cast<int>(osc) + GKICK_OSC_GROUP_SIZE * static_cast<int>(layer);
                        state->setOscillatorEnabled(index, osc == GeonkickApi::OscillatorType::Oscillator1);
                        state->setOscillatorFunction(index, GeonkickApi::FunctionType::Sine);
                        state->setOscillatorPhase(index, 0);
                        state->setOscillatorAmplitue(index, 0.26);
                        state->setOscillatorFrequency(index, 800);
                        state->setOscillatorPitchShift(index, 12);
                        state->setOscillatorFilterEnabled(index, false);
                        state->setOscillatorFilterType(index, GeonkickApi::FilterType::LowPass);
                        state->setOscillatorFilterCutOffFreq(index, 800);
                        state->setOscillatorFilterFactor(index, 10);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Amplitude);
                        state->setOscillatorEnvelopeApplyType(index,
                                                              GeonkickApi::EnvelopeType::Frequency,
                                                              GeonkickApi::EnvelopeApplyType::Logarithmic);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Frequency);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::PitchShift);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::DistortionDrive);
                        std::vector<EnvelopePoint> env = envelope;
                        env[0].setY(0.5);
                        env[1].setY(0.5);
                        state->setOscillatorEnvelopePoints(index, env, GeonkickApi::EnvelopeType::PitchShift);
                        state->setOscillatorEnvelopePoints(index,
							   envelope,
							   GeonkickApi::EnvelopeType::FilterCutOff);
			state->setOscillatorEnvelopePoints(index,
							   envelope,
							   GeonkickApi::EnvelopeType::FilterQFactor);
			state->setOscillatorEnvelopeApplyType(index,
							      GeonkickApi::EnvelopeType::FilterCutOff,
							      GeonkickApi::EnvelopeApplyType::Logarithmic);
                        state->setOscillatorEnvelopePoints(index, envelope, EnvelopeType::DistortionDrive);
                        state->setOscDistortionEnabled(index, false);
                        state->setOscDistortionType(index, DistortionType::SoftClippingTan);
                        state->setOscDistortionOutLimiter(index, 1.0);
                        state->setOscDistortionInLimiter(index, 1.0);
                        state->setOscDistortionDrive(index, 1.0);
                }
        }

        return state;
}

void GeonkickApi::setPercussionState(const std::unique_ptr<PercussionState> &state)
{
        if (!state)
                return;

        geonkick_enable_synthesis(geonkickApi, false);
        geonkick_enable_percussion(geonkickApi, state->getId(), state->isEnabled());
        auto currentId = currentPercussion();
        geonkick_set_current_percussion(geonkickApi, state->getId());
        setPercussionName(state->getId(), state->getName());
        setPercussionPlayingKey(state->getId(), state->getPlayingKey());
        setPercussionChannel(state->getId(), state->getChannel());
        setPercussionMidiChannel(state->getId(), state->getMidiChannel());
        enableNoteOff(state->getId(), state->isNoteOffEnabled());
        mutePercussion(state->getId(), state->isMuted());
        soloPercussion(state->getId(), state->isSolo());
        auto nLayers = numberOfLayers();
        for (size_t i = 0; i < nLayers; i++) {
                enbaleLayer(static_cast<Layer>(i), state->isLayerEnabled(static_cast<Layer>(i)));
                setLayerAmplitude(static_cast<Layer>(i), state->getLayerAmplitude(static_cast<Layer>(i)));
        }
        setLimiterValue(state->getLimiterValue());
        tuneAudioOutput(state->getId(), state->isOutputTuned());
        setKickLength(state->getKickLength());
        setKickAmplitude(state->getKickAmplitude());
        enableKickFilter(state->isKickFilterEnabled());
        setKickFilterFrequency(state->getKickFilterFrequency());
        setKickFilterQFactor(state->getKickFilterQFactor());
        setKickFilterType(state->getKickFilterType());
        setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude));
        setKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff,
				 state->getKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff));
	setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff));
	setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterQFactor,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterQFactor));
	setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive));
        setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume));

        for (size_t i = 0; i < nLayers; i++) {
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator1, state);
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator2, state);
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator3, state);
        }
        enableDistortion(state->isDistortionEnabled());
        setDistortionType(state->getDistortionType());
        setDistortionInLimiter(state->getDistortionInLimiter());
        setDistortionOutLimiter(state->getDistortionOutLimiter());
        setDistortionDrive(state->getDistortionDrive());

        geonkick_set_current_percussion(geonkickApi, currentId);
        geonkick_enable_synthesis(geonkickApi, true);
}

void GeonkickApi::setPercussionState(const std::string &data)
{
        auto state = getDefaultPercussionState();
        state->loadData(data);
        setPercussionState(state);
}

std::unique_ptr<PercussionState> GeonkickApi::getPercussionState(size_t id) const
{
        if (id == currentPercussion()) {
                return getPercussionState();
        } else {
                auto tmpId = currentPercussion();
                auto res = geonkick_set_current_percussion(geonkickApi, id);
                if (res != GEONKICK_OK) {
                        geonkick_set_current_percussion(geonkickApi, tmpId);
                        return getPercussionState();
                }
                auto state = getPercussionState();
                geonkick_set_current_percussion(geonkickApi, tmpId);
                return state;
        }
}

std::unique_ptr<PercussionState> GeonkickApi::getPercussionState() const
{
        auto state = std::make_unique<PercussionState>();
        state->setId(currentPercussion());
        state->setName(getPercussionName(state->getId()));
        state->setLimiterValue(limiterValue());
        state->tuneOutput(isAudioOutputTuned(state->getId()));
        state->setPlayingKey(getPercussionPlayingKey(state->getId()));
        state->setChannel(getPercussionChannel(state->getId()));
        state->setMidiChannel(getPercussionMidiChannel(state->getId()));
        state->setNoteOffEnabled(isNoteOffEnabled(state->getId()));
        state->setMute(isPercussionMuted(state->getId()));
        state->setSolo(isPercussionSolo(state->getId()));
        auto nLayers = numberOfLayers();
        for (size_t i = 0; i < nLayers; i++) {
                state->setLayerEnabled(static_cast<Layer>(i), isLayerEnabled(static_cast<Layer>(i)));
                state->setLayerAmplitude(static_cast<Layer>(i), getLayerAmplitude(static_cast<Layer>(i)));
        }
        state->setKickLength(kickLength());
        state->setKickAmplitude(kickAmplitude());
        state->enableKickFilter(isKickFilterEnabled());
        state->setKickFilterFrequency(kickFilterFrequency());
        state->setKickFilterQFactor(kickFilterQFactor());
        state->setKickFilterType(kickFilterType());
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude));
	state->setKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff,
                                     getKickEnvelopeApplyType(GeonkickApi::EnvelopeType::FilterCutOff));
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff));
	state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterQFactor,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterQFactor));
	state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionDrive));
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::DistortionVolume));


        for (size_t i = 0; i < nLayers; i++) {
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator1, state);
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator2, state);
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator3, state);
        }
        state->enableDistortion(isDistortionEnabled());
        state->setDistortionInLimiter(getDistortionInLimiter());
        state->setDistortionOutLimiter(getDistortionOutLimiter());
        state->setDistortionDrive(getDistortionDrive());

        return state;
}

void GeonkickApi::getOscillatorState(GeonkickApi::Layer layer,
                                     OscillatorType osc,
                                     const std::unique_ptr<PercussionState> &state) const
{
        auto temp = currentLayer;
        currentLayer = layer;
        auto index = static_cast<int>(osc);
        state->setCurrentLayer(layer);
        state->setOscillatorEnabled(index, isOscillatorEnabled(index));
        state->setOscillatorFunction(index, oscillatorFunction(index));
        state->setOscillatorSample(index, getOscillatorSample(index));
        state->setOscillatorPhase(index, oscillatorPhase(index));
        state->setOscillatorSeed(index, oscillatorSeed(index));
        state->setOscillatorAmplitue(index, oscillatorAmplitude(index));
        state->setOscillatorFrequency(index, oscillatorFrequency(index));
        state->setOscillatorPitchShift(index, oscillatorPitchShift(index));
        state->setOscillatorNoiseDensity(index, oscillatorNoiseDensity(index));
        state->setOscillatorFilterEnabled(index, isOscillatorFilterEnabled(index));
        state->setOscillatorFilterType(index, getOscillatorFilterType(index));
        state->setOscillatorFilterCutOffFreq(index, getOscillatorFilterCutOffFreq(index));
        state->setOscillatorFilterFactor(index, getOscillatorFilterFactor(index));
        auto points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::Amplitude);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Amplitude);
        auto applyType = getOscillatorEnvelopeApplyType(index, GeonkickApi::EnvelopeType::Frequency);
        state->setOscillatorEnvelopeApplyType(index, GeonkickApi::EnvelopeType::Frequency, applyType);
        points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::Frequency);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Frequency);
        points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::PitchShift);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::PitchShift);
        points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::NoiseDensity);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::NoiseDensity);
        applyType = getOscillatorEnvelopeApplyType(index, GeonkickApi::EnvelopeType::FilterCutOff);
        state->setOscillatorEnvelopeApplyType(index, GeonkickApi::EnvelopeType::FilterCutOff, applyType);
        points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::FilterCutOff);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::FilterCutOff);
	points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::FilterQFactor);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::FilterQFactor);
        state->setOscillatorAsFm(index, isOscillatorAsFm(index));

        // Distortion
        points = oscillatorEvelopePoints(index, EnvelopeType::DistortionDrive);
        state->setOscillatorEnvelopePoints(index, points, EnvelopeType::DistortionDrive);
        state->setOscDistortionEnabled(index, isOscDistortionEnabled(index));
        state->setOscDistortionOutLimiter(index, getOscDistortionOutLimiter(index));
        state->setOscDistortionInLimiter(index, getOscDistortionInLimiter(index));
        state->setOscDistortionDrive(index, getOscDistortionDrive(index));

        currentLayer = temp;
}

void GeonkickApi::setOscillatorState(GeonkickApi::Layer layer,
                                     OscillatorType oscillator,
                                     const std::unique_ptr<PercussionState> &state)
{
        auto temp = currentLayer;
        currentLayer = layer;
        auto osc = static_cast<int>(oscillator);
        state->setCurrentLayer(layer);
        enableOscillator(osc, state->isOscillatorEnabled(osc));
        setOscillatorFunction(osc, state->oscillatorFunction(osc));
        setOscillatorSample(state->getOscillatorSample(osc), osc);
        setOscillatorPhase(osc, state->oscillatorPhase(osc));
        setOscillatorSeed(osc, state->oscillatorSeed(osc));
        setOscillatorAmplitude(osc, state->oscillatorAmplitue(osc));
        setOscillatorFrequency(osc, state->oscillatorFrequency(osc));
        setOscillatorPitchShift(osc, state->oscillatorPitchShift(osc));
        setOscillatorNoiseDensity(osc, state->oscillatorNoiseDensity(osc));
        enableOscillatorFilter(osc, state->isOscillatorFilterEnabled(osc));
        setOscillatorFilterType(osc, state->oscillatorFilterType(osc));
        setOscillatorFilterCutOffFreq(osc, state->oscillatorFilterCutOffFreq(osc));
        setOscillatorFilterFactor(osc, state->oscillatorFilterFactor(osc));
        setOscillatorEvelopePoints(osc, EnvelopeType::Amplitude,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::Amplitude));
        setOscillatorEnvelopeApplyType(osc, EnvelopeType::Frequency,
                                       state->getOscillatorEnvelopeApplyType(osc,
                                                                             EnvelopeType::Frequency));
        setOscillatorEvelopePoints(osc, EnvelopeType::Frequency,
                                   state->oscillatorEnvelopePoints(osc,
                                                                   EnvelopeType::Frequency));
        setOscillatorEvelopePoints(osc, EnvelopeType::PitchShift,
                                   state->oscillatorEnvelopePoints(osc,
                                                                   EnvelopeType::PitchShift));
        setOscillatorEvelopePoints(osc, EnvelopeType::NoiseDensity,
                                   state->oscillatorEnvelopePoints(osc,
                                                                   EnvelopeType::NoiseDensity));
	setOscillatorEnvelopeApplyType(osc, EnvelopeType::FilterCutOff,
				       state->getOscillatorEnvelopeApplyType(osc,
									     EnvelopeType::FilterCutOff));
        setOscillatorEvelopePoints(osc, EnvelopeType::FilterCutOff,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::FilterCutOff));
	setOscillatorEvelopePoints(osc, EnvelopeType::FilterQFactor,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::FilterQFactor));
        setOscillatorEvelopePoints(osc, EnvelopeType::DistortionDrive,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::DistortionDrive));
        oscEnableDistortion(osc, state->isOscDistortionEnabled(osc));
        setOscDistortionType(osc, state->getOscDistortionType(osc));
        setOscDistortionInLimiter(osc, state->getOscDistortionInLimiter(osc));
        setOscDistortionOutLimiter(osc, state->getOscDistortionOutLimiter(osc));
        setOscDistortionDrive(osc, state->getOscDistortionDrive(osc));
        setOscillatorAsFm(osc, state->isOscillatorAsFm(osc));
        setOscillatorAsFm(osc, state->isOscillatorAsFm(osc));
        setOscillatorFmK(osc, state->getOscillatorFmK(osc));
        currentLayer = temp;
}

std::unique_ptr<KitState> GeonkickApi::getKitState() const
{
        auto kit = std::make_unique<KitState>();
        kit->setName(getKitName());
        kit->setAuthor(getKitAuthor());
        kit->setUrl(getKitUrl());
        size_t i = 0;
        for (const auto &id : ordredPercussionIds()) {
                auto state = getPercussionState(id);
                state->setId(i);
		GEONKICK_LOG_DEBUG("PER: " << state->getName() << ": id = " << state->getId());
                kit->addPercussion(std::move(state));
                i++;
        }
        return kit;
}

bool GeonkickApi::setKitState(const std::string &data)
{
        auto state = std::make_unique<KitState>();
        state->fromJson(data);
        return setKitState(state);
}

bool GeonkickApi::setKitState(const std::unique_ptr<KitState> &state)
{
        auto n = numberOfInstruments();
        for (decltype(n) i = 0; i < n; i++)
                enablePercussion(i, false);
        setKitName(state->getName());
        setKitAuthor(state->getAuthor());
        setKitUrl(state->getUrl());
        clearOrderedPercussionIds();
        for (const auto &per: state->percussions()) {
                GEONKICK_LOG_DEBUG("PER: " << per->getName() << ": id = " << per->getId());
                setPercussionState(per);
                addOrderedPercussionId(per->getId());
        }

        if (!percussionIdList.empty())
                setCurrentPercussion(percussionIdList.front());
        else
                 setCurrentPercussion(0);
        return true;
}

std::vector<std::unique_ptr<Oscillator>> GeonkickApi::oscillators(void)
{
        std::vector<std::unique_ptr<Oscillator>> oscillators;
        size_t n = 0;
        geonkick_get_oscillators_number(geonkickApi, &n);
        for (decltype(n) i = 0; i < n; i++)
                oscillators.push_back(std::make_unique<Oscillator>(this, static_cast<Oscillator::Type>(i % GKICK_OSC_GROUP_SIZE)));
        return oscillators;
}

std::vector<EnvelopePoint> GeonkickApi::oscillatorEvelopePoints(int oscillatorIndex,  EnvelopeType envelope) const
{
        struct gkick_envelope_point_info *buf = NULL;
        std::vector<EnvelopePoint> points;
        size_t npoints = 0;
        geonkick_osc_envelope_get_points(geonkickApi,
                                         getOscIndex(oscillatorIndex),
                                         static_cast<int>(envelope),
                                         &buf,
                                         &npoints);
        for (decltype(npoints) i = 0; i < npoints; i++) {
                EnvelopePoint p;
                p.setX(buf[i].x);
                p.setY(buf[i].y);
                p.setAsControlPoint(buf[i].control_point);
                points.emplace_back(std::move(p));
        }
        free(buf);
        return points;
}

GeonkickApi::EnvelopeApplyType GeonkickApi::getOscillatorEnvelopeApplyType(int index,
									   EnvelopeType envelope) const
{
	enum gkick_envelope_apply_type applyType;
	geonkick_osc_envelope_get_apply_type(geonkickApi,
					     static_cast<size_t>(getOscIndex(index)),
					     static_cast<size_t>(envelope),
					     &applyType);
	return static_cast<EnvelopeApplyType>(applyType);
}

void GeonkickApi::setOscillatorEvelopePoints(int index,
                                             EnvelopeType envelope,
                                             const std::vector<EnvelopePoint> &points)
{
        if (points.empty())
                return;

        std::vector<struct gkick_envelope_point_info> data(points.size(), {0});
        for (decltype(points.size()) i = 0; i < points.size(); i++) {
                data[i].x = points[i].x();
                data[i].y = points[i].y();
                data[i].control_point = points[i].isControlPoint();
        }

        geonkick_osc_envelope_set_points(geonkickApi,
                                         getOscIndex(index),
                                         static_cast<int>(envelope),
                                         reinterpret_cast<const struct gkick_envelope_point_info *>(data.data()),
                                         points.size());
}

void GeonkickApi::setOscillatorEnvelopeApplyType(int index,
						 EnvelopeType envelope,
						 EnvelopeApplyType applyType)
{
	geonkick_osc_envelope_set_apply_type(geonkickApi,
					     getOscIndex(index),
					     static_cast<int>(envelope),
					     static_cast<enum gkick_envelope_apply_type>(applyType));
}

void GeonkickApi::addOscillatorEnvelopePoint(int oscillatorIndex,
                                             EnvelopeType envelope,
                                             const EnvelopePoint &point)
{
        struct gkick_envelope_point_info info{static_cast<float>(point.x()),
                                              static_cast<float>(point.y()),
                                              point.isControlPoint()};
        geonkick_osc_envelope_add_point(geonkickApi,
                                        getOscIndex(oscillatorIndex),
                                        static_cast<int>(envelope),
                                        &info);
}

void GeonkickApi::removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex)
{
        geonkick_osc_envelope_remove_point(geonkickApi,
                                           getOscIndex(oscillatorIndex),
                                           static_cast<int>(envelope),
                                           pointIndex);
}

void GeonkickApi::updateOscillatorEvelopePoint(int oscillatorIndex,
                                               EnvelopeType envelope,
                                               int pointIndex,
                                               const EnvelopePoint &point)
{
        struct gkick_envelope_point_info info{static_cast<float>(point.x()),
                                              static_cast<float>(point.y()),
                                              point.isControlPoint()};
        geonkick_osc_envelope_update_point(geonkickApi, getOscIndex(oscillatorIndex),
                                           static_cast<int>(envelope),
                                           pointIndex,
                                           &info);
}


void GeonkickApi::setOscillatorFunction(int oscillatorIndex, FunctionType function)
{
        geonkick_set_osc_function(geonkickApi,
                                  getOscIndex(oscillatorIndex),
                                  static_cast<enum geonkick_osc_func_type>(function));
}

void GeonkickApi::setOscillatorPhase(int oscillatorIndex, gkick_real phase)
{
        geonkick_set_osc_phase(geonkickApi,
                               getOscIndex(oscillatorIndex),
                               phase);
}

gkick_real GeonkickApi::oscillatorPhase(int oscillatorIndex) const
{
        gkick_real phase = 0;
        geonkick_get_osc_phase(geonkickApi,
                               getOscIndex(oscillatorIndex),
                               &phase);
        return phase;
}

void GeonkickApi::setOscillatorSeed(int oscillatorIndex, int seed)
{
        geonkick_set_osc_seed(geonkickApi,
                              getOscIndex(oscillatorIndex),
                              seed);
}

int GeonkickApi::oscillatorSeed(int oscillatorIndex) const
{
        unsigned int seed = 0;
        geonkick_get_osc_seed(geonkickApi,
                              getOscIndex(oscillatorIndex),
                              &seed);
        return seed;
}

GeonkickApi::FunctionType GeonkickApi::oscillatorFunction(int oscillatorIndex) const
{

        enum geonkick_osc_func_type function;
        geonkick_get_osc_function(geonkickApi,
                                  getOscIndex(oscillatorIndex),
                                  &function);
        return static_cast<FunctionType>(function);
}

bool GeonkickApi::setKickLength(double length)
{
        auto res = geonkick_set_length(geonkickApi, length / 1000);
        return res == GEONKICK_OK;
}

double GeonkickApi::kickLength(void) const
{
        gkick_real length = 0;
        geonkick_get_length(geonkickApi, &length);
        return 1000 * length;
}

bool GeonkickApi::setKickAmplitude(double amplitude)
{
        auto res = geonkick_kick_set_amplitude(geonkickApi, amplitude);
        return res == GEONKICK_OK;
}

double GeonkickApi::kickAmplitude() const
{
        gkick_real amplitude = 0;
        geonkick_kick_get_amplitude(geonkickApi, &amplitude);
        return static_cast<double>(amplitude);
}

std::vector<EnvelopePoint> GeonkickApi::getKickEnvelopePoints(EnvelopeType envelope) const
{
        struct gkick_envelope_point_info *buf;
        std::vector<EnvelopePoint> points;
        size_t npoints = 0;
        geonkick_kick_envelope_get_points(geonkickApi,
                                          static_cast<enum geonkick_envelope_type>(envelope),
                                          &buf,
                                          &npoints);
        for (decltype(npoints) i = 0; i < npoints; i++) {
                EnvelopePoint p;
                p.setX(buf[i].x);
                p.setY(buf[i].y);
                p.setAsControlPoint(buf[i].control_point);
                points.push_back(std::move(p));
        }

        if (buf)
                free(buf);
        return points;
}

GeonkickApi::EnvelopeApplyType GeonkickApi::getKickEnvelopeApplyType(EnvelopeType envelope) const
{
	enum gkick_envelope_apply_type applyType;
	geonkick_kick_env_get_apply_type(geonkickApi,
					 static_cast<enum geonkick_envelope_type>(envelope),
					 &applyType);
	return static_cast<EnvelopeApplyType>(applyType);
}

void GeonkickApi::setKickEnvelopePoints(EnvelopeType envelope,
                                        const std::vector<EnvelopePoint> &points)
{
        std::vector<struct gkick_envelope_point_info> data(points.size(), {0});
        for (decltype(points.size()) i = 0; i < points.size(); i++) {
                data[i].x             = points[i].x();
                data[i].y             = points[i].y();
                data[i].control_point = points[i].isControlPoint();
        }

        geonkick_kick_envelope_set_points(geonkickApi,
                                          static_cast<enum geonkick_envelope_type>(envelope),
                                          reinterpret_cast<const struct gkick_envelope_point_info*>(data.data()),
                                          points.size());
}

void GeonkickApi::setKickEnvelopeApplyType(EnvelopeType envelope, EnvelopeApplyType applyType)
{
        geonkick_kick_env_set_apply_type(geonkickApi,
					 static_cast<enum geonkick_envelope_type>(envelope),
					 static_cast<enum gkick_envelope_apply_type>(applyType));
}

bool GeonkickApi::enableKickFilter(bool b)
{
        auto res = geonkick_kick_filter_enable(geonkickApi, b);
        return res == GEONKICK_OK;
}

bool GeonkickApi::isKickFilterEnabled() const
{
        int enabled = 0;
        geonkick_kick_filter_is_enabled(geonkickApi, &enabled);
        return enabled;
}

bool GeonkickApi::setKickFilterType(FilterType type)
{
        auto res = geonkick_set_kick_filter_type(geonkickApi,
                                                 static_cast<enum gkick_filter_type>(type));
        return res == GEONKICK_OK;
}

GeonkickApi::FilterType GeonkickApi::kickFilterType() const
{
        enum gkick_filter_type type;
        geonkick_get_kick_filter_type(geonkickApi, &type);
        return static_cast<FilterType>(type);
}

bool GeonkickApi::setKickFilterFrequency(double frequency)
{
        auto res = geonkick_kick_set_filter_frequency(geonkickApi, frequency);
        return res == GEONKICK_OK;
}

double GeonkickApi::kickFilterFrequency(void) const
{
        gkick_real frequency;
        geonkick_kick_get_filter_frequency(geonkickApi, &frequency);
        return static_cast<double>(frequency);
}

bool GeonkickApi::setKickFilterQFactor(double factor)
{
        auto res = geonkick_kick_set_filter_factor(geonkickApi, factor);
        return res == GEONKICK_OK;
}

double GeonkickApi::kickFilterQFactor() const
{
        gkick_real factor = 0;
        geonkick_kick_get_filter_factor(geonkickApi, &factor);
        return static_cast<double>(factor);
}

void GeonkickApi::addKickEnvelopePoint(EnvelopeType envelope, const EnvelopePoint &point)
{
        struct gkick_envelope_point_info info{static_cast<float>(point.x()),
                                              static_cast<float>(point.y()),
                                              point.isControlPoint()};
        geonkick_kick_add_env_point(geonkickApi,
                                    static_cast<enum geonkick_envelope_type>(envelope),
                                    &info);
}

void GeonkickApi::updateKickEnvelopePoint(EnvelopeType envelope,
                                          int index,
                                          const EnvelopePoint &point)
{
        struct gkick_envelope_point_info info{static_cast<float>(point.x()),
                                              static_cast<float>(point.y()),
                                              point.isControlPoint()};
        geonkick_kick_update_env_point(geonkickApi,
                                       static_cast<enum geonkick_envelope_type>(envelope),
                                       index,
                                       &info);
}

 void GeonkickApi::removeKickEnvelopePoint(EnvelopeType envelope, int pointIndex)
{
        geonkick_kick_remove_env_point(geonkickApi,
                                       static_cast<enum geonkick_envelope_type>(envelope),
                                       pointIndex);
}

bool GeonkickApi::setOscillatorAmplitude(int oscillatorIndex, double amplitude)
{
        auto res = geonkick_set_osc_amplitude(geonkickApi,
                                              getOscIndex(oscillatorIndex),
                                              amplitude);
	if (res != GEONKICK_OK)
		return false;

	return true;
}

void GeonkickApi::setOscillatorAsFm(int oscillatorIndex, bool b)
{
        geonkick_osc_set_fm(geonkickApi,
                            getOscIndex(oscillatorIndex),
                            b);
}

void GeonkickApi::setOscillatorFmK(int oscillatorIndex, double k)
{
        geonkick_osc_set_fm_k(geonkickApi,
                              getOscIndex(oscillatorIndex),
                              k);
}

bool GeonkickApi::isOscillatorAsFm(int oscillatorIndex) const
{
        bool fm = false;
        geonkick_osc_is_fm(geonkickApi,
                           getOscIndex(oscillatorIndex),
                           &fm);
        return fm;
}

void GeonkickApi::enableOscillator(int oscillatorIndex, bool enable)
{
        if (enable)
                geonkick_enable_oscillator(geonkickApi, getOscIndex(oscillatorIndex));
        else
                geonkick_disable_oscillator(geonkickApi, getOscIndex(oscillatorIndex));
}

double GeonkickApi::oscillatorAmplitude(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_amplitude(geonkickApi,
                                       getOscIndex(oscillatorIndex),
                                       &value) != GEONKICK_OK)
		return 0;
	return value;
}

bool GeonkickApi::setOscillatorFrequency(int oscillatorIndex, double frequency)
{
	return geonkick_set_osc_frequency(geonkickApi,
                                          getOscIndex(oscillatorIndex),
                                          frequency) != GEONKICK_OK;
}

double GeonkickApi::oscillatorFrequency(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_frequency(geonkickApi,
                                       getOscIndex(oscillatorIndex),
                                       &value) != GEONKICK_OK) {
                return 0;
        }
	return value;
}

bool GeonkickApi::setOscillatorPitchShift(int oscillatorIndex, double semitones)
{
	return geonkick_set_osc_pitch_shift(geonkickApi,
                                            getOscIndex(oscillatorIndex),
                                            semitones) == GEONKICK_OK;
}

double GeonkickApi::oscillatorPitchShift(int oscillatorIndex) const
{
	gkick_real semitones = 0;
	if (geonkick_get_osc_pitch_shift(geonkickApi,
                                         getOscIndex(oscillatorIndex),
                                         &semitones) != GEONKICK_OK) {
                return 0;
        }
	return semitones;
}

bool GeonkickApi::setOscillatorNoiseDensity(int oscillatorIndex, double density)
{
	return geonkick_set_osc_noise_density(geonkickApi,
                                              getOscIndex(oscillatorIndex),
                                              density) == GEONKICK_OK;
}

double GeonkickApi::oscillatorNoiseDensity(int oscillatorIndex) const
{
	gkick_real density = 0;
	if (geonkick_get_osc_noise_density(geonkickApi,
                                           getOscIndex(oscillatorIndex),
                                           &density) != GEONKICK_OK) {
                return 0;
        }
	return density;
}

bool GeonkickApi::isOscillatorEnabled(int oscillatorIndex) const
{
        int enabled = 0;
        geonkick_is_oscillator_enabled(geonkickApi,
                                       getOscIndex(oscillatorIndex),
                                       &enabled);
        return enabled;
}

double GeonkickApi::kickMaxLength(void) const
{
        gkick_real len = 0;
        geonkick_get_max_length(geonkickApi, &len);
        return len * 1000;
}

bool GeonkickApi::enableOscillatorFilter(int oscillatorIndex, bool enable)
{
        auto res = geonkick_enbale_osc_filter(geonkickApi,
                                              getOscIndex(oscillatorIndex),
                                              enable);
        return res == GEONKICK_OK;
}

bool GeonkickApi::isOscillatorFilterEnabled(int oscillatorIndex) const
{
        int enabled = false;
        geonkick_osc_filter_is_enabled(geonkickApi,
                                       getOscIndex(oscillatorIndex),
                                       &enabled);
        return enabled;
}

bool GeonkickApi::setOscillatorFilterType(int oscillatorIndex, FilterType filter)
{
        auto res = geonkick_set_osc_filter_type(geonkickApi,
                                                getOscIndex(oscillatorIndex),
                                                static_cast<enum gkick_filter_type>(filter));
        return res == GEONKICK_OK;
}

GeonkickApi::FilterType GeonkickApi::getOscillatorFilterType(int oscillatorIndex) const
{
        enum gkick_filter_type type;
        geonkick_get_osc_filter_type(geonkickApi,
                                     getOscIndex(oscillatorIndex),
                                     &type);
        return static_cast<FilterType>(type);
}

bool GeonkickApi::setOscillatorFilterCutOffFreq(int oscillatorIndex, double frequency)
{
        auto res = geonkick_set_osc_filter_cutoff_freq(geonkickApi,
                                                       getOscIndex(oscillatorIndex),
                                                       frequency);
        return res == GEONKICK_OK;
}

double GeonkickApi::getOscillatorFilterCutOffFreq(int oscillatorIndex) const
{
        gkick_real frequency = 0;
        geonkick_get_osc_filter_cutoff_freq(geonkickApi,
                                            getOscIndex(oscillatorIndex),
                                            &frequency);
        return frequency;
}

bool GeonkickApi::setOscillatorFilterFactor(int oscillatorIndex, double factor)
{
        auto res = geonkick_set_osc_filter_factor(geonkickApi,
                                                  getOscIndex(oscillatorIndex),
                                                  factor);
        return res == GEONKICK_OK;
}

double GeonkickApi::getOscillatorFilterFactor(int oscillatorIndex) const
{
        gkick_real factor = 0;
        geonkick_get_osc_filter_factor(geonkickApi,
                                       getOscIndex(oscillatorIndex),
                                       &factor);
        return factor;
}

bool GeonkickApi::isOscDistortionEnabled(int oscillatorIndex) const
{
        bool enabled = false;
        geonkick_osc_distortion_is_enabled(geonkickApi,
                                           getOscIndex(oscillatorIndex),
                                           &enabled);
        return enabled;
}

bool GeonkickApi::oscEnableDistortion(int oscillatorIndex, bool b)
{
        auto res = geonkick_osc_distortion_enable(geonkickApi,
                                                  getOscIndex(oscillatorIndex),
                                                  b);
        return res == GEONKICK_OK;
}

GeonkickApi::DistortionType GeonkickApi::getOscDistortionType(int oscillatorIndex) const
{
        enum gkick_distortion_type distortionType = GEONKICK_DISTORTION_SOFT_CLIPPING_TANH;
        geonkick_osc_distortion_get_type(geonkickApi,
                                         getOscIndex(oscillatorIndex),
                                         &distortionType);
        return static_cast<GeonkickApi::DistortionType>(distortionType);
}

bool GeonkickApi::setOscDistortionType(int oscillatorIndex, DistortionType type)
{
        auto res = geonkick_osc_distortion_set_type(geonkickApi,
                                                    getOscIndex(oscillatorIndex),
                                                    static_cast<enum gkick_distortion_type>(type));
        return res == GEONKICK_OK;
}

double GeonkickApi::getOscDistortionInLimiter(int oscillatorIndex) const
{
        float value = 0.0f;
        geonkick_osc_distortion_get_in_limiter(geonkickApi,
                                               getOscIndex(oscillatorIndex),
                                               &value);
        return value;
}

bool GeonkickApi::setOscDistortionInLimiter(int oscillatorIndex, double val)
{
        auto res = geonkick_osc_distortion_set_in_limiter(geonkickApi,
                                                          getOscIndex(oscillatorIndex),
                                                          val);
        return res == GEONKICK_OK;
}

double GeonkickApi::getOscDistortionOutLimiter(int oscillatorIndex) const
{
        float value = 0.0f;
        geonkick_osc_distortion_get_out_limiter(geonkickApi,
                                                getOscIndex(oscillatorIndex),
                                                &value);
        return value;
}

bool GeonkickApi::setOscDistortionOutLimiter(int oscillatorIndex, double val)
{
        auto res = geonkick_osc_distortion_set_out_limiter(geonkickApi,
                                                           getOscIndex(oscillatorIndex),
                                                           val);
        return res == GEONKICK_OK;
}

double GeonkickApi::getOscDistortionDrive(int oscillatorIndex) const
{
        float value = 0.0;
        geonkick_osc_distortion_get_drive(geonkickApi,
                                          getOscIndex(oscillatorIndex),
                                          &value);
        return value;
}

bool GeonkickApi::setOscDistortionDrive(int oscillatorIndex, double val)
{
        auto res = geonkick_osc_distortion_set_drive(geonkickApi,
                                                     getOscIndex(oscillatorIndex),
                                                     val);
        return res == GEONKICK_OK;
}

double GeonkickApi::limiterValue() const
{
        gkick_real val = 0;
        geonkick_get_limiter_value(geonkickApi, &val);
        return val;
}

void GeonkickApi::setLimiterValue(double value)
{
        geonkick_set_limiter_value(geonkickApi, value);
}

void GeonkickApi::kickUpdatedCallback(void *arg,
                                      gkick_real *buff,
                                      size_t size,
                                      size_t id)
{
        GEONKICK_LOG_DEBUG("size[" << id << "] : " << size);
        std::vector<gkick_real> buffer(size, 0);
        std::memcpy(buffer.data(), buff, size * sizeof(gkick_real));
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj)
                obj->updateKickBuffer(std::move(buffer), id);
}

void GeonkickApi::limiterCallback(void *arg, size_t index, gkick_real val)
{
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj)
                obj->setLimiterLevelerValue(index, val);
}

void GeonkickApi::setLimiterLevelerValue(size_t index, double val)
{
        if (index < limiterLevelers.size())
                limiterLevelers[index] = val;
}

double GeonkickApi::getLimiterLevelerValue(size_t index) const
{
        if (index == static_cast<size_t>(-1))
                index = currentPercussion();
        if (index < limiterLevelers.size())
                return limiterLevelers[index];
        return 0;
}

void GeonkickApi::updateKickBuffer(const std::vector<gkick_real> &&buffer,
                                   size_t id)
{
        GEONKICK_LOG_DEBUG("id: " << id);
        std::lock_guard<std::mutex> lock(apiMutex);
        if (id < numberOfInstruments()) {
                GEONKICK_LOG_DEBUG("kickBuffers[id] = buffer" << id);
                kickBuffers[id] = buffer;
        }
        if (eventQueue && id == currentPercussion()) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([&](void){ kickUpdated(); });
                eventQueue->postAction(std::move(act));
                GEONKICK_LOG_DEBUG("eventQueue->postAction / kickUpdated()");
        }
}

std::vector<gkick_real> GeonkickApi::getInstrumentBuffer(int id) const
{
        {
                std::lock_guard<std::mutex> lock(apiMutex);
                if (static_cast<decltype(kickBuffers.size())>(id) < kickBuffers.size())
                        return kickBuffers[id];
        }
        return std::vector<gkick_real>();
}

std::vector<gkick_real> GeonkickApi::getKickBuffer() const
{
        std::lock_guard<std::mutex> lock(apiMutex);
        return kickBuffers[currentPercussion()];
}

int GeonkickApi::getSampleRate() const
{
        int sample_rate;
        if (geonkick_get_sample_rate(geonkickApi, &sample_rate) != GEONKICK_OK)
                sample_rate = 0;

        return sample_rate;
}

// This function is called only from the audio thread.
void GeonkickApi::setKeyPressed(bool b, int note, int velocity)
{
        geonkick_key_pressed(geonkickApi, b, note, velocity);
}

void GeonkickApi::playKick(int id)
{
        if (id < 0)
                id = currentPercussion();
        geonkick_play(geonkickApi, id);
}

void GeonkickApi::playSamplePreview()
{
        geonkick_play_sample_preview(geonkickApi);
}

void GeonkickApi::process(float** out, size_t offset, size_t size)
{
        geonkick_audio_process(geonkickApi, out, offset, size);
}

bool GeonkickApi::enableDistortion(bool enable)
{
        auto res = geonkick_distortion_enable(geonkickApi, enable);
        return res == GEONKICK_OK;
}

bool GeonkickApi::isDistortionEnabled() const
{
        bool enabled = false;
        geonkick_distortion_is_enabled(geonkickApi, &enabled);
        return enabled;
}

bool GeonkickApi::setDistortionType(GeonkickApi::DistortionType type)
{
        auto res = geonkick_distortion_set_type(geonkickApi, static_cast<enum gkick_distortion_type>(type));
        return res == GEONKICK_OK;
}

GeonkickApi::DistortionType GeonkickApi::getDistortionType() const
{
        enum gkick_distortion_type type = GEONKICK_DISTORTION_SOFT_CLIPPING_TANH;
        geonkick_distortion_get_type(geonkickApi, &type);
        return static_cast<GeonkickApi::DistortionType>(type);
}

bool GeonkickApi::setDistortionInLimiter(double value)
{
        auto res = geonkick_distortion_set_in_limiter(geonkickApi, value);
        return res == GEONKICK_OK;
}

double GeonkickApi::getDistortionInLimiter() const
{
        float value = 0.0f;
        geonkick_distortion_get_in_limiter(geonkickApi, &value);
        return value;
}

bool GeonkickApi::setDistortionOutLimiter(double value)
{
        auto res = geonkick_distortion_set_out_limiter(geonkickApi, value);
        return res == GEONKICK_OK;
}

double GeonkickApi::getDistortionOutLimiter(void) const
{
        gkick_real value = 0;
        geonkick_distortion_get_out_limiter(geonkickApi, &value);
        return value;
}

bool GeonkickApi::setDistortionDrive(double drive)
{
        auto res = geonkick_distortion_set_drive(geonkickApi, drive);
        return res == GEONKICK_OK;
}

double GeonkickApi::getDistortionDrive(void) const
{
        gkick_real drive = 0;
        geonkick_distortion_get_drive(geonkickApi, &drive);
        return drive;
}

void GeonkickApi::registerCallbacks(bool b)
{
        if (b) {
                geonkick_set_kick_buffer_callback(geonkickApi,
                                                  &GeonkickApi::kickUpdatedCallback,
                                                  this);
                geonkick_set_kick_limiter_callback(geonkickApi,
                                                   &GeonkickApi::limiterCallback,
                                                   this);
        } else {
                geonkick_set_kick_buffer_callback(geonkickApi, NULL, NULL);
                geonkick_set_kick_limiter_callback(geonkickApi, NULL, NULL);
        }
}

bool GeonkickApi::isJackEnabled() const
{
        return jackEnabled;
}

void GeonkickApi::setStandalone(bool b)
{
        standaloneInstance = b;
}

bool GeonkickApi::isStandalone() const
{
        return standaloneInstance;
}

void GeonkickApi::triggerSynthesis()
{
        geonkick_enable_synthesis(geonkickApi, true);
}

void GeonkickApi::setLayer(Layer layer)
{
        currentLayer = layer;
}

GeonkickApi::Layer GeonkickApi::layer() const
{
        return currentLayer;
}

void GeonkickApi::setLayerAmplitude(Layer layer, double amplitude)
{
        geonkick_group_set_amplitude(geonkickApi,
                                     static_cast<size_t>(layer),
                                     amplitude);
}

double GeonkickApi::getLayerAmplitude(Layer layer) const
{
        gkick_real amplitude = 0;
        geonkick_group_get_amplitude(geonkickApi,
                                     static_cast<size_t>(layer),
                                     &amplitude);
        return amplitude;
}

void GeonkickApi::enbaleLayer(Layer layer, bool enable)
{
        geonkick_enable_group(geonkickApi,
                              static_cast<int>(layer),
                              enable);
}

bool GeonkickApi::isLayerEnabled(Layer layer) const
{
        bool enabled = false;
        geonkick_group_enabled(geonkickApi,
                               static_cast<int>(layer),
                               &enabled);
        return enabled;
}

int GeonkickApi::getOscIndex(int index) const
{
        return index + GKICK_OSC_GROUP_SIZE * static_cast<int>(currentLayer);
}

std::filesystem::path
GeonkickApi::currentWorkingPath(const std::string &key) const
{
        auto it = workingPaths.find(key);
        if (it != workingPaths.end())
                return it->second;
        return getSettings("GEONKICK_CONFIG/HOME_PATH");
}

void GeonkickApi::setCurrentWorkingPath(const std::string &key,
                                        const std::filesystem::path &path)
{
        workingPaths[key] = path;
}

bool GeonkickApi::setPercussionLimiter(size_t id, double val)
{
	return geonkick_percussion_set_limiter(geonkickApi, id, val) == GEONKICK_OK;
}

double GeonkickApi::percussionLimiter(size_t id) const
{
        gkick_real val = 0.0f;
        geonkick_percussion_get_limiter(geonkickApi, id, &val);
        return val;
}

bool GeonkickApi::mutePercussion(size_t id, bool b)
{
        return geonkick_percussion_mute(geonkickApi, id, b) == GEONKICK_OK;
}

bool GeonkickApi::isPercussionMuted(size_t id) const
{
        bool muted = false;
        geonkick_percussion_is_muted(geonkickApi, id, &muted);
        return muted;
}

bool GeonkickApi::soloPercussion(size_t id, bool b)
{
        return geonkick_percussion_solo(geonkickApi, id, b) == GEONKICK_OK;
}

bool GeonkickApi::isPercussionSolo(size_t id) const
{
        bool solo = false;
        geonkick_percussion_is_solo(geonkickApi, id, &solo);
        return solo;
}

bool GeonkickApi::enableNoteOff(size_t id, bool b)
{
        return geonkick_percussion_enable_note_off(geonkickApi, id, b) == GEONKICK_OK;
}

bool GeonkickApi::isNoteOffEnabled(size_t id) const
{
        bool enabled = false;
        geonkick_percussion_note_off_enabled(geonkickApi, id, &enabled);
        return enabled;
}

int GeonkickApi::getUnusedPercussion() const
{
        int index;
        geonkick_unused_percussion(geonkickApi, &index);
        return index;
}

bool GeonkickApi::enablePercussion(int index, bool enable)
{
        auto res = geonkick_enable_percussion(geonkickApi, index, enable);
        return res == GEONKICK_OK;
}

bool GeonkickApi::isPercussionEnabled(int index) const
{
        bool enabled = false;
        geonkick_is_percussion_enabled(geonkickApi, index, &enabled);
        return enabled;
}

size_t GeonkickApi::enabledPercussions() const
{
        auto n = numberOfInstruments();
        size_t enabled  = 0;
        for (decltype(n) i = 0; i < n; i++) {
                if (isPercussionEnabled(i))
                        enabled++;
        }
        return enabled;
}

bool GeonkickApi::setPercussionPlayingKey(int index, int key)
{
        auto res = geonkick_set_playing_key(geonkickApi,
                                            index,
                                            key);
        GEONKICK_LOG_DEBUG("index " << index << " key:" << key);
        return res == GEONKICK_OK;
}

int GeonkickApi::getPercussionPlayingKey(int index) const
{
        signed char key = -1;
        geonkick_get_playing_key(geonkickApi,
                                 index,
                                 &key);
        return key;
}

int GeonkickApi::percussionsReferenceKey() const
{
        // MIDI key A4
        return 69;
}

bool GeonkickApi::setPercussionChannel(int index, size_t channel)
{
        auto res = geonkick_set_percussion_channel(geonkickApi,
                                                   index,
                                                   channel);
        return res == GEONKICK_OK;
}

int GeonkickApi::getPercussionChannel(int index) const
{
        size_t channel;
        auto res = geonkick_get_percussion_channel(geonkickApi,
                                                   index,
                                                   &channel);
        if (res != GEONKICK_OK)
                return -1;
        return channel;
}

bool GeonkickApi::setPercussionMidiChannel(int index, size_t channel)
{
        auto res = geonkick_set_midi_channel(geonkickApi,
                                             index,
                                             channel);
        return res == GEONKICK_OK;
}

int GeonkickApi::getPercussionMidiChannel(int index) const
{
        signed char channel;
        auto res = geonkick_get_midi_channel(geonkickApi,
                                             index,
                                             &channel);
        if (res != GEONKICK_OK)
                return -1;
        return channel;
}

bool GeonkickApi::forceMidiChannel(size_t channel, bool force)
{
        auto res = geonkick_force_midi_channel(geonkickApi, channel, force);
        return res == GEONKICK_OK;
}

bool GeonkickApi::isMidiChannelForced() const
{
        bool forced = false;
        geonkick_ged_forced_midi_channel(geonkickApi, nullptr, &forced);
        return forced;
}

bool GeonkickApi::setPercussionName(int index, const std::string &name)
{
        auto res = geonkick_set_percussion_name(geonkickApi,
                                                index,
                                                name.c_str(),
                                                name.size());
        return res == GEONKICK_OK;
}

std::string GeonkickApi::getPercussionName(int index) const
{
        auto n = numberOfInstruments();
        if (index > -1 && index < static_cast<decltype(index)>(n)) {
                char name[30];
                geonkick_get_percussion_name(geonkickApi,
                                             index,
                                             name,
                                             sizeof(name));
                return name;
        }
        return "";
}

void GeonkickApi::setSettings(const std::string &key, const std::string &value)
{
        uiSettings->setSettings(key, value);
}

std::string GeonkickApi::getSettings(const std::string &key) const
{
        return uiSettings->getSettings(key);
}

void GeonkickApi::tuneAudioOutput(int id, bool tune)
{
        geonkick_tune_audio_output(geonkickApi,
                                   id,
                                   tune);
}

bool GeonkickApi::isAudioOutputTuned(int id) const
{
        bool tune = false;
        geonkick_is_audio_output_tuned(geonkickApi,
                                       id,
                                       &tune);
        return tune;
}

bool GeonkickApi::setCurrentPercussion(int index)
{
        auto res = geonkick_set_current_percussion(geonkickApi, index);
        return res == GEONKICK_OK;
}

size_t GeonkickApi::currentPercussion() const
{
        size_t index = 0;
        geonkick_get_current_percussion(geonkickApi, &index);
        return index;
}

void GeonkickApi::setOscillatorSample(const std::string &file,
                                      int oscillatorIndex)
{
        int sRate = Geonkick::defaultSampleRate;
        geonkick_get_sample_rate(geonkickApi, &sRate);
        std::vector<gkick_real> sampleData = loadSample(file,
                                                        kickMaxLength() / 1000,
                                                        sRate,
                                                        1);
        if (!sampleData.empty())
                setOscillatorSample(sampleData, oscillatorIndex);
}

void GeonkickApi::setOscillatorSample(const std::vector<float> &sample,
                                      int oscillatorIndex)
{
        geonkick_set_osc_sample(geonkickApi,
                                getOscIndex(oscillatorIndex),
                                sample.data(),
                                sample.size());
}

std::vector<float>
GeonkickApi::getOscillatorSample(int oscillatorIndex) const
{
        gkick_real *data = nullptr;
        size_t size = 0;
        geonkick_get_osc_sample(geonkickApi,
                                getOscIndex(oscillatorIndex),
                                &data,
                                &size);
        if (data)
                return std::vector<float>(data, data + size);
        return {};
}

std::vector<gkick_real> GeonkickApi::loadSample(const std::string &file,
                                                double length,
                                                int sampleRate,
                                                int channels)
{
        GEONKICK_UNUSED(channels);

        SF_INFO sndinfo;
        memset(&sndinfo, 0, sizeof(sndinfo));
        SNDFILE *sndFile = sf_open(file.c_str(), SFM_READ, &sndinfo);
        if (!sndFile) {
                GEONKICK_LOG_ERROR("can't open sample file");
                return std::vector<gkick_real>();
        }

        auto formatType = sndinfo.format & SF_FORMAT_TYPEMASK;
        if (formatType != SF_FORMAT_FLAC
            && formatType != SF_FORMAT_WAV
            && formatType != SF_FORMAT_FLAC
            && formatType != SF_FORMAT_WAVEX
            && formatType != SF_FORMAT_OGG) {
                GEONKICK_LOG_ERROR(std::hex << "unsupported audio format");
                sf_close(sndFile);
                return std::vector<gkick_real>();
        }

        std::vector<float> data(sndinfo.samplerate * length * sndinfo.channels, 0.0f);
        auto n = sf_read_float(sndFile, data.data(), data.size());
        sf_close(sndFile);
        if (static_cast<decltype(data.size())>(n) < 1) {
                GEONKICK_LOG_ERROR("error on reading samples");
                return std::vector<gkick_real>();
        } else if (static_cast<decltype(data.size())>(n) < data.size()) {
                GEONKICK_LOG_DEBUG("read less then data, resize to " << n);
                data.resize(n);
        }

        if (sndinfo.channels > 1) {
                GEONKICK_LOG_DEBUG("multichannel file, get only the first channel");
                for (decltype(data.size()) i = 0; i < data.size(); i += sndinfo.channels)
                        data[i / sndinfo.channels] = data[i];
                data.resize(data.size() / sndinfo.channels);
        }

        float max = std::fabs(*std::max_element(data.begin(), data.end(),
                                                [](float a, float b){ return fabs(a) < fabs(b); }));
        if (max > std::numeric_limits<float>::min()) {
                float k = 1.0f / max;
                for (auto &v: data)
                        v *= k;
        }

        if (sampleRate != sndinfo.samplerate) {
                GEONKICK_LOG_DEBUG("different sample rate " << sndinfo.samplerate
                                   << ", resample to " << sampleRate);
                float f = static_cast<float>(sndinfo.samplerate) / sampleRate;
                float x = 0.0f;
                std::vector<float> out_data;
                for (decltype(data.size()) i = 0; i < data.size() - 1;) {
                        float d = x - i;
                        float val = data[i] * (1.0f - d) + data[i + 1] * d;
                        x += f;
                        i = x;
                        out_data.push_back(val);
                }
                return out_data;
        }

        return data;
}

void GeonkickApi::setKitName(const std::string &name)
{
        kitName = name;
}

std::string GeonkickApi::getKitName() const
{
        return kitName;
}

void GeonkickApi::setKitAuthor(const std::string &author)
{
        kitAuthor = author;
}

std::string GeonkickApi::getKitAuthor() const
{
        return kitAuthor;
}

void GeonkickApi::setKitUrl(const std::string &url)
{
        kitUrl = url;
}

std::string GeonkickApi::getKitUrl() const
{
        return kitUrl;
}

void GeonkickApi::copyToClipboard()
{
        clipboardPercussion = getPercussionState();
}

void GeonkickApi::pasteFromClipboard()
{
        if (clipboardPercussion) {
                auto state = std::make_unique<PercussionState>(*clipboardPercussion);
                auto currId = currentPercussion();
                state->setId(currId);
                state->setName(getPercussionName(currId));
                state->setPlayingKey(getPercussionPlayingKey(currId));
                state->setChannel(getPercussionChannel(currId));
                state->setMidiChannel(getPercussionMidiChannel(currId));
                state->setMute(isPercussionMuted(currId));
                state->setSolo(isPercussionSolo(currId));
                setPercussionState(state);
        }
}

void GeonkickApi::notifyUpdateGraph()
{
        if (eventQueue) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([&](void){ action kickUpdated(); });
                eventQueue->postAction(std::move(act));
        }
}

void GeonkickApi::notifyUpdateParameters()
{
        if (eventQueue) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([&](void){ action stateChanged(); });
                eventQueue->postAction(std::move(act));
        }
}

void GeonkickApi::notifyUpdateGui()
{
        if (eventQueue) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([&](void){
                                action kickUpdated();
                                action stateChanged();
                        });
                eventQueue->postAction(std::move(act));
        }
}

void GeonkickApi::notifyPercussionUpdated(int id)
{
        if (eventQueue) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([this, id](void){
                                GEONKICK_LOG_DEBUG("update percussion :" << id);
                                action percussionUpdated(id);
                        });
                eventQueue->postAction(std::move(act));
        }
}

void GeonkickApi::notifyKitUpdated()
{
        if (eventQueue) {
                auto act = std::make_unique<RkAction>();
                act->setCallback([&](void){
                                action kitUpdated();
                        });
                eventQueue->postAction(std::move(act));
        }
}

std::vector<int> GeonkickApi::ordredPercussionIds() const
{
        return percussionIdList;
}

void GeonkickApi::removeOrderedPercussionId(int id)
{
        for (auto it = percussionIdList.begin(); it != percussionIdList.end(); ++it) {
                if (*it == id) {
                        percussionIdList.erase(it);
                        break;
                }
        }
}

void GeonkickApi::addOrderedPercussionId(int id)
{
        removeOrderedPercussionId(id);
        percussionIdList.push_back(id);
}

void GeonkickApi::clearOrderedPercussionIds()
{
        percussionIdList.clear();
}

bool GeonkickApi::moveOrdrepedPercussionId(int index, int n)
{
        if (index < 0)
                return false;

        auto size = percussionIdList.size();
        for (decltype(percussionIdList.size()) i = 0; i < size; i++) {
                if (percussionIdList[i] == index) {
                        auto newId = static_cast<decltype(n)>(i) + n;
                        if (newId > -1 && static_cast<decltype(size)>(newId) < size) {
                                std::swap(percussionIdList[i], percussionIdList[newId]);
                                return true;
                        }
                }
        }
        return false;
}

void GeonkickApi::loadPresets()
{
        std::unordered_set<std::string> prestsPaths;
        auto presetsPathSufix = std::filesystem::path(GEONKICK_APP_NAME) / "presets";
        std::filesystem::path presetsPath = getSettings("GEONKICK_CONFIG/USER_PRESETS_PATH");
        prestsPaths.insert(presetsPath.string());

#ifdef GEONKICK_DATA_DIR
        prestsPaths.insert((std::filesystem::path(GEONKICK_DATA_DIR) / presetsPathSufix).string());
#endif // GEONKICK_DATA_DIR

        const char *dataDirs = std::getenv("XDG_DATA_DIRS");
        if (dataDirs == nullptr || *dataDirs == '\0') {
                prestsPaths.insert((std::filesystem::path("/usr/share") / presetsPathSufix).string());
                prestsPaths.insert((std::filesystem::path("/usr/local/share") / presetsPathSufix).string());
        } else {
                std::stringstream ss(dataDirs);
                std::string path;
                while (std::getline(ss, path, ':'))
                        prestsPaths.insert((std::filesystem::path(path) / presetsPathSufix).string());
        }

        for (const auto &path: prestsPaths) {
                try {
                        if (std::filesystem::exists(path))
                                loadPresetsFolders(path);
                } catch(const std::exception& e) {
                        GEONKICK_LOG_ERROR("error on reading path: " << path << ": " << e.what());
                }
        }

        // Load custom preset folders.
        GeonkickConfig cfg;
        for (const auto &folder: cfg.getCustomPresetFolders()) {
                auto presetFolder = std::make_unique<PresetFolder>(folder);
                presetFolder->setAsCustom();
                presetsFoldersList.emplace_back(std::move(presetFolder));
        }
}

void GeonkickApi::loadPresetsFolders(const std::filesystem::path &path)
{
        try {
                for (const auto &entry : std::filesystem::directory_iterator(path)) {
                        if (!entry.path().empty() && std::filesystem::is_directory(entry.path())) {
                                auto presetFolder = std::make_unique<PresetFolder>(entry.path());
                                if (presetFolder->numberOfPresets() > 0)
                                        presetsFoldersList.push_back(std::move(presetFolder));
                        }
                }
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading path: " << path);
        }
}

void GeonkickApi::setupPaths()
{
	DesktopPaths desktopPaths;
	setSettings("GEONKICK_CONFIG/HOME_PATH", desktopPaths.getHomePath().string());
	setSettings("GEONKICK_CONFIG/USER_PRESETS_PATH", desktopPaths.getPresetsPath().string());
	setSettings("GEONKICK_CONFIG/USER_DATA_PATH", desktopPaths.getDataPath().string());

	try {
                if (!std::filesystem::exists(desktopPaths.getDataPath())) {
                        if (!std::filesystem::create_directories(desktopPaths.getDataPath())) {
                                GEONKICK_LOG_ERROR("can't create path " << desktopPaths.getDataPath());
                                return;
                        }
                }

                if (!std::filesystem::exists(desktopPaths.getPresetsPath())) {
                        if (!std::filesystem::create_directories(desktopPaths.getPresetsPath())) {
                                GEONKICK_LOG_ERROR("can't create path " << desktopPaths.getPresetsPath());
                                return;
                        }
                }
	} catch(const std::exception& e) {
                GEONKICK_LOG_ERROR("error on setup user data paths: " << e.what());
        }
}

PresetFolder* GeonkickApi::getPresetFolder(size_t index) const
{
        if (index < presetsFoldersList.size())
                return presetsFoldersList[index].get();
        return nullptr;
}

PresetFolder* GeonkickApi::addPresetFolder(const std::filesystem::path &folder, bool custom)
{
        auto it = std::find_if(presetsFoldersList.cbegin(),
                               presetsFoldersList.cend(), [&folder](const auto &e)
                               {
                                       return e->path() == folder;
                               });
        if (it == presetsFoldersList.cend()) {
                auto presetFolder = std::make_unique<PresetFolder>(folder);
                presetFolder->setAsCustom(custom);
                GeonkickConfig cfg;
                cfg.addCustomPresetFolder(presetFolder->path());
                cfg.save();
                return presetsFoldersList.emplace_back(std::move(presetFolder)).get();
        }
        return nullptr;
}

bool GeonkickApi::removePresetFolder(const PresetFolder *folder)
{
        auto folderPath = folder->path();
        presetsFoldersList.erase(std::remove_if(presetsFoldersList.begin(),
                                                presetsFoldersList.end(),
                                                [&folder](const auto &p)
                                                { return p->path() == folder->path(); }),
                                 presetsFoldersList.end());

        GeonkickConfig cfg;
        cfg.removeCustomPresetFolder(folderPath);
        cfg.save();
        return true;
}

size_t GeonkickApi::numberOfPresetFolders() const
{
        return presetsFoldersList.size();
}

UiSettings* GeonkickApi::getUiSettings() const
{
        return uiSettings.get();
}

void GeonkickApi::setState(const std::string &data)
{
        rapidjson::Document document;
        document.Parse(data.c_str());
        if (!document.IsObject())
                return;

        for (const auto &m: document.GetObject()) {
                if (m.name == "UiSettings" && m.value.IsObject())
                        uiSettings->fromJsonObject(m.value);
                if (m.name == "KitState" && m.value.IsObject()) {
                        auto kitState = std::make_unique<KitState>();
                        kitState->fromJsonObject(m.value);
                        setKitState(std::move(kitState));
                }
        }
}

std::string GeonkickApi::getState() const
{
        std::ostringstream jsonStream;
        jsonStream << "{\"UiSettings\": " << std::endl;
        jsonStream << uiSettings->toJson() << ", " << std::endl;
        jsonStream << "\"KitState\": " << std::endl;
        jsonStream << getKitState()->toJson() << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

std::vector<gkick_real> GeonkickApi::setPreviewSample(const std::string &file)
{
        int sRate = Geonkick::defaultSampleRate;
        geonkick_get_sample_rate(geonkickApi, &sRate);
        std::vector<gkick_real> sampleData = loadSample(file,
                                                        kickMaxLength() / 1000,
                                                        sRate,
                                                        1);
        if (!sampleData.empty()) {
                geonkick_set_preview_sample(geonkickApi, sampleData.data(), sampleData.size());
                return sampleData;
        }
        return std::vector<float>();
}

void GeonkickApi::setSamplePreviewLimiter(double val)
{
        geonkick_set_sample_preview_limiter(geonkickApi, val);
}

double GeonkickApi::samplePreviewLimiter() const
{
        gkick_real val = 0;
        geonkick_get_sample_preview_limiter(geonkickApi, &val);
        return val;
}

double GeonkickApi::getScaleFactor() const
{
        return scaleFactor;
}

void GeonkickApi::setScaleFactor(double factor)
{
        scaleFactor = factor;
        GeonkickConfig config;
        config.setScaleFactor(scaleFactor);
        config.save();
}
