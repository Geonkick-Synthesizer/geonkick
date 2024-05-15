/**
 * File name: RkSystemWindow.cpp
 * Project: Redkite (A lightweight graphics widget toolkit for embedded GUI)
 *
 * Copyright (C) 2023 Iurie Nistor
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
#include "RkSystemWindow.h"
#include "RkEvent.h"
#include "RkPlatform.h"
#include "RkMain.h"
#include "RkWidgetImpl.h"
#include "RkEventQueueImpl.h"

#ifdef RK_OS_WIN
#include "RkWindowWin.h"
#elif RK_OS_MAC
#include "RkWindowMac.h"
#else // X11
#include "RkWindowX.h"
#undef KeyPress
#undef KeyRelease
#undef Paint
#undef FocusIn
#undef FocusOut
#endif

#include <RkPainter.h>

RkSystemWindow::RkSystemWindow(RkWidget *widget, const RkNativeWindowInfo* parent)
        : isWindowClosed{false}
        , topWidget{widget}
#ifdef RK_OS_WIN
        , platformWindow{std::make_unique<RkWindowWin>(parent)}
#else // X11
        , platformWindow{std::make_unique<RkWindowX>(parent)}
#endif // X11
        , windowSize{platformWindow->size()}
        , windowBackground{platformWindow->background()}
        , isGrabKeyEnabled{false}
        , isPropagateGrabKey{true}
        , hoverWidget{nullptr}
        , mouseCaptureWidget{nullptr}
        , focusWidget{nullptr}
{
        platformWindow->init();
}

RkSystemWindow::~RkSystemWindow()
{
}

void RkSystemWindow::setTitle(const std::string &title)
{
        platformWindow->setTitle(windowTitle);
}

const std::string& RkSystemWindow::title() const
{
        return windowTitle;
}


void RkSystemWindow::show(bool b)
{
        platformWindow->show(b);
}

const RkNativeWindowInfo* RkSystemWindow::nativeWindowInfo() const
{
        return platformWindow->nativeWindowInfo();
}

RkCanvasInfo* RkSystemWindow::getCanvasInfo() const
{
        return platformWindow->getCanvasInfo();
}

void RkSystemWindow::freeCanvasInfo()
{
        platformWindow->freeCanvasInfo();
}

RkImage& RkSystemWindow::getImage()
{
        return systemWindowImage;
}

RkWindowId RkSystemWindow::id() const
{
        return platformWindow->id();
}

void RkSystemWindow::setSize(const RkSize &size)
{
        platformWindow->setSize(size);
        RK_LOG_DEBUG("size w: " << size.width() << ", h: " << size.width());
        systemWindowImage = RkImage(size);
        systemWindowImage.fill(platformWindow->background());
}

RkSize RkSystemWindow::size() const
{
        return platformWindow->size();
}

void RkSystemWindow::setPosition(const RkPoint &p)
{
        platformWindow->setPosition(p);
}

RkPoint RkSystemWindow::position() const
{
        return platformWindow->position();
}

void RkSystemWindow::setBackgroundColor(const RkColor &color)
{
        platformWindow->setBackgroundColor(color);
}

const RkColor& RkSystemWindow::background() const
{
        return platformWindow->background();
}

void RkSystemWindow::enableGrabKey(bool b)
{
}

bool RkSystemWindow::grabKeyEnabled() const
{
        return false;
}

void RkSystemWindow::propagateGrabKey(bool b)
{
}

bool RkSystemWindow::propagateGrabKeyEnabled() const
{
        return false;
}

RkSystemWindow::WidgetEventList
RkSystemWindow::processEvent(const RkEvent *event)
{
        if (!topWidget->isVisible())
                return {};

        switch(event->type()) {
        case RkEvent::Type::Close:
        {
                RK_LOG_DEBUG("RkEvent::Type::Close");
                WidgetEventList events;
                events.emplace_back(std::make_pair(topWidget, std::make_unique<RkCloseEvent>()));
                return events;
        }
        case RkEvent::Type::Paint:
        {
                RK_LOG_DEBUG("RkEvent::Type::Paint");
                RkPainter painter(this);
                painter.drawImage(systemWindowImage, 0, 0);
                break;
        }
        case RkEvent::Type::MouseButtonPress:
        case RkEvent::Type::MouseButtonRelease:
        case RkEvent::Type::MouseMove:
        case RkEvent::Type::MouseDoubleClick:
                return processMouseEvent(static_cast<const RkMouseEvent*>(event));
        case RkEvent::Type::Resize:
        {
                platformWindow->resizeCanvas();
                systemWindowImage = RkImage(platformWindow->size());
                systemWindowImage.fill(platformWindow->background());
                RK_IMPL_PTR(topWidget)->update(true);
                WidgetEventList events;
                events.emplace_back(std::make_pair(topWidget, std::make_unique<RkResizeEvent>()));
                return events;
        }
        case RkEvent::Type::KeyPressed:
        case RkEvent::Type::KeyReleased:
        {
                WidgetEventList events;
                if (focusWidget) {
                        auto keyEvent = std::make_unique<RkKeyEvent>();
                        *keyEvent.get() = *static_cast<const RkKeyEvent*>(event);
                        events.emplace_back(std::make_pair(focusWidget, std::move(keyEvent)));
                }
                return events;
        }
        default:
                RK_LOG_DEBUG("unknown event");
                break;
        }

        return {};
}

RkSystemWindow::WidgetEventList RkSystemWindow::processMouseEvent(const RkMouseEvent* event)
{
        WidgetEventList events;
        RkWidget *widget = nullptr;
        if (mouseCaptureWidget)
                widget = mouseCaptureWidget;
        else {
                const auto& popups = RK_IMPL_PTR(topWidget->eventQueue())->getPopupWidgets();
                if (!popups.empty()) {
                        widget = getWidgetByGlobalPoint(popups.front(), event->point());
                        if (widget == popups.front() && !containsGlobalPoint(popups.front(), event->point()))
                                widget = getWidgetByGlobalPoint(topWidget, event->point());
                } else {
                        widget = getWidgetByGlobalPoint(topWidget, event->point());
                }
        }

        if (event->type() == RkEvent::Type::MouseButtonPress)
                mouseCaptureWidget = widget;

        if (widgetExists(widget) && widget->isVisible()) {
                auto mouseEvent = std::make_unique<RkMouseEvent>();
                mouseEvent->setType(event->type());
                mouseEvent->setButton(event->button());
                mouseEvent->setPoint(widget->mapToLocal(event->point()));
                events.emplace_back(std::make_pair(widget, std::move(mouseEvent)));
        }

        if (widget != hoverWidget) {
                std::unique_ptr<RkHoverEvent> hoverEvent;
                if (hoverWidget && widgetExists(hoverWidget) && hoverWidget->isVisible()) {
                        hoverEvent = std::make_unique<RkHoverEvent>();
                        hoverEvent->setHover(false);
                        events.emplace_back(std::make_pair(hoverWidget, std::move(hoverEvent)));
                }

                if (widgetExists(widget) && widget->isVisible()) {
                        hoverEvent = std::make_unique<RkHoverEvent>();
                        hoverEvent->setHover(true);
                        events.emplace_back(std::make_pair(widget, std::move(hoverEvent)));
                        hoverWidget = widget;
                }
        }

        if (event->type() == RkEvent::Type::MouseButtonRelease)
                mouseCaptureWidget = nullptr;

        return events;
}

bool RkSystemWindow::containsGlobalPoint(RkWidget* widget, const RkPoint &globalPoint) const
{
        auto globalTopLeft = widget->mapToGlobal({0, 0});
        auto globalBottomRight = globalTopLeft + RkPoint(widget->width(), widget->height());
        return RkRect(globalTopLeft, globalBottomRight).contains(globalPoint);
}

RkWidget* RkSystemWindow::getWidgetByGlobalPoint(RkWidget *widget, const RkPoint &globalPoint)
{
        for (auto &child: widget->children()) {
                auto childWidget = dynamic_cast<RkWidget*>(child);
                if (childWidget && childWidget->isVisible()
                    && containsGlobalPoint(childWidget, globalPoint))
                        return getWidgetByGlobalPoint(childWidget, globalPoint);
        }
        return widget;
}

void RkSystemWindow::event(RkEvent *event)
{
}

void RkSystemWindow::closeEvent(RkCloseEvent *event)
{
        //        isWindowClosed = true;
        //        topWidget->event(event);
}

void RkSystemWindow::keyPressEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::keyReleaseEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::shortcutEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::mouseMoveEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::mouseButtonPressEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::mouseButtonReleaseEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::mouseDoubleClickEvent(RkMouseEvent *event)
{
        mouseButtonPressEvent(event);
}

void RkSystemWindow::wheelEvent(RkWheelEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::dropEvent(RkDropEvent *event)
{
        //        if (parentWidget())
        //                getTopWidget()->dropEvent(event);
}

void RkSystemWindow::moveEvent(RkMoveEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::resizeEvent(RkResizeEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::paintEvent(RkPaintEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::showEvent(RkShowEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::hideEvent(RkHideEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::focusEvent([[maybe_unused]] RkFocusEvent *event)
{
        update();
}

void RkSystemWindow::hoverEvent(RkHoverEvent *event)
{
        RK_UNUSED(event);
}

void RkSystemWindow::update()
{
        platformWindow->update();
}

//const RkCanvasInfo* RkSystemWindow::getCanvasInfo() const
//{
//       return platformWindow->getCanvasInfo();
//}

//void RkSystemWindow::freeCanvasInfo()
//{
//       return platformWindow->freeCanvasInfo();
//}

RkRect RkSystemWindow::rect() const
{
        return RkRect(0, 0, 200, 200);//  return platformWindow->rect();
}

void RkSystemWindow::close()
{
        isWindowClosed = true;
}

bool RkSystemWindow::isClosed() const
{
        return isWindowClosed;
}

void RkSystemWindow::setTopWidget(RkWidget *widget)
{
        topWidget = widget;
}

RkWidget* RkSystemWindow::getTopWidget() const
{
        return topWidget;
}

void RkSystemWindow::setFocus(bool b)
{
        //impl_ptr->setFocus(b);
}

bool RkSystemWindow::hasFocus() const
{
        return true;// return impl_ptr->hasFocus();
}

void RkSystemWindow::setPointerShape(Rk::PointerShape shape)
{
        //impl_ptr->setPointerShape(shape);
}

Rk::PointerShape RkSystemWindow::pointerShape() const
{
        return Rk::PointerShape::NoShape;//impl_ptr->pointerShape();
}

void RkSystemWindow::setScaleFactor(double factor)
{
        platformWindow->setScaleFactor(factor);
        RK_IMPL_PTR(topWidget->eventQueue())->setScaleFactor(factor);
}

double RkSystemWindow::scaleFactor() const
{
        return platformWindow->getScaleFactor();
}

void RkSystemWindow::setFocusWidget(RkWidget *widget, bool b)
{
        RK_LOG_DEV_DEBUG("widget: " << widget);
        if (widget != focusWidget) {
                if (focusWidget) {
                        auto focusEvent = std::make_unique<RkFocusEvent>(RkEvent::Type::FocusedOut);
                        RK_IMPL_PTR(focusWidget->eventQueue())->postEvent(focusWidget, std::move(focusEvent));
                }
                focusWidget = widget;
                auto focusType = b ? RkEvent::Type::FocusedIn : RkEvent::Type::FocusedOut;
                auto focusEvent = std::make_unique<RkFocusEvent>(focusType);
                RK_IMPL_PTR(focusWidget->eventQueue())->postEvent(focusWidget, std::move(focusEvent));
                if (!b)
                        focusWidget = nullptr;
        } else if (!b) {
                if (widget->parentWidget())
                        focusWidget = widget->parentWidget();
                else
                        focusWidget = topWidget;
        }
}

RkWidget* RkSystemWindow::getFocusWidget() const
{
        return focusWidget;
}

bool RkSystemWindow::widgetExists(RkWidget *widget) const
{
        auto eventQueueImpl = RK_IMPL_PTR(topWidget->eventQueue());
        return eventQueueImpl && eventQueueImpl->objectExists(widget);
}

#ifdef RK_OS_WIN
void RkSystemWindow::setEventQueue(RkEventQueue *eventQueue)
{
    platformWindow->setEventQueue(eventQueue);
}
#endif // RK_OS_WIN
