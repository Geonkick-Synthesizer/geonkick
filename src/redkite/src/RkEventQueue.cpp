/**
 * File name: RkEventQueue.cpp
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

#include "RkEventQueueImpl.h"

#include <RkAction.h>

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

void RkEventQueue::postEvent(RkObject *obj, std::unique_ptr<RkEvent> event)
{
        o_ptr->postEvent(obj, std::move(event));
}

void RkEventQueue::postAction(std::unique_ptr<RkAction> act)
{
        o_ptr->postAction(std::move(act));
}

void RkEventQueue::processQueue()
{
        o_ptr->processQueue();
}
