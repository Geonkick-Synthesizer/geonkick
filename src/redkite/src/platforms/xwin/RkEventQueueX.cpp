/**
 * File name: RkQueueX.cpp
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

#include "RkWidget.h"
#include "RkEventQueueX.h"
#include "RkLog.h"

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

RkEventQueueX::RkEventQueueX()
        : xDisplay{nullptr}
        , keyModifiers{0}
        , scaleFactor{1.0}
{
        RK_LOG_DEBUG("called");
}

RkEventQueueX::~RkEventQueueX()
{
        RK_LOG_DEBUG("called");
}

bool RkEventQueueX::pending() const
{
        if (xDisplay)
                return XPending(xDisplay) > 0;
        return false;
}

void RkEventQueueX::setDisplay(Display *display)
{
        xDisplay = display;
}

Display* RkEventQueueX::display() const
{
        return xDisplay;
}

std::vector<std::unique_ptr<RkEvent>>
RkEventQueueX::getEvents()
{
        std::vector<std::unique_ptr<RkEvent>> events;
        while (pending()) {
                XEvent e;
                XNextEvent(xDisplay, &e);
                RK_LOG_DEBUG("New event");
                std::unique_ptr<RkEvent> event = nullptr;
                switch (e.type)
                {
                case Expose:
                        RK_LOG_DEBUG("Expose");
                        if (reinterpret_cast<XExposeEvent*>(&e)->count == 0)
                                event = std::make_unique<RkPaintEvent>();
                break;
                case KeyPress:
                        event = getKeyEvent(&e);
                        break;
                case KeyRelease:
                        event = getKeyEvent(&e);
                        break;
                case FocusIn:
                case FocusOut:
                        event = getFocusEvent(&e);
                        break;
                case ButtonPress:
                        event = getButtonPressEvent(&e);
                        break;
                case ButtonRelease:
                {
                        auto mouseEvent = std::make_unique<RkMouseEvent>();
                        auto buttonEvent = reinterpret_cast<XButtonEvent*>(&e);
                        mouseEvent->setX(static_cast<double>(buttonEvent->x / scaleFactor));
                        mouseEvent->setY(static_cast<double>(buttonEvent->y / scaleFactor));
                        mouseEvent->setType(RkEvent::Type::MouseButtonRelease);
                        event = std::move(mouseEvent);
                        break;
                }
                case MotionNotify:
                        event = getMouseMove(&e);
                        break;
                case ConfigureNotify:
                        event = std::make_unique<RkResizeEvent>();
                        break;
                case EnterNotify:
                case LeaveNotify:
                {
                        auto hoveEvent = std::make_unique<RkHoverEvent>();
                        hoveEvent->setHover(e.type == EnterNotify);
                        event = std::move(hoveEvent);
                        break;
                }
                case ClientMessage:
                {
                        auto atom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", True);
                        if (static_cast<Atom>(e.xclient.data.l[0]) == atom)
                                event = std::make_unique<RkCloseEvent>();
                        break;
                }
                default:
                        break;
                }

                if (event)
                        events.emplace_back(std::move(event));
        }
        return events;
}

std::unique_ptr<RkEvent> RkEventQueueX::getButtonPressEvent(XEvent *e)
{
        auto buttonEvent = reinterpret_cast<XButtonEvent*>(e);
        auto mouseEvent = std::make_unique<RkMouseEvent>();
        mouseEvent->setTime(std::chrono::system_clock::time_point(std::chrono::milliseconds(buttonEvent->time)));
        mouseEvent->setX(static_cast<double>(buttonEvent->x / scaleFactor));
        mouseEvent->setY(static_cast<double>(buttonEvent->y / scaleFactor));

        switch (buttonEvent->button)
        {
        case Button1:
                mouseEvent->setButton(RkMouseEvent::ButtonType::Left);
                break;
        case Button2:
                mouseEvent->setButton(RkMouseEvent::ButtonType::Middle);
                break;
        case Button3:
                mouseEvent->setButton(RkMouseEvent::ButtonType::Right);
                break;
        case Button4:
                mouseEvent->setButton(RkMouseEvent::ButtonType::WheelUp);
                break;
        case Button5:
                mouseEvent->setButton(RkMouseEvent::ButtonType::WheelDown);
                break;
        default:
                mouseEvent->setButton(RkMouseEvent::ButtonType::Unknown);
        }

        auto diff = mouseEvent->time().time_since_epoch() - lastTimePressed.time_since_epoch();
        if (std::chrono::duration_cast<std::chrono::microseconds>(diff).count() < 300000
            && buttonEvent->button != Button4 && buttonEvent->button != Button5)
                mouseEvent->setType(RkEvent::Type::MouseDoubleClick);
        lastTimePressed = mouseEvent->time();

        return mouseEvent;
}

std::unique_ptr<RkEvent> RkEventQueueX::getMouseMove(XEvent *e)
{
        auto buttonEvent = reinterpret_cast<XMotionEvent*>(e);
        auto mouseEvent = std::make_unique<RkMouseEvent>();
        mouseEvent->setTime(std::chrono::system_clock::time_point(std::chrono::milliseconds(buttonEvent->time)));
        mouseEvent->setType(RkEvent::Type::MouseMove);
        mouseEvent->setX(static_cast<double>(buttonEvent->x / scaleFactor));
        mouseEvent->setY(static_cast<double>(buttonEvent->y / scaleFactor));
        return mouseEvent;
}

std::unique_ptr<RkEvent> RkEventQueueX::getKeyEvent(XEvent *e)
{
        auto event = std::make_unique<RkKeyEvent>();
        event->setType(e->type == KeyPress ? RkEvent::Type::KeyPressed : RkEvent::Type::KeyReleased);
        auto keyCode = XkbKeycodeToKeysym(xDisplay,
                                          reinterpret_cast<XKeyEvent*>(e)->keycode,
                                          0, keyModifiers & static_cast<int>(Rk::KeyModifiers::Shift) ? 1 : 0);
        event->setKey(fromKeysym(keyCode));
        updateModifiers(event->key(), event->type());
        if (keyModifiers != static_cast<int>(Rk::KeyModifiers::NoModifier))
                event->setModifiers(keyModifiers);
        return event;
}

void RkEventQueueX::updateModifiers(Rk::Key key, RkEvent::Type type)
{
        if (key == Rk::Key::Key_Shift_Left
            || key == Rk::Key::Key_Shift_Right
            || key == Rk::Key::Key_Control_Left
            || key == Rk::Key::Key_Control_Right)
        {
                if (type == RkEvent::Type::KeyPressed)
                        keyModifiers |= static_cast<int>(key) >> 16;
                else
                        keyModifiers &= ~(static_cast<int>(key) >> 16);
        }
}

Rk::Key RkEventQueueX::fromKeysym(int keycode) const
{
        // Latine1
        if (0x00000020 <= keycode && keycode <= 0x000000ff)
                return static_cast<Rk::Key>(keycode);

        switch (keycode)
        {
        // Key modifiers
        case XK_Shift_L: return Rk::Key::Key_Shift_Left;
        case XK_Shift_R: return Rk::Key::Key_Shift_Right;
        case XK_Control_L: return Rk::Key::Key_Control_Left;
        case XK_Control_R: return Rk::Key::Key_Control_Right;
        case XK_Caps_Lock: return Rk::Key::Key_Caps_Lock;
        case XK_Shift_Lock: return Rk::Key::Key_Shift_Lock;
        case XK_Meta_L: return Rk::Key::Key_Meta_Left;
        case XK_Meta_R: return Rk::Key::Key_Meta_Right;
        case XK_Alt_L: return Rk::Key::Key_Alt_Left;
        case XK_Alt_R: return Rk::Key::Key_Alt_Right;
        case XK_Super_L: return Rk::Key::Key_Super_Left;
        case XK_Super_R: return Rk::Key::Key_Super_Right;
        case XK_Hyper_L: return Rk::Key::Key_Hyper_Left;
        case XK_Hyper_R: return Rk::Key::Key_Hyper_Right;

        case XK_space: return Rk::Key::Key_Space;

        // Pointer control keys
        case XK_Home:  return Rk::Key::Key_Home;
        case XK_Left:  return Rk::Key::Key_Left;
        case XK_Up:    return Rk::Key::Key_Up;
        case XK_Right: return Rk::Key::Key_Right;
        case XK_Down:  return Rk::Key::Key_Down;
        case XK_Page_Up: return Rk::Key::Key_Page_Up;
        case XK_Page_Down: return Rk::Key::Key_Page_Down;
        case XK_End: return Rk::Key::Key_End;
        case XK_Begin: return Rk::Key::Key_Begin;

        case XK_BackSpace: return Rk::Key::Key_BackSpace;
        case XK_Tab: return Rk::Key::Key_Tab;
        case XK_Linefeed: return Rk::Key::Key_Linefeed;
        case XK_Clear: return Rk::Key::Key_Clear;
        case XK_Return: return Rk::Key::Key_Return;
        case XK_Pause: return Rk::Key::Key_Pause;
        case XK_Scroll_Lock: return Rk::Key::Key_Scroll_Lock;
        case XK_Sys_Req: return Rk::Key::Key_Sys_Req;
        case XK_Escape: return Rk::Key::Key_Escape;
        case XK_Delete: return Rk::Key::Key_Delete;

        default: return Rk::Key::Key_None;
        }
}

std::unique_ptr<RkEvent> RkEventQueueX::getFocusEvent(XEvent *e)
{
        auto event = std::make_unique<RkFocusEvent>();
        event->setType(e->type == FocusIn ? RkEvent::Type::FocusedIn : RkEvent::Type::FocusedOut);
        return event;
}

void RkEventQueueX::setScaleFactor(double factor)
{
        scaleFactor = factor;
}

double RkEventQueueX::getScaleFactor() const
{
        return scaleFactor;
}

