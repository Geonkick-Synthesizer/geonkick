#include "geonkick_state.h"

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
