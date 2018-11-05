/**
 * File name: geonkick_state.h
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

#ifndef GEONKICK_STATE_H
#define GEONKICK_STATE_H

class GeonkickState {
 public:
        GeonkickState();
        GeonkickState(const GeonkickState &other);
        GeonkickState(GeonkickState &&other);
        GeonkickState& operator = (const GeonkickState &other);
        GeonkickState& operator = (GeonkickState &&other);

        limiterValue();
        getKickLength();
        getKickAmplitude();
        isKickFilterEnabled();
        kickFilterFrequency();
        kickEnvelopePoints();
        kickFilterFactor();
        kickEnvelopePoints();
        isOscillatorEnabled(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorFunction(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorAmplitue(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorFrequency(static_cast<GeonkickState::Oscillator>(osc)));
        isOscillatorFilterEnabled(static_cast<GeonkickState::Oscillator>(osc)));
        ocillatorFilterType(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorFilterCutOffFreq(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorFilterFactor(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorEnvelopePoints(static_cast<GeonkickState::Oscillator>(osc)),
                                   GeonkickState::Envelope::Amplitude), EnvelopeType::Amplitude);

        setLimiterValue();
        setKickLength();
        setKickAmplitude();
        enabledKickFilter(bool b);
        setKickFilterFrequency();
        setKickFilterFactor();
        setKickEnvelopePoints();
        enabledOscillator(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorFunction(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorAmplitue(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorFrequency(static_cast<GeonkickState::Oscillator>(osc)));
        oscillatorFilterEnabled(static_cast<GeonkickState::Oscillator>(osc)));
        setOcillatorFilterType(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorFilterCutOffFreq(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorFilterFactor(static_cast<GeonkickState::Oscillator>(osc)));
        setOscillatorEnvelopePoints(static_cast<GeonkickState::Oscillator>(osc)),

private:
        double limiterValue;
        double kickLength;
        double kickAplitude;
        bool   kickFilterEnabled;
        double kickFilterFrequency;
        double setKickFilterFactor;
        QPolygonF setKickEnvelopePoints;
bool enabledOscillator;
FunctionType setOscillatorFunction;
double setOscillatorAmplitue;
double setOscillatorFrequency;
bool oscillatorFilterEnabled;
FilterType setOcillatorFilterType;
double setOscillatorFilterCutOffFreq;
double setOscillatorFilterFactor;
QPolygonF setOscillatorEnvelopePoints;
}

#endif // GEONKICK_STATE_H
