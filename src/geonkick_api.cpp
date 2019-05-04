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

GeonkickApi::GeonkickApi()
        : eventQueue{nullptr}
        , geonkickApi{nullptr}
        , updateLimiterLeveler{false}
        , limiterLevelerVal{0}
        , jackEnabled{false}
        , standaloneInstance{false}
{
}

GeonkickApi::~GeonkickApi()
{
  	if (geonkickApi)
                geonkick_free(&geonkickApi);
}

void GeonkickApi::setEventQueue(const std::shared_ptr<RkEventQueue> &evq)
{
        eventQueue = evq;
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
        state->setKickLength(300);
        state->setKickAmplitude(0.8);
        state->enableKickFilter(false);
        state->setKickFilterFrequency(200);
        state->setKickFilterQFactor(1.0);
        state->setKickFilterType(GeonkickApi::FilterType::LowPass);
        std::vector<RkRealPoint> envelope;
        envelope.push_back({0, 1});
        envelope.push_back({1, 1});
        state->setKickEnvelopePoints(envelope);
        state->enableCompressor(false);
        state->setCompressorAttack(0.01);
        state->setCompressorRelease(0.01);
        state->setCompressorThreshold(0);
        state->setCompressorRatio(1);
        state->setCompressorKnee(0);
        state->setCompressorMakeup(1);
        state->enableDistortion(false);
        state->setDistortionVolume(10);
        state->setDistortionDrive(0);

        std::vector<GeonkickApi::OscillatorType> oscillators = {
                GeonkickApi::OscillatorType::Oscillator1,
                GeonkickApi::OscillatorType::Oscillator2,
                GeonkickApi::OscillatorType::Noise,
        };

        for (auto const &osc: oscillators) {
                int index = static_cast<int>(osc);
                if (osc == GeonkickApi::OscillatorType::Oscillator1)
                        state->setOscillatorEnabled(index, true);
                else
                        state->setOscillatorEnabled(index, false);

                if (osc == GeonkickApi::OscillatorType::Noise)
                        state->setOscillatorFunction(index, GeonkickApi::FunctionType::NoiseWhite);
                else
                        state->setOscillatorFunction(index, GeonkickApi::FunctionType::Sine);

                state->setOscillatorAmplitue(index, 0.26);
                state->setOscillatorFrequency(index, 800);
                state->setOscillatorFilterEnabled(index, false);
                state->setOscillatorFilterType(index, GeonkickApi::FilterType::LowPass);
                state->setOscillatorFilterCutOffFreq(index, 800);
                state->setOscillatorFilterFactor(index, 1);
                state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Amplitude);
                if (osc != GeonkickApi::OscillatorType::Noise)
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Frequency);
        }

        return state;
}

void GeonkickApi::setState(const std::shared_ptr<GeonkickState> &state)
{
        if (!state)
                return;

        geonkick_enable_synthesis(geonkickApi, 0);
        setLimiterValue(state->getLimiterValue());
        setKickLength(state->getKickLength());
        setKickAmplitude(state->getKickAmplitude());
        enableKickFilter(state->isKickFilterEnabled());
        setKickFilterFrequency(state->getKickFilterFrequency());
        setKickFilterQFactor(state->getKickFilterQFactor());
        setKickFilterType(state->getKickFilterType());
        setKickEnvelopePoints(state->getKickEnvelopePoints());
        setOscillatorState(OscillatorType::Oscillator1, state);
        setOscillatorState(OscillatorType::Oscillator2, state);
        setOscillatorState(OscillatorType::Noise, state);
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
        setState(std::make_shared<GeonkickState>(data));
}

