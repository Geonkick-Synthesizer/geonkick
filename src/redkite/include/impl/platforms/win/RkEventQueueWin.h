/**
 * File name: RkQueueWin.h
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

#ifndef RK_EVENT_QUEUE_WIN_H
#define RK_EVENT_QUEUE_WIN_H

#include "RkPlatform.h"

class RkEvent;

// TODO: define a common interface for platform event queue

class RkEventQueueWin
{
 public:
        RkEventQueueWin();
	~RkEventQueueWin();
        RkEventQueueWin(const RkEventQueueWin &other) = delete;
        RkEventQueueWin& operator=(const RkEventQueueWin &other) = delete;
        RkEventQueueWin(RkEventQueueWin &&other) = delete;
        RkEventQueueWin& operator=(RkEventQueueWin &&other) = delete;
        void dispatchEvents();
        void setScaleFactor(double factor);
        double getScaleFactor() const;
		
private:
        double scaleFactor;
};

#endif // RK_EVENT_QUEUE_X_H
