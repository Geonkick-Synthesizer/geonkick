/**
 * File name: RkMainImpl.cpp
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

#include "RkLog.h"
#include "RkWidget.h"
#include "RkMainImpl.h"
#include "RkPlatform.h"
#include "RkEventQueueImpl.h"
#include "RkSystemWindow.h"

#include <chrono>
#include <thread>

RkMain::RkMainImpl::RkMainImpl(RkMain *inf)
        : inf_ptr{inf}
        , eventQueue{std::make_unique<RkEventQueue>()}
{
        RK_UNUSED(inf_ptr);
        RK_LOG_DEBUG("called");
}

RkMain::RkMainImpl::RkMainImpl(RkMain *inf, int argc, char **argv)
        : inf_ptr{inf}
        , eventQueue{std::make_unique<RkEventQueue>()}
{
        RK_UNUSED(inf_ptr);
        RK_UNUSED(argc);
        RK_UNUSED(argv);
        RK_LOG_DEBUG("called");
}

RkMain::RkMainImpl::~RkMainImpl()
{
        RK_LOG_DEBUG("called");
}

RkSystemWindow* RkMain::RkMainImpl::setTopWidget(RkWidget* widget, const RkNativeWindowInfo *parent)
{
        return RK_IMPL_PTR(eventQueue)->setTopWidget(widget, parent);
}

RkWidget* RkMain::RkMainImpl::topWidget() const
{
        if (RK_IMPL_PTR(eventQueue)->getSystemWindow())
                return RK_IMPL_PTR(eventQueue)->getSystemWindow()->getTopWidget();
        return nullptr;
}

RkEventQueue* RkMain::RkMainImpl::getEventQueue() const
{
        return eventQueue.get();
}

int RkMain::RkMainImpl::exec(bool block)
{
        if (!RK_IMPL_PTR(eventQueue)->getSystemWindow()) {
                RK_LOG_ERROR("the system window not defined");
		return 1;
	}

        if (!block) {
                eventQueue->processQueue();
        } else {
                for (; block ;) {
                        eventQueue->processQueue();
                        if (RK_IMPL_PTR(eventQueue)->getSystemWindow()->isClosed()) {
                                RK_LOG_DEBUG("exit");
                                break;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
        }

        return 0;
}
