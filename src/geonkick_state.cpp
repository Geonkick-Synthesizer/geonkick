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

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

GeonkickState::GeonkickState() :
        limiterValue(0),
        kickLength(0),
        kickAmplitude(0),
        kickFilterEnabled(0),
        kickFilterFrequency(0),
        kickFilterQFactor(0),
        kickFilterType(GeonkickApi::FilterType::LowPass),
        oscillators{
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator1), std::make_shared<Oscillator>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator2), std::make_shared<Oscillator>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Noise), std::make_shared<Oscillator>()}
        }
{
}

GeonkickState::GeonkickState(const QByteArray &data) :
        limiterValue(0),
        kickLength(0),
        kickAmplitude(0),
        kickFilterEnabled(0),
        kickFilterFrequency(0),
        kickFilterQFactor(0),
        kickFilterType(GeonkickApi::FilterType::LowPass),
        oscillators{
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator1), std::make_shared<Oscillator>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Oscillator2), std::make_shared<Oscillator>()},
            {static_cast<int>(GeonkickApi::OscillatorType::Noise), std::make_shared<Oscillator>()}
        }
{
        QJsonDocument document = QJsonDocument::fromBinaryData(data);
        QJsonObject object = document.object();
        parseKickObject(object.take("kick"));
        for (const auto& val: oscillators)
                parseOscillatorObject(val.first, object.take("osc" + std::string::number(val.first)));
}

void GeonkickState::parseKickObject(const auto &kick)
{
        if (kick.isNull() || !kick.isObject())
                return;

        auto limiter = kick.toObject().take("limiter");
        if (!limiter.isNull() && limiter.isDouble()) {
                setLimiterValue(limiter.toDouble());
        }

        auto envelope = kick.toObject().take("ampl_env");
        if (!envelope.isNull() && envelope.isObject()) {
                setKickLength(envelope.toObject().take("length").toDouble());
                setKickAmplitude(envelope.toObject().take("amplitude").toDouble());
                QPolygonF points = parseEnvelopeArray(envelope.toObject().take("points").toArray());
                setKickEnvelopePoints(points);
        }

        auto filter = kick.toObject().take("filter");
        if (!filter.isNull() && filter.isObject()) {
                enableKickFilter(filter.toObject().take("enabled").toBool());
                setKickFilterFrequency(filter.toObject().take("cutoff").toDouble());
                setKickFilterQFactor(filter.toObject().take("factor").toDouble());
                setKickFilterType(static_cast<GeonkickApi::FilterType>(filter.toObject().take("type").toInt()));
        }

        auto compressor = kick.toObject().take("compressor");
        if (!compressor.isNull() && compressor.isObject()) {
                enableCompressor(compressor.toObject().take("enabled").toBool());
                setCompressorAttack(compressor.toObject().take("attack").toDouble());
                setCompressorRelease(compressor.toObject().take("release").toDouble());
                setCompressorThreshold(compressor.toObject().take("threshold").toDouble());
                setCompressorRatio(compressor.toObject().take("ratio").toDouble());
                setCompressorKnee(compressor.toObject().take("knee").toDouble());
                setCompressorMakeup(compressor.toObject().take("makeup").toDouble());
        }

        auto distortion = kick.toObject().take("distortion");
        if (!distortion.isNull() && distortion.isObject()) {
                enableDistortion(distortion.toObject().take("enabled").toBool());
                setDistortionVolume(distortion.toObject().take("volume").toDouble());
                setDistortionDrive(distortion.toObject().take("drive").toDouble());
        }
}

