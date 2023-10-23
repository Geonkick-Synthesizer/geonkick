/**
 * File name: RkObserver.h
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

#ifndef RK_OBSERVER_H
#define RK_OBSERVER_H

#include "Rk.h"
#include "RkLog.h"

class RkObject;

class RK_EXPORT RkObserver {
 public:
        RkObserver(RkObject *obj = nullptr)
                : observerObject{obj} { }
        virtual ~RkObserver() = default;
        RkObject *object() { return observerObject; }
        void setObject(RkObject *obj) { observerObject = obj; }
 private:
        RkObject *observerObject;
};

#endif // RK_OBSERVER_H
