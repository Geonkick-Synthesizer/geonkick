/**
 * File name: RkAction.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#ifndef RK_ACTION_H
#define RK_ACTION_H

#include "Rk.h"
#include "RkObject.h"

class RK_EXPORT RkAction {
 public:
        explicit RkAction(RkObject *obj = nullptr)
                : actionObject{obj} {}

        virtual ~RkAction() = default;

        void setCallback(const std::function<void(void)> &cb)
        {
                actionCallback = cb;
        }

        void call()
        {
                if (actionCallback)
                        actionCallback();
        }

        RkObject *object() { return actionObject; }

  private:
        RkObject *actionObject;
        std::function<void(void)> actionCallback;
};

#endif // RK_ACTION_H

