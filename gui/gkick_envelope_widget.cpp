/**
 * File name: gkick_envelope_widget.cpp
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

#include "gkick_envelope_widget.h"
#include "general_envelope.h"
#include "envelope_draw_area.h"
#include "geonkick_button.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

GKickEnvelopeWidget::GKickEnvelopeWidget(GeonkickWidget *parent,
                                         std::shared_ptr<GKickApi> &api,
                                         std::vector<std::shared_ptr<GKickOscillator>> &oscillators)
          : GeonkickWidget(parent),
          drawArea(nullptr),
          showAmplitudeEnvButton(nullptr),
          showFrequencyEnvButton(nullptr)

{
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_1]));
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_2]));
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_NOISE]));
        envelopes.push_back(std::make_shared<GeneralEnvelope>(api));

        QVBoxLayout *envelopeLayout = new QVBoxLayout(this);
        setLayout(envelopeLayout);
        envelopeLayout->setSpacing(0);
        envelopeLayout->setContentsMargins(0, 0, 0, 0);

        // Create drawing area.
        drawArea = new EnvelopeDrawingArea(this, envelopes[static_cast<int>(Envelope::General)]);
        drawArea->setContentsMargins(0, 0, 0, 0);
        envelopeLayout->addWidget(drawArea);

        createButtomMenu();
        showGeneralEnvelope();
}

GKickEnvelopeWidget::~GKickEnvelopeWidget()
{

}

void GKickEnvelopeWidget::createButtomMenu()
{
        auto buttomAreaWidget = new GeonkickWidget(this);// Create bottom area.
        buttomAreaWidget->setContentsMargins(0, 0, 0, 0);

        layout()->addWidget(buttomAreaWidget);
        showAmplitudeEnvButton = new GeonkickButton(buttomAreaWidget);
        connect(showAmplitudeEnvButton, SIGNAL(toggled(bool)), this, SLOT(showAmplitudeEnvelope()));
        showAmplitudeEnvButton->setPressedImage(QPixmap("./themes/geontime/show_ampl_env_active.png"));
        showAmplitudeEnvButton->setUnpressedImage(QPixmap("./themes/geontime/show_ampl_env.png"));
        showFrequencyEnvButton = new GeonkickButton(buttomAreaWidget);
        connect(showFrequencyEnvButton, SIGNAL(toggled(bool)), this, SLOT(showFrequencyEnvelope()));
        showFrequencyEnvButton->setPressedImage(QPixmap("./themes/geontime/show_freq_env_active.png"));
        showFrequencyEnvButton->setUnpressedImage(QPixmap("./themes/geontime/show_freq_env.png"));
        buttomAreaWidget->setFixedSize(drawArea->width(), showAmplitudeEnvButton->height() + 2);

        osccillator1EvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(osccillator1EvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showOsc1Envelope()));
        osccillator1EvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_osc1_envelopes_button_active.png"));
        osccillator1EvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_osc1_envelopes_button.png"));

        osccillator2EvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(osccillator2EvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showOsc2Envelope()));
        osccillator2EvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_osc2_envelopes_button_active.png"));
        osccillator2EvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_osc2_envelopes_button.png"));

        noiseEvelopesButton = new GeonkickButton(buttomAreaWidget);
        connect(noiseEvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showNoiseEnvelope()));
        noiseEvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_noise_envelopes_button_active.png"));
        noiseEvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_noise_envelopes_button.png"));

        generalEvelopesButton = new GeonkickButton(buttomAreaWidget);
        generalEvelopesButton->setPressed(true);
        connect(generalEvelopesButton, SIGNAL(toggled(bool)), this, SLOT(showGeneralEnvelope()));
        generalEvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_general_envelopes_button_active.png"));
        generalEvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_general_envelopes_button.png"));

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
        buttomAreaLayout->setContentsMargins(0, 0, 0, 0);
}

void GKickEnvelopeWidget::showGeneralEnvelope()
{
        GKICK_LOG_INFO("called");
        //        generalEvelopesButton->setPressed(true);
        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(envelopes[static_cast<int>(Envelope::General)]);
}

void GKickEnvelopeWidget::showOsc1Envelope()
{
        generalEvelopesButton->setPressed(false);
        //osccillator1EvelopesButton->setPressed(true);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        if (envelopes[static_cast<int>(Envelope::Oscillator1)]->type()
            == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelopes[static_cast<int>(Envelope::Oscillator1)]);
}

void GKickEnvelopeWidget::showOsc2Envelope()
{
        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        //osccillator2EvelopesButton->setPressed(true);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        if (envelopes[static_cast<int>(Envelope::Oscillator1)]->type()
            == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelopes[static_cast<int>(Envelope::Oscillator2)]);
}

void GKickEnvelopeWidget::showNoiseEnvelope()
{
        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        //noiseEvelopesButton->setPressed(true);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(envelopes[static_cast<int>(Envelope::Noise)]);
}

void GKickEnvelopeWidget::showAmplitudeEnvelope()
{
        GKICK_LOG_INFO("HERE");
        auto envelope = drawArea->getEnvelope();
        showFrequencyEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(GKickEnvelope::ENV_TYPE_AMPLITUDE)) {
                GKICK_LOG_INFO("HERE1");
                envelope->setType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
                drawArea->update();
        }
}

void GKickEnvelopeWidget::showFrequencyEnvelope()
{
        GKICK_LOG_INFO("HERE");
        auto envelope = drawArea->getEnvelope();
        showAmplitudeEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(GKickEnvelope::ENV_TYPE_FREQUENCY)) {
                GKICK_LOG_INFO("HERE1");
                envelope->setType(GKickEnvelope::ENV_TYPE_FREQUENCY);
                drawArea->update();
        }
}

