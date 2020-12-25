/**
 * File name: kick_graph.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include <RkEventQueue.h>

KickGraph::KickGraph(RkObject *parent, GeonkickApi *api, const RkSize &size)
        : RkObject(parent)
        , geonkickApi{api}
        , graphThread{nullptr}
        , graphSize{size}
        , isRunning{true}
        , updateGraph{true}
{
        RK_ACT_BIND(geonkickApi, kickUpdated, RK_ACT_ARGS(), this, updateGraphBuffer());
}

KickGraph::~KickGraph()
{
        isRunning = false;
        threadConditionVar.notify_one();
        graphThread->join();
}

void KickGraph::start()
{
       graphThread = std::make_unique<std::thread>(&KickGraph::drawKickGraph, this);
}

void KickGraph::updateGraphBuffer()
{
        if (!graphThread)
                start();
        std::unique_lock<std::mutex> lock(graphMutex);
        kickBuffer = geonkickApi->getKickBuffer();
        updateGraph = true;
        if (kickBuffer.empty())
                geonkickApi->triggerSynthesis();
        threadConditionVar.notify_one();
}

void KickGraph::drawKickGraph()
{
        while (isRunning) {
                // Ignore too many updates. The last udpate will be processed.
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
                std::unique_lock<std::mutex> lock(graphMutex);
                if (!updateGraph)
                        threadConditionVar.wait(lock);
                if (!isRunning)
                        break;

                if (kickBuffer.empty()) {
                        updateGraph = false;
                        continue;
                }

                auto graphImage = std::make_shared<RkImage>(graphSize.width(), graphSize.height());
                RkPainter painter(graphImage.get());
                RkPen pen(RkColor(59, 130, 4, 255));
                painter.setPen(pen);

                std::vector<RkPoint> graphPoints(kickBuffer.size());
                gkick_real k = static_cast<gkick_real>(graphSize.width()) / kickBuffer.size();

                /**
                 * In this loop there is an implementation of an
                 * antialiasing algorithm that reduces in most of
                 * the cases antialiasing, and at the same time
                 * reduces and normalizes the size of the buffer.
                 */
                int j = 0;
                RkPoint prev;
                for (decltype(kickBuffer.size()) i = 0; i < kickBuffer.size(); i++) {
                        int x = k * i;
                        int y = graphSize.height() * 0.5 * (1 - kickBuffer[i]);
                        RkPoint p(k * i, graphSize.height() * 0.5 * (1 - kickBuffer[i]));
                        if (p == prev)
                                continue;
                        else
                                prev = p;
                        graphPoints[j++] = p;

                        int i0 = i;
                        int ymin, ymax;
                        ymin = ymax = y;
                        while (++i < kickBuffer.size()) {
                                if (x != static_cast<int>(k * i))
                                        break;
                                y = graphSize.height() * 0.5 * (1 - kickBuffer[i]);
                                if (ymin > y)
                                        ymin = y;
                                if (ymax < y)
                                        ymax = y;
                        }

                        if (i - i0 > 4) {
                                graphPoints[j++] = {x, ymin};
                                graphPoints[j++] = {x, ymax};
                                graphPoints[j++] = {x, y};
                        }
                }
                graphPoints.resize(j);
                painter.drawPolyline(graphPoints);
                if (eventQueue()) {
                        auto act = std::make_unique<RkAction>(this);
                        act->setCallback([this, graphImage](void){ graphUpdated(graphImage); });
                        graphImage.reset();
                        eventQueue()->postAction(std::move(act));
                }
                updateGraph = false;
        }
}
