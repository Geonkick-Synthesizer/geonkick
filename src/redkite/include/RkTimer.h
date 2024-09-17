/**
 * File name: RkTimer.h
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

#ifndef RK_TIMER_H
#define RK_TIMER_H

#include "Rk.h"
#include "RkObject.h"

class RkEventQueue;

class RkTimer: public RkObject {
  public:
        explicit RkTimer(RkObject *parent, long unsigned int interval = 0);
        virtual ~RkTimer();
        RK_DECL_ACT(timeout, timeout(), RK_ARG_TYPE(), RK_ARG_VAL());
        void start();
        void stop();
        bool started() const;
        long long unsigned int interval() const;
        void setInterval(int val);
        bool isTimeout() const;
        void callTimeout();

 protected:
        long long unsigned int getCurrentTime() const;

 private:
        long long unsigned int timerInterval;
        bool timerStarted;
        long long unsigned int lastTime;
        RK_DISABLE_COPY(RkTimer);
        RK_DISABLE_MOVE(RkTimer);
};

#endif // RK_TIMER_H
