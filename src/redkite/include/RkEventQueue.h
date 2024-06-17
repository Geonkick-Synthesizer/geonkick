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

class RkObject;
class RkEvent;
class RkAction;

class RkEventQueue {
 public:
        RkEventQueue();
        virtual ~RkEventQueue();
        void postEvent(RkObject *obj, std::unique_ptr<RkEvent> event);
        void postAction(std::unique_ptr<RkAction> act);
        void processQueue();

 protected:
        RK_DECLARE_O_PTR(RkEventQueue);
        RkEventQueue(std::unique_ptr<RkEventQueueImpl> impl);

 private:
        RK_DISABLE_COPY(RkEventQueue);
        RK_DISABLE_MOVE(RkEventQueue);
        friend class RkObject;
};

#endif // RK_EVENT_QUEUE_H
