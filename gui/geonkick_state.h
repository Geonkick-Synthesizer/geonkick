/**
 * File name: gkickapi.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include <unordered_map>

class GeonkickState
{
 public:

        GeonkickState();

        void setLimiterValue(double val);
        void setKickLength(double val);
        void setKickAmplitude(double val);
        void enableKickFilter(bool b);
        void setKickFilterFrequency(double val);
        void setKickFilterQFactor(double val);
        void setKickFilterType(GeonkickApi::FilterType type);
        void setKickEnvelopePoints(const QPolygonF &points);

        double getLimiterValue() const;
        double getKickLength() const;
        double getKickAmplitude() const;
        bool   isKickFilterEnabled() const;
        double getKickFilterFrequency() const;
        double getKickFilterQFactor() const;
        GeonkickApi::FilterType getKickFilterType() const;
        QPolygonF getKickEnvelopePoints() const;

        void setOscillatorEnabled(int index, bool b);
        void setOscillatorFunction(int index, GeonkickApi::FunctionType type);
        void setOscillatorAmplitue(int index, double val);
        void setOscillatorFrequency(int index, double val);
        void setOscillatorFilterEnabled(int index, bool b);
        void setOscillatorFilterType(int index, GeonkickApi::FilterType type);
        void setOscillatorFilterCutOffFreq(int index, double val);
        void setOscillatorFilterFactor(int index, double val);
        void setOscillatorEnvelopePoints(int index, const QPointF &points, GeonkickApi::EnvelopeType);

        bool isOscillatorEnabled(int index);
        GeonkickApi::FunctionType oscillatorFunction(int index);
        double oscillatorAmplitue(int index);
        double oscillatorFrequency(int index);
        bool isOscillatorFilterEnabled(int index);
        GeonkickApi::FilterType oscillatorFilterType(int index);
        double oscillatorFilterCutOffFreq(int index);
        double oscillatorFilterFactor(int index);
        QPolygonF oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type);

private:
        struct Oscillator {
                GeonkickApi::OscillatorType type;
                bool isEnabled;
                GeonkickApi::FunctionType function;
                double amplitude;
                double frequency;
                bool isFilterEnabled;
                GeonkickApi::FilterType filterType;
                double filterFrequency;
                double filterFactor;
                QPolygonF amplitudeEnvelope;
                QPolygonF frequencyEnvelope;
        };

        std::shared_ptr<Oscillator>& getOscillator(int index);

        double limiterValue;
        double kickLength;
        double kickAmplitude;
        bool   kickFilterEnabled;
        double kickFilterFrequency;
        double kickFilterQFactor;
        GeonkickApi::FilterType kickFilterType;
        QPolygonF kickEnvelopePoints;
        std::vector<int, std::shared_ptr<Oscillator>> oscillators;
};

#endif // GEONKICK_STATE_H
