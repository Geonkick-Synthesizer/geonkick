/**
 * File name: RkQueueWin.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "RkEventQueueWin.h"
#include "RkLog.h"
#include "RkWidget.h"
#include "RkEvent.h"

RkEventQueueWin::RkEventQueueWin()
: scaleFactor{1.0}
{
        RK_LOG_DEBUG("called");
}

RkEventQueueWin::~RkEventQueueWin()
{
}

void RkEventQueueWin::setScaleFactor(double factor)
{
        scaleFactor = factor;
}

double RkEventQueueWin::getScaleFactor() const
{
        return scaleFactor;
}

void RkEventQueueWin::dispatchEvents()
{
        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
        }
}

void RkEventQueueWin::addEvent(std::unique_ptr<RkEvent> event)
{
    eventList.emplace_back(std::move(event));
}

std::vector<std::unique_ptr<RkEvent>> RkEventQueueWin::getEvents()
{
    return std::move(eventList);
}
