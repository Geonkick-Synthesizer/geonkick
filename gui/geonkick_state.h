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

#include <QJsonDocument>

class GeonkickState
{
 public:

        GeonkickState();
        GeonkickState(const QByteArray &data);

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
        void setOscillatorEnvelopePoints(int index, const QPolygonF &points, GeonkickApi::EnvelopeType envelope);

        bool isOscillatorEnabled(int index) const;
        GeonkickApi::FunctionType oscillatorFunction(int index) const;
        double oscillatorAmplitue(int index) const;
        double oscillatorFrequency(int index) const;
        bool isOscillatorFilterEnabled(int index) const;
        GeonkickApi::FilterType oscillatorFilterType(int index) const;
        double oscillatorFilterCutOffFreq(int index) const;
        double oscillatorFilterFactor(int index) const;
        QPolygonF oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const;

        void enableCompressor(bool enable);
        bool isCompressorEnabled() const;
        void setCompressorAttack(double attack);
        void setCompressorRelease(double release);
        void setCompressorThreshold(double threshold);
        void setCompressorRatio(double ratio);
        void setCompressorKnee(double knee);
        void setCompressorMakeup(double makeup);
        double getCompressorAttack() const;
        double getCompressorRelease() const;
        double getCompressorThreshold() const;
        double getCompressorRatio() const;
        double getCompressorKnee() const;
        double getCompressorMakeup() const;

        QByteArray toRawData() const;
        QByteArray toJson() const;

protected:
        void parseKickObject(const auto &kick);
        void parseOscillatorObject(int index, const auto &osc);
        QPolygonF parseEnvelopeArray(const auto &envelopeArray);
        QJsonDocument getJsonDocument() const;

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

        struct Compressor {
                bool enabled;
                double attack;
                double release;
                double threshold;
                double ratio;
                double knee;
                double makeup;
        };

        struct Distortion {
                bool enabled;
                double drive;
                double volume;
        };

        std::shared_ptr<Oscillator> getOscillator(int index) const;

        double limiterValue;
        double kickLength;
        double kickAmplitude;
        bool   kickFilterEnabled;
        double kickFilterFrequency;
        double kickFilterQFactor;
        GeonkickApi::FilterType kickFilterType;
        QPolygonF kickEnvelopePoints;
        std::unordered_map<int, std::shared_ptr<Oscillator>> oscillators;
        Compressor compressor;
        Distortion distortion;
};

#endif // GEONKICK_STATE_H
