/**
 * File name: RkEventQueueImpl.cpp
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

#include "RkSystemWindow.h"
#include "RkWidgetImpl.h"
#include "RkEventQueueImpl.h"
#include "RkTimer.h"
#include "RkAction.h"

#ifdef RK_OS_WIN
#include "RkEventQueueWin.h"
#elif RK_OS_MAC
#include "RkEventQueueMac.h"
#else // X11
#include "RkEventQueueX.h"
#endif


RkEventQueue::RkEventQueueImpl::RkEventQueueImpl(RkEventQueue* queueInterface)
        : inf_ptr{queueInterface}
        , systemWindow{nullptr}
#ifdef RK_OS_WIN
        , platformEventQueue{std::make_unique<RkEventQueueWin>()}
#elif RK_OS_MAC
        , platformEventQueue{std::make_unique<RkEventQueueMac>()}
#else // X11
        , platformEventQueue{std::make_unique<RkEventQueueX>()}
#endif
{
        RK_LOG_DEBUG("called");
}

RkEventQueue::RkEventQueueImpl::~RkEventQueueImpl()
{
        RK_LOG_DEBUG("called");
}

RkSystemWindow* RkEventQueue::RkEventQueueImpl::setTopWidget(RkWidget *widget,
                                                  const RkNativeWindowInfo* parent)
{
        if (!systemWindow) {
                systemWindow = std::make_unique<RkSystemWindow>(widget, parent);
#ifdef RK_OS_WIN
#elif RK_OS_MAC
#else
                platformEventQueue->setDisplay(systemWindow->nativeWindowInfo()->display);
#endif
                addObject(widget);
                RK_IMPL_PTR(widget)->setSystemWindow(systemWindow.get());
        }
        return systemWindow.get();
}

RkSystemWindow* RkEventQueue::RkEventQueueImpl::getSystemWindow() const
{
        if (systemWindow)
                return systemWindow.get();
        return nullptr;
}

bool RkEventQueue::RkEventQueueImpl::objectExists(RkObject *obj) const
{
        return objectsList.find(obj) != objectsList.end();
}

void RkEventQueue::RkEventQueueImpl::addObject(RkObject *obj)
{
 	if (!obj || objectExists(obj))
 		return;
				
        objectsList.insert(obj);
        if (!obj->eventQueue())
                obj->setEventQueue(inf_ptr);
}

void RkEventQueue::RkEventQueueImpl::addShortcut(RkObject *obj,
                                                 Rk::Key key,
                                                 Rk::KeyModifiers modifier)
{
        auto res = shortcutsList.find(static_cast<int>(key));
        if (res != shortcutsList.end()) {
                RK_LOG_DEBUG("obj: " << obj << " key: " << static_cast<int>(key)
                             << ", modifier: " << static_cast<int>(modifier)
                             << "  hashKey: " << static_cast<int>(key));
                res->second->addObject(obj);
        } else {
                RK_LOG_DEBUG("new: obj : " << obj << " key: " << static_cast<int>(key)
                             << ", modifier: " << static_cast<int>(modifier)
                             << "  hashKey: " << static_cast<int>(key));
                auto shortcut = std::make_unique<RkShortcut>(key, modifier);
                shortcut->addObject(obj);
                shortcutsList.insert({static_cast<int>(key), std::move(shortcut)});
        }
}

void RkEventQueue::RkEventQueueImpl::removeShortcut(RkObject *obj,
                                                    Rk::Key key,
                                                    Rk::KeyModifiers modifier)
{
        auto res = shortcutsList.find(static_cast<int>(key));
        if (res != shortcutsList.end()) {
                res->second->removeObject(obj);
                if (res->second->objects().empty())
                        shortcutsList.erase(static_cast<int>(key));
        }
}

void RkEventQueue::RkEventQueueImpl::removeObject(RkObject *obj)
{
        if (objectsList.find(obj) != objectsList.end()) {
                objectsList.erase(obj);
                removeObjectShortcuts(obj);
        }
}

void RkEventQueue::RkEventQueueImpl::removeObjectShortcuts(RkObject *obj)
{
        for (auto &shortcut : shortcutsList) {
                if (shortcut.second->hasObject(obj))
                        shortcut.second->removeObject(obj);
        }
}

void RkEventQueue::RkEventQueueImpl::postEvent(RkObject *obj, std::unique_ptr<RkEvent> event)
{
        if (obj && event && objectExists(obj)) {
                std::lock_guard<std::mutex> lock(eventsQueueMutex);
                eventsQueue.push_back({obj, std::move(event)});
        }
}

void RkEventQueue::RkEventQueueImpl::processSystemEvent(std::unique_ptr<RkEvent> event)
{
        if (systemWindow)
                systemWindow->event(event.get());
}

void RkEventQueue::RkEventQueueImpl::processEvents()
{
        if (systemWindow) {
                auto systemEvents = platformEventQueue->getEvents();
                for (auto &event: systemEvents) {
                        auto widgetEvents = systemWindow->processEvent(event.get());
                        for (auto &e: widgetEvents) {
                                if (e.first && e.first->isShown() && e.second)
                                        postEvent(e.first, std::move(e.second));
                        }
                }
        }
        /**
         * Move events in a separeted queue for processing
         * because during the processing the execution of some events
         * may add new events into the queue and this for
         * in some cases can lead to a infinite loop.
         */
        decltype(eventsQueue) queue;
        {
                std::lock_guard<std::mutex> lock(eventsQueueMutex);
                queue = std::move(eventsQueue);
        }

        bool repaintSystemWindow = false;
        for (const auto &e: queue) {
                if (e.first) {
                        RK_IMPL_PTR(e.first)->event(e.second.get());
                        if (e.second.get()->type() == RkEvent::Type::Paint)
                                repaintSystemWindow = true;
                }
        }

        if (repaintSystemWindow)
                systemWindow->update();
}

