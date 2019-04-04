/**
 * File name: envelope_widget.cpp
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

#include "envelope_widget.h"
//#include "general_envelope.h"
#include "envelope_draw_area.h"
//#include "geonkick_button.h"
//#include "kick_graph.h"

EnvelopeWidget::EnvelopeWidget(GeonkickWidget *parent,
                               GeonkickApi *api,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
          : GeonkickWidget(parent)
          , drawArea{nullptr}
          , showAmplitudeEnvButton{nullptr}
          , showFrequencyEnvButton{nullptr}

{
        // Create drawing area.
        drawArea = new EnvelopeWidgetDrawingArea(this);
        drawArea->show();

        /*        QRect rect = drawArea->getDrawingArea();
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)];
        std::shared_ptr<Envelope> envelope = std::make_shared<OscillatorEnvelope>(oscillator, rect);
        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.push_back(envelope);
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)];
        envelope = std::make_shared<OscillatorEnvelope>(oscillator, rect);
        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.push_back(envelope);
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)];
        envelope = std::make_shared<OscillatorEnvelope>(oscillator, rect);
        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.push_back(envelope);
        envelope = std::make_shared<GeneralEnvelope>(api, rect);
        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.push_back(envelope);
        auto kickGraph = new KickGraph(this, api);
        kickGraph->setDrawingArea(rect);
        drawArea->setKickGraph(kickGraph);

        createButtomMenu();
        showGeneralEnvelope();*/
}

EnvelopeWidget::~EnvelopeWidget()
{

}

void EnvelopeWidget::createButtomMenu()
{
        /*        auto buttomAreaWidget = new GeonkickWidget(this);
        buttomAreaWidget->setContentsMargins(0, 0, 0, 0);

        layout()->addWidget(buttomAreaWidget);
        showAmplitudeEnvButton = new GeonkickButton(buttomAreaWidget);
        connect(showAmplitudeEnvButton, SIGNAL(toggled(bool)), this, SLOT(showAmplitudeEnvelope()));
        showAmplitudeEnvButton->setPressedImage(QPixmap(":/show_ampl_env_active.png"));
        showAmplitudeEnvButton->setUnpressedImage(QPixmap(":/show_ampl_env.png"));
        showFrequencyEnvButton = new GeonkickButton(buttomAreaWidget);
        connect(showFrequencyEnvButton, SIGNAL(toggled(bool)), this, SLOT(showFrequencyEnvelope()));
        showFrequencyEnvButton->setPressedImage(QPixmap(":/show_freq_env_active.png"));
        showFrequencyEnvButton->setUnpressedImage(QPixmap(":/show_freq_env.png"));
        buttomAreaWidget->setFixedSize(drawArea->width(), showAmplitudeEnvButton->height() + 2);

        osccillator1EvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(osccillator1EvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showOsc1Envelope()));
        osccillator1EvelopesButton->setPressedImage(QPixmap(":/show_osc1_envelopes_button_active.png"));
        osccillator1EvelopesButton->setUnpressedImage(QPixmap(":/show_osc1_envelopes_button.png"));

        osccillator2EvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(osccillator2EvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showOsc2Envelope()));
        osccillator2EvelopesButton->setPressedImage(QPixmap(":/show_osc2_envelopes_button_active.png"));
        osccillator2EvelopesButton->setUnpressedImage(QPixmap(":/show_osc2_envelopes_button.png"));

        noiseEvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(noiseEvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showNoiseEnvelope()));
        noiseEvelopesButton->setPressedImage(QPixmap(":/show_noise_envelopes_button_active.png"));
        noiseEvelopesButton->setUnpressedImage(QPixmap(":/show_noise_envelopes_button.png"));

        generalEvelopesButton = new GeonkickButton(buttomAreaWidget);
        generalEvelopesButton->setPressed(true);
        connect(generalEvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showGeneralEnvelope()));
        generalEvelopesButton->setPressedImage(QPixmap(":/show_general_envelopes_button_active.png"));
        generalEvelopesButton->setUnpressedImage(QPixmap(":/show_general_envelopes_button.png"));

        QHBoxLayout *buttomAreaLayout = new QHBoxLayout(buttomAreaWidget);
        buttomAreaLayout->addWidget(showAmplitudeEnvButton);
        buttomAreaLayout->setAlignment(showAmplitudeEnvButton, Qt::AlignLeft);
        buttomAreaLayout->addWidget(showFrequencyEnvButton);
        buttomAreaLayout->setAlignment(showFrequencyEnvButton, Qt::AlignLeft);
        buttomAreaLayout->addStretch(1);
        buttomAreaLayout->addWidget(osccillator1EvelopesButton);
        buttomAreaLayout->setAlignment(osccillator1EvelopesButton, Qt::AlignRight);
        buttomAreaLayout->addWidget(osccillator2EvelopesButton);
        buttomAreaLayout->setAlignment(osccillator2EvelopesButton, Qt::AlignRight);
        buttomAreaLayout->addWidget(noiseEvelopesButton);
        buttomAreaLayout->setAlignment(noiseEvelopesButton, Qt::AlignRight);
        buttomAreaLayout->addWidget(generalEvelopesButton);
        buttomAreaLayout->setAlignment(generalEvelopesButton, Qt::AlignRight);
        buttomAreaWidget->setLayout(buttomAreaLayout);
        buttomAreaLayout->setSpacing(0);
        buttomAreaLayout->setContentsMargins(0, 0, 0, 0);*/
}

void EnvelopeWidget::showGeneralEnvelope()
{
        /*        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(envelopes[static_cast<int>(EnvelopeType::General)]);*/
}

void EnvelopeWidget::showOsc1Envelope()
{
        /*        generalEvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        if (envelopes[static_cast<int>(EnvelopeType::Oscillator1)]->type()
            == Envelope::Type::Amplitude) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelopes[static_cast<int>(EnvelopeType::Oscillator1)]);*/
}

void EnvelopeWidget::showOsc2Envelope()
{
        /*        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        if (envelopes[static_cast<int>(EnvelopeType::Oscillator2)]->type()
            == Envelope::Type::Amplitude) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelopes[static_cast<int>(EnvelopeType::Oscillator2)]);*/
}

void EnvelopeWidget::showNoiseEnvelope()
{
        /*        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(envelopes[static_cast<int>(EnvelopeType::Noise)]);*/
}

void EnvelopeWidget::showAmplitudeEnvelope()
{
        /*        auto envelope = drawArea->getEnvelope();
        showFrequencyEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(Envelope::Type::Amplitude)) {
                envelope->setType(Envelope::Type::Amplitude);
                drawArea->update();
                }*/
}

void EnvelopeWidget::showFrequencyEnvelope()
{
        /*        auto envelope = drawArea->getEnvelope();
        showAmplitudeEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(Envelope::Type::Frequency)) {
                envelope->setType(Envelope::Type::Frequency);
                drawArea->update();
        }
        */
}

void EnvelopeWidget::hideEnvelope(bool b)
{
        //        drawArea->setHideEnvelope(b);
}

