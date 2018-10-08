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

        return true;
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
        emit kickUpdated();
}

void GeonkickApi::removeOscillatorEvelopePoint(int oscillatorIndex, EnvelopeType envelope, int pointIndex)
{
        geonkick_osc_envelope_remove_point(geonkickApi, oscillatorIndex,
                                           static_cast<int>(envelope), pointIndex);
        emit kickUpdated();
}

void GeonkickApi::updateOscillatorEvelopePoint(int oscillatorIndex,
                                        EnvelopeType envelope,
                                        int pointIndex,
                                        const QPointF &point)
{
        geonkick_osc_envelope_update_point(geonkickApi, oscillatorIndex,
                                           static_cast<int>(envelope),
                                           pointIndex, point.x(), point.y());
        emit kickUpdated();
}


void GeonkickApi::setOscillatorFunction(int oscillatorIndex, FunctionType function)
{
        geonkick_set_osc_function(geonkickApi, oscillatorIndex,
                                  static_cast<enum geonkick_osc_func_type>(function));
        emit kickUpdated();
}

GeonkickApi::FunctionType GeonkickApi::oscillatorFunction(int oscillatorIndex) const
{
        // IMPREMENT API
        return FunctionType::Sine;
}

void GeonkickApi::setKickLength(double length)
{
        if (geonkick_set_length(geonkickApi, length / 1000) == GEONKICK_OK) {
                emit kickLengthUpdated(length);
                emit kickUpdated();
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
        emit kickAmplitudeUpdated(amplitude);
        emit kickUpdated();
        // Implement API.
}

double GeonkickApi::kickAmplitude() const
{
        // Implement API.
        return 1;
}

void GeonkickApi::setKickFilterFrequency(double frequency)
{
        emit kickUpdated();
}

double GeonkickApi::kickFilterFrequency(void) const
{
        // Implement API.
}

void GeonkickApi::setKickFilterQFactor(double factor)
{
        // Implement API.
        emit kickUpdated();
}

double GeonkickApi::kickFilterQFactor() const
{
        // Implement API.
}

bool GeonkickApi::setOscillatorAmplitude(int oscillatorIndex, double value)
{
	if (geonkick_set_osc_amplitude(geonkickApi, oscillatorIndex, value) != GEONKICK_OK) {
		return false;
	}
        emit kickUpdated();

	return true;
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
        emit kickUpdated();


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

void GeonkickApi::addKickEnvelopePoint(double x, double y)
{
        Q_UNUSED(x);
        Q_UNUSED(y);
        // Implement API.
        emit kickUpdated();
}

void GeonkickApi::updateKickEnvelopePoint(double x, double y)
{
        Q_UNUSED(x);
        Q_UNUSED(y);
        // Implement API.
        emit kickUpdated();
}

 void GeonkickApi::removeKickEnvelopePoint(int pointIndex)
{
        Q_UNUSED(pointIndex);
        // Implement API.
        emit kickUpdated();
}

void GeonkickApi::enableOscillator(int oscillatorIndex, bool enable)
{
        if (enable) {
                geonkick_enable_oscillator(geonkickApi, oscillatorIndex);
        } else {
                geonkick_disable_oscillator(geonkickApi, oscillatorIndex);
        }
}

bool GeonkickApi::isOscillatorEnabled(int oscillatorIndex)
{
        int enabled = 0;
        geonkick_is_oscillator_enabled(geonkickApi, oscillatorIndex, &enabled);
        return enabled;
}

double GeonkickApi::kickMaxLength(void) const
{
        return 1000;
        // Implement API.
}

void GeonkickApi::setOscillatorFilterType(int oscillatorIndex, FilterType filter)
{
        Q_UNUSED(oscillatorIndex);
        Q_UNUSED(filter);
        // Implement API.
        emit kickUpdated();
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

void GeonkickApi::getKickBuffer(std::vector<float> &buffer)
{
        geonkick_get_kick_buffer(geonkickApi, buffer.data(), buffer.size());
}
