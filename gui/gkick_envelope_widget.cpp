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
          envelopeTitleLabel(NULL),
          drawArea(NULL),
          showAmplitudeEnvButton(NULL),
          showFrequencyEnvButton(NULL)

{
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_1]));
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_2]));
        envelopes.push_back(std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_NOISE]));
        envelopes.push_back(std::make_shared<GeneralEnvelope>(api));
        currentEnvelope = envelopes[GKickEnvelope::ENV_CATEGORY_GENERAL];

        QVBoxLayout *envelopeLayout = new QVBoxLayout(this);
        setLayout(envelopeLayout);
        envelopeLayout->setSpacing(0);

        // Create top area.
        envelopeTitleLabel = new QLabel(currentEnvelope->name(), this);
        envelopeTitleLabel->setAlignment(Qt::AlignHCenter);
        envelopeLayout->addWidget(envelopeTitleLabel);
        envelopeLayout->setStretchFactor(envelopeTitleLabel, 0);

        // Create drawing area.
        drawArea = new EnvelopeDrawingArea(this, currentEnvelope);
        envelopeLayout->addWidget(drawArea);
        envelopeLayout->setStretchFactor(drawArea, 1);

        // Create bottom area.
        showAmplitudeEnvButton = new QPushButton(tr("AMPL"), this);
        showFrequencyEnvButton = new QPushButton(tr("FREQ"), this);
        connect(showAmplitudeEnvButton, SIGNAL(pressed()), this, SLOT(showAmplitudeEnvelope()));
        connect(showFrequencyEnvButton, SIGNAL(pressed()), this, SLOT(showFrequencyEnvelope()));
        QWidget* buttomAreaWidget = new QWidget(this);
        QHBoxLayout *buttomAreaLayout = new QHBoxLayout(buttomAreaWidget);
        buttomAreaLayout->addWidget(showAmplitudeEnvButton);
        buttomAreaLayout->addWidget(showFrequencyEnvButton);
        buttomAreaWidget->setLayout(buttomAreaLayout);
        envelopeLayout->addWidget(buttomAreaWidget);
        envelopeLayout->setStretchFactor(buttomAreaWidget, 0);
        updateButtonArea();
}

GKickEnvelopeWidget::~GKickEnvelopeWidget()
{

}

void GKickEnvelopeWidget::updateButtonArea()
{
        if (currentEnvelope->category() == GKickEnvelope::ENV_CATEGORY_OSC_1
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
        }
}

void GKickEnvelopeWidget::viewEnvelope(GKickEnvelope::EnvelopeCategory category)
{
        currentEnvelope = envelopes[category];
        envelopeTitleLabel->setText(currentEnvelope->name());
        drawArea->setEnvelope(currentEnvelope);
        updateButtonArea();
        drawArea->update();
}

void GKickEnvelopeWidget::showAmplitudeEnvelope()
{
       currentEnvelope->setType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
       drawArea->update();
}

void GKickEnvelopeWidget::showFrequencyEnvelope()
{
        currentEnvelope->setType(GKickEnvelope::ENV_TYPE_FREQUENCY);
        drawArea->update();
}

