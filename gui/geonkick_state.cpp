#include "geonkick_state.h"

GeonkickState::GeonkickState() :
        limiterValue(0),
        kickLength(0),
        kickAmplitude(0),
        kickFilterEnabled(0),
        kickFilterFrequency(0),
        kickFilterQFactor(0),
        kickFilterType(GeonkickApi::LowPass)
        //        oscillators{
        //  {GeonkickApi::OscillatorType::Oscillator1, std::make_shared<Oscillator>()}
        //   {GeonkickApi::OscillatorType::Oscillator1, std::make_shared<Oscillator>()}
        //   {GeonkickApi::OscillatorType::Noise, std::make_shared<Oscillator>()}}
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

std::shared_ptr<Oscillator>& GeonkickState::getOscillator(int index)
{
        return oscillators[1]
        auto it = oscillators.find(index);
        if (it != oscillators.end()) {
                return it.second;
        }

        return nullptr;
}

void GeonkickState::setOscillatorEnabled(int index, bool b)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->isEnbaled = b;
        }
}

void GeonkickState::setOscillatorFunction(int index, GeonkickApi::FunctionType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->functionType = b;
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
                oscillator->filterEnabled = b;
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

void GeonkickState::setOscillatorEnvelopePoints(int index, const QPointF &points)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                oscillator->filterFactor = val;
        }
}

bool GeonkickState::isOscillatorEnabled(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->isEnabled;
        }

        return false;
}

GeonkickApi::FunctionType GeonkickState::oscillatorFunction(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->function;
        }

        return GeonkickApi::FunctionType::Sine;
}

double GeonkickState::oscillatorAmplitue(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->amplitude;
        }
        return 0;
}

double GeonkickState::oscillatorFrequency(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->amplitude;
        }
        return 0;
}

bool GeonkickState::isOscillatorFilterEnabled(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterEnabled;
        }
        return false;
}

FiterType GeonkickState::oscillatorFilterType(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterType;
        }
        return FilterType::LowPass;
}

double GeonkickState::oscillatorFilterCutOffFreq(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterFrequency;
        }
        return 0;
}

double GeonkickState::oscillatorFilterFactor(int index)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                return oscillator->filterFactor;
        }
        return 0;
}

QPolygonF GeonkickState::oscillatorEnvelopePoints(int index, GeonkickApi::EnvelopeType type)
{
        auto oscillator = getOscillator(index);
        if (oscillator) {
                if (type == GeonkickApi::EnvelopeType::Amplitude)
                        return oscillator->amplitudeEnvelope;
                else
                        return oscillator->frequencyEnvelope;
        }

        return QPloygonF();
}

