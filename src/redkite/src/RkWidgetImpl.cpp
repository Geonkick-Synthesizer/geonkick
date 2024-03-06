/**
 * File name: RkWidgetImpl.cpp
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

#include "RkWidgetImpl.h"
#include "RkEvent.h"
#include "RkPainter.h"
#include "RkSystemWindow.h"
#include "RkEventQueueImpl.h"

RkWidget::RkWidgetImpl::RkWidgetImpl(RkWidget* widgetInterface,
                                     RkMain* mainApp,
                                     const RkNativeWindowInfo *parent,
                                     Rk::WidgetFlags flags)
        : RkObject::RkObjectImpl(widgetInterface, nullptr, Rk::ObjectType::Widget)
        , inf_ptr{widgetInterface}
        , topWidget{true}
        , systemWindow{nullptr}
        , widgetClosed{false}
        , widgetMaximumSize{1000000, 1000000}
        , widgetSize{250, 250}
        , widgetBorderWidth{0}
        , widgetAttributes{defaultWidgetAttributes()}
        , widgetFlags{flags}
        , widgetModality{(static_cast<int>(flags) & static_cast<int>(Rk::WidgetFlags::Dialog)) ? Rk::Modality::ModalTopWidget : Rk::Modality::NonModal}
        , widgetPointerShape{Rk::PointerShape::Arrow}
        , isWidgetShown{false}
        , isGrabKeyEnabled{false}
        , isPropagateGrabKey{true}
{
        RK_LOG_DEBUG("called");
}

RkWidget::RkWidgetImpl::RkWidgetImpl(RkWidget* widgetInterface,
                                     RkWidget* parent,
                                     Rk::WidgetFlags flags)
        : RkObject::RkObjectImpl(widgetInterface, parent, Rk::ObjectType::Widget)
        , inf_ptr{widgetInterface}
        , topWidget{false}
        , systemWindow{parent ? RK_IMPL_PTR(parent)->getSystemWindow() : nullptr}
        , widgetClosed{false}
        , widgetMaximumSize{1000000, 1000000}
        , widgetBorderWidth{0}
        , widgetAttributes{defaultWidgetAttributes()}
        , widgetFlags{flags}
        , widgetModality{(static_cast<int>(flags) & static_cast<int>(Rk::WidgetFlags::Dialog)) ? Rk::Modality::ModalTopWidget : Rk::Modality::NonModal}
        , widgetPointerShape{Rk::PointerShape::Arrow}
	, isWidgetShown{false}
        , isGrabKeyEnabled{false}
        , isPropagateGrabKey{true}
{
        RK_LOG_DEBUG("called");
}

RkWidget::RkWidgetImpl::~RkWidgetImpl()
{
        RK_LOG_DEBUG("called");
}

bool RkWidget::RkWidgetImpl::isTopWidget() const
{
        return topWidget;
}

void RkWidget::RkWidgetImpl::setSystemWindow(RkSystemWindow *window)
{
        systemWindow = window;
}

RkSystemWindow* RkWidget::RkWidgetImpl::getSystemWindow() const
{
        return systemWindow;
}

RkCanvasInfo* RkWidget::RkWidgetImpl::getCanvasInfo() const
{
        RK_LOG_DEBUG("called");
        return systemWindow->getImage().getCanvasInfo();
}

void RkWidget::RkWidgetImpl::freeCanvasInfo()
{
        //        systemWindow->freeCanvasInfo();
}

void RkWidget::RkWidgetImpl::setEventQueue(RkEventQueue *queue)
{
        RkObjectImpl::setEventQueue(queue);
}

Rk::WidgetFlags RkWidget::RkWidgetImpl::getWidgetFlags() const
{
        return widgetFlags;
}

Rk::WidgetAttribute RkWidget::RkWidgetImpl::defaultWidgetAttributes()
{
        return static_cast<Rk::WidgetAttribute>(static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled)
                                                | static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled)
                                                | static_cast<int>(Rk::WidgetAttribute::CloseInputEnabled));

}

void RkWidget::RkWidgetImpl::show(bool b)
{
	isWidgetShown = b;
        if (isTopWidget())
                systemWindow->show(isWidgetShown);
}

bool RkWidget::RkWidgetImpl::isShown() const
{
	return isWidgetShown;
}

void RkWidget::RkWidgetImpl::setTitle(const std::string &title)
{
        widgetTitle = title;
        if (isTopWidget())
                systemWindow->setTitle(widgetTitle);
}

const std::string& RkWidget::RkWidgetImpl::title() const
{
        return widgetTitle;
}

bool RkWidget::RkWidgetImpl::isClose() const
{
        return widgetClosed;
}

void RkWidget::RkWidgetImpl::event(RkEvent *event)
{
        RkObject::RkObjectImpl::event(event);
        switch (event->type())
        {
        case RkEvent::Type::Paint:
                processPaintEvent(static_cast<RkPaintEvent*>(event));
                break;
        case RkEvent::Type::KeyPressed:
                RK_LOG_DEBUG("RkEvent::Type::KeyPressed: " << title());
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled)) {
                        auto keyEvent = static_cast<RkKeyEvent*>(event);
                        if (keyEvent->isShortcut())
                                inf_ptr->shortcutEvent(keyEvent);
                        else
                                inf_ptr->keyPressEvent(keyEvent);
                }
                break;
        case RkEvent::Type::KeyReleased:
                RK_LOG_DEBUG("RkEvent::Type::KeyReleased: " << title());
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled)) {
                        auto keyEvent = static_cast<RkKeyEvent*>(event);
                        if (keyEvent->isShortcut())
                                inf_ptr->shortcutEvent(keyEvent);
                        else
                                inf_ptr->keyReleaseEvent(keyEvent);
                }
                break;
        case RkEvent::Type::FocusedIn:
                RK_LOG_DEBUG("RkEvent::Type::FocsedIn:" << title());
                inf_ptr->focusEvent(static_cast<RkFocusEvent*>(event));
                break;
        case RkEvent::Type::FocusedOut:
                RK_LOG_DEBUG("RkEvent::Type::FocsedOut: " << title());
                inf_ptr->focusEvent(static_cast<RkFocusEvent*>(event));
                break;
        case RkEvent::Type::MouseButtonPress:
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled))
                        inf_ptr->mouseButtonPressEvent(static_cast<RkMouseEvent*>(event));
                break;
        case RkEvent::Type::MouseDoubleClick:
                RK_LOG_DEBUG("RkEvent::Type::MouseDoubleClick:" << title());
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled))
                        inf_ptr->mouseDoubleClickEvent(static_cast<RkMouseEvent*>(event));
                break;
        case RkEvent::Type::MouseButtonRelease:
                RK_LOG_DEBUG("RkEvent::Type::MouseButtonRelease:" << title());
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled))
                        inf_ptr->mouseButtonReleaseEvent(static_cast<RkMouseEvent*>(event));
                break;
        case RkEvent::Type::MouseMove:
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled))
                        inf_ptr->mouseMoveEvent(static_cast<RkMouseEvent*>(event));
                break;
        case RkEvent::Type::Drop:
                RK_LOG_DEBUG("RkEvent::Type::Drop:" << title());
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled))
                        inf_ptr->dropEvent(static_cast<RkDropEvent*>(event));
                break;
        case RkEvent::Type::Hover:
                RK_LOG_DEBUG("RkEvent::Type::Hover:" << title());
                inf_ptr->hoverEvent(static_cast<RkHoverEvent*>(event));
                break;
        case RkEvent::Type::Resize:
                if (isTopWidget())
                        widgetSize = systemWindow->size();
                inf_ptr->resizeEvent(static_cast<RkResizeEvent*>(event));
                break;
	case RkEvent::Type::Show:
		isWidgetShown = true;
                inf_ptr->showEvent(static_cast<RkShowEvent*>(event));
                break;
	case RkEvent::Type::Hide:
		isWidgetShown = false;
                inf_ptr->hideEvent(static_cast<RkHideEvent*>(event));
                break;
        case RkEvent::Type::DeleteChild:
                RK_LOG_DEBUG("RkEvent::Type::DeleteChild:" << title());
                delete static_cast<RkDeleteChild*>(event)->child();
                break;
        case RkEvent::Type::Close:
                RK_LOG_DEBUG("RkEvent::Type::Close");
                if (static_cast<int>(widgetAttributes) & static_cast<int>(Rk::WidgetAttribute::CloseInputEnabled)) {
                        inf_ptr->closeEvent(static_cast<RkCloseEvent*>(event));
                        if (isTopWidget())
                                systemWindow->close();
                }
                break;
        default:
                break;
                RK_LOG_DEBUG("RkEvent::Type::Unknown:" << title());
        }
}

void RkWidget::RkWidgetImpl::processPaintEvent(RkPaintEvent* event)
{
        RkPainter painter(inf_ptr);
        auto globalPosition = inf_ptr->mapToGlobal({0, 0});
        painter.translate(globalPosition);
        painter.fillRect(rect(), background());
        inf_ptr->paintEvent(event);
        painter.translate({-globalPosition.x(), -globalPosition.y()});
        processChildrenEvents(event);
}

void RkWidget::RkWidgetImpl::processChildrenEvents(RkEvent *event)
{
        for (auto &ch: inf_ptr->children()) {
                auto widget = dynamic_cast<RkWidget*>(ch);
                if (widget) {
                        RK_IMPL_PTR(widget)->event(event);
                }
        }
}

void RkWidget::RkWidgetImpl::setSize(const RkSize &size)
{
        widgetSize = size;
        if (isTopWidget())
                systemWindow->setSize(widgetSize);
        RK_IMPL_PTR(getEventQueue())->postEvent(inf_ptr, std::move(std::make_unique<RkResizeEvent>()));
}

const RkSize& RkWidget::RkWidgetImpl::size() const
{
        return widgetSize;
}

void RkWidget::RkWidgetImpl::setMinimumSize(const RkSize &size)
{
        widgetMinimumSize = size;
}

const RkSize& RkWidget::RkWidgetImpl::minimumSize() const
{
        return widgetMinimumSize;
}

void RkWidget::RkWidgetImpl::setMaximumSize(const RkSize &size)
{
        widgetMinimumSize = size;
}

const RkSize& RkWidget::RkWidgetImpl::maximumSize() const
{
        return widgetMaximumSize;
}

void RkWidget::RkWidgetImpl::setPosition(const RkPoint &position)
{
        widgetPosition = position;
        if (isTopWidget())
                systemWindow->setPosition(widgetPosition);
}

const RkPoint& RkWidget::RkWidgetImpl::position() const
{
        return widgetPosition;
}

void RkWidget::RkWidgetImpl::setBorderWidth(int width)
{
        widgetBorderWidth = width;
}

int RkWidget::RkWidgetImpl::borderWidth() const
{
        return widgetBorderWidth;
}

void RkWidget::RkWidgetImpl::setBorderColor(const RkColor &color)
{
        widgetBorderColor = color;
}

const RkColor& RkWidget::RkWidgetImpl::borderColor() const
{
        return widgetBorderColor;
}

void RkWidget::RkWidgetImpl::setBackgroundColor(const RkColor &color)
{
        widgetBackground = color;
        if (isTopWidget())
                systemWindow->setBackgroundColor(widgetBackground);
}

const RkColor& RkWidget::RkWidgetImpl::background() const
{
        return widgetBackground;
}

RkRect RkWidget::RkWidgetImpl::rect() const
{
        return RkRect({0, 0}, size());
}

void RkWidget::RkWidgetImpl::update(bool updateChildren)
{
        RK_IMPL_PTR(getEventQueue())->postEvent(inf_ptr, std::move(std::make_unique<RkPaintEvent>()));
        if (updateChildren) {
                for (auto &ch: inf_ptr->children()) {
                        auto widget = dynamic_cast<RkWidget*>(ch);
                        if (widget)
                                RK_IMPL_PTR(widget)->update(updateChildren);
                }
        }
}

Rk::Modality RkWidget::RkWidgetImpl::modality() const
{
        return widgetModality;
}

void RkWidget::RkWidgetImpl::setWidgetAttribute(Rk::WidgetAttribute attribute)
{
        widgetAttributes = static_cast<Rk::WidgetAttribute>(static_cast<int>(widgetAttributes) | static_cast<int>(attribute));
}

void RkWidget::RkWidgetImpl::clearWidgetAttribute(Rk::WidgetAttribute attribute)
{
        widgetAttributes = static_cast<Rk::WidgetAttribute>(static_cast<int>(widgetAttributes) & (~static_cast<int>(attribute)));
}

Rk::WidgetAttribute RkWidget::RkWidgetImpl::getWidgetAttributes() const
{
        return widgetAttributes;
}

void RkWidget::RkWidgetImpl::setFocus(bool b)
{
        // TODO: implement?
}

bool RkWidget::RkWidgetImpl::hasFocus() const
{
        // TODO: implement?
        return false;
}

void RkWidget::RkWidgetImpl::setTextColor(const RkColor &color)
{
        widgetTextColor = color;
}

const RkColor& RkWidget::RkWidgetImpl::textColor() const
{
        return widgetTextColor;
}

const RkColor& RkWidget::RkWidgetImpl::color() const
{
        return widgetDrawingColor;
}

void RkWidget::RkWidgetImpl::setColor(const RkColor &color)
{
        widgetDrawingColor = color;
}

const RkFont& RkWidget::RkWidgetImpl::font() const
{
        return widgetFont;
}

void RkWidget::RkWidgetImpl::setFont(const RkFont &font)
{
        widgetFont = font;
}

void RkWidget::RkWidgetImpl::setPointerShape(Rk::PointerShape shape)
{
        widgetPointerShape = shape;
        if (isTopWidget())
                systemWindow->setPointerShape(widgetPointerShape);
}

Rk::PointerShape RkWidget::RkWidgetImpl::pointerShape() const
{
        return widgetPointerShape;
}

void RkWidget::RkWidgetImpl::enableGrabKey(bool b)
{
        isGrabKeyEnabled = b;
}

bool RkWidget::RkWidgetImpl::grabKeyEnabled() const
{
        return isGrabKeyEnabled;
}

void RkWidget::RkWidgetImpl::propagateGrabKey(bool b)
{
        isPropagateGrabKey = b;
}

bool RkWidget::RkWidgetImpl::propagateGrabKeyEnabled() const
{
        return isPropagateGrabKey;
}

bool RkWidget::RkWidgetImpl::pointerIsOverWindow() const
{
        // TODO: implement?
        return false;
}
