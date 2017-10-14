/**
 * File name: envelope_draw_area.cpp
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
 *
 * This file is part of Geonkick.
 *
 * MashaFile is free software; you can redistribute it and/or modify
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

EnvelopeDrawingArea::EnvelopeDrawingArea(QWidget *parent,
                                         std::shared_ptr<GKickEnvelope> &envelope)
        : QWidget(parent),
          currentEnvelope(envelope)
{
}

~EnvelopeDrawingArea()
{
}

void EnvelopeDrawingArea::setEnvelope(std::<GKickEnvelope> &envelope)
{
        currentEnvelope = envelope;
}

void EnvelopeDrawingArea::update()
{
        QPainter painter(this);
        currentEnvelope->draw(painter);
}

void EnvelopeDrawingArea::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event);
        update();
}

void
OscillatorWidget::mousePressEvent(QMouseEvent *event)
{
        if (event->button() == Qt::RightButton) {
                currentEnvelope->removePoint(QPointF(event->x() - xPadding,
                                                     height() - (event->y() + yPadding)));
                update();
                return;
        }

        mousePoint.setX(event->x());
        mousePoint.setY(event->y());
        currnetEnvelope->selectPoint(QPointF(event->x() - xPadding,
                                             height() - (event->y() + yPadding)));
        if (currnetEnvelope->hasSelected()) {
                update();
        }
}

void
OscillatorWidget::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event);
        if (currentEnvelope->hasSelected()) {
                currentEnvelope->unselectPoint();
                update();
        }
}

void
OscillatorWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
        currentEnvelope->addPoint(QPointF(event->x() - xPadding, height() - (event->y() + yPadding)));
        update();
}

void
OscillatorWidget::mouseMoveEvent(QMouseEvent *event)
{
        if (!currentEnvelope->hasSelected()) {
                return;
        }

        currentEnvelope->moveSelectedPoint(event->x(), event->y());
        mousePoint.setX(event->x());
        mousePoint.setY(event->y());
        update();
}
