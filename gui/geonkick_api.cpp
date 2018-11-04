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

#include <geonkick.h>

#include <memory>

GeonkickApi::GeonkickApi(QObject *parent) :
        QObject(parent),
        geonkickApi(nullptr)
{
}

GeonkickApi::~GeonkickApi()
{
  	if (geonkickApi) {
                geonkick_free(&geonkickApi);
  	}
}

bool GeonkickApi::init()
{
  	if (geonkick_create(&geonkickApi) != GEONKICK_OK) {
	        GEONKICK_LOG_ERROR("can't create geonkick API");
                return false;
  	}

        /*geonkickApi->setKickLength(300);
        geonkickApi->setKickAmplitude(0.8);
        geonkickApi->setKickFilterFrequency(200);
        oscillators = geonkickApi->oscillators();

        // Oscillator 1
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)];
        oscillator->setFrequency(1000);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);
	oscillator->setFunction(Oscillator::FunctionType::Sine);
        oscillator->setAmplitude(0.05);

        // Oscillator 2
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)];
        oscillator->setFrequency(1000);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);
	oscillator->setFunction(Oscillator::FunctionType::Sine);
        oscillator->setAmplitude(0.05);
        // Noise
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)];
	oscillator->setFunction(Oscillator::FunctionType::Noise);
	oscillator->setAmplitude(0.05);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);*/

        geonkick_set_kick_buffer_callback(geonkickApi, &GeonkickApi::kickUpdatedCallback, this);

        return true;
}

bool GeonkickApi::setApiState(const GeonkickApiState &state)
{
        setLimiterValue(state.limiterValue());

        setKickLength(state.kickLength());
        setKickAmplitude(state.kickAmplitude());
        enableKickFilter(state.isKickFilterEnabled());
        setKickFilterFrequency(state.kickFilterFrequency());
        setKickEnvelopePoints(state.kickEnvelopePoints());
        setKickFilterQFactor(state.kickFilterFactor());

        setOscillatorState(OscillatorType::Oscillator1, state);
        setOscillatorState(OscillatorType::Oscillator2, state);
        setOscillatorState(OscillatorType::Noise, state);
}

void GeonkickApi::setOscillatorState(OscillatorType oscillator, const GeonkickState &state)
{
        auto osc = static_cast<int>(oscillator);
        enableOscillator(osc, state.isOscillatorEnabled(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorFunction(osc, state.oscillatorFunction(static_cast<GeonkickState::Oscillator>(osc)));
}

std::vector<Oscillator*> GeonkickApi::oscillators(void)
{
        std::vector<Oscillator*> oscillators;
        size_t n = 0;
        geonkick_get_oscillators_number(geonkickApi, &n);
        for (size_t i = 0; i < n; i++) {
                oscillators.push_back(new Oscillator(this, static_cast<Oscillator::Type>(i)));
        }

        return oscillators;
}

QPolygonF GeonkickApi::oscillatorEvelopePoints(int oscillatorIndex,  EnvelopeType envelope) const
{
        gkick_real *buf;
        QPolygonF points;
        size_t npoints = 0;

        geonkick_osc_envelope_get_points(geonkickApi, oscillatorIndex,
                                         static_cast<int>(envelope), &buf, &npoints);
        for (size_t i = 0; i < 2 * npoints; i += 2) {
                points.push_back(QPointF(buf[i], buf[i+1]));
        }

        if (buf != NULL) {
                free(buf);
        }

        return points;
}

void GeonkickApi::addOscillatorEnvelopePoint(int oscillatorIndex, EnvelopeType envelope, const QPointF &point)
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
                                        const QPointF &point)
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
                emit kickLengthUpdated(length);
        };
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
                emit kickAmplitudeUpdated(amplitude);
        }
}

double GeonkickApi::kickAmplitude() const
{
        gkick_real amplitude;
        geonkick_kick_get_amplitude(geonkickApi, &amplitude);
        return static_cast<double>(amplitude);
}

QPolygonF GeonkickApi::getKickEnvelopePoints() const
{
        gkick_real *buf;
        QPolygonF points;
        size_t npoints = 0;
        geonkick_kick_envelope_get_points(geonkickApi, &buf, &npoints);
        for (size_t i = 0; i < 2 * npoints; i += 2) {
                points.push_back(QPointF(buf[i], buf[i+1]));
        }

        if (buf != NULL) {
                free(buf);
        }

        return points;
}

void GeonkickApi::enableKickFilter(bool b)
{
        geonkick_kick_filter_enable(geonkickApi, b);
}

void GeonkickApi::setKickFilterType(FilterType type)
{
        geonkick_set_kick_filter_type(geonkickApi, static_cast<enum gkick_filter_type>(type));
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
	if (geonkick_set_osc_amplitude(geonkickApi, oscillatorIndex, value) != GEONKICK_OK) {
		return false;
	}

	return true;
}

void GeonkickApi::enableOscillator(int oscillatorIndex, bool enable)
{
        if (enable) {
                geonkick_enable_oscillator(geonkickApi, oscillatorIndex);
        } else {
                geonkick_disable_oscillator(geonkickApi, oscillatorIndex);
        }
}

double GeonkickApi::oscillatorAmplitude(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_amplitude(geonkickApi, oscillatorIndex, &value) != GEONKICK_OK) {
		return 0;
	}

	return value;
}

bool GeonkickApi::setOscillatorFrequency(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_frequency(geonkickApi, oscillatorIndex, value) != GEONKICK_OK) {
		return false;
	}

	return true;
}

double GeonkickApi::oscillatorFrequency(int oscillatorIndex) const
{
	gkick_real value = 0;
	if (geonkick_get_osc_frequency(geonkickApi, oscillatorIndex, &value) != GEONKICK_OK) {
                return 0;
	}

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
}

void GeonkickApi::setLimiterValue(double value)
{
        geonkick_set_limiter_value(geonkickApi, value);
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
        return std::move(buffer);
}

void GeonkickApi::kickUpdatedCallback(void *arg)
{
        GeonkickApi *obj = static_cast<GeonkickApi*>(arg);
        if (obj) {
                obj->emitKickUpdated();
        }
}

void GeonkickApi::emitKickUpdated()
{
        emit kickUpdated();
}

int GeonkickApi::getSampleRate()
{
        int sampleRate;
        if (geonkick_get_sample_rate(geonkickApi, &sampleRate) != GEONKICK_OK) {
                sampleRate = 0;
        }

        return sampleRate;
}
