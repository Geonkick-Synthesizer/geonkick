/**
 * File name: envelope_draw_area.cpp
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

#include "envelope_draw_area.h"
#include "gkick_envelope.h"

#include <QPainter>
#include <QMouseEvent>

EnvelopeDrawingArea::EnvelopeDrawingArea(GeonkickWidget *parent,
                                         std::shared_ptr<GKickEnvelope> &envelope)
          : GeonkickWidget(parent),
          currentEnvelope(envelope),
          xPadding(10),
          yPadding(10)
{
        QPixmap pixmap("./themes/geontime/envelope_bk.png");
        setBackgroundImage(pixmap);
        setFixedSize(pixmap.size().width(), pixmap.size().height() - 50);
}

EnvelopeDrawingArea::~EnvelopeDrawingArea()
{
}

void EnvelopeDrawingArea::setEnvelope(std::shared_ptr<GKickEnvelope> &envelope)
{
        currentEnvelope = envelope;
}

void EnvelopeDrawingArea::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event);
        QPainter painter(this);
        currentEnvelope->draw(painter);
}

void
EnvelopeDrawingArea::mousePressEvent(QMouseEvent *event)
{
        if (event->button() == Qt::RightButton) {
                currentEnvelope->removePoint(QPointF(event->x() - xPadding,
                                                     height() - (event->y() + yPadding)));
                update();
                return;
        }

        mousePoint.setX(event->x());
        mousePoint.setY(event->y());
        currentEnvelope->selectPoint(QPointF(event->x() - xPadding,
                                             height() - (event->y() + yPadding)));
        if (currentEnvelope->hasSelected()) {
                update();
        }
}

void
EnvelopeDrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event);
        if (currentEnvelope->hasSelected()) {
                currentEnvelope->unselectPoint();
                update();
        }
}

void
EnvelopeDrawingArea::mouseDoubleClickEvent(QMouseEvent *event)
{
        currentEnvelope->addPoint(QPointF(event->x() - xPadding, height() - (event->y() + yPadding)));
        update();
}

void
EnvelopeDrawingArea::mouseMoveEvent(QMouseEvent *event)
{
        if (!currentEnvelope->hasSelected()) {
                return;
        }

        currentEnvelope->moveSelectedPoint(event->x(), event->y());
        mousePoint.setX(event->x());
        mousePoint.setY(event->y());
        update();
}
