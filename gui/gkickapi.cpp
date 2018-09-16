/**
 * File name: gkickapi.cpp
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

#include "gkickapi.h"
#include "gkick_oscillator.h"
#include "globals.h"

#include <geonkick.h>

#include <memory>

GKickApi::GKickApi() :
        gKickApi(nullptr)
{
}

GKickApi::~GKickApi()
{
  	if (gKickApi) {
                geonkick_free(&gKickApi);
  	}
}

bool GKickApi::init()
{
  	if (geonkick_create(&gKickApi) != GEONKICK_OK) {
	        GKICK_LOG_ERROR("can't create geonkick API");
                return false;
  	}

        return true;
}

std::vector<std::shared_ptr<GKickOscillator>> GKickApi::getOscillators(void)
{
        std::vector<std::shared_ptr<GKickOscillator>> oscillators;
        if (!gKickApi) {
                return oscillators;
        }

        int n = geonkick_get_oscillators_number(gKickApi);
        for (int i = 0; i < n; i++) {
                std::shared_ptr<GKickOscillator> osc;
                osc = std::make_shared<GKickOscillator>(this, (GKickOscillator::OscillatorType)i);
                oscillators.push_back(osc);
        }

        return oscillators;
}

QPolygonF GKickApi::getOscEvelopePoints(int osc, int envelope) const
{
        double *buf;
        QPolygonF points;
        size_t npoints = 0;

        if (!gKickApi) {
                return points;
        }

        geonkick_osc_envelope_get_points(gKickApi, osc, envelope, &buf, &npoints);
        for (size_t i = 0; i < 2 * npoints; i += 2) {
                points.push_back(QPointF(buf[i], buf[i+1]));
        }

        if (buf != NULL) {
                free(buf);
        }

        return points;
}


void GKickApi::addOscEnvelopePoint(int osc, int envelope,const QPointF &point)
{
        if (!gKickApi) {
                return;
        }

        geonkick_osc_envelope_add_point(gKickApi,
                                        osc,
                                        envelope,
                                        point.x(), point.y());
}

void GKickApi::removeOscEvelopePoint(int osc, int envelope, int index)
{
        if (!gKickApi) {
                return;
        }

        geonkick_osc_envelope_remove_point(gKickApi, osc, envelope, index);
}

void GKickApi::updateOscEvelopePoint(int osc, int envelope,
                                     int index, const QPointF &point)
{
        if (!gKickApi) {
                return;
        }

        geonkick_osc_envelope_update_point(gKickApi, osc, envelope,
                                           index, point.x(), point.y());
}


void GKickApi::setOscFunction(int oscillatorIndex, enum geonkick_osc_func_type type)
{
        if (!gKickApi) {
                return;
        }

        geonkick_set_osc_function(gKickApi, oscillatorIndex, (enum geonkick_osc_func_type)type);
}

enum geonkick_osc_func_type GKickApi::getOscFunction(int oscillatorIndex) const
{
        if (!gKickApi) {
                return GEONKICK_OSC_FUNC_UNKNOWN;
        }

        // IMPREMENT API
        return GEONKICK_OSC_FUNC_SINE;
}

void GKickApi::setKickLength(double len)
{
        if (!gKickApi) {
                return;
        }

        if (geonkick_set_length(gKickApi, len) == GEONKICK_OK) {
                emit kickLengthUpdated(len);
        };
}

double GKickApi::getKickLength(void) const
{
        if (!gKickApi) {
	          return 0;
        }
        double len = 0.0;
        geonkick_get_length(gKickApi, &len);
        return len;
}

void GKickApi::setMaxLength(double len)
{
}

double GKickApi::getMaxLength() const
{
        return 1.5;
}

void GKickApi::setAmplitude(double val)
{
}

double GKickApi::getAmplitude() const
{
        return 1.0;
}

void GKickApi::setKickFilterFrequency(double f)
{
}

double GKickApi::getKickFilterFrequency(void) const
{
}

void GKickApi::setKickFilterQFactor(double f)
{
}

double GKickApi::getKickFilterQFactor() const
{
}

bool GKickApi::setOscAmplitudeValue(int oscillatorIndex, double v)
{
	if (!gKickApi) {
		return false;
	}

        GKICK_LOG_INFO("AMPLITUDE :" << v);
	if (geonkick_set_osc_amplitude_val(gKickApi, oscillatorIndex, v)
	    != GEONKICK_OK) {
		return false;
	}

	return true;
}

double GKickApi::getOscAmplitudeValue(int oscillatorIndex) const
{
	double v;

	if (!gKickApi) {
		return 0.0;
	}

	v = 0.0;
	if (geonkick_get_osc_amplitude_val(gKickApi, oscillatorIndex, &v)
	    != GEONKICK_OK) {
		return 0.0;
	}

	return v;
}

bool GKickApi::setOscFrequencyValue(int oscillatorIndex, double v)
{
	if (!gKickApi) {
		return false;
	}

	if (geonkick_set_osc_frequency_val(gKickApi, oscillatorIndex, v)
	    != GEONKICK_OK) {
		return false;
	}

	return true;
}

double GKickApi::getOscFrequencyValue(int oscillatorIndex) const
{
	double v;

	if (!gKickApi) {
		return 0.0;
	}

	v = 0.0;
	if (geonkick_get_osc_frequency_val(gKickApi, oscillatorIndex, &v)
	    != GEONKICK_OK) {
                return 0.0;
	}

	return v;
}

void GKickApi::addEnvelopePoint(double x, double y)
{
        Q_UNUSED(x);
        Q_UNUSED(y);
        GKICK_LOG_INFO("IMPLEMENT API");
}

void GKickApi::updateEnvelopePoint(unsigned int index, double x, double y)
{
        Q_UNUSED(index);
        Q_UNUSED(x);
        Q_UNUSED(y);
        GKICK_LOG_INFO("IMPLEMENT API");
}

void GKickApi::removeEnvelopePoint(unsigned int index)
{
        Q_UNUSED(index);
        GKICK_LOG_INFO("IMPLEMENT API");
}

