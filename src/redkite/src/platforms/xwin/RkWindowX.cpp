/**
 * File name: RkWindowX.cpp
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
#include "RkWindowX.h"
#include "RkCanvasInfo.h"

#include <X11/cursorfont.h>
#include <X11/Xatom.h>

RkWindowX::RkWindowX(const RkNativeWindowInfo *parent)
        : parentWindowInfo{parent ? *parent : RkNativeWindowInfo() }
         , xDisplay{parent ? parent->display : nullptr}
         , screenNumber{parent ? parent->screenNumber : 0}
         , xWindow{0}
         , winBorderWidth{0}
         , winBorderColor{255, 255, 255}
         , winBackgroundColor{255, 255, 255}
         , canvasInfo{nullptr}
         , windowInfo{nullptr}
         , scaleFactor{parent ? parent->scaleFactor : 1.0}
 {
         RK_LOG_DEBUG("called: d: " << xDisplay << ", s: " << screenNumber);
 }

RkWindowX::RkWindowX(const RkNativeWindowInfo &parent)
        : parentWindowInfo{parent}
        , xDisplay{parent.display}
        , screenNumber{parent.screenNumber}
        , xWindow{0}
        , winBorderWidth{0}
        , winBorderColor{255, 255, 255}
        , winBackgroundColor{255, 255, 255}
        , canvasInfo{nullptr}
        , windowInfo{nullptr}
        , scaleFactor{parent.scaleFactor}
{
        RK_LOG_DEBUG("called: d: " << xDisplay << ", s: " << screenNumber);
}

RkWindowX::~RkWindowX()
{
        RK_LOG_DEBUG("called");
        if (xDisplay) {
                freeCanvasInfo();
                XDestroyWindow(xDisplay, xWindow);
                if (!hasParent())
                        XCloseDisplay(xDisplay);
        }
}

bool RkWindowX::hasParent() const
{
        return parentWindowInfo.display != nullptr;
}

bool RkWindowX::openDisplay()
{
        xDisplay = XOpenDisplay(nullptr);
        screenNumber = DefaultScreen(xDisplay);
        return xDisplay != nullptr;
}

bool RkWindowX::init()
{
        RK_LOG_DEBUG("called");
        if (!hasParent()) {
                if (!openDisplay()) {
                        RK_LOG_ERROR("can't open display");
                        return false;
                }
	}

        Window parent = 0;
        parent = hasParent() ? parentWindowInfo.window : RootWindow(xDisplay, screenNumber);
        auto res = XMatchVisualInfo(xDisplay, screenNumber, 32, TrueColor, &visualInfo);
        if (res == 0) {
                RK_LOG_ERROR("visual info was not found");
                return false;
        }

        XSetWindowAttributes attr;
        unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask;
        attr.background_pixmap = None;
        attr.colormap = XCreateColormap(xDisplay, parent, visualInfo.visual, AllocNone);
        attr.border_pixel = winBorderColor.argb();
        attr.background_pixel = winBackgroundColor.argb();

        attr.event_mask = ExposureMask
                          | KeyPressMask | KeyReleaseMask | KeymapStateMask | FocusChangeMask
                          | ButtonPressMask | ButtonReleaseMask
                          | EnterWindowMask | LeaveWindowMask | StructureNotifyMask
                          | PropertyChangeMask
                          | PointerMotionMask;
        attr.override_redirect = False;

        auto pos = position();
        auto winSize = size();
        RK_LOG_DEBUG("create window: d: " << xDisplay << ", p: " << parent);
        RK_LOG_DEBUG("size[" << winSize.width() * scaleFactor
                     <<", " << winSize.height() * scaleFactor << "]");
        xWindow = XCreateWindow(xDisplay, parent,
                                pos.x() * scaleFactor, pos.y() * scaleFactor,
                                winSize.width() * scaleFactor, winSize.height() * scaleFactor,
                                winBorderWidth,
                                visualInfo.depth,
                                InputOutput,
                                visualInfo.visual,
                                mask,
                                &attr);
        if (!xWindow) {
                RK_LOG_ERROR("can't create window");
                return false;
        }

        deleteWindowAtom = XInternAtom(display(), "WM_DELETE_WINDOW", True);
        XSetWMProtocols(xDisplay, xWindow, &deleteWindowAtom, 1);
        createCanvasInfo();
        windowInfo = std::make_unique<RkNativeWindowInfo>();
        windowInfo->display      = xDisplay;
        windowInfo->screenNumber = screenNumber;
        windowInfo->window       = xWindow;
        windowInfo->scaleFactor  = scaleFactor;
        RK_LOG_DEBUG("window created");
        return true;
}

void RkWindowX::show(bool b)
{
        RK_LOG_DEBUG("called, b = " << b);
        if (isWindowCreated()) {
                if (b) {
                        RK_LOG_DEBUG("XMapRaised");
                        XMapRaised(display(), xWindow);
                } else {
                        RK_LOG_DEBUG("XUnmapWindow");
                        XUnmapWindow(display(), xWindow);
                }
        }
}

Display* RkWindowX::display() const
{
        return xDisplay;
}

const RkNativeWindowInfo* RkWindowX::nativeWindowInfo() const
{
        return windowInfo ? windowInfo.get() : nullptr;
}

void RkWindowX::setTitle(const std::string &title)
{
        if (isWindowCreated() && !title.empty())
                XStoreName(xDisplay, xWindow, title.c_str());
}

bool RkWindowX::isWindowCreated() const
{
        return xDisplay != nullptr && xWindow;
}

RkSize RkWindowX::size() const
{
        if (isWindowCreated()) {
                XWindowAttributes attributes;
                XGetWindowAttributes(xDisplay, xWindow, &attributes);
                return RkSize(static_cast<double>(attributes.width) / scaleFactor,
                              static_cast<double>(attributes.height) / scaleFactor);
        }

        return {250, 250};
}

void RkWindowX::setSize(const RkSize &size)
{
        if (isWindowCreated() && size.width() > 0 && size.height() > 0) {
                RK_LOG_DEBUG("w:" << size.width() * scaleFactor <<
                             ", " << size.height() * scaleFactor);
                XResizeWindow(display(), xWindow, size.width() * scaleFactor,
                              size.height() * scaleFactor);
        }
}

RkPoint RkWindowX::position() const
{
        if (isWindowCreated()) {
                XWindowAttributes attributes;
                XGetWindowAttributes(xDisplay, xWindow, &attributes);
                return RkPoint(static_cast<double>(attributes.x) / scaleFactor,
                               static_cast<double>(attributes.y) / scaleFactor);
        }
        return {0, 0};
}

void RkWindowX::setPosition(const RkPoint &position)
{
        if (isWindowCreated()) {
                int x = position.x();
                int y = position.y();
                XMoveWindow(display(), xWindow, x * scaleFactor, y * scaleFactor);
        }
}

void RkWindowX::setBorderWidth(int width)
{
        winBorderWidth = width * scaleFactor;
        if (isWindowCreated())
                XSetWindowBorderWidth(display(), xWindow, winBorderWidth);
}

int RkWindowX::borderWidth() const
{
        return static_cast<double>(winBorderWidth) / scaleFactor;
}

void RkWindowX::setBorderColor(const RkColor &color)
{
        winBorderColor = color;
        if (isWindowCreated())
                XSetWindowBorder(display(), xWindow, winBorderColor.argb());
}

const RkColor& RkWindowX::borderColor() const
{
        return winBorderColor;
}

void RkWindowX::setBackgroundColor(const RkColor &color)
{
        winBackgroundColor = color;
        if (isWindowCreated())
                XSetWindowBackground(display(), xWindow, winBackgroundColor.argb());
}

const RkColor& RkWindowX::background() const
{
        return winBackgroundColor;
}

RkWindowId RkWindowX::id() const
{
        RkWindowId id;
        id.id = xWindow;
        return id;
}

void RkWindowX::update()
{
        if (isWindowCreated()) {
                XExposeEvent event;
                event.type       = Expose;
                event.send_event = false;
                event.display    = display();
                event.window     = xWindow;
                event.x          = 0;
                event.y          = 0;
                event.width      = size().width() * scaleFactor;
                event.height     = size().height() * scaleFactor;
                event.count      = 0;
                XSendEvent(display(),
                           xWindow,
                           True,
                           ExposureMask,
                           reinterpret_cast<XEvent*>(&event));
        }
}

#ifdef RK_GRAPHICS_CAIRO_BACKEND
void RkWindowX::createCanvasInfo()
{
        RK_LOG_DEBUG("called");
        canvasInfo = std::make_unique<RkCanvasInfo>();
        canvasInfo->cairo_surface = cairo_xlib_surface_create(display(), xWindow,
                                                              visualInfo.visual,
                                                              size().width() * scaleFactor, size().height() * scaleFactor);
	if (!canvasInfo->cairo_surface) {
                RK_LOG_ERROR("error on creating Cairo Win32 surface");
                return;
        }
        cairo_surface_set_device_scale(canvasInfo->cairo_surface, scaleFactor, scaleFactor);
}

void RkWindowX::resizeCanvas()
{
        cairo_xlib_surface_set_size(canvasInfo->cairo_surface,
                                    size().width() * scaleFactor,
                                    size().height() * scaleFactor);
        //cairo_surface_set_device_scale(canvasInfo->cairo_surface, scaleFactor, scaleFactor);
}

RkCanvasInfo* RkWindowX::getCanvasInfo() const
{
        return canvasInfo ? canvasInfo.get() : nullptr;
}

void RkWindowX::freeCanvasInfo()
{
        if (canvasInfo)
                cairo_surface_destroy(canvasInfo->cairo_surface);
}

#else
#error No graphics backend defined
#endif // RK_GRAPHICS_CAIRO_BACKEND

void RkWindowX::setFocus(bool b)
{
        if (b)
                XSetInputFocus(display(),
                               xWindow,
                               RevertToParent,
                               CurrentTime);
        else
                XSetInputFocus(display(),
                               None,
                               RevertToParent,
                               CurrentTime);
}

bool RkWindowX::hasFocus() const
{
        Window focus_return;
        int revert_to;
        XGetInputFocus(display(), &focus_return, &revert_to);
        return focus_return == xWindow;
}

void RkWindowX::setPointerShape(Rk::PointerShape shape)
{
        if (!isWindowCreated())
                return;

        Cursor pointer;
        switch (shape)
        {
        case Rk::PointerShape::Arrow:
                pointer = XCreateFontCursor(display(), XC_arrow);
        break;
        case Rk::PointerShape::IBeam:
                pointer = XCreateFontCursor(display(), XC_xterm);
        break;
        default:
                return;
        };
        XDefineCursor(display(), xWindow, pointer);
}

bool RkWindowX::pointerIsOverWindow() const
{
        if (isWindowCreated()) {
                Window child;
                int proot_x;
                int proot_y;
                Window root_win;
                unsigned int mask;
                int win_x;
                int win_y;
                XQueryPointer(display(),
                              xWindow,
                              &root_win,
                              &child,
                              &proot_x,
                              &proot_y,
                              &win_x,
                              &win_y,
                              &mask);

                RK_UNUSED(root_win);
                RK_UNUSED(mask);
                RK_UNUSED(win_x);
                RK_UNUSED(win_y);
                return child == xWindow;
        }
        return false;
}

void RkWindowX::setScaleFactor(double factor)
{
        auto p = position();
        auto z = size();
        scaleFactor = factor;
        windowInfo->scaleFactor = scaleFactor;
        setPosition(p);
        setSize(z);
        resizeCanvas();
}

double RkWindowX::getScaleFactor() const
{
        return scaleFactor;
}

