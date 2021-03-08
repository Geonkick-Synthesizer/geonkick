/**
 * File name: RkTransition.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#ifndef RK_TRANSITION_H
#define RK_TRANSITION_H

#include "RkObject.h"
#include "RkTimer.h"
#include <math.h>

class RK_EXPORT RkTransition : public RkObject {
 public:
        enum class TransitionDirection: int {
                TransitionForward = 0,
                TransitionBackward = 1,
                TransitionRepeat = 2,
                TransitionCycle = 3
        };

        enum class TransitionType: int {
                TransitionLinear = 0,
                TransitionAccelerated = 1,
        };

        explicit RkTransition(RkObject *parent = nullptr)
                : RkObject{parent}
                , transitionDirection{TransitionDirection::TransitionForward}
                , transitionType{TransitionType::TransitionLinear}
                , transitionTimer{new RkTimer(this, 30)}
                , startValue{0}
                , endValue{0}
                , currentValue{0}
                , speedValue{1}
        {
                RK_ACT_BIND(transitionTimer, timeout, RK_ARG_TYPE(), this, onTimeout());
        }

        virtual ~RkTransition() = default;

        void setRange(int start, int end)
        {
                startValue = start;
                endValue = end;
        }

        void setCallback(const std::function<void(int)> &cb)
        {
                actionCallback = cb;
        }

        void start()
        {
                currentValue = startValue;
                transitionTimer->start();
        }

        void stop()
        {
                transitionTimer->stop();
        }

        void setDirection(TransitionDirection direction)
        {
                transitionDirection = direction;
        }

        void setTransitionType(TransitionType type)
        {
                transitionType = type;
        }

        void setSpeed(int speed)
        {
                if (speed < 1)
                        speedValue = 1;
                else if (speed > 10)
                        speedValue = 10;
                else
                        speedValue = speed;
        }


  private:

        void onTimeout()
        {
                if (actionCallback) {
                        int tempVal = currentValue;
                        auto curVal = calculateCurrentValue();
                        if (curVal != tempVal)
                                actionCallback(curVal);
                }
        }

        int calculateCurrentValue()
        {
                if (transitionType == TransitionType::TransitionLinear) {
                        currentValue += std::fabs(endValue - startValue) * ((double)speedValue / 50);
                        if (currentValue >= endValue) {
                                currentValue = endValue;
                                transitionTimer->stop();
                        }
                }
                return currentValue;
        }

        std::function<void(int)> actionCallback;
        TransitionDirection transitionDirection;
        TransitionType transitionType;
        RkTimer *transitionTimer;
        int startValue;
        int endValue;
        double currentValue;
        int speedValue;
};

#endif // RK_TRANSITION_H

