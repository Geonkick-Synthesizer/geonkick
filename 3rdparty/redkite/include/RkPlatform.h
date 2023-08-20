/**
 * File name: RkPlatform.h
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

#ifndef RK_PLATFORM_H
#define RK_PLATFORM_H

#include "Rk.h"
#include "RkLog.h"

#include <X11/Xlib.h>

struct RK_EXPORT RkWindowId {
        Window id;
};

struct _DndClass;

class RK_EXPORT RkNativeWindowInfo
{
 public:

 RkNativeWindowInfo() :
        display{nullptr}
        , screenNumber{0}
        , window{0}
        , scaleFactor{1}{}
        ~RkNativeWindowInfo() = default;
        Display* display;
        int screenNumber;
        Window window;
        double scaleFactor;
};

RkNativeWindowInfo RK_EXPORT rk_from_native_x11(Display* display,
                                                int screenNumber,
                                                Window window);
RkWindowId RK_EXPORT rk_id_from_x11(Window window);

#endif // RK_PLATFORM_H