void GeonkickState::parseOscillatorObject(int index, const auto &osc)
{
        if (osc.isNull() || !osc.isObject())
                return;

        setOscillatorEnabled(index, osc.toObject().take("enabled").toBool());
        setOscillatorFunction(index, static_cast<GeonkickApi::FunctionType>(osc.toObject().take("function").toInt()));
        auto envelope = osc.toObject().take("ampl_env");
        if (!envelope.isNull() && envelope.isObject()) {
                setOscillatorAmplitue(index, envelope.toObject().take("amplitude").toDouble());
                QPolygonF points = parseEnvelopeArray(envelope.toObject().take("points").toArray());
                setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Amplitude);
        }

        if (static_cast<GeonkickApi::OscillatorType>(index) != GeonkickApi::OscillatorType::Noise){
                envelope = osc.toObject().take("freq_env");
                if (!envelope.isNull() && envelope.isObject()) {
                        setOscillatorFrequency(index, envelope.toObject().take("amplitude").toDouble());
                        QPolygonF points = parseEnvelopeArray(envelope.toObject().take("points").toArray());
                        setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Frequency);
                }
        }

        auto filter = osc.toObject().take("filter");
        if (!filter.isNull() && filter.isObject()) {
                setOscillatorFilterEnabled(index, filter.toObject().take("enabled").toBool());
                setOscillatorFilterCutOffFreq(index, filter.toObject().take("cutoff").toDouble());
                setOscillatorFilterFactor(index, filter.toObject().take("factor").toDouble());
                setOscillatorFilterType(index, static_cast<GeonkickApi::FilterType>(filter.toObject().take("type").toInt()));
        }
}

QPolygonF GeonkickState::parseEnvelopeArray(const auto &envelopeArray)
{
        QPolygonF points;
        for (auto it = envelopeArray.constBegin(); it != envelopeArray.constEnd(); ++it) {
                auto point = it->toArray();
                if (point.count() == 2) {
                        points << QPointF(point.takeAt(0).toDouble(), point.takeAt(1).toDouble());
                }
        }

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

void GeonkickState::setKickEnvelopePoints(const QPolygonF &points)
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

QPolygonF GeonkickState::getKickEnvelopePoints() const
{
        return kickEnvelopePoints;
}

std::shared_ptr<GeonkickState::Oscillator> GeonkickState::getOscillator(int index) const
{
        auto it = oscillators.find(index);
        if (it != oscillators.end()) {
                return it->second;
        }

        return nullptr;
}

void GeonkickState::setOscillatorEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->isEnabled = b;
        }
}

void GeonkickState::setOscillatorFunction(int index, GeonkickApi::FunctionType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->function = type;
        }
}

void GeonkickState::setOscillatorAmplitue(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->amplitude = val;
        }
}

void GeonkickState::setOscillatorFrequency(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->frequency = val;
        }
}

void GeonkickState::setOscillatorFilterEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->isFilterEnabled = b;
        }
}

void GeonkickState::setOscillatorFilterType(int index, GeonkickApi::FilterType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->filterType = type;
        }
}

void GeonkickState::setOscillatorFilterCutOffFreq(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->filterFrequency = val;
        }
}

void GeonkickState::setOscillatorFilterFactor(int index, double val)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->filterFactor = val;
        }
}

void GeonkickState::setOscillatorEnvelopePoints(int index,
                                                const QPolygonF &points,
                                                GeonkickApi::EnvelopeType envelope)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                if (envelope == GeonkickApi::EnvelopeType::Amplitude) {
                        oscillator->amplitudeEnvelope = points;
                } else {
                        oscillator->frequencyEnvelope = points;
                }
        }
}

bool GeonkickState::isOscillatorEnabled(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->isEnabled;
        }

        return false;
}

GeonkickApi::FunctionType GeonkickState::oscillatorFunction(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->function;
        }

        return GeonkickApi::FunctionType::Sine;
}

double GeonkickState::oscillatorAmplitue(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->amplitude;
        }
        return 0;
}

double GeonkickState::oscillatorFrequency(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->frequency;
        }
        return 0;
}

bool GeonkickState::isOscillatorFilterEnabled(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->isFilterEnabled;
        }
        return false;
}

