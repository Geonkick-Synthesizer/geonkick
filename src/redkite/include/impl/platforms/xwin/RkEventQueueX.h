/**
 * File name: RkQueueX.h
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

#ifndef RK_EVENT_QUEUE_X_H
#define RK_EVENT_QUEUE_X_H

#include "RkEvent.h"
#include "RkPlatform.h"

#include <queue>
#include <regex>

class RkEvent;
class RkObject;

class RkEventQueueX
{
 public:
        RkEventQueueX();
	~RkEventQueueX();
        bool pending() const;
        void setDisplay(Display *display);
        Display* display() const;
        std::vector<std::pair<RkWindowId, std::unique_ptr<RkEvent>>> getEvents();
        void setScaleFactor(double factor);
        double getScaleFactor() const;

 protected:
        std::unique_ptr<RkEvent> getButtonPressEvent(XEvent *e);
        std::unique_ptr<RkEvent> getMouseMove(XEvent *e);
        std::unique_ptr<RkEvent> getKeyEvent(XEvent *e);
        std::unique_ptr<RkEvent> getFocusEvent(XEvent *e);
        void updateModifiers(Rk::Key key, RkEvent::Type type);
        Rk::Key fromKeysym(int keycode) const;
        static std::string decodeUri(const std::string &dropFilePath);

 private:
        RK_DISABLE_COPY(RkEventQueueX);
        RK_DISABLE_MOVE(RkEventQueueX);
        Display* xDisplay;
        std::chrono::system_clock::time_point lastTimePressed;
        mutable int keyModifiers;
        double scaleFactor;
};

#endif // RK_EVENT_QUEUE_X_H
