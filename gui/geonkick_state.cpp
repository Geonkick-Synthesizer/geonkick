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
                parseOscillatorObject(val.first, object.take("osc" + QString::number(val.first)));
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
                setKickFilterType(static_cast<GeonkickApi::FilterType>(filter.toObject().take("factor").toInt()));
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

        envelope = osc.toObject().take("freq_env");
        if (!envelope.isNull() && envelope.isObject()) {
                setOscillatorAmplitue(index, envelope.toObject().take("amplitude").toDouble());
                QPolygonF points = parseEnvelopeArray(envelope.toObject().take("points").toArray());
                setOscillatorEnvelopePoints(index, points, GeonkickApi::EnvelopeType::Frequency);
        }

        auto filter = osc.toObject().take("filter");
        if (!filter.isNull() && filter.isObject()) {
                enableKickFilter(filter.toObject().take("enabled").toBool());
                setKickFilterFrequency(filter.toObject().take("cutoff").toDouble());
                setKickFilterQFactor(filter.toObject().take("factor").toDouble());
                setKickFilterType(static_cast<GeonkickApi::FilterType>(filter.toObject().take("factor").toInt()));
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

QByteArray GeonkickState::toRawData() const
{
        QJsonObject state;
        for (const auto& val: oscillators)
        {
                QJsonObject osc;
                int index = val.first;
                osc["enabled"] = QJsonValue(isOscillatorEnabled(index));
                osc["function"] = QJsonValue(static_cast<int>(oscillatorFunction(index)));
                osc["ampl_env"] = QJsonValue(QJsonObject({{"amplitude", oscillatorAmplitue(index)}}));
                auto points = oscillatorEnvelopePoints(index, GeonkickApi::EnvelopeType::Amplitude);
                QJsonArray jsonArray;
                for (const auto &point: points) {
                        jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
                }
                osc["ampl_env"] = QJsonValue(QJsonObject({{"points", jsonArray}}));
                osc["freq_env"] = QJsonValue(QJsonObject({{"aplitude", oscillatorFrequency(index)}}));
                points = oscillatorEnvelopePoints(index, GeonkickApi::EnvelopeType::Frequency);
                if (static_cast<GeonkickApi::OscillatorType>(index) != GeonkickApi::OscillatorType::Noise) {
                        jsonArray = QJsonArray();
                        for (const auto &point: points) {
                                jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
                        }
                        osc["ampl_freq"] = QJsonValue(QJsonObject({{"points", jsonArray}}));
                }
                osc["filter"] = QJsonValue(QJsonObject({{"enabled", isOscillatorFilterEnabled(index)}}));
                osc["filter"] = QJsonValue(QJsonObject({{"type", static_cast<int>(oscillatorFilterType(index))}}));
                osc["filter"] = QJsonValue(QJsonObject({{"cutoff", oscillatorFilterCutOffFreq(index)}}));
                osc["filter"] = QJsonValue(QJsonObject({{"factor", oscillatorFilterFactor(index)}}));
                state["osc" + QString::number(index)] = osc;
        }

        QJsonObject kick;
        kick["limiter"] = getLimiterValue();
        kick["ampl_env"] = QJsonValue(QJsonObject({{"length", getKickLength()}}));
        kick["ampl_env"] = QJsonValue(QJsonObject({{"amplitude", getKickAmplitude()}}));
        auto points = getKickEnvelopePoints();
        QJsonArray jsonArray;
        for (const auto &point: points) {
                jsonArray.push_back(QJsonValue(QJsonArray({{point.x(), point.y()}})));
        }
        kick["ampl_env"] = QJsonValue(QJsonObject({{"points", jsonArray}}));
        kick["filter"] = QJsonValue(QJsonObject({{"enabled", isKickFilterEnabled()}}));
        kick["filter"] = QJsonValue(QJsonObject({{"type", static_cast<int>(getKickFilterType())}}));
        kick["filter"] = QJsonValue(QJsonObject({{"cutoff", getKickFilterFrequency()}}));
        kick["filter"] = QJsonValue(QJsonObject({{"factor", getKickFilterQFactor()}}));
        state["kick"] = kick;
        return QJsonDocument(state).toBinaryData();
}
