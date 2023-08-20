
/**
 * File name: RkEventQueueImpl.h
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

#ifndef RK_EVENT_QUEUE_IMPL_H
#define RK_EVENT_QUEUE_IMPL_H

#include "RkEventQueue.h"
#include "RkPlatform.h"
#include "RkEvent.h"
#include "RkShortcut.h"

#ifdef RK_OS_WIN
        class RkEventQueueWin;
#elif RK_OS_MAC
        class RkEventQueueMac;
#else // X11
        class RkEventQueueX;
#endif

class RkEventQueue::RkEventQueueImpl {
 public:
        explicit RkEventQueueImpl(RkEventQueue* interface);
        virtual ~RkEventQueueImpl();

        bool objectExists(RkObject *t) const;
        void addObject(RkObject *obj);
        void addShortcut(RkObject *obj,
                         Rk::Key key,
                         Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeShortcut(RkObject *obj,
                            Rk::Key key,
                            Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeObject(RkObject *obj);
        RkWidget* findWidget(const RkWindowId &id) const;
        void removeObjEvents(RkObject *obj);
        void postEvent(RkObject *obj, std::unique_ptr<RkEvent> event);
        void processEvent(RkObject *obj, RkEvent *event);
        void processEvents();
        void postAction(std::unique_ptr<RkAction> act);
        void processActions();
        void subscribeTimer(RkTimer *timer);
        void unsubscribeTimer(RkTimer *timer);
        void processTimers();
        void clearEvents(const RkObject *obj);
        void clearActions(const RkObject *obj);
        RkObject* findObjectByName(const std::string &name) const;
        void setScaleFactor(double factor);

 protected:
        void processShortcuts(RkKeyEvent *event, RkObject *excludedObj);
        void processPopups(RkWidget *widget, RkEvent* event);
        void removeObjectShortcuts(RkObject *obj);
        bool isTopWidget(RkObject *obj) const;

 private:
        RK_DECALRE_INTERFACE_PTR(RkEventQueue);
        RK_DISABLE_COPY(RkEventQueueImpl);
        RK_DISABLE_MOVE(RkEventQueueImpl);
        std::unordered_set<RkObject*> objectsList;
        std::unordered_map<unsigned long long int, RkObject*> windowIdsMap;
        std::unordered_map<unsigned long long int, std::unique_ptr<RkShortcut>> shortcutsList;
        std::vector<std::pair<RkObject*, std::unique_ptr<RkEvent>>> eventsQueue;
        std::vector<std::unique_ptr<RkAction>> actionsQueue;
        std::unordered_set<RkTimer*> timersList;
        std::unordered_map<unsigned long long int, RkObject*> popupList;
        std::mutex actionsQueueMutex;

#ifdef RK_OS_WIN
        std::unique_ptr<RkEventQueueWin> platformEventQueue;
#elif RK_OS_MAC
        std::unique_ptr<RkEventQueueMac> platformEventQueue;
#else // X11
        std::unique_ptr<RkEventQueueX> platformEventQueue;
#endif
};

#endif // RK_EVENT_QUEUE_IMPL_H