std::shared_ptr<GeonkickState> GeonkickApi::getState()
{
        auto state = std::make_shared<GeonkickState>();

        state->setLimiterValue(limiterValue());
        state->setKickLength(kickLength());
        state->setKickAmplitude(kickAmplitude());
        state->enableKickFilter(isKickFilterEnabled());
        state->setKickFilterFrequency(kickFilterFrequency());
        state->setKickFilterQFactor(kickFilterQFactor());
        state->setKickFilterType(kickFilterType());
        state->setKickEnvelopePoints(getKickEnvelopePoints());
        getOscillatorState(OscillatorType::Oscillator1, state);
        getOscillatorState(OscillatorType::Oscillator2, state);
        getOscillatorState(OscillatorType::Noise, state);
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

void GeonkickApi::getOscillatorState(OscillatorType osc, const std::shared_ptr<GeonkickState> &state)
{
        int index = static_cast<int>(osc);
        state->setOscillatorEnabled(index, isOscillatorEnabled(index));
        state->setOscillatorFunction(index, oscillatorFunction(index));
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
}

void GeonkickApi::setOscillatorState(OscillatorType oscillator, const std::shared_ptr<GeonkickState> &state)
{
        auto osc = static_cast<int>(oscillator);
        enableOscillator(osc, state->isOscillatorEnabled(static_cast<int>(osc)));
        setOscillatorFunction(osc, state->oscillatorFunction(static_cast<int>(osc)));
        setOscillatorAmplitude(osc, state->oscillatorAmplitue(static_cast<int>(osc)));
        if (oscillator != OscillatorType::Noise)
                setOscillatorFrequency(osc, state->oscillatorFrequency(static_cast<int>(osc)));
        enableOscillatorFilter(osc, state->isOscillatorFilterEnabled(static_cast<int>(osc)));
        setOscillatorFilterType(osc, state->oscillatorFilterType(static_cast<int>(osc)));
        setOscillatorFilterCutOffFreq(osc, state->oscillatorFilterCutOffFreq(static_cast<int>(osc)));
        setOscillatorFilterFactor(osc, state->oscillatorFilterFactor(static_cast<int>(osc)));
        setOscillatorEvelopePoints(osc, EnvelopeType::Amplitude,
                                   state->oscillatorEnvelopePoints(static_cast<int>(osc), EnvelopeType::Amplitude));
        if (oscillator != OscillatorType::Noise)
                setOscillatorEvelopePoints(osc, EnvelopeType::Frequency,
                                           state->oscillatorEnvelopePoints(static_cast<int>(osc),
                                           EnvelopeType::Frequency));
}

std::vector<std::unique_ptr<Oscillator>> GeonkickApi::oscillators(void)
{
        std::vector<std::unique_ptr<Oscillator>> oscillators;
        size_t n = 0;
        geonkick_get_oscillators_number(geonkickApi, &n);
        for (decltype(n) i = 0; i < n; i++)
                oscillators.push_back(std::move(std::make_unique<Oscillator>(this, static_cast<Oscillator::Type>(i))));
        return oscillators;
}

std::vector<RkRealPoint> GeonkickApi::oscillatorEvelopePoints(int oscillatorIndex,  EnvelopeType envelope) const
{
        gkick_real *buf;
        std::vector<RkRealPoint> points;
        size_t npoints = 0;
        geonkick_osc_envelope_get_points(geonkickApi, oscillatorIndex, static_cast<int>(envelope), &buf, &npoints);
        for (decltype(npoints) i = 0; i < 2 * npoints; i += 2)
                points.push_back(RkRealPoint(buf[i], buf[i+1]));

        if (buf != NULL)
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

        geonkick_osc_envelope_set_points(geonkickApi, index, static_cast<int>(envelope), buff, points.size());
}

void GeonkickApi::addOscillatorEnvelopePoint(int oscillatorIndex,
                                             EnvelopeType envelope,
                                             const RkRealPoint &point)
{
        geonkick_osc_envelope_add_point(geonkickApi, oscillatorIndex,
                                        static_cast<int>(envelope), point.x(), point.y());
}

void GeonkickApi::removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex)
{
        geonkick_osc_envelope_remove_point(geonkickApi, oscillatorIndex,
                                           static_cast<int>(envelope), pointIndex);
}

void GeonkickApi::updateOscillatorEvelopePoint(int oscillatorIndex,
                                               EnvelopeType envelope,
                                               int pointIndex,
                                               const RkRealPoint &point)
{
        geonkick_osc_envelope_update_point(geonkickApi, oscillatorIndex,
                                           static_cast<int>(envelope),
                                           pointIndex, point.x(), point.y());
}


