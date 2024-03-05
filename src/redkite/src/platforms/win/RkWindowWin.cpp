/**
 * File name: RkWindowWin.cpp
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

#include "RkLog.h"
#include "RkWindowWin.h"
#include "RkCanvasInfo.h"

RkWindowWin::RkWindowWin(const RkNativeWindowInfo *parent)
        : parentWindowInfo{parent ? *parent : RkNativeWindowInfo() }
	, windowInfo{nullptr}
        , windowHandle{0}
        , winBorderWidth{0}
        , winBorderColor{255, 255, 255}
        , backgroundColor{255, 255, 255}
        , eventQueue{nullptr}
        , canvasInfo{nullptr}
        , scaleFactor{parent ? parent->scaleFactor : 1.0}
{
}

RkWindowWin::RkWindowWin(const RkNativeWindowInfo &parent)
        : parentWindowInfo{parent}
        , windowHandle{0}
        , winBorderWidth{0}
        , winBorderColor{255, 255, 255}
        , backgroundColor{255, 255, 255}
        , eventQueue{nullptr}
        , canvasInfo{nullptr}
        , scaleFactor{parent.scaleFactor}
{
}

RkWindowWin::~RkWindowWin()
{
	freeCanvasInfo();
        if (windowHandle.id)
                DestroyWindow(windowHandle.id);
}

Rk::WidgetFlags RkWindowWin::flags() const
{
        return windowFlags;
}

bool RkWindowWin::hasParent() const
{
        return parentWindowInfo.window != nullptr;
}

bool RkWindowWin::init()
{
        RK_LOG_DEBUG("hasParent(): " << hasParent());
        auto className = hasParent() ? parentWindowInfo.className : rk_win_api_class_name();
        RK_LOG_DEBUG("className: " << className);
        auto winStyle = hasParent() ? (WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE) : (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
        if (static_cast<int>(windowFlags) & static_cast<int>(Rk::WidgetFlags::Popup))
                winStyle = WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE;
        auto pos = position();
        auto winSize = size();
        windowHandle.id = CreateWindowExA(0,
                                          className.c_str(),
                                          "RkWidget",
                                          winStyle,
                                          pos.x() * scaleFactor,
                                          pos.y() * scaleFactor,
                                          winSize.width() * scaleFactor,
                                          winSize.height() * scaleFactor,
                                          !hasParent() ? nullptr : parentWindowInfo.window,
                                          nullptr,
                                          hasParent() ? parentWindowInfo.instance : rk_win_api_instance(),
                                          nullptr);

        if (!windowHandle.id) {
                RK_LOG_ERROR("can't create window");
                return false;
        }

        SetTimer(windowHandle.id, RK_MAIN_WINDOW_TIMER_ID, 1, nullptr);

        if (eventQueue)
                SetWindowLongPtr(windowHandle.id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventQueue));
        
        return true;
}

void RkWindowWin::show(bool b)
{
        if (isWindowCreated()) {
                ShowWindow(windowHandle.id, b ? SW_SHOW : SW_HIDE);
                UpdateWindow(windowHandle.id);
        }
}

const RkNativeWindowInfo* RkWindowWin::nativeWindowInfo()
{
        if (isWindowCreated()) {
                windowInfo            = std::make_unique<RkNativeWindowInfo>();
                windowInfo->className = hasParent() ? parentWindowInfo.className : rk_win_api_class_name();
                windowInfo->window    = windowHandle.id;
                windowInfo->instance  = hasParent() ? parentWindowInfo.instance : rk_win_api_instance();
                windowInfo->scaleFactor = scaleFactor;
                return windowInfo.get();
        }

        return nullptr;
}

void RkWindowWin::setTitle(const std::string &title)
{
        if (isWindowCreated() && !title.empty())
                SetWindowTextA(windowHandle.id, title.c_str());
}

bool RkWindowWin::isWindowCreated() const
{
        return windowHandle.id != nullptr;
}

RkSize RkWindowWin::size() const
{
        if (isWindowCreated()) {
                RECT rect;
                GetWindowRect(windowHandle.id, &rect);
                return RkSize (static_cast<double>(rect.right - rect.left) / scaleFactor,
                               static_cast<double>(rect.bottom - rect.top) / scaleFactor);
        }
        return {250, 250};
}

void RkWindowWin::setSize(const RkSize &winSize)
{
        if (isWindowCreated() && winSize.width() > 0 && winSize.height() > 0) {
                SetWindowPos(windowHandle.id, 0, 0, 0,
                             winSize.width() * scaleFactor,
                             winSize.height() * scaleFactor,
                             SWP_NOMOVE | SWP_NOZORDER);
        }
}

RkPoint RkWindowWin::position() const
{
        if (isWindowCreated()) {
                RECT rect;
                GetWindowRect(windowHandle.id, &rect);
                RECT rect0 = rect;
                if (hasParent())
                        MapWindowPoints(windowHandle.id, GetParent(windowHandle.id), reinterpret_cast<LPPOINT>(&rect), 2);
                return RkPoint(static_cast<double>(rect.left - rect0.left) / scaleFactor,
                               static_cast<double>(rect.top - rect0.top) / scaleFactor);
        }
        return {0, 0};
}

void RkWindowWin::setPosition(const RkPoint &pos)
{
        if (isWindowCreated()) {
                SetWindowPos(windowHandle.id, 0,
                             pos.x() * scaleFactor,
                             pos.y() * scaleFactor,
                             0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
}

void RkWindowWin::setBorderWidth(int width)
{
	// IMPLEMENT
        winBorderWidth = scaleFactor * width;
}

int RkWindowWin::borderWidth() const
{
	//IMPLEMENT
	return static_cast<double>(winBorderWidth) / scaleFactor;
}

void RkWindowWin::setBorderColor(const RkColor &color)
{
        winBorderColor = color;
}

const RkColor& RkWindowWin::borderColor() const
{
	return winBorderColor;
}

void RkWindowWin::setBackgroundColor(const RkColor &background)
{
        backgroundColor = background;
}

RkWindowId RkWindowWin::id() const
{
        RkWindowId id;
        id.id = windowHandle.id;
        return id;
}

const RkColor& RkWindowWin::background() const
{
	return backgroundColor;
}

void RkWindowWin::update()
{
        InvalidateRect(windowHandle.id, NULL, FALSE);
}

#ifdef RK_GRAPHICS_CAIRO_BACKEND
void RkWindowWin::createCanvasInfo()
{
        canvasInfo = std::make_unique<RkCanvasInfo>();
        auto hDC = GetDC(windowHandle.id);
        canvasInfo->cairo_surface = cairo_win32_surface_create(hDC);
        if (!canvasInfo->cairo_surface) {
                RK_LOG_ERROR("error on creating Cairo Win32 surface");
                return;
        }
        resizeCanvas();
}

void RkWindowWin::resizeCanvas()
{
        if (canvasInfo)
                cairo_surface_set_device_scale(canvasInfo->cairo_surface, scaleFactor, scaleFactor);
}

const RkCanvasInfo* RkWindowWin::getCanvasInfo()
{
        if (!canvasInfo)
                createCanvasInfo();
        return canvasInfo ? canvasInfo.get() : nullptr;
}

void RkWindowWin::freeCanvasInfo()
{
        if (canvasInfo) {
                auto hDC = cairo_win32_surface_get_dc(canvasInfo->cairo_surface);
                cairo_surface_destroy(canvasInfo->cairo_surface);
                ReleaseDC(windowHandle.id, hDC);
                RK_LOG_DEBUG("Cairo Win32 surface destroyed");
        }
        canvasInfo.reset();
}
#else
#error No graphics backend defined
#endif // RK_GRAPHICS_CAIRO_BACKEND

void RkWindowWin::setFocus(bool b)
{
        if (!isWindowCreated())
                return;
        if (b)
                SetFocus(windowHandle.id);
}

bool RkWindowWin::hasFocus()
{
        if (!isWindowCreated())
                return false;
        return GetFocus() == windowHandle.id;
}

bool RkWindowWin::pointerIsOverWindow() const
{
        if (!isWindowCreated())
                return false;
	POINT cursorPos;
        GetCursorPos(&cursorPos);
        return WindowFromPoint(cursorPos) == windowHandle.id;
}

void RkWindowWin::setPointerShape(Rk::PointerShape shape)
{
        // IMPLEMENT
}

void RkWindowWin::setEventQueue(RkEventQueue *queue)
{
        if (!isWindowCreated())
                return;

        eventQueue = queue;
        RK_LOG_DEBUG("eventQueue: " << eventQueue);
        SetWindowLongPtr(windowHandle.id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventQueue));
        InvalidateRect(windowHandle.id, nullptr, FALSE);
        UpdateWindow(windowHandle.id);
}

void RkWindowWin::setScaleFactor(double factor)
{
        auto p = position();
        auto z = size();
        scaleFactor = factor;
        windowInfo->scaleFactor = scaleFactor;
        setPosition(p);
        setSize(z);
        resizeCanvas();
}

double RkWindowWin::getScaleFactor() const
{
	return scaleFactor;
}

