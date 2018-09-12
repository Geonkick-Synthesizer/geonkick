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
        currentEnvelope = envelopes[GKickEnvelope::ENV_CATEGORY_GENERAL];

        QVBoxLayout *envelopeLayout = new QVBoxLayout(this);
        setLayout(envelopeLayout);
        envelopeLayout->setSpacing(0);
        envelopeLayout->setContentsMargins(10, 0, 0, 0);

        // Create drawing area.
        drawArea = new EnvelopeDrawingArea(this, currentEnvelope);
        drawArea->setContentsMargins(10, 0, 0, 0);
        envelopeLayout->addWidget(drawArea);

        createButtomMenu();
        //updateButtonArea();
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
        showAmplitudeEnvButton->setPressedImage(QPixmap("./themes/geontime/show_ampl_env_active.png"));
        showAmplitudeEnvButton->setUnpressedImage(QPixmap("./themes/geontime/show_ampl_env.png"));
        showFrequencyEnvButton = new GeonkickButton(buttomAreaWidget);
        showFrequencyEnvButton->setPressedImage(QPixmap("./themes/geontime/show_freq_env_active.png"));
        showFrequencyEnvButton->setUnpressedImage(QPixmap("./themes/geontime/show_freq_env.png"));
        buttomAreaWidget->setFixedSize(drawArea->width(), showAmplitudeEnvButton->height() + 2);
        connect(showAmplitudeEnvButton, SIGNAL(pressed()), this, SLOT(showAmplitudeEnvelope()));
        connect(showFrequencyEnvButton, SIGNAL(pressed()), this, SLOT(showFrequencyEnvelope()));

        osccillator1EvelopesButton = new GeonkickButton(buttomAreaWidget);
        osccillator1EvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_osc1_envelopes_button_active.png"));
        osccillator1EvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_osc1_envelopes_button.png"));
        osccillator2EvelopesButton = new GeonkickButton(buttomAreaWidget);
        osccillator2EvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_osc2_envelopes_button_active.png"));
        osccillator2EvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_osc2_envelopes_button.png"));
        noiseEvelopesButton = new GeonkickButton(buttomAreaWidget);
        noiseEvelopesButton->setPressedImage(QPixmap("./themes/geontime/show_noise_envelopes_button_active.png"));
        noiseEvelopesButton->setUnpressedImage(QPixmap("./themes/geontime/show_noise_envelopes_button.png"));
        generalEvelopesButton = new GeonkickButton(buttomAreaWidget);
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

void GKickEnvelopeWidget::updateButtonArea()
{
        /*if (currentEnvelope->category() == GKickEnvelope::ENV_CATEGORY_OSC_1
            || currentEnvelope->category() == GKickEnvelope::ENV_CATEGORY_OSC_2)  {
                showAmplitudeEnvButton->setVisible(true);
                showFrequencyEnvButton->setVisible(true);
        } else {
                showAmplitudeEnvButton->setVisible(false);
                showFrequencyEnvButton->setVisible(false);
        }

        if (currentEnvelope->type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                showAmplitudeEnvButton->setDown(true);
                showFrequencyEnvButton->setDown(false);
        } else {
                showAmplitudeEnvButton->setDown(false);
                showFrequencyEnvButton->setDown(true);
                }*/
}

void GKickEnvelopeWidget::viewEnvelope(GKickEnvelope::EnvelopeCategory category)
{
        /*        currentEnvelope = envelopes[category];
        envelopeTitleLabel->setText(currentEnvelope->name());
        drawArea->setEnvelope(currentEnvelope);
        updateButtonArea();
        drawArea->update();*/
}

void GKickEnvelopeWidget::showAmplitudeEnvelope()
{
        /*       currentEnvelope->setType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
                 drawArea->update();*/
}

void GKickEnvelopeWidget::showFrequencyEnvelope()
{
        /*        currentEnvelope->setType(GKickEnvelope::ENV_TYPE_FREQUENCY);
                  drawArea->update();*/
}