void GeonkickApi::setOscillatorFunction(int oscillatorIndex, FunctionType function)
{
        geonkick_set_osc_function(geonkickApi, oscillatorIndex,
                                  static_cast<enum geonkick_osc_func_type>(function));
}

GeonkickApi::FunctionType GeonkickApi::oscillatorFunction(int oscillatorIndex) const
{

        enum geonkick_osc_func_type function;
        geonkick_get_osc_function(geonkickApi, oscillatorIndex, &function);
        return static_cast<FunctionType>(function);
}

void GeonkickApi::setKickLength(double length)
{
        if (geonkick_set_length(geonkickApi, length / 1000) == GEONKICK_OK) {
                //         emit kickLengthUpdated(length);
        }
}

double GeonkickApi::kickLength(void) const
{
        gkick_real length = 0;
        geonkick_get_length(geonkickApi, &length);
        return 1000 * length;
}

void GeonkickApi::setKickAmplitude(double amplitude)
{
        if (geonkick_kick_set_amplitude(geonkickApi, amplitude) == GEONKICK_OK) {
                //emit kickAmplitudeUpdated(amplitude);
        }
}

double GeonkickApi::kickAmplitude() const
{
        gkick_real amplitude;
        geonkick_kick_get_amplitude(geonkickApi, &amplitude);
        return static_cast<double>(amplitude);
}

std::vector<RkRealPoint> GeonkickApi::getKickEnvelopePoints() const
{
        gkick_real *buf;
        std::vector<RkRealPoint> points;
        size_t npoints = 0;
        geonkick_kick_envelope_get_points(geonkickApi, &buf, &npoints);
        for (auto i = 0; i < 2 * npoints; i += 2)
                points.push_back(RkRealPoint(buf[i], buf[i+1]));

        if (buf != NULL)
                free(buf);
        return points;
}

void GeonkickApi::setKickEnvelopePoints(const std::vector<RkRealPoint> &points)
{
        std::vector<gkick_real> data(2 * points.size() * sizeof(gkick_real), 0);
        auto *buff = data.data();
        for (decltype(points.size()) i = 0; i < points.size(); i++) {
                buff[2 * i]     = points[i].x();
                buff[2 * i + 1] = points[i].y();
        }

        geonkick_kick_envelope_set_points(geonkickApi, buff, points.size());
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
        gkick_real factor;
        geonkick_kick_get_filter_factor(geonkickApi, &factor);
        return static_cast<double>(factor);
}

void GeonkickApi::addKickEnvelopePoint(double x, double y)
{
        geonkick_kick_add_env_point(geonkickApi, x, y);
}

void GeonkickApi::updateKickEnvelopePoint(int index, double x, double y)
{
        geonkick_kick_update_env_point(geonkickApi, index, x, y);
}

 void GeonkickApi::removeKickEnvelopePoint(int pointIndex)
{
        geonkick_kick_remove_env_point(geonkickApi, pointIndex);
}

bool GeonkickApi::setOscillatorAmplitude(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_amplitude(geonkickApi, oscillatorIndex, value) != GEONKICK_OK)
		return false;

	return true;
}

void GeonkickApi::enableOscillator(int oscillatorIndex, bool enable)
{
        if (enable)
                geonkick_enable_oscillator(geonkickApi, oscillatorIndex);
        else
                geonkick_disable_oscillator(geonkickApi, oscillatorIndex);
}

double GeonkickApi::oscillatorAmplitude(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_amplitude(geonkickApi, oscillatorIndex, &value) != GEONKICK_OK)
		return 0;
	return value;
}

bool GeonkickApi::setOscillatorFrequency(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_frequency(geonkickApi, oscillatorIndex, value) != GEONKICK_OK)
		return false;
	return true;
}

double GeonkickApi::oscillatorFrequency(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_frequency(geonkickApi, oscillatorIndex, &value) != GEONKICK_OK)
                return 0;
	return value;
}

bool GeonkickApi::isOscillatorEnabled(int oscillatorIndex)
{
        int enabled = 0;
        geonkick_is_oscillator_enabled(geonkickApi, oscillatorIndex, &enabled);
        return enabled;
}

double GeonkickApi::kickMaxLength(void) const
{
        gkick_real len;
        geonkick_get_max_length(geonkickApi, &len);
        return len * 1000;
}

