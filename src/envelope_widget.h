/**
 * File name: envelope_widget.h
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

#ifndef GEONKICK_ENVELOPE_WIDGET_H
#define GEONKICK_ENVELOPE_WIDGET_H

#include "oscillator.h"
//#include "oscillator_envelope.h"
#include "geonkick_widget.h"


class EnvelopeWidgetDrawingArea;
class GeonkickButton;
class Envelope;

class EnvelopeWidget : public GeonkickWidget
{
public:

     enum class EnvelopeType:int {
                   Oscillator1 = static_cast<int>(Oscillator::Type::Oscillator1),
                   Oscillator2 = static_cast<int>(Oscillator::Type::Oscillator2),
                   Noise       = static_cast<int>(Oscillator::Type::Noise),
                   General
     };

     explicit EnvelopeWidget(GeonkickWidget *parent,
                             GeonkickApi *api,
                             const std::vector<std::unique_ptr<Oscillator>> &oscillators);
     ~EnvelopeWidget();
     void hideEnvelope(bool b);

     // protected slots:
     void showAmplitudeEnvelope();
     void showFrequencyEnvelope();
     void showGeneralEnvelope();
     void showOsc1Envelope();
     void showOsc2Envelope();
     void showNoiseEnvelope();

 private:
     void createButtomMenu();
     Envelope *currentEnvelope;
     std::vector<std::shared_ptr<Envelope>> envelopes;
     EnvelopeWidgetDrawingArea *drawArea;
     GeonkickButton *showAmplitudeEnvButton;
     GeonkickButton *showFrequencyEnvButton;
     GeonkickButton *osccillator1EvelopesButton;
     GeonkickButton *osccillator2EvelopesButton;
     GeonkickButton *noiseEvelopesButton;
     GeonkickButton *generalEvelopesButton;
};

#endif
