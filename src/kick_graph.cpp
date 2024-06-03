/**
 * File name: kick_graph.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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
#include <RkAction.h>

KickGraph::KickGraph(RkObject *parent, GeonkickApi *api, const RkSize &size)
        : RkObject(parent)
        , geonkickApi{api}
        , graphThread{nullptr}
        , graphSize{size}
        , isRunning{true}
        , redrawGraph{true}
        , zoomFactor{1.0}
        , timeOrigin{0.0}
        , valueOrigin{0.0}
{
        RK_ACT_BIND(geonkickApi, kickUpdated, RK_ACT_ARGS(), this, updateGraphBuffer());
        graphThread = std::make_unique<std::thread>(&KickGraph::drawKickGraph, this);
}

KickGraph::~KickGraph()
{
        isRunning = false;
        threadConditionVar.notify_one();
        graphThread->join();
}

void KickGraph::setZoom(double val)
{
        std::unique_lock<std::mutex> lock(graphMutex);
        zoomFactor = val;
        updateGraph(false);
}

double KickGraph::getZoom() const
{
        std::unique_lock<std::mutex> lock(graphMutex);
        return zoomFactor;
}

void KickGraph::setTimeOrigin(double val)
{
        std::unique_lock<std::mutex> lock(graphMutex);
        timeOrigin = val;
        updateGraph(false);
}

double KickGraph::getTimeOrigin() const
{
        std::unique_lock<std::mutex> lock(graphMutex);
        return timeOrigin;
}

void KickGraph::setValueOrigin(double val)
{
        std::unique_lock<std::mutex> lock(graphMutex);
        valueOrigin = val;
        updateGraph(false);
}

double KickGraph::getValueOrigin() const
{
        std::unique_lock<std::mutex> lock(graphMutex);
        return valueOrigin;
}

void KickGraph::updateGraph(bool lock)
{
        if (lock) {
                std::unique_lock<std::mutex> lock(graphMutex);
                redrawGraph = true;
        } else {
                redrawGraph = true;
        }
        threadConditionVar.notify_one();
}

void KickGraph::updateGraphBuffer()
{
        std::unique_lock<std::mutex> lock(graphMutex);
        kickBuffer = geonkickApi->getKickBuffer();
        if (kickBuffer.empty())
                geonkickApi->triggerSynthesis();
        updateGraph(false);
}

void KickGraph::drawKickGraph()
{
        while (isRunning) {
                // Ignore too many updates. The last update will be processed.
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
                std::unique_lock<std::mutex> lock(graphMutex);
                GEONKICK_LOG_INFO("wait for drawing...");
                if (!redrawGraph)
                        threadConditionVar.wait(lock);
                GEONKICK_LOG_INFO("start draw1...");
                if (!isRunning)
                        break;
                GEONKICK_LOG_INFO("start draw2...");
                if (kickBuffer.empty()) {
                        redrawGraph = false;
                        continue;
                }
                GEONKICK_LOG_INFO("start draw3...");
                auto graphImage = std::make_shared<RkImage>(graphSize.width(), graphSize.height());
                RkPainter painter(graphImage.get());
                RkPen pen(RkColor(59, 130, 4, 255));
                painter.setPen(pen);
                std::vector<RkPoint> graphPoints(kickBuffer.size());
                auto buffSize = static_cast<double>(kickBuffer.size()) / zoomFactor;
                gkick_real k = static_cast<gkick_real>(graphSize.width()) / buffSize;
                size_t indexOffset = (kickBuffer.size() / geonkickApi->kickLength()) * timeOrigin;
                GEONKICK_LOG_INFO("indexOffset{GR}: " << indexOffset);

                /**
                 * In this loop there is an implementation of an
                 * antialiasing algorithm that reduces in most of
                 * the cases antialiasing, and at the same time
                 * reduces and normalizes the size of the buffer.
                 */
                int j = 0;
                RkPoint prev;
                for (decltype(kickBuffer.size()) i = indexOffset; i < kickBuffer.size(); i++) {
                        int x = k * (i - indexOffset);
                        int y = graphSize.height() * 0.5 * (1 - kickBuffer[i]);
                        RkPoint p(k * (i - indexOffset), graphSize.height() * 0.5 * (1 - kickBuffer[i]));
                        if (p == prev)
                                continue;
                        else
                                prev = p;
                        graphPoints[j++] = p;

                        int i0 = i;
                        int ymin, ymax;
                        ymin = ymax = y;
                        while (++i < kickBuffer.size()) {
                                if (x != static_cast<int>(k * (i - indexOffset)))
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
                //                for (auto &p: graphPoints)
                //                        p.setY(zoomFactor * p.y());
                painter.drawPolyline(graphPoints);
                if (eventQueue()) {
                        auto act = std::make_unique<RkAction>(this);
                        act->setCallback([this, graphImage](void){ graphUpdated(graphImage); });
                        graphImage.reset();
                        eventQueue()->postAction(std::move(act));
                }
                redrawGraph = false;
        }
}
