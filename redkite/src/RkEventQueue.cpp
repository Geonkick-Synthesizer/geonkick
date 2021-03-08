/**
 * File name: RkEventQueue.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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

#include "RkEventQueue.h"
#include "RkEventQueueImpl.h"

RkEventQueue::RkEventQueue()
        : o_ptr{std::make_unique<RkEventQueueImpl>(this)}
{
}

RkEventQueue::RkEventQueue(std::unique_ptr<RkEventQueueImpl> impl)
        : o_ptr{std::move(impl)}
{
}

RkEventQueue::~RkEventQueue()
{
}

void RkEventQueue::addObject(RkObject *obj)
{
        o_ptr->addObject(obj);
}

void RkEventQueue::addShortcut(RkObject *obj,
                               Rk::Key key,
                               Rk::KeyModifiers modifier)
{
        o_ptr->addShortcut(obj, key, modifier);
}

void RkEventQueue::removeShortcut(RkObject *obj,
                                  Rk::Key key,
                                  Rk::KeyModifiers modifier)
{
        o_ptr->removeShortcut(obj, key, modifier);
}

void RkEventQueue::removeObject(RkObject *obj)
{
        o_ptr->removeObject(obj);
}

void RkEventQueue::postEvent(RkObject *obj, std::unique_ptr<RkEvent> event)
{
        o_ptr->postEvent(obj, std::move(event));
}

void RkEventQueue::postAction(std::unique_ptr<RkAction> act)
{
        o_ptr->postAction(std::move(act));
}

void RkEventQueue::subscribeTimer(RkTimer *timer)
{
        if (timer)
                o_ptr->subscribeTimer(timer);
}

void RkEventQueue::unsubscribeTimer(RkTimer *timer)
{
        if (timer)
                o_ptr->unsubscribeTimer(timer);
}

void RkEventQueue::processEvents()
{
        o_ptr->processEvents();
}

void RkEventQueue::processActions()
{
        o_ptr->processActions();
}

void RkEventQueue::processTimers()
{
        o_ptr->processTimers();
}

void RkEventQueue::processQueue()
{
#ifdef RK_LOG_DEBUG_LEVEL
        static int n = 0;
        if (++n > 10000) {
                RK_LOG_DEBUG("called");
                n = 0;
        }
#endif // RK_LOG_DEBUG_LEVEL
        // The order is important.
        processTimers();
        processActions();
        processEvents();
}

void RkEventQueue::clearObjectEvents(const RkObject *obj)
{
        if (obj)
                o_ptr->clearEvents(obj);
}

void RkEventQueue::clearObjectActions(const RkObject *act)
{
        if (act)
                o_ptr->clearActions(act);
}

void RkEventQueue::clearEvents()
{
        o_ptr->clearEvents(nullptr);
}

void RkEventQueue::clearActions()
{
        o_ptr->clearActions(nullptr);
}

void RkEventQueue::clearQueue()
{
        clearEvents();
        clearActions();
}

RkObject* RkEventQueue::findObjectByName(const std::string &name) const
{
        return o_ptr->findObjectByName(name);
}

void RkEventQueue::setScaleFactor(double factor)
{
        o_ptr->setScaleFactor(factor);
}
