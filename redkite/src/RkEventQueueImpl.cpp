/**
 * File name: RkEventQueueImpl.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "RkWidget.h"
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


RkEventQueue::RkEventQueueImpl::RkEventQueueImpl(RkEventQueue* interface)
        : inf_ptr{interface}
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

bool RkEventQueue::RkEventQueueImpl::objectExists(RkObject *obj) const
{
        return objectsList.find(obj) != objectsList.end();
}

void RkEventQueue::RkEventQueueImpl::addObject(RkObject *obj)
{
        RK_LOG_DEBUG("add object: " << obj);
 	if (!obj || objectExists(obj))
 		return;

        if (obj->type() == Rk::ObjectType::Widget) {
                RK_LOG_DEBUG("obj " << obj << " is widget");
                auto widgetImpl = dynamic_cast<RkWidget::RkWidgetImpl*>(obj->o_ptr.get());
                if (!widgetImpl) {
                        RK_LOG_ERROR("can't cast o_ptr to RkWidgetImpl");
                        return;
                }
 #if !defined(RK_OS_WIN) && !defined(RK_OS_MAC)
         // Set the display from the top window.
                if (!widgetImpl->parent() && !platformEventQueue->display()) {
                        RK_LOG_DEBUG("widget " << obj << " is top window");
                        platformEventQueue->setDisplay(widgetImpl->nativeWindowInfo()->display);
                }
 #else
 #error platform not implemented
 #endif

                RK_LOG_DEBUG("add widget window id");
                auto id = widgetImpl->nativeWindowInfo()->window;
                windowIdsMap.insert({id, obj});
                if (static_cast<int>(widgetImpl->windowFlags())
                    & static_cast<int>(Rk::WindowFlags::Popup)) {
                        popupList.insert({id, obj});
                        RK_LOG_DEBUG("poup added: " << obj);
                }
        }

        objectsList.insert(obj);
        if (!obj->eventQueue())
                obj->setEventQueue(inf_ptr);
}

void RkEventQueue::RkEventQueueImpl::addShortcut(RkObject *obj,
                                                 Rk::Key key,
                                                 Rk::KeyModifiers modifier)
{
        unsigned long long int hashKey = ((static_cast<unsigned long long int>(key) << 4)
                                          | static_cast<unsigned long long int>(modifier));
        auto res = shortcutsList.find(hashKey);
        if (res != shortcutsList.end()) {
                RK_LOG_DEBUG("obj: " << obj << " key: " << static_cast<int>(key)
                             << ", modifier: " << static_cast<int>(modifier)
                             << "  hashKey: " << hashKey);
                res->second->addObject(obj);
        } else {
                RK_LOG_DEBUG("new: obj : " << obj << " key: " << static_cast<int>(key)
                             << ", modifier: " << static_cast<int>(modifier)
                             << "  hashKey: " << hashKey);
                auto shortcut = std::make_unique<RkShortcut>(key, modifier);
                shortcut->addObject(obj);
                shortcutsList.insert({hashKey, std::move(shortcut)});
        }
}

void RkEventQueue::RkEventQueueImpl::removeShortcut(RkObject *obj,
                                                    Rk::Key key,
                                                    Rk::KeyModifiers modifier)
{
        unsigned long long int hashKey = ((static_cast<unsigned long long int>(key) << 4)
                                          | static_cast<unsigned long long int>(modifier));
        auto res = shortcutsList.find(hashKey);
        if (res != shortcutsList.end()) {
                res->second->removeObject(obj);
                if (res->second->objects().empty())
                        shortcutsList.erase(hashKey);
        }
}

void RkEventQueue::RkEventQueueImpl::removeObject(RkObject *obj)
{
        if (objectsList.find(obj) != objectsList.end()) {
                objectsList.erase(obj);
                removeObjectShortcuts(obj);
                if (obj->type() == Rk::ObjectType::Widget) {
                        auto widgetImpl = dynamic_cast<RkWidget::RkWidgetImpl*>(obj->o_ptr.get());
                        if (!widgetImpl) {
                                RK_LOG_ERROR("can't cast o_ptr to RkWidgetImpl");
                                return;
                        }
                        auto id = widgetImpl->nativeWindowInfo()->window;
                        if (windowIdsMap.find(id) != windowIdsMap.end()) {
                                RK_LOG_DEBUG("widget id removed from queue");
                                windowIdsMap.erase(id);
                                if (popupList.find(id) != popupList.end())
                                        popupList.erase(id);
                        }
                }
        }
}

void RkEventQueue::RkEventQueueImpl::removeObjectShortcuts(RkObject *obj)
{
        for (auto &shortcut : shortcutsList) {
                if (shortcut.second->hasObject(obj))
                        shortcut.second->removeObject(obj);
        }
}

RkWidget* RkEventQueue::RkEventQueueImpl::findWidget(const RkWindowId &id) const
{
        auto it = windowIdsMap.find(id.id);
        if (it != windowIdsMap.end()) {
                if (it->second->type() == Rk::ObjectType::Widget) {
                        auto widget = dynamic_cast<RkWidget*>(it->second);
                        if (!widget) {
                                RK_LOG_ERROR("can't cast RkObject[" << it->second << "] to RkWidget");
                                return nullptr;
                        }
                        return widget;
                }
        }

        return nullptr;
}

void RkEventQueue::RkEventQueueImpl::postEvent(RkObject *obj, std::unique_ptr<RkEvent> event)
{
        eventsQueue.push_back({obj, std::move(event)});
}

void RkEventQueue::RkEventQueueImpl::processEvent(RkObject *obj, RkEvent *event)
{
        // Do not process events for objects that were removed from the event queue.
        if (objectExists(obj))
                obj->event(event);
}

void RkEventQueue::RkEventQueueImpl::processEvents()
{
        auto events = platformEventQueue->getEvents();
        if (!events.empty()) {
                for (auto &event: events) {
                        auto widget = findWidget(event.first);
                        if (widget) {
                                auto pair = std::make_pair<RkObject*,
                                            std::unique_ptr<RkEvent>>(widget, std::move(event.second));
                                eventsQueue.push_back(std::move(pair));
                        }
                }
        }
        events.clear();

        /**
         * Moving events in a separeted queue for processing
         * because during the processing the execution of some events
         * may add new events into the queue and this for
         * in some cases can lead to a infinite looping.
         */
        decltype(eventsQueue) queue = std::move(eventsQueue);
        for (const auto &e: queue) {
                if (e.second->type() == RkEvent::Type::KeyPressed)
                        processShortcuts(dynamic_cast<RkKeyEvent*>(e.second.get()), e.first);
                if (!popupList.empty() && dynamic_cast<RkWidget*>(e.first))
                        processPopups(dynamic_cast<RkWidget*>(e.first), e.second.get());
                processEvent(e.first, e.second.get());
        }
}

