
GeonkickState::GeonkickState() :
        limiterValue(0),
        kickLength(0),
        kickAmplitude(0),
        kickFilterEnabled(0),
        kickFilterFrequency(0),
        kickFilterQFactor(0),
        kickFilterType(GeonkickApi::LowPass)
{
}

void GeonkickState::setLimiterValue(double val)
{
        limiterValue = val;
}

void GeonkickState::setKickLength(double val)
{
}

void GeonkickState::setKickAmplitude(double val)
{
}

void GeonkickState::enableKickFilter(bool b)
{
}

void GeonkickState::setKickFilterFrequency(double val)
{
}

void GeonkickState::setKickFilterQFactor(double val)
{
}

void GeonkickState::setKickFilterType(GeonkickApi::FilterType type)
{
}

void GeonkickState::setKickEnvelopePoints(const QPolygonF &points)
{
}

double GeonkickState::getLimiterValue() const
{
}

double GeonkickState::getKickLength() const
{
}

double GeonkickState::getKickAmplitude() const
{
}

bool GeonkickState::isKickFilterEnabled() const
{
}

double GeonkickState::getKickFilterFrequency() const
{
}

double GeonkickState::getKickFilterQFactor() const
{
}

GeonkickApi::FilterType GeonkickState::getKickFilterType() const
{
}

QPolygonF GeonkickState::getKickEnvelopePoints() const
{
}

void GeonkickState::setOscillatorEnabled(int index, bool b)
{
}

void GeonkickState::setOscillatorFunction(int index, GeonkickApi::FunctionType type)
{
}

void GeonkickState::setOscillatorAmplitue(int index, double val)
{
}

void GeonkickState::setOscillatorFrequency(int index, double val)
{
}

void GeonkickState::setOscillatorFilterEnabled(int index, bool b)
{
}

void GeonkickState::setOscillatorFilterType(int index, GeonkickApi::FilterType type)
{
}

void GeonkickState::setOscillatorFilterCutOffFreq(int index, double val)
{
}

void GeonkickState::setOscillatorFilterFactor(int index, double val)
{
}

void GeonkickState::setOscillatorEnvelopePoints(int index, const QPointF &points)
{
}

bool GeonkickState::isOscillatorEnabled(int index)
{
}

GeonkickApi::FunctionType GeonkickState::oscillatorFunction(int index)
{
}

double GeonkickState::oscillatorAmplitue(int index)
{
}

double GeonkickState::oscillatorFrequency(int index)
{
}

bool GeonkickState::isOscillatorFilterEnabled(int index)
{
}

FiterType GeonkickState::oscillatorFilterType(int index)
{
}

double GeonkickState::oscillatorFilterCutOffFreq(int index)
{
}

double GeonkickState::oscillatorFilterFactor(int index)
{
}

QPolygonF GeonkickState::oscillatorEnvelopePoints(int index)
{
}


