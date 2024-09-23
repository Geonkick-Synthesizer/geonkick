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
#include "envelope.h"

#include <RkEventQueue.h>
#include <RkAction.h>

KickGraph::KickGraph(RkObject *parent, GeonkickApi *api, const RkSize &size)
        : RkObject(parent)
        , geonkickApi{api}
        , graphThread{nullptr}
        , graphSize{size}
        , isRunning{true}
        , redrawGraph{true}
        , currentEnvelope{nullptr}
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

void KickGraph::setEnvelope(Envelope * envelope)
{
        std::unique_lock<std::mutex> lock(graphMutex);
        currentEnvelope = envelope;
        updateGraph(false);
}

Envelope* KickGraph::getEnvelope() const
{
        std::unique_lock<std::mutex> lock(graphMutex);
        return currentEnvelope;
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
        {
                std::unique_lock<std::mutex> lock(graphMutex);
                kickBuffer = geonkickApi->getKickBuffer();
                if (kickBuffer.empty())
                        geonkickApi->triggerSynthesis();
                updateGraph(false);
        }
        threadConditionVar.notify_one();
}

void KickGraph::drawKickGraph()
{
        while (isRunning) {
                // Ignore too many updates. The last update will be processed.
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
                std::unique_lock<std::mutex> lock(graphMutex);
                if (!redrawGraph)
                        threadConditionVar.wait(lock);

                if (!isRunning)
                        break;
                if (!currentEnvelope || kickBuffer.empty()) {
                        redrawGraph = false;
                        continue;
                }
                const auto zoomFactor = currentEnvelope->getZoom();
                const auto timeOrigin = currentEnvelope->getTimeOrigin();
                // const auto envelopeAmplitude = currentEnvelope->envelopeAmplitude();
                // const auto valueOrigin = currentEnvelope->getValueOrigin() / envelopeAmplitude;
                auto graphImage = std::make_shared<RkImage>(graphSize.width(), graphSize.height());
                RkPainter painter(graphImage.get());
                RkPen pen(RkColor(59, 130, 4, 255));
                painter.setPen(pen);
                std::vector<RkRealPoint> graphPoints;
                const auto instrumentBuffer = kickBuffer;
                graphPoints.reserve(instrumentBuffer.size());
                const auto buffSize = static_cast<double>(instrumentBuffer.size()) / zoomFactor;
                const gkick_real k = static_cast<gkick_real>(graphSize.width()) / buffSize;
                const size_t indexOffset = (instrumentBuffer.size() / geonkickApi->kickLength()) * timeOrigin;
                const auto instrumentGraphSize = graphSize;
                redrawGraph = false;
                lock.unlock();

                /**
                 * In this loop there is an implementation of an
                 * antialiasing algorithm that reduces in most of
                 * the cases antialiasing, and at the same time
                 * reduces and normalizes the size of the buffer.
                 */
                RkRealPoint prev;
                painter.translate({0, instrumentGraphSize.height()});
                for (decltype(instrumentBuffer.size()) i = indexOffset; i < instrumentBuffer.size(); i++) {
                        const double x = k * (i - indexOffset);
                        // TODO:
                        // const double value = -zoomFactor * (instrumentGraphSize.height() / 2
                        //                               + instrumentGraphSize.height() * (instrumentBuffer[i] / 2
                        //                               - valueOrigin));
                        const double value = -(instrumentGraphSize.height() / 2
                                               + instrumentGraphSize.height() * (instrumentBuffer[i] / 2));
                        double y = value;
                        RkRealPoint p(k * (i - indexOffset), value);
                        if (p == prev)
                                continue;
                        else
                                prev = p;
                        graphPoints.push_back(p);

                        const int i0 = i;
                        double ymin, ymax;
                        ymin = ymax = y;
                        while (++i < instrumentBuffer.size()) {
                                if (x != k * (i - indexOffset))
                                        break;
                                y = instrumentGraphSize.height() - value;
                                ymin = std::min(ymin, y);
                                ymax = std::min(ymax, y);
                        }

                        if (i - i0 > 4) {
                                graphPoints.emplace_back(RkRealPoint(x, ymin));
                                graphPoints.emplace_back(RkRealPoint(x, ymax));
                                graphPoints.emplace_back(RkRealPoint(x, y));
                        }
                }
                graphPoints.shrink_to_fit();
                painter.drawPolyline(graphPoints);
                if (eventQueue()) {
                        auto act = std::make_unique<RkAction>(this);
                        act->setCallback([this, graphImage](void){ graphUpdated(graphImage); });
                        graphImage.reset();
                        eventQueue()->postAction(std::move(act));
                }
        }
}
