/**
 * File name: RkTimer.cpp
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

#include "RkTimer.h"
#include "RkEventQueue.h"

#include <chrono>

RkTimer::RkTimer(RkObject *parent, long unsigned int interval)
        : RkObject(parent)
        , timerInterval{interval}
        , timerStarted{false}
        , lastTime{0}
{
        if (eventQueue())
                eventQueue()->subscribeTimer(this);
}

RkTimer::~RkTimer()
{
        if (eventQueue())
                eventQueue()->unsubscribeTimer(this);
}

void RkTimer::start()
{
        timerStarted = true;
        lastTime = getCurrentTime();
}

bool RkTimer::started() const
{
        return timerStarted;
}

void RkTimer::stop()
{
        timerStarted = false;
}

long long unsigned int RkTimer::interval() const
{
        return timerInterval;
}

void RkTimer::setInterval(int val)
{
        timerInterval = val;
}

bool RkTimer::isTimeout() const
{
        if (!timerStarted || timerInterval < 0)
                return false;

        if (getCurrentTime() - lastTime > timerInterval) {
                return true;
        }
        return false;
}

long long unsigned int RkTimer::getCurrentTime() const
{
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void RkTimer::callTimeout()
{
        timeout();
        lastTime = getCurrentTime();
}
