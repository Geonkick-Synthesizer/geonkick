/**
 * File name: RkEventQueueImpl.h
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

#ifndef RK_EVENT_QUEUE_IMPL_H
#define RK_EVENT_QUEUE_IMPL_H

#include "RkEventQueue.h"
#include "RkPlatform.h"
#include "RkEvent.h"
#include "RkShortcut.h"

class RkWidget;
class RkSystemWindow;
class RkTimer;

#ifdef RK_OS_WIN
        class RkEventQueueWin;
#elif RK_OS_MAC
        class RkEventQueueMac;
#else // X11
        class RkEventQueueX;
#endif

class RkEventQueue::RkEventQueueImpl {
 public:
        explicit RkEventQueueImpl(RkEventQueue* queueInterface);
        virtual ~RkEventQueueImpl();
        RkSystemWindow* setTopWidget(RkWidget *widget, const RkNativeWindowInfo *parent = nullptr);
        RkSystemWindow* getSystemWindow() const;
        bool objectExists(RkObject *t) const;
        void addObject(RkObject *obj);
        void addShortcut(RkObject *obj,
                         Rk::Key key,
                         Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeShortcut(RkObject *obj,
                            Rk::Key key,
                            Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeObject(RkObject *obj);
        void removeObjEvents(RkObject *obj);
        void postEvent(RkObject *obj, std::unique_ptr<RkEvent> event);
        void processEvents();
        void postAction(std::unique_ptr<RkAction> act);
        void processActions();
        void subscribeTimer(RkTimer *timer);
        void unsubscribeTimer(RkTimer *timer);
        void processTimers();
        void clearEvents(const RkObject *obj);
        void clearActions(const RkObject *obj);
        void processQueue();
        void addPopup(RkWidget* popup);
        void removePopup(RkWidget* popup);
        const std::vector<RkWidget*>& getPopupWidgets() const;
        void setScaleFactor(double factor);
        double scaleFactor() const;
#ifdef RK_OS_WIN
        RkEventQueueWin* getPlatformEventQueue() const;
#endif // RK_OS_WIN


 protected:
        void processPopups(RkWidget *widget, RkEvent* event);
        void processShortcuts(RkKeyEvent *event);
        void removeObjectShortcuts(RkObject *obj);
        bool isTopWidget(RkObject *obj) const;

 private:
        RK_DECALRE_INTERFACE_PTR(RkEventQueue);
        RK_DISABLE_COPY(RkEventQueueImpl);
        RK_DISABLE_MOVE(RkEventQueueImpl);
        std::unique_ptr<RkSystemWindow> systemWindow;
        std::unordered_set<RkObject*> objectsList;
        std::vector<RkWidget*> popupList;
        std::unordered_map<int, std::unique_ptr<RkShortcut>> shortcutsList;
        std::mutex eventsQueueMutex;
        std::vector<std::pair<RkObject*, std::unique_ptr<RkEvent>>> eventsQueue;
        std::mutex actionsQueueMutex;
        std::vector<std::unique_ptr<RkAction>> actionsQueue;
        std::unordered_set<RkTimer*> timersList;

#ifdef RK_OS_WIN
        std::unique_ptr<RkEventQueueWin> platformEventQueue;
#elif RK_OS_MAC
        std::unique_ptr<RkEventQueueMac> platformEventQueue;
#else // X11
        std::unique_ptr<RkEventQueueX> platformEventQueue;
#endif
};

#endif // RK_EVENT_QUEUE_IMPL_H