void GeonkickApi::enableOscillatorFilter(int oscillatorIndex, bool enable)
{
        geonkick_enbale_osc_filter(geonkickApi, oscillatorIndex, enable);
}

bool GeonkickApi::isOscillatorFilterEnabled(int oscillatorIndex)
{
        int enabled;
        geonkick_osc_filter_is_enabled(geonkickApi, oscillatorIndex, &enabled);
        return enabled;
}

void GeonkickApi::setOscillatorFilterType(int oscillatorIndex, FilterType filter)
{
        geonkick_set_osc_filter_type(geonkickApi, oscillatorIndex, static_cast<enum gkick_filter_type>(filter));
}

GeonkickApi::FilterType GeonkickApi::getOscillatorFilterType(int oscillatorIndex)
{
        enum gkick_filter_type type;
        geonkick_get_osc_filter_type(geonkickApi, oscillatorIndex, &type);
        return static_cast<FilterType>(type);
}

void GeonkickApi::setOscillatorFilterCutOffFreq(int oscillatorIndex, double frequency)
{
        geonkick_set_osc_filter_cutoff_freq(geonkickApi, oscillatorIndex, frequency);
}

double GeonkickApi::getOscillatorFilterCutOffFreq(int oscillatorIndex)
{
        gkick_real frequency;
        geonkick_get_osc_filter_cutoff_freq(geonkickApi, oscillatorIndex, &frequency);
        return frequency;
}

void GeonkickApi::setOscillatorFilterFactor(int oscillatorIndex, double factor)
{
        geonkick_set_osc_filter_factor(geonkickApi, oscillatorIndex, factor);
}

double GeonkickApi::getOscillatorFilterFactor(int oscillatorIndex)
{
        gkick_real factor;
        geonkick_get_osc_filter_factor(geonkickApi, oscillatorIndex, &factor);
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
        //        emit kickUpdated();
}

void GeonkickApi::getKickBuffer(std::vector<gkick_real> &buffer)
{
        geonkick_get_kick_buffer(geonkickApi, buffer.data(), buffer.size());
}

std::vector<gkick_real> GeonkickApi::getKickBuffer()
{
        std::vector<gkick_real> buffer;
        size_t size;
        geonkick_get_kick_buffer_size(geonkickApi, &size);
        buffer.resize(size);
        geonkick_get_kick_buffer(geonkickApi, buffer.data(), size);
        return buffer;
}

void GeonkickApi::kickUpdatedCallback(void *arg)
{
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj)
                obj->emitKickUpdated();
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

void GeonkickApi::emitKickUpdated()
{
        if (eventQueue)
                eventQueue->postAction([&](void){ kickUpdated(); });
}

int GeonkickApi::getSampleRate()
{
        int sampleRate;
        if (geonkick_get_sample_rate(geonkickApi, &sampleRate) != GEONKICK_OK)
                sampleRate = 0;

        return sampleRate;
}

void GeonkickApi::setKeyPressed(bool b, int velocity)
{
        geonkick_key_pressed(geonkickApi, b, velocity);
}

gkick_real GeonkickApi::getAudioFrame()
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
        gkick_real val;
        geonkick_compressor_get_attack(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorRelease() const
{
        gkick_real val;
        geonkick_compressor_get_release(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorThreshold() const
{
        gkick_real val;
        geonkick_compressor_get_threshold(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorRatio() const
{
        gkick_real val;
        geonkick_compressor_get_ratio(geonkickApi, &val);
        if (val < 0 )
        return val;
}

double GeonkickApi::getCompressorKnee() const
{
        gkick_real val;
        geonkick_compressor_get_knee(geonkickApi, &val);
        return val;
}

double GeonkickApi::getCompressorMakeup() const
{
        gkick_real val;
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
        gkick_real volume;
        geonkick_distortion_get_volume(geonkickApi, &volume);
        return volume;
}

void GeonkickApi::setDistortionDrive(double drive)
{
        geonkick_distortion_set_drive(geonkickApi, drive);
}

double GeonkickApi::getDistortionDrive(void) const
{
        gkick_real drive;
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