void RkEventQueue::RkEventQueueImpl::processPopups(RkWidget *widget, RkEvent* event)
{
        if (event->type() == RkEvent::Type::MouseButtonPress) {
                for (auto it = popupList.begin(); it != popupList.end();) {
                        auto w = static_cast<RkWidget*>((*it).second);
                        if (widget != w && !w->isChild(widget)) {
                                w->close();
                                it = popupList.erase(it);
                        } else {
                                ++it;
                        }
                }
        }
}

bool RkEventQueue::RkEventQueueImpl::isTopWidget(RkObject *obj) const
{
        if (objectExists(obj)) {
                auto widget = dynamic_cast<RkWidget*>(obj);
                if (widget && widget->getTopWidget() == widget)
                        return true;
        }
        return false;
}

void RkEventQueue::RkEventQueueImpl::processShortcuts(RkKeyEvent *event, RkObject *excludedObj)
{
        if (!event || !excludedObj) {
                RK_LOG_ERROR("wrong arguments");
                return;
        }

        unsigned long long int hashKey = ((static_cast<unsigned long long int>(event->key()) << 4)
                                          | static_cast<unsigned long long int>(event->modifiers()));

        RK_LOG_DEBUG("key: " << static_cast<int>(event->key())
                     << ", modifier: " << static_cast<int>(event->modifiers())
                     << "  hashKey: " << hashKey);

        auto res = shortcutsList.find(hashKey);
        if (res != shortcutsList.end()) {
                for (const auto &obj : res->second->objects()) {
                        if (excludedObj != obj)
                                processEvent(obj, event);
                }
        } else {
                RK_LOG_DEBUG("can't find shortcut");
        }
}

void RkEventQueue::RkEventQueueImpl::postAction(std::unique_ptr<RkAction> act)
{
        std::lock_guard<std::mutex> lock(actionsQueueMutex);
        actionsQueue.push_back(std::move(act));
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
                if (!act->object() || objectExists(act->object()))
                        act->call();
        }
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

void RkEventQueue::RkEventQueueImpl::clearEvents(const RkObject *obj)
{
        if (!obj)
                return;
        RK_LOG_DEBUG("clear object " << obj << " events");
        eventsQueue.erase(std::remove_if(eventsQueue.begin(),
                                         eventsQueue.end(),
                                         [obj](std::pair<RkObject*, std::unique_ptr<RkEvent>> &ev) {
                                                 if (ev.first == obj) {
                                                         RK_LOG_DEBUG("clear: [obj: " << obj << "] ev: "
                                                                      << ev.second.get());
                                                         return true;
                                                 }
                                                 return false;
                                         })
                          , eventsQueue.end());
}

void RkEventQueue::RkEventQueueImpl::clearActions(const RkObject *obj)
{
        if (!obj)
                return;

        std::lock_guard<std::mutex> lock(actionsQueueMutex);
        actionsQueue.erase(std::remove_if(actionsQueue.begin(), actionsQueue.end(),
                                          [obj](const std::unique_ptr<RkAction> &act)
                                          {
                                                  if (act->object() && act->object() == obj) {
                                                          RK_LOG_DEBUG("clear: [obj: " << obj << "] act: "
                                                                       << act.get());
                                                          return true;

                                                  }
                                                  return false;
                                          })
                           , actionsQueue.end());
}

RkObject* RkEventQueue::RkEventQueueImpl::findObjectByName(const std::string &name) const
{
        // TODO: use less complexity O(1) with hashtable.
        for (auto it = objectsList.cbegin(); it != objectsList.cend(); ++it) {
                if ((*it)->name() == name)
                        return *it;
        }
        return nullptr;
}

void RkEventQueue::RkEventQueueImpl::setScaleFactor(double factor)
{
        platformEventQueue->setScaleFactor(factor);
}