void RkEventQueue::RkEventQueueImpl::processPopups(RkWidget *widget, RkEvent* event)
{
        /*        if (event->type() == RkEvent::Type::MouseButtonPress) {
                for (auto it = popupList.begin(); it != popupList.end();) {
                        auto w = static_cast<RkWidget*>((*it).second);
                        if (widget != w && !w->isChild(widget)) {
                                RK_LOG_DEBUG("w->close()");
                                w->close();
                                it = popupList.erase(it);
                        } else {
                                ++it;
                        }
                }
                }*/
}

void RkEventQueue::RkEventQueueImpl::processShortcuts(RkKeyEvent *event)
{
        /*        if (!event) {
                RK_LOG_ERROR("wrong arguments");
                return;
        }

        if (static_cast<RkKeyEvent*>(event)->isShortcut())
                return;

        auto hashKey = static_cast<int>(event->key());
        RK_LOG_DEBUG("key: " << static_cast<int>(event->key())
                     << ", modifier: " << static_cast<int>(event->modifiers())
                     << "  hashKey: " << hashKey);

        auto res = shortcutsList.find(hashKey);
        if (res != shortcutsList.end()) {
                for (RkObject* obj : res->second->objects()) {
                        auto shurtcutEvent = std::make_unique<RkKeyEvent>(event->type());
                        shurtcutEvent->setKey(event->key());
                        shurtcutEvent->setModifiers(event->modifiers());
                        shurtcutEvent->setShortcut();
                        auto pair = std::make_pair(obj, std::move(shurtcutEvent));
                        eventsQueue.push_back(std::move(pair));
                }
        } else {
                RK_LOG_DEBUG("can't find shortcut");
                }*/
}

void RkEventQueue::RkEventQueueImpl::postAction(std::unique_ptr<RkAction> act)
{
        std::lock_guard<std::mutex> lock(actionsQueueMutex);
        actionsQueue.push_back(std::move(act));
}

void RkEventQueue::RkEventQueueImpl::subscribeTimer(RkTimer *timer)
{
        timersList.insert(timer);
}

void RkEventQueue::RkEventQueueImpl::unsubscribeTimer(RkTimer *timer)
{
        if (timersList.find(timer) != timersList.end())
                timersList.erase(timer);
}

void RkEventQueue::RkEventQueueImpl::processTimers()
{
        for (const auto &timer: timersList) {
                if (timer->started() && timer->isTimeout())
                        timer->callTimeout();
        }
}

void RkEventQueue::RkEventQueueImpl::processActions()
{
        decltype(actionsQueue) q;
        {
                std::lock_guard<std::mutex> lock(actionsQueueMutex);
                /**
                 * Moving actions in a separeted queue for processing
                 * because during the processing the execution of some actions
                 * may add new actions into the queue and this for
                 * in some cases can lead to a infinite looping.
                 */
                q = std::move(actionsQueue);
        }

        for (const auto &act: q) {
                // Do not process actions for objects that were removed from the event queue.
                if (!act->object() || objectExists(act->object())) {
                        act->call();
                }
        }
}

void RkEventQueue::RkEventQueueImpl::processQueue()
{
        // The order is important.
        processTimers();
        processActions();
        processEvents();
}

/*RkObject* RkEventQueue::RkEventQueueImpl::findObjectByName(const std::string &name) const
{
        // TODO: use less complexity O(1) with hashtable.
        for (auto it = objectsList.cbegin(); it != objectsList.cend(); ++it) {
                if ((*it)->name() == name)
                        return *it;
        }
        return nullptr;
        }*/
