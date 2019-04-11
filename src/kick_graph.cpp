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

KickGraph::KickGraph(GeonkickApi *api)
        : geonkickApi{api}
        , kickBuffer{48000 * geonkickApi->kickLength() / 1000}
{
        RK_ACT_BIND(api, kickUpdated, RK_ACT_ARGS(), this, updateGraphBuffer());
        for (auto i = 0; i < 100; i++)
                kickBuffer.push_back(1000 * (0.5 - sin(i)));
}

KickGraph::~KickGraph()
{
}

void KickGraph::draw(RkPainter &painter)
{
                if (!cacheGraphImage.isNull())
                        painter.drawImage(cacheGraphImage, drawingArea.topLeft().x(), drawingArea.topLeft().y());
}

void KickGraph::setDrawingArea(const RkRect &rect)
{
        RK_LOG_INFO("called");
        drawingArea = rect;
        {
                RkImage im(drawingArea.size());
                cacheGraphImage = im;
        }
        //   cacheGraphImage.fill(RkColor(0, 0, 0, 0));
        //geonkickApi->getKickBuffer(kickBuffer);
        //        drawKickGraph();
}

void KickGraph::updateGraphBuffer()
{
        auto len = 48000 * geonkickApi->kickLength() / 1000;
        if (kickBuffer.size() != len) {
                RK_LOG_INFO("called");
                kickBuffer.resize(len);
        }
        geonkickApi->getKickBuffer(kickBuffer);
        drawKickGraph();
}

void KickGraph::drawKickGraph()
{
        if (kickBuffer.empty())
                return;

        cacheGraphImage.fill(RkColor(0, 0, 0, 0));
        RkPainter painter(&cacheGraphImage);
        //        painter.fillRect(RkRect(0, 0, cacheGraphImage.width(), cacheGraphImage.height()), RkColor(0, 0, 0, 0));
        RkPen pen(RkColor(59, 130, 4, 255));
        //        pen.setJoinStyle(Qt::MiterJoin);
        painter.setPen(pen);
        int w = drawingArea.width();
        int h = drawingArea.height();
        //        painter.setRenderHints(QPainter::Antialiasing, true);

        std::vector<RkPoint> graphPoints(kickBuffer.size());
        gkick_real k = static_cast<gkick_real>(w) / kickBuffer.size();
        for (decltype(kickBuffer.size()) i = 0; i < kickBuffer.size(); i++) {
                graphPoints[i].setX(k * i);
                graphPoints[i].setY(h * (0.5 - kickBuffer[i]));
        }
        painter.drawPolyline(graphPoints);
                /*                emit graphUpdated();*/
}
