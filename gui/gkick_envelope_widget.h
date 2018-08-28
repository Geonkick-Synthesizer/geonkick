/**
 * File name: gkick_envelope_widget.h
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

#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "gkick_oscillator.h"
#include "oscillator_envelope.h"
#include "gkick_envelope.h"
#include "geonkick_widget.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class EnvelopeDrawingArea;
class QLabel;
class QPushButton;

class GKickEnvelopeWidget : public GeonkickWidget
{
   Q_OBJECT
public:

     GKickEnvelopeWidget(GeonkickWidget *parent,
                       std::shared_ptr<GKickApi> &api,
                       std::vector<std::shared_ptr<GKickOscillator>> &oscillators);
     ~GKickEnvelopeWidget();
     void setBackgourndImage(const QPixmap &pixmap);

   public slots:

     void viewEnvelope(GKickEnvelope::EnvelopeCategory category);

 protected:
     void updateButtonArea();

 protected slots:
     void showAmplitudeEnvelope();
     void showFrequencyEnvelope();

 private:
     std::shared_ptr<GKickEnvelope> currentEnvelope;
     std::vector<std::shared_ptr<GKickEnvelope>> envelopes;
     QLabel *envelopeTitleLabel;
     EnvelopeDrawingArea *drawArea;
     QPushButton *showAmplitudeEnvButton;
     QPushButton *showFrequencyEnvButton;
     int xPadding;
     int yPadding;
     QPointF originPoint;
     QPointF mousePoint;
     QPixmap *backgorundImage;
};

#endif
