/**
 * File name: geonkick_api.cpp
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

#include "geonkick_api.h"
#include "oscillator.h"
#include "globals.h"
#include "geonkick_state.h"

#include <RkEventQueue.h>

#include <geonkick.h>
#include <sndfile.h>

GeonkickApi::GeonkickApi()
        :geonkickApi{nullptr}
        , updateLimiterLeveler{false}
        , limiterLevelerVal{0}
        , jackEnabled{false}
        , standaloneInstance{false}
        , eventQueue{nullptr}
        , currentLayer{Layer::Layer1}
{
}

GeonkickApi::~GeonkickApi()
{
  	if (geonkickApi)
                geonkick_free(&geonkickApi);
}

void GeonkickApi::setEventQueue(RkEventQueue *queue)
{
        std::lock_guard<std::mutex> lock(apiMutex);
        eventQueue = queue;
}

bool GeonkickApi::init()
{
  	if (geonkick_create(&geonkickApi) != GEONKICK_OK) {
	        GEONKICK_LOG_ERROR("can't create geonkick API");
                return false;
  	}
        jackEnabled = geonkick_is_module_enabed(geonkickApi, GEONKICK_MODULE_JACK);
        setState(getDefaultState());
        return true;
}

std::shared_ptr<GeonkickState> GeonkickApi::getDefaultState()
{
        std::shared_ptr<GeonkickState> state = std::make_shared<GeonkickState>();
        state->setLimiterValue(1.0);
        state->tuneOutput(false);
        state->setKickLength(300);
        state->setKickAmplitude(0.8);
        state->enableKickFilter(false);
        state->setKickFilterFrequency(200);
        state->setKickFilterQFactor(1.0);
        state->setKickFilterType(GeonkickApi::FilterType::LowPass);
        std::vector<RkRealPoint> envelope;
        envelope.push_back({0, 1});
        envelope.push_back({1, 1});
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude, envelope);
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff, envelope);
        state->enableCompressor(false);
        state->setCompressorAttack(0.01);
        state->setCompressorRelease(0.01);
        state->setCompressorThreshold(0);
        state->setCompressorRatio(1);
        state->setCompressorKnee(0);
        state->setCompressorMakeup(1);
        state->enableDistortion(false);
        state->setDistortionVolume(0.1);
        state->setDistortionDrive(0);

        std::vector<GeonkickApi::OscillatorType> oscillators = {
                GeonkickApi::OscillatorType::Oscillator1,
                GeonkickApi::OscillatorType::Oscillator2,
                GeonkickApi::OscillatorType::Noise
        };

        std::vector<GeonkickApi::Layer> layers = {
                GeonkickApi::Layer::Layer1,
                GeonkickApi::Layer::Layer2,
                GeonkickApi::Layer::Layer3
        };


        for (auto layer: layers) {
                state->setLayerEnabled(layer, layer == Layer::Layer1);
                state->setLayerAmplitude(layer, 1.0);
                for (auto const &osc: oscillators) {
                        int index = static_cast<int>(osc) + GKICK_OSC_GROUP_SIZE * static_cast<int>(layer);
                        state->setOscillatorEnabled(index, osc == GeonkickApi::OscillatorType::Oscillator1);
                        if (osc == GeonkickApi::OscillatorType::Noise) {
                                state->setOscillatorFunction(index, GeonkickApi::FunctionType::NoiseWhite);
                        } else {
                                state->setOscillatorFunction(index, GeonkickApi::FunctionType::Sine);
                                state->setOscillatorPhase(index, 0);
                        }

                        state->setOscillatorAmplitue(index, 0.26);
                        state->setOscillatorFrequency(index, 800);
                        state->setOscillatorFilterEnabled(index, false);
                        state->setOscillatorFilterType(index, GeonkickApi::FilterType::LowPass);
                        state->setOscillatorFilterCutOffFreq(index, 800);
                        state->setOscillatorFilterFactor(index, 1);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Amplitude);
                        if (osc != GeonkickApi::OscillatorType::Noise)
                                state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Frequency);
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::FilterCutOff);
                }
        }

        return state;
}

void GeonkickApi::setState(const std::shared_ptr<GeonkickState> &state)
{
        if (!state)
                return;

        geonkick_enable_synthesis(geonkickApi, 0);
        for (int i = 0; i < 3; i++) {
                enbaleLayer(static_cast<Layer>(i), state->isLayerEnabled(static_cast<Layer>(i)));
                setLayerAmplitude(static_cast<Layer>(i), state->getLayerAmplitude(static_cast<Layer>(i)));
        }
        setLimiterValue(state->getLimiterValue());
        tuneAudioOutput(state->isOutputTuned());
        setKickLength(state->getKickLength());
        setKickAmplitude(state->getKickAmplitude());
        enableKickFilter(state->isKickFilterEnabled());
        setKickFilterFrequency(state->getKickFilterFrequency());
        setKickFilterQFactor(state->getKickFilterQFactor());
        setKickFilterType(state->getKickFilterType());
        setKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::Amplitude));
        setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                              state->getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff));
        for (int i = 0; i < 3; i++) {
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator1, state);
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator2, state);
                setOscillatorState(static_cast<Layer>(i), OscillatorType::Noise, state);
        }
        enableCompressor(state->isCompressorEnabled());
        setCompressorAttack(state->getCompressorAttack());
        setCompressorRelease(state->getCompressorRelease());
        setCompressorThreshold(state->getCompressorThreshold());
        setCompressorRatio(state->getCompressorRatio());
        setCompressorKnee(state->getCompressorKnee());
        setCompressorMakeup(state->getCompressorMakeup());
        enableDistortion(state->isDistortionEnabled());
        setDistortionVolume(state->getDistortionVolume());
        setDistortionDrive(state->getDistortionDrive());
        geonkick_enable_synthesis(geonkickApi, 1);
}

void GeonkickApi::setState(const std::string &data)
{
        auto state = getDefaultState();
        state->loadData(data);
        setState(state);
}

std::shared_ptr<GeonkickState> GeonkickApi::getState()
{
        auto state = std::make_shared<GeonkickState>();
        state->setLimiterValue(limiterValue());
        state->tuneOutput(isAudioOutputTuned());
        for (int i = 0; i < 3; i++) {
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
        state->setKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff,
                                     getKickEnvelopePoints(GeonkickApi::EnvelopeType::FilterCutOff));

        for (int i = 0; i < 3; i++) {
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator1, state);
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Oscillator2, state);
                getOscillatorState(static_cast<Layer>(i), OscillatorType::Noise, state);
        }
        state->enableCompressor(isCompressorEnabled());
        state->setCompressorAttack(getCompressorAttack());
        state->setCompressorRelease(getCompressorRelease());
        state->setCompressorThreshold(getCompressorThreshold());
        state->setCompressorRatio(getCompressorRatio());
        state->setCompressorKnee(getCompressorKnee());
        state->setCompressorMakeup(getCompressorMakeup());
        state->enableDistortion(isDistortionEnabled());
        state->setDistortionVolume(getDistortionVolume());
        state->setDistortionDrive(getDistortionDrive());

        return state;
}

void GeonkickApi::getOscillatorState(GeonkickApi::Layer layer,
                                     OscillatorType osc,
                                     const std::shared_ptr<GeonkickState> &state)
{
        auto temp = currentLayer;
        currentLayer = layer;
        auto index = static_cast<int>(osc);
        state->setCurrentLayer(layer);
        state->setOscillatorEnabled(index, isOscillatorEnabled(index));
        state->setOscillatorFunction(index, oscillatorFunction(index));
        state->setOscillatorSample(getOscillatorSample(index), index);
        if (osc != OscillatorType::Noise)
                state->setOscillatorPhase(index, oscillatorPhase(index));
        state->setOscillatorAmplitue(index, oscillatorAmplitude(index));
        state->setOscillatorFrequency(index, oscillatorFrequency(index));
        state->setOscillatorFilterEnabled(index, isOscillatorFilterEnabled(index));
        state->setOscillatorFilterType(index, getOscillatorFilterType(index));
        state->setOscillatorFilterCutOffFreq(index, getOscillatorFilterCutOffFreq(index));
        state->setOscillatorFilterFactor(index, getOscillatorFilterFactor(index));
        auto points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::Amplitude);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Amplitude);
        if (osc != OscillatorType::Noise) {
                points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::Frequency);
                state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Frequency);
        }
        points = oscillatorEvelopePoints(index, GeonkickApi::EnvelopeType::FilterCutOff);
        state->setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::FilterCutOff);
        state->setOscillatorAsFm(index, isOscillatorAsFm(index));
        currentLayer = temp;
}

void GeonkickApi::setOscillatorState(GeonkickApi::Layer layer,
                                     OscillatorType oscillator,
                                     const std::shared_ptr<GeonkickState> &state)
{
        auto temp = currentLayer;
        currentLayer = layer;
        auto osc = static_cast<int>(oscillator);
        state->setCurrentLayer(layer);
        enableOscillator(osc, state->isOscillatorEnabled(osc));
        setOscillatorFunction(osc, state->oscillatorFunction(osc));
        setOscillatorSample(state->getOscillatorSample(osc));
        if (oscillator != OscillatorType::Noise)
                setOscillatorPhase(osc, state->oscillatorPhase(osc));
        setOscillatorAmplitude(osc, state->oscillatorAmplitue(osc));
        if (oscillator != OscillatorType::Noise)
                setOscillatorFrequency(osc, state->oscillatorFrequency(osc));
        enableOscillatorFilter(osc, state->isOscillatorFilterEnabled(osc));
        setOscillatorFilterType(osc, state->oscillatorFilterType(osc));
        setOscillatorFilterCutOffFreq(osc, state->oscillatorFilterCutOffFreq(osc));
        setOscillatorFilterFactor(osc, state->oscillatorFilterFactor(osc));
        setOscillatorEvelopePoints(osc, EnvelopeType::Amplitude,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::Amplitude));
        if (oscillator != OscillatorType::Noise) {
                setOscillatorEvelopePoints(osc, EnvelopeType::Frequency,
                                           state->oscillatorEnvelopePoints(osc,
                                           EnvelopeType::Frequency));
        }
        setOscillatorEvelopePoints(osc, EnvelopeType::FilterCutOff,
                                   state->oscillatorEnvelopePoints(osc, EnvelopeType::FilterCutOff));
        setOscillatorAsFm(osc, state->isOscillatorAsFm(osc)));

        currentLayer = temp;
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

std::vector<RkRealPoint> GeonkickApi::oscillatorEvelopePoints(int oscillatorIndex,  EnvelopeType envelope) const
{
        gkick_real *buf;
        std::vector<RkRealPoint> points;
        size_t npoints = 0;
        geonkick_osc_envelope_get_points(geonkickApi,
                                         getOscIndex(oscillatorIndex),
                                         static_cast<int>(envelope), &buf, &npoints);
        for (decltype(npoints) i = 0; i < 2 * npoints; i += 2)
                points.push_back(RkRealPoint(buf[i], buf[i+1]));

        if (buf)
                free(buf);

        return points;
}

void GeonkickApi::setOscillatorEvelopePoints(int index,
                                             EnvelopeType envelope,
                                             const std::vector<RkRealPoint> &points)
{
        if (points.empty())
                return;

        std::vector<gkick_real> data(2 * points.size() * sizeof(gkick_real), 0);
        gkick_real *buff = reinterpret_cast<gkick_real*>(data.data());
        for (decltype(points.size()) i = 0; i < points.size(); i++) {
                buff[2 * i]     = points[i].x();
                buff[2 * i + 1] = points[i].y();
        }

        geonkick_osc_envelope_set_points(geonkickApi, getOscIndex(index),
                                         static_cast<int>(envelope), buff, points.size());
}

void GeonkickApi::addOscillatorEnvelopePoint(int oscillatorIndex,
                                             EnvelopeType envelope,
                                             const RkRealPoint &point)
{
        geonkick_osc_envelope_add_point(geonkickApi, getOscIndex(oscillatorIndex),
                                        static_cast<int>(envelope), point.x(), point.y());
}

void GeonkickApi::removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex)
{
        geonkick_osc_envelope_remove_point(geonkickApi, getOscIndex(oscillatorIndex),
                                           static_cast<int>(envelope), pointIndex);
}

void GeonkickApi::updateOscillatorEvelopePoint(int oscillatorIndex,
                                               EnvelopeType envelope,
                                               int pointIndex,
                                               const RkRealPoint &point)
{
        geonkick_osc_envelope_update_point(geonkickApi, getOscIndex(oscillatorIndex),
                                           static_cast<int>(envelope),
                                           pointIndex, point.x(), point.y());
}


void GeonkickApi::setOscillatorFunction(int oscillatorIndex, FunctionType function)
{
        geonkick_set_osc_function(geonkickApi, getOscIndex(oscillatorIndex),
                                  static_cast<enum geonkick_osc_func_type>(function));
}

void GeonkickApi::setOscillatorPhase(int oscillatorIndex, gkick_real phase)
{
        geonkick_set_osc_phase(geonkickApi, getOscIndex(oscillatorIndex), phase);
}

gkick_real GeonkickApi::oscillatorPhase(int oscillatorIndex) const
{
        gkick_real phase = 0;
        geonkick_get_osc_phase(geonkickApi, getOscIndex(oscillatorIndex), &phase);
        return phase;
}

GeonkickApi::FunctionType GeonkickApi::oscillatorFunction(int oscillatorIndex) const
{

        enum geonkick_osc_func_type function;
        geonkick_get_osc_function(geonkickApi, getOscIndex(oscillatorIndex), &function);
        return static_cast<FunctionType>(function);
}

void GeonkickApi::setKickLength(double length)
{
        geonkick_set_length(geonkickApi, length / 1000);
}

double GeonkickApi::kickLength(void) const
{
        gkick_real length = 0;
        geonkick_get_length(geonkickApi, &length);
        return 1000 * length;
}

void GeonkickApi::setKickAmplitude(double amplitude)
{
        geonkick_kick_set_amplitude(geonkickApi, amplitude);
}

double GeonkickApi::kickAmplitude() const
{
        gkick_real amplitude = 0;
        geonkick_kick_get_amplitude(geonkickApi, &amplitude);
        return static_cast<double>(amplitude);
}

std::vector<RkRealPoint> GeonkickApi::getKickEnvelopePoints(EnvelopeType envelope) const
{
        gkick_real *buf;
        std::vector<RkRealPoint> points;
        size_t npoints = 0;
        geonkick_kick_envelope_get_points(geonkickApi, static_cast<enum geonkick_envelope_type>(envelope),
                                          &buf, &npoints);
        for (decltype(npoints) i = 0; i < 2 * npoints; i += 2)
                points.push_back(RkRealPoint(buf[i], buf[i+1]));

        if (buf)
                free(buf);
        return points;
}

void GeonkickApi::setKickEnvelopePoints(EnvelopeType envelope,const std::vector<RkRealPoint> &points)
{
        std::vector<gkick_real> data(2 * points.size() * sizeof(gkick_real), 0);
        auto *buff = data.data();
        for (decltype(points.size()) i = 0; i < points.size(); i++) {
                buff[2 * i]     = points[i].x();
                buff[2 * i + 1] = points[i].y();
        }

        geonkick_kick_envelope_set_points(geonkickApi, static_cast<enum geonkick_envelope_type>(envelope),
                                          buff, points.size());
}

void GeonkickApi::enableKickFilter(bool b)
{
        geonkick_kick_filter_enable(geonkickApi, b);
}

bool GeonkickApi::isKickFilterEnabled() const
{
        int enabled = 0;
        geonkick_kick_filter_is_enabled(geonkickApi, &enabled);
        return enabled;
}

void GeonkickApi::setKickFilterType(FilterType type)
{
        geonkick_set_kick_filter_type(geonkickApi, static_cast<enum gkick_filter_type>(type));
}

GeonkickApi::FilterType GeonkickApi::kickFilterType() const
{
        enum gkick_filter_type type;
        geonkick_get_kick_filter_type(geonkickApi, &type);
        return static_cast<FilterType>(type);
}

void GeonkickApi::setKickFilterFrequency(double frequency)
{
        geonkick_kick_set_filter_frequency(geonkickApi, frequency);
}

double GeonkickApi::kickFilterFrequency(void) const
{
        gkick_real frequency;
        geonkick_kick_get_filter_frequency(geonkickApi, &frequency);
        return static_cast<double>(frequency);
}

void GeonkickApi::setKickFilterQFactor(double factor)
{
        geonkick_kick_set_filter_factor(geonkickApi, factor);
}

double GeonkickApi::kickFilterQFactor() const
{
        gkick_real factor = 0;
        geonkick_kick_get_filter_factor(geonkickApi, &factor);
        return static_cast<double>(factor);
}

void GeonkickApi::addKickEnvelopePoint(EnvelopeType envelope, double x, double y)
{
        geonkick_kick_add_env_point(geonkickApi, static_cast<enum geonkick_envelope_type>(envelope), x, y);
}

void GeonkickApi::updateKickEnvelopePoint(EnvelopeType envelope, int index, double x, double y)
{
        geonkick_kick_update_env_point(geonkickApi, static_cast<enum geonkick_envelope_type>(envelope), index, x, y);
}

 void GeonkickApi::removeKickEnvelopePoint(EnvelopeType envelope, int pointIndex)
{
        geonkick_kick_remove_env_point(geonkickApi, static_cast<enum geonkick_envelope_type>(envelope), pointIndex);
}

bool GeonkickApi::setOscillatorAmplitude(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_amplitude(geonkickApi, getOscIndex(oscillatorIndex), value) != GEONKICK_OK)
		return false;

	return true;
}

void GeonkickApi::setOscillatorAsFm(int oscillatorIndex, bool b)
{
        geonkick_osc_set_fm(geonkickApi, getOscIndex(oscillatorIndex), b);
}

bool GeonkickApi::isOscillatorAsFm(int oscillatorIndex) const
{
        bool fm = false;
        geonkick_osc_is_fm(geonkickApi, getOscIndex(oscillatorIndex), &fm);
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
	if (geonkick_get_osc_amplitude(geonkickApi, getOscIndex(oscillatorIndex), &value) != GEONKICK_OK)
		return 0;
	return value;
}

bool GeonkickApi::setOscillatorFrequency(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_frequency(geonkickApi, getOscIndex(oscillatorIndex), value) != GEONKICK_OK)
		return false;
	return true;
}

double GeonkickApi::oscillatorFrequency(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_frequency(geonkickApi, getOscIndex(oscillatorIndex), &value) != GEONKICK_OK)
                return 0;
	return value;
}

bool GeonkickApi::isOscillatorEnabled(int oscillatorIndex)
{
        int enabled = 0;
        geonkick_is_oscillator_enabled(geonkickApi, getOscIndex(oscillatorIndex), &enabled);
        return enabled;
}

double GeonkickApi::kickMaxLength(void) const
{
        gkick_real len = 0;
        geonkick_get_max_length(geonkickApi, &len);
        return len * 1000;
}

void GeonkickApi::enableOscillatorFilter(int oscillatorIndex, bool enable)
{
        geonkick_enbale_osc_filter(geonkickApi, getOscIndex(oscillatorIndex), enable);
}

bool GeonkickApi::isOscillatorFilterEnabled(int oscillatorIndex)
{
        int enabled = false;
        geonkick_osc_filter_is_enabled(geonkickApi, getOscIndex(oscillatorIndex), &enabled);
        return enabled;
}

void GeonkickApi::setOscillatorFilterType(int oscillatorIndex, FilterType filter)
{
        geonkick_set_osc_filter_type(geonkickApi,
                                     getOscIndex(oscillatorIndex),
                                     static_cast<enum gkick_filter_type>(filter));
}

GeonkickApi::FilterType GeonkickApi::getOscillatorFilterType(int oscillatorIndex)
{
        enum gkick_filter_type type;
        geonkick_get_osc_filter_type(geonkickApi, getOscIndex(oscillatorIndex), &type);
        return static_cast<FilterType>(type);
}

void GeonkickApi::setOscillatorFilterCutOffFreq(int oscillatorIndex, double frequency)
{
        geonkick_set_osc_filter_cutoff_freq(geonkickApi, getOscIndex(oscillatorIndex), frequency);
}

double GeonkickApi::getOscillatorFilterCutOffFreq(int oscillatorIndex)
{
        gkick_real frequency = 0;
        geonkick_get_osc_filter_cutoff_freq(geonkickApi, getOscIndex(oscillatorIndex), &frequency);
        return frequency;
}

void GeonkickApi::setOscillatorFilterFactor(int oscillatorIndex, double factor)
{
        geonkick_set_osc_filter_factor(geonkickApi, getOscIndex(oscillatorIndex), factor);
}

double GeonkickApi::getOscillatorFilterFactor(int oscillatorIndex)
{
        gkick_real factor = 0;
        geonkick_get_osc_filter_factor(geonkickApi, getOscIndex(oscillatorIndex), &factor);
        return factor;
}

double GeonkickApi::limiterValue()
{
        gkick_real val = 0;
        geonkick_get_limiter_value(geonkickApi, &val);
        return val;
}

void GeonkickApi::setLimiterValue(double value)
{
        geonkick_set_limiter_value(geonkickApi, value);
}

void GeonkickApi::kickUpdatedCallback(void *arg, gkick_real *buff, size_t size)
{
        std::vector<gkick_real> buffer(size, 0);
        std::memcpy(buffer.data(), buff, size * sizeof(gkick_real));
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj)
                obj->updateKickBuffer(std::move(buffer));
}

void GeonkickApi::limiterCallback(void *arg, gkick_real val)
{
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj)
                obj->setLimiterVal(val);
}

void GeonkickApi::setLimiterVal(double val)
{
        limiterLevelerVal = val;
}

double GeonkickApi::getLimiterLevelerValue() const
{
        return limiterLevelerVal;
}

void GeonkickApi::updateKickBuffer(const std::vector<gkick_real> &&buffer)
{
        std::lock_guard<std::mutex> lock(apiMutex);
        kickBuffer = buffer;
        if (eventQueue)
                eventQueue->postAction([&](void){ kickUpdated(); });
}

std::vector<gkick_real> GeonkickApi::getKickBuffer() const
{
        std::lock_guard<std::mutex> lock(apiMutex);
        return kickBuffer;
}

int GeonkickApi::getSampleRate() const
{
        int sampleRate;
        if (geonkick_get_sample_rate(geonkickApi, &sampleRate) != GEONKICK_OK)
                sampleRate = 0;

        return sampleRate;
}

void GeonkickApi::setKeyPressed(bool b, int note, int velocity)
{
        geonkick_key_pressed(geonkickApi, b, note, velocity);
}

// This function is called only from the audio thread.
gkick_real GeonkickApi::getAudioFrame() const
{
        gkick_real val;
        geonkick_get_audio_frame(geonkickApi, &val);
        return val;
}

void GeonkickApi::enableCompressor(bool enable)
{
        geonkick_compressor_enable(geonkickApi, enable);
}

bool GeonkickApi::isCompressorEnabled() const
{
        int enabled = false;
        geonkick_compressor_is_enabled(geonkickApi, &enabled);
        return enabled;
}

void GeonkickApi::setCompressorAttack(double attack)
{
        geonkick_compressor_set_attack(geonkickApi, attack);
}

void GeonkickApi::setCompressorRelease(double release)
{
        geonkick_compressor_set_release(geonkickApi, release);
}

void GeonkickApi::setCompressorThreshold(double threshold)
{
        geonkick_compressor_set_threshold(geonkickApi, threshold);
}

void GeonkickApi::setCompressorRatio(double ratio)
{
        geonkick_compressor_set_ratio(geonkickApi, ratio);
}

void GeonkickApi::setCompressorKnee(double knee)
{
        geonkick_compressor_set_knee(geonkickApi, knee);
}

void GeonkickApi::setCompressorMakeup(double makeup)
{
        geonkick_compressor_set_makeup(geonkickApi, makeup);
}

double GeonkickApi::getCompressorAttack() const
{
        gkick_real val = 0;
        geonkick_compressor_get_attack(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorRelease() const
{
        gkick_real val = 0;
        geonkick_compressor_get_release(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorThreshold() const
{
        gkick_real val = 0;
        geonkick_compressor_get_threshold(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorRatio() const
{
        gkick_real val = 0;
        geonkick_compressor_get_ratio(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorKnee() const
{
        gkick_real val = 0;
        geonkick_compressor_get_knee(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorMakeup() const
{
        gkick_real val = 0;
        geonkick_compressor_get_makeup(geonkickApi, &val);
        return val;
}

void GeonkickApi::enableDistortion(bool enable)
{
        geonkick_distortion_enable(geonkickApi, enable);
}

bool GeonkickApi::isDistortionEnabled() const
{
        int enabled = false;
        geonkick_distortion_is_enabled(geonkickApi, &enabled);
        return enabled;
}

void GeonkickApi::setDistortionVolume(double volume)
{
        geonkick_distortion_set_volume(geonkickApi, volume);
}

double GeonkickApi::getDistortionVolume(void) const
{
        gkick_real volume = 0;
        geonkick_distortion_get_volume(geonkickApi, &volume);
        return volume;
}

void GeonkickApi::setDistortionDrive(double drive)
{
        geonkick_distortion_set_drive(geonkickApi, drive);
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
                geonkick_set_kick_buffer_callback(geonkickApi, &GeonkickApi::kickUpdatedCallback, this);
                geonkick_set_kick_limiter_callback(geonkickApi, &GeonkickApi::limiterCallback, this);
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
        geonkick_enable_synthesis(geonkickApi, 1);
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
        geonkick_group_set_amplitude(geonkickApi, static_cast<size_t>(layer), amplitude);
}

double GeonkickApi::getLayerAmplitude(Layer layer) const
{
        gkick_real amplitude = 0;
        geonkick_group_get_amplitude(geonkickApi, static_cast<size_t>(layer), &amplitude);
        return amplitude;
}

void GeonkickApi::enbaleLayer(Layer layer, bool enable)
{
        geonkick_enable_group(geonkickApi, static_cast<int>(layer), enable);
}

bool GeonkickApi::isLayerEnabled(Layer layer) const
{
        bool enabled = false;
        geonkick_group_enabled(geonkickApi, static_cast<int>(layer), &enabled);
        return enabled;
}

int GeonkickApi::getOscIndex(int index) const
{
        return index + GKICK_OSC_GROUP_SIZE * static_cast<int>(currentLayer);
}

std::filesystem::path GeonkickApi::currentWorkingPath(const std::string &key) const
{
        auto it = workingPaths.find(key);
        if (it != workingPaths.end())
                return it->second;
        return std::filesystem::current_path();
}

void GeonkickApi::setCurrentWorkingPath(const std::string &key, const std::filesystem::path &path)
{
        workingPaths[key] = path;
}

void GeonkickApi::setSettings(const std::string &key, const std::string &value)
{
        apiSettings[key] = value;
}

std::string GeonkickApi::getSettings(const std::string &key) const
{
        auto it = apiSettings.find(key);
        if (it != apiSettings.end())
                return it->second;
        return "";
}

void GeonkickApi::tuneAudioOutput(bool tune)
{
        geonkick_tune_audio_output(geonkickApi, tune);
}

bool GeonkickApi::isAudioOutputTuned() const
{
        bool tune = false;
        geonkick_is_audio_output_tuned(geonkickApi, &tune);
        return tune;
}

void GeonkickApi::setOscillatorSample(const std::string &file, int oscillatorIndex)
{
        int rateRate = 48000;
        geonkick_get_sample_rate(geonkickApi, &rateRate);
        std::vector<gkick_real> sampleData = loadSample(file, kickMaxLength() / 1000, rateRate, 1);
        if (!sampleData.empty()) {
                geonkick_set_osc_sample(geonkickApi,
                                        getOscIndex(oscillatorIndex),
                                        sampleData.data(),
                                        sampleData.size());
        }
}

void GeonkickApi::setOscillatorSample(const std::<float> &sample, int oscillatorIndex)
{
        geonkick_set_osc_sample(geonkickApi,
                                getOscIndex(oscillatorIndex),
                                sample.data(),
                                sample.size());
}

std::vector<gkick_real> GeonkickApi::loadSample(const std::string &file,
                                                double length,
                                                int smapleRate,
                                                int channels)
{
        GEONKICK_UNUSED(channels);

        SF_INFO sndinfo;
        memset(&sndinfo, 0, sizeof(sndinfo));
        SNDFILE *sndFile = sf_open(file.c_str(), SFM_READ, &sndinfo);
        if (!sndFile) {
                GEONKICK_LOG_ERROR("can't open samle file");
                return std::vector<gkick_real>();
        }

        std::vector<float> data(smapleRate * length, 0.0f);
        auto n = sf_read_float(sndFile, data.data(), data.size());
        sf_close(sndFile);
        if (n > 0)
                return data;
        return std::vector<gkick_real>();
}
