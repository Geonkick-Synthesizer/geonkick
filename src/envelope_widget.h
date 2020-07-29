/**
 * File name: envelope_widget.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://iuriepage.wordpress.com)
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
#include "geonkick_widget.h"
#include "geonkick_api.h"
#include "envelope.h"

class RkContainer;
class EnvelopeWidgetDrawingArea;
class GeonkickButton;

class EnvelopeWidget : public GeonkickWidget
{
public:

     explicit EnvelopeWidget(GeonkickWidget *parent,
                             GeonkickApi *api,
                             const std::vector<std::unique_ptr<Oscillator>> &oscillators);
     void hideEnvelope(bool b);
     void showEnvelopeType(Envelope::Type type);
     void updateGui();
     RK_DECL_ACT(requestUpdateGui, requestUpdateGui(), RK_ARG_TYPE(), RK_ARG_VAL());
     RK_DECL_ACT(envelopeTypeSelected,
                 envelopeTypeSelected(Envelope::Type type),
                 RK_ARG_TYPE(Envelope::Type),
                 RK_ARG_VAL(type));

 protected:
     void showEnvelope(Envelope::Category category);
     Envelope* getEnvelope(Envelope::Category category);
     void updateKickGraph(std::shared_ptr<RkImage> graphImage);
     void createLayersButtons(GeonkickWidget *buttomAreaWidget);
     void setLayer(GeonkickApi::Layer layer);
     RkWidget* addSeparator(RkWidget *parent,
                            RkContainer *layout,
                            Rk::Alignment alignment = Rk::Alignment::AlignLeft,
                            int maring = 10);

 private:
     void createButtomMenu();
     std::unordered_map<int, std::shared_ptr<Envelope>> envelopes;
     EnvelopeWidgetDrawingArea *drawArea;
     GeonkickButton *showAmplitudeEnvButton;
     GeonkickButton *showFrequencyEnvButton;
     RkWidget* frequencySeparator;
     GeonkickButton *showFilterEnvButton;
     GeonkickButton *oscillator1EvelopesButton;
     GeonkickButton *oscillator2EvelopesButton;
     GeonkickButton *noiseEvelopesButton;
     GeonkickButton *generalEvelopesButton;
     GeonkickButton *layer1Button;
     GeonkickButton *layer2Button;
     GeonkickButton *layer3Button;
     GeonkickApi *geonkickApi;
     RkContainer *menuContainer;
};

#endif
