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
        if (!cacheGraphImage.isNull()) {
                painter.drawPixmap(drawingArea, cacheGraphImage);
        }
}

void KickGraph::setDrawingArea(const QRect &rect)
{
        drawingArea = rect;
        cacheGraphImage = QPixmap(drawingArea.size());
        cacheGraphImage.fill(Qt::transparent);
        kickBuffer.resize(drawingArea.width());
        geonkickApi->getKickBuffer(kickBuffer);
        drawKickGraph();
}

void KickGraph::updateGraphBuffer()
{
        geonkickApi->getKickBuffer(kickBuffer);
        drawKickGraph();
}

void KickGraph::drawKickGraph()
{
        if (kickBuffer.empty()) {
                return;
        }

        cacheGraphImage.fill(Qt::transparent);
        QPainter painter(&cacheGraphImage);
        QPen pen(QColor(59, 130, 4, 200));
        pen.setWidth(2);
        pen.setJoinStyle(Qt::MiterJoin);
        painter.setPen(pen);

        int w = drawingArea.width();
        int h = drawingArea.height();
        int k = kickBuffer.size() / w;
        QPolygonF graphPoints;
        for (auto i = 0; i < w; i++) {
                graphPoints << QPointF(i, h * (0.5  - kickBuffer[i * k]));
        }

        painter.setRenderHints(QPainter::SmoothPixmapTransform
                               | QPainter::Antialiasing, true);
        painter.drawPolyline(graphPoints);
        painter.end();
}
