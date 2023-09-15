/**
 * File name: RkPlatform.h
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

#ifndef RK_PLATFORM_H
#define RK_PLATFORM_H

#include "Rk.h"
#include "RkLog.h"

#ifdef RK_OS_WIN
//#include <windows.h>

using HWND = long long unsigned int;
using HINSTANCE = long long unsigned int;
using LPCSTR = char*;

struct RK_EXPORT RkWindowId {
    RkWindowId(HWND arg /*= nullptr*/) : id(arg) {}
    HWND id;
};

class RK_EXPORT RkNativeWindowInfo {
	 public:
        RkNativeWindowInfo(HWND arg /*= nullptr*/)
		: window(arg) {}
		~RkNativeWindowInfo() = default;
        HINSTANCE instance;
        std::string className;
        HWND window;
		double scaleFactor;
};

HINSTANCE RK_EXPORT rk_win_api_instance();
std::string RK_EXPORT rk_win_api_class_name();
RkNativeWindowInfo RK_EXPORT rk_from_native_win(HINSTANCE instance, LPCSTR className, HWND window);
RkWindowId RK_EXPORT rk_id_from_win(HWND window);
//#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
//struct ID2D1Factory1;
//struct IDWriteFactory;
//#ID2D1Factory1* RK_EXPORT rk_direct2d_factory();
//#IDWriteFactory* RK_EXPORT rk_direct_write_factory();
//#endif // RK_DIRECT2D_GRAPHICS_BACKEND

#define RK_WIN_MESSAGE_PAINT (WM_USER + 0x0001)

#elif RK_OS_MAC
// to be defined
#else // X11 as default
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

#endif // X11
#endif // RK_PLATFORM_H
