/**
 * File name: envelope_draw_area.cpp
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

#include "envelope_draw_area.h"
#include "envelope.h"
#include "kick_graph.h"

#include <RkPainter.h>
#include <RkEvent.h>

EnvelopeWidgetDrawingArea::EnvelopeWidgetDrawingArea(GeonkickWidget *parent)
          : GeonkickWidget(parent)
          , currentEnvelope{nullptr}
          , kickGraph{nullptr}
          , hideEnvelope{false}
{
        setFixedSize(850, 300);
        int padding = 50;
        drawingArea = RkRect(1.1 * padding, padding / 2, width() - 1.5 * padding, height() - 1.2 * padding);
        setBackgroundColor(40, 40, 40);
}

EnvelopeWidgetDrawingArea::~EnvelopeWidgetDrawingArea()
{
}

void EnvelopeWidgetDrawingArea::setEnvelope(Envelope* envelope)
{
        if (envelope) {
                //if (currentEnvelope)
                        // disconnect(currentEnvelope.get(), 0, this, 0);
                currentEnvelope = envelope;
                if (currentEnvelope) {
                        /*                        connect(currentEnvelope.get(), SIGNAL(envelopeUpdated()),
                                this, SLOT(envelopeUpdated()));
                        connect(currentEnvelope.get(), SIGNAL(envelopeUpdated()),
                        this, SLOT(envelopeUpdated()));*/
                        update();
                }
        }
}

void EnvelopeWidgetDrawingArea::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
        RK_UNUSED(event);
        RkImage image(width(), height());
        RkPainter painter(&image);
        painter.fillRect(rect(), background());

        if (currentEnvelope)
                currentEnvelope->draw(painter, Envelope::DrawLayer::Axies);

        if (kickGraph)
                kickGraph->draw(painter);

        if (currentEnvelope && !isHideEnvelope())
                currentEnvelope->draw(painter, Envelope::DrawLayer::Envelope);

        auto pen = painter.pen();
        pen.setWidth(1);
        pen.setColor({20, 20, 20, 255});
        painter.setPen(pen);
        painter.drawRect({0, 0, width() - 1, height() - 1});

        RkPainter paint(this);
        paint.drawImage(image, 0, 0);
}

void EnvelopeWidgetDrawingArea::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        if (event->button() != RkMouseEvent::ButtonType::Right
            && event->button() != RkMouseEvent::ButtonType::Left)
                return;

        RkPoint point(event->x() - drawingArea.left(),
                      drawingArea.bottom() - event->y());
        if (event->button() == RkMouseEvent::ButtonType::Right) {
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
                if (currentEnvelope->hasSelected())
                        update();
        }
}

void EnvelopeWidgetDrawingArea::mouseButtonReleaseEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        RK_UNUSED(event);
        if (currentEnvelope && currentEnvelope->hasSelected()) {
                currentEnvelope->unselectPoint();
                update();
        }
}

void EnvelopeWidgetDrawingArea::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        if (event->button() != RkMouseEvent::ButtonType::Right) {
                RkPoint point(event->x() - drawingArea.left(), drawingArea.bottom() - event->y());
                if (currentEnvelope) {
                        currentEnvelope->addPoint(point);
                        update();
                }
        }
}

void EnvelopeWidgetDrawingArea::mouseMoveEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        if (currentEnvelope && currentEnvelope->hasSelected()) {
                RkPoint point(event->x() - drawingArea.left(), drawingArea.bottom() - event->y());
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

Envelope* EnvelopeWidgetDrawingArea::getEnvelope() const
{
        return currentEnvelope;
}

const RkRect EnvelopeWidgetDrawingArea::getDrawingArea()
{
        return drawingArea;
}

void EnvelopeWidgetDrawingArea::setKickGraph(std::unique_ptr<KickGraph> &graph)
{
        kickGraph = std::move(graph);
        kickGraph->setDrawingArea(drawingArea);
        //        connect(kickGraph, SIGNAL(graphUpdated()), this, SLOT(update()));
}

KickGraph* EnvelopeWidgetDrawingArea::getKickGraph() const
{
        return kickGraph.get();
}

bool EnvelopeWidgetDrawingArea::isHideEnvelope() const
{
        return hideEnvelope;
}

void EnvelopeWidgetDrawingArea::setHideEnvelope(bool b)
{
        if (hideEnvelope != b) {
                hideEnvelope = b;
                update();
        }
}
