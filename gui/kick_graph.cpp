/**
 * File name: kick_graph.cpp
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

#include "kick_graph.h"
#include "geonkick_api.h"
#include "globals.h"

KickGraph::KickGraph(QObject *parent, GeonkickApi *api)
        : QObject(parent),
          geonkickApi(api)
{
        connect(api, SIGNAL(kickUpdated()), this, SLOT(updateGraphBuffer()));
}

KickGraph::~KickGraph()
{
}

void KickGraph::draw(QPainter &painter)
{
        //        drawKickGraph();
        if (!cacheGraphImage.isNull()) {
                painter.drawPixmap(drawingArea, cacheGraphImage);
        }
}

void KickGraph::setDrawingArea(const QRect &rect)
{
        drawingArea = rect;
        cacheGraphImage = QPixmap(drawingArea.size());
        cacheGraphImage.fill(Qt::transparent);
        drawKickGraph();
}

void KickGraph::updateGraphBuffer()
{
        kickBuffer = geonkickApi->getKickBuffer();
        drawKickGraph();
}

void KickGraph::drawKickGraph()
{
        /*        if (!cacheGraphImage.isNull()) {
                static const QPointF points[4] = {
                        QPointF(10.0, 80.0),
                        QPointF(20.0, 10.0),
                        QPointF(80.0, 30.0),
                        QPointF(90.0, 70.0)
                };

                GEONKICK_LOG_DEBUG("is null:" << cacheGraphImage.isNull());
                QPainter painter(&cacheGraphImage);
                painter.setPen(QPen(QColor(125, 125, 125, 255)));
                painter.drawPolygon(points, 4);
                painter.end();
                }*/
}
