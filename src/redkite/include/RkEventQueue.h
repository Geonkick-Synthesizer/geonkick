/**
 * File name: RkEventQueue.h
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

#ifndef RK_EVENT_QUEUE_H
#define RK_EVENT_QUEUE_H

#include "Rk.h"
#include "RkObject.h"
#include "RkAction.h"

class RkEvent;
class RkTimer;
class RkWindowId;
class RkWidget;

class RK_EXPORT RkEventQueue {
 public:
        RkEventQueue();
        virtual ~RkEventQueue();
        void addObject(RkObject *obj);
        void addShortcut(RkObject *obj,
                         Rk::Key key,
                         Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeShortcut(RkObject *obj,
                            Rk::Key key,
                            Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeObject(RkObject *obj);
        void postEvent(RkObject *obj, std::unique_ptr<RkEvent> event);
        void postEvent(const RkWindowId &id, std::unique_ptr<RkEvent> event);
        void postAction(std::unique_ptr<RkAction> act);
        void subscribeTimer(RkTimer *timer);
        void unsubscribeTimer(RkTimer *timer);
        void processEvents();
        void processActions();
        void processTimers();
        void processQueue();
        void clearObjectEvents(const RkObject *obj);
        void clearObjectActions(const RkObject *obj);
        void clearEvents();
        void clearActions();
        void clearQueue();
        RkObject* findObjectByName(const std::string &name) const;
        void setScaleFactor(double factor);
        double getScaleFactor() const;
        RkWidget* getWidget(const RkWindowId &id) const;
        void dispatchEvents();

 protected:
        RK_DECLARE_IMPL(RkEventQueue);
        RkEventQueue(std::unique_ptr<RkEventQueueImpl> impl);

 private:
        RK_DISABLE_COPY(RkEventQueue);
        RK_DISABLE_MOVE(RkEventQueue);
};

#endif // RK_EVENT_QUEUE_H