GeonkickApi::FilterType GeonkickState::oscillatorFilterType(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterType;
        }
        return GeonkickApi::FilterType::LowPass;
}

double GeonkickState::oscillatorFilterCutOffFreq(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterFrequency;
        }
        return 0;
}

double GeonkickState::oscillatorFilterFactor(int index) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterFactor;
        }
        return 0;
}

QPolygonF GeonkickState::oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type) const
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                if (type == GeonkickApi::EnvelopeType::Amplitude)
                        return oscillator->amplitudeEnvelope;
                else
                        return oscillator->frequencyEnvelope;
        }

        return QPolygonF();
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

QByteArray GeonkickState::toRawData() const
{
        return getJsonDocument().toBinaryData();
}

QByteArray GeonkickState::toJson() const
{
        return getJsonDocument().toJson();
}

QJsonDocument GeonkickState::getJsonDocument() const
{
                QJsonObject state;
        for (const auto& val: oscillators)
        {
                QJsonObject osc;
                int index = val.first;
                osc["enabled"] = QJsonValue(isOscillatorEnabled(index));
                osc["function"] = QJsonValue(static_cast<int>(oscillatorFunction(index)));

                QJsonObject envelope;
                envelope.insert("amplitude", oscillatorAmplitue(index));
                auto points = oscillatorEnvelopePoints(index, GeonkickApi::EnvelopeType::Amplitude);
                QJsonArray jsonArray;
                for (const auto &point: points) {
                        jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
                }
                envelope.insert("points", jsonArray);
                osc["ampl_env"] = envelope;

                envelope = QJsonObject();
                envelope.insert("amplitude", oscillatorFrequency(index));
                points = oscillatorEnvelopePoints(index, GeonkickApi::EnvelopeType::Frequency);
                jsonArray = QJsonArray();
                for (const auto &point: points) {
                        jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
                }
                envelope.insert("points", jsonArray);
                osc["freq_env"] = envelope;

                QJsonObject filter;
                filter.insert("enabled", isOscillatorFilterEnabled(index));
                filter.insert("type", static_cast<int>(static_cast<int>(oscillatorFilterType(index))));
                filter.insert("cutoff", oscillatorFilterCutOffFreq(index));
                filter.insert("factor", oscillatorFilterFactor(index));
                osc["filter"] = filter;
                state["osc" + std::string::number(index)] = osc;
        }

        QJsonObject kick;
        kick["limiter"] = getLimiterValue();
        QJsonObject envelope;
        envelope.insert("length", static_cast<double>(getKickLength()));
        envelope.insert("amplitude", static_cast<double>(getKickAmplitude()));
        auto points = getKickEnvelopePoints();
        QJsonArray jsonArray;
        for (const auto &point: points) {
                jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
        }
        envelope.insert("points", jsonArray);
        kick["ampl_env"] = envelope;

        QJsonObject filter;
        filter.insert("enabled", isKickFilterEnabled());
        filter.insert("type", static_cast<int>(getKickFilterType()));
        filter.insert("cutoff", getKickFilterFrequency());
        filter.insert("factor", getKickFilterQFactor());
        kick["filter"] = filter;

        QJsonObject compressor;
        compressor.insert("enabled", isCompressorEnabled());
        compressor.insert("attack", getCompressorAttack());
        compressor.insert("release", getCompressorRelease());
        compressor.insert("threshold", getCompressorThreshold());
        compressor.insert("ratio", getCompressorRatio());
        compressor.insert("knee", getCompressorKnee());
        compressor.insert("makeup", getCompressorMakeup());
        kick["compressor"] = compressor;

        QJsonObject distortion;
        distortion.insert("enabled", isDistortionEnabled());
        distortion.insert("volume", getDistortionVolume());
        distortion.insert("drive", getDistortionDrive());
        kick["distortion"] = distortion;

        state["kick"] = kick;
        return QJsonDocument(state);
}
