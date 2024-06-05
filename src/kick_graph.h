/**
 * File name: kick_graph.h
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

#ifndef GEONKICK_GRAPTH_H
#define GEONKICK_GRAPTH_H

#include "geonkick_api.h"

#include <RkPainter.h>

#include <condition_variable>

class RkEventQueue;
class Envelope;

class KickGraph : public RkObject {

public:

     KickGraph(RkObject* parent, GeonkickApi *api, const RkSize &size);
     ~KickGraph();
     void start();
     RK_DECL_ACT(graphUpdated,
                 graphUpdated(std::shared_ptr<RkImage> graphImage),
                 RK_ARG_TYPE(std::shared_ptr<RkImage>),
                 RK_ARG_VAL(graphImage));
     void setGraphLegnth(double val);
     void setEnvelope(Envelope * envelope);
     Envelope* getEnvelope() const;
     void updateGraph(bool lock = true);
     void updateGraphBuffer();

protected:
     void drawKickGraph();

private:
     GeonkickApi *geonkickApi;
     std::unique_ptr<std::thread> graphThread;
     mutable std::mutex  graphMutex;
     std::condition_variable threadConditionVar;
     std::vector<gkick_real> kickBuffer;
     RkSize graphSize;
     std::atomic<bool> isRunning;
     bool redrawGraph;
     Envelope *currentEnvelope;
};

#endif // GEONKICK_GRAPH
