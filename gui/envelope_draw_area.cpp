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
#include "envelope.h"
#include "kick_graph.h"

#include <QPainter>
#include <QMouseEvent>

EnvelopeWidgetDrawingArea::EnvelopeWidgetDrawingArea(GeonkickWidget *parent)
          : GeonkickWidget(parent),
            currentEnvelope(nullptr),
            kickGraph(nullptr)
{
        setFixedSize(850, 300);
        int padding = 50;
        drawingArea = QRect(1.1 * padding, padding / 2, width() - 1.5 * padding, height() - 1.2 * padding);

        QPalette pal;
        pal.setColor(QPalette::Background, QColor(40, 40, 40));
        setAutoFillBackground(true);
        setPalette(pal);
}

EnvelopeWidgetDrawingArea::~EnvelopeWidgetDrawingArea()
{
}

void EnvelopeWidgetDrawingArea::setEnvelope(std::shared_ptr<Envelope> &envelope)
{
        if (envelope) {
                if (currentEnvelope) {
                        disconnect(currentEnvelope.get(), 0, this, 0);
                }

                currentEnvelope = envelope;
                if (currentEnvelope) {
                        connect(currentEnvelope.get(), SIGNAL(envelopeUpdated()),
                                this, SLOT(envelopeUpdated()));
                        connect(currentEnvelope.get(), SIGNAL(envelopeUpdated()),
                                this, SLOT(envelopeUpdated()));
                        update();
                }
        }
}

void EnvelopeWidgetDrawingArea::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event);
        QPainter painter(this);

        if (kickGraph) {
                kickGraph->draw(painter);
        }

        if (currentEnvelope) {
                currentEnvelope->draw(painter);
        }
}

void
EnvelopeWidgetDrawingArea::mousePressEvent(QMouseEvent *event)
{
        QPoint point(event->x() - drawingArea.x(),
                      drawingArea.bottomRight().y() - event->y());
        if (event->button() == Qt::RightButton) {
                if (currentEnvelope) {
                        currentEnvelope->removePoint(point);
                        update();
                }
                return;
        }

        mousePoint.setX(event->x());
        mousePoint.setY(event->y());
        if (currentEnvelope) {
                currentEnvelope->selectPoint(point);
                if (currentEnvelope->hasSelected()) {
                        update();
                }
        }
}

void
EnvelopeWidgetDrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event);
        if (currentEnvelope && currentEnvelope->hasSelected()) {
                currentEnvelope->unselectPoint();
                update();
        }
}

void
EnvelopeWidgetDrawingArea::mouseDoubleClickEvent(QMouseEvent *event)
{
        if (event->button() != Qt::RightButton) {
                QPoint point(event->x() - drawingArea.x(),
                              drawingArea.bottomLeft().y() - event->y());
                if (currentEnvelope) {
                        currentEnvelope->addPoint(point);
                }
                update();
        }
}

void
EnvelopeWidgetDrawingArea::mouseMoveEvent(QMouseEvent *event)
{
        if (currentEnvelope && currentEnvelope->hasSelected()) {
                QPoint point(event->x() - drawingArea.x(), drawingArea.bottomLeft().y() - event->y());
                currentEnvelope->moveSelectedPoint(point.x(), point.y());
                mousePoint.setX(event->x());
                mousePoint.setY(event->y());
                update();
        }
}

void EnvelopeWidgetDrawingArea::envelopeUpdated()
{
        update();
}

std::shared_ptr<Envelope> EnvelopeWidgetDrawingArea::getEnvelope() const
{
        return currentEnvelope;
}

const QRect EnvelopeWidgetDrawingArea::getDrawingArea()
{
        return drawingArea;
}

void EnvelopeWidgetDrawingArea::setKickGraph(KickGraph *graph)
{
        kickGraph = graph;
        kickGraph->setDrawingArea(drawingArea);
}

KickGraph* EnvelopeWidgetDrawingArea::getKickGraph()
{
        return kickGraph;
}
