/**
 * File name: RkWidget.cpp
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
#include "RkWidget.h"
#include "RkEvent.h"
#include "RkWidgetImpl.h"
#include "RkEventQueueImpl.h"
#include "RkPlatform.h"
#include "RkMain.h"
#include "RkMainImpl.h"

RkWidget::RkWidget(RkMain& mainApp, Rk::WidgetFlags flags)
        : RkObject(nullptr, std::make_unique<RkWidgetImpl>(this, &mainApp, nullptr, flags))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        RK_IMPL_PTR((&mainApp))->setTopWidget(this, nullptr);
        update();
}

RkWidget::RkWidget(RkMain& mainApp, const RkNativeWindowInfo &parent, Rk::WidgetFlags flags)
        : RkObject(nullptr, std::make_unique<RkWidgetImpl>(this, &mainApp, &parent, flags))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        RK_IMPL_PTR((&mainApp))->setTopWidget(this, &parent);
        update();
}

RkWidget::RkWidget(RkWidget *parent, Rk::WidgetFlags flags)
        : RkObject(parent, std::make_unique<RkWidgetImpl>(this, parent, flags))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        if (flags == Rk::WidgetFlags::Popup)
                RK_IMPL_PTR(eventQueue())->addPopup(this);

        if (modality() == Rk::Modality::ModalTopWidget) {
                auto topWidget = getTopWidget();
                if (topWidget)
                        topWidget->disableInput();
                else if (parentWidget() && modality() == Rk::Modality::ModalParent)
                        parentWidget()->disableInput();
        }
        update();
}

RkWidget::RkWidget(RkWidget *parent, std::unique_ptr<RkWidgetImpl> impl)
        : RkObject(parent, std::move(impl))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        if (modality() == Rk::Modality::ModalTopWidget) {
                auto topWidget = getTopWidget();
                if (topWidget)
                        topWidget->disableInput();
                else if (parentWidget() && modality() == Rk::Modality::ModalParent)
                        parentWidget()->disableInput();
        }
}

RkWidget::~RkWidget()
{
        RK_LOG_DEBUG("called: " << this);
        if (widgetFlags() == Rk::WidgetFlags::Popup)
                RK_IMPL_PTR(eventQueue())->removePopup(this);
        if (hasFocus())
                setFocus(false);
        if (parentWidget()) {
                if (modality() == Rk::Modality::ModalTopWidget) {
                        if (!parentWidget()->isModal()) {
                                auto topWidget = getTopWidget();
                                if (topWidget)
                                        topWidget->enableInput();
                        } else {
                                // Enable inputs only for parent widget and its
                                // childs since it is modal.
                                parentWidget()->enableInput();
                        }
                } else if (modality() == Rk::Modality::ModalParent) {
                        parentWidget()->enableInput();
                }
                parentWidget()->update();
        }
}

RkCanvasInfo* RkWidget::getCanvasInfo() const
{
        return impl_ptr->getCanvasInfo();
}

void RkWidget::freeCanvasInfo()
{
        impl_ptr->freeCanvasInfo();
}

Rk::WidgetFlags RkWidget::widgetFlags() const
{
        return impl_ptr->getWidgetFlags();
}

void RkWidget::setTitle(const std::string &title)
{
        impl_ptr->setTitle(title);
}

const std::string& RkWidget::title() const
{
        return impl_ptr->title();
}


void RkWidget::show(bool b)
{
        if (!name().empty())
                RK_LOG_DEV_DEBUG("name" << name() << ", show: " << b);
        setVisible(b);
}

void RkWidget::hide()
{
        show(false);
}

RkWidget* RkWidget::parentWidget() const
{
        return dynamic_cast<RkWidget*>(parent());
}

bool RkWidget::isClose() const
{
        return impl_ptr->isClose();
}

void RkWidget::setSize(int w, int h)
{
        impl_ptr->setSize(RkSize(w, h));
}

void RkWidget::setSize(const RkSize &size)
{
        setSize(size.width(), size.height());
}

const RkSize& RkWidget::size() const
{
        return impl_ptr->size();
}

void RkWidget::setMinimumSize(const RkSize &size)
{
        impl_ptr->setMinimumSize(size);
}

const RkSize& RkWidget::minimumSize() const
{
        return impl_ptr->minimumSize();
}

void RkWidget::setMaximumSize(const RkSize & size)
{
        return impl_ptr->setMaximumSize(size);
}

const RkSize& RkWidget::maximumSize() const
{
        return impl_ptr->maximumSize();
}

void RkWidget::setWidth(int w)
{
        setSize(RkSize(w, impl_ptr->size().height()));
}

int RkWidget::width() const
{
        return size().width();
}

int RkWidget::minimumWidth() const
{
        return impl_ptr->minimumSize().width();
}

int RkWidget::maximumWidth() const
{
        return impl_ptr->maximumSize().width();
}

void RkWidget::setHeight(int h)
{
        setSize(RkSize(impl_ptr->size().width(),  h));
}

int RkWidget::height() const
{
        return size().height();
}

int RkWidget::minimumHeight() const
{
        return impl_ptr->minimumSize().height();
}

int RkWidget::maximumHeight() const
{
        return impl_ptr->maximumSize().height();
}

void RkWidget::setMinimumWidth(int width)
{
        impl_ptr->setMinimumSize({width, minimumSize().height()});
}

void RkWidget::setMinimumHeight(int height)
{
        impl_ptr->setMinimumSize({minimumSize().width(), height});
}

void RkWidget::setMaximumWidth(int width)
{
        impl_ptr->setMaximumSize({width, maximumSize().height()});
}

void RkWidget::setMaximumHeight(int height)
{
        impl_ptr->setMaximumSize({ maximumSize().width(), height});
}

void RkWidget::setFixedWidth(int width)
{
        setMinimumWidth(width);
        setMaximumWidth(width);
        setWidth(width);
}

void RkWidget::setFixedHeight(int height)
{
        setMinimumHeight(height);
        setMaximumHeight(height);
        setHeight(height);
}

void RkWidget::setFixedSize(const RkSize &size)
{
        setMinimumWidth(size.width());
        setMaximumWidth(size.width());
        setMinimumHeight(size.height());
        setMaximumHeight(size.height());
        setSize(size);
}

void RkWidget::setFixedSize(int width, int height)
{
        setFixedSize(RkSize(width, height));
}

int RkWidget::x() const
{
        return impl_ptr->position().x();
}

void RkWidget::setX(int x)
{
        impl_ptr->setPosition(RkPoint(x, impl_ptr->position().y()));
}

int RkWidget::y() const
{
        return impl_ptr->position().y();
}

void RkWidget::setY(int y)
{
        return impl_ptr->setPosition(RkPoint(impl_ptr->position().x(), y));
}

void RkWidget::setPosition(int x, int y)
{
        impl_ptr->setPosition(RkPoint(x, y));
}

void RkWidget::setPosition(const RkPoint &p)
{
        impl_ptr->setPosition(p);
}

const RkPoint& RkWidget::position() const
{
        return impl_ptr->position();
}

void RkWidget::setBorderWidth(int width)
{
        impl_ptr->setBorderWidth(width);
}

int RkWidget::borderWidth() const
{
        return impl_ptr->borderWidth();
}

void RkWidget::setBorderColor(int red, int green, int blue)
{
        red   = std::clamp(red, 0, 255);
        green = std::clamp(green, 0, 255);
        blue  = std::clamp(blue, 0, 255);
        impl_ptr->setBorderColor(RkColor(red, green, blue));
}

const RkColor& RkWidget::borderColor() const
{
        return impl_ptr->borderColor();
}

void RkWidget::setBackgroundColor(int red, int green, int blue)
{
        red   = std::clamp(red, 0, 255);
        green = std::clamp(green, 0, 255);
        blue  = std::clamp(blue, 0, 255);
        impl_ptr->setBackgroundColor(RkColor(red, green, blue));
}

void RkWidget::setBackgroundColor(const RkColor &color)
{
        impl_ptr->setBackgroundColor(color);
}

const RkColor& RkWidget::background() const
{
        return impl_ptr->background();
}

void RkWidget::setTextColor(const RkColor &color)
{
        return impl_ptr->setTextColor(color);
}

const RkColor& RkWidget::textColor() const
{
        return impl_ptr->textColor();
}

const RkColor& RkWidget::color() const
{
        return impl_ptr->color();
}

void RkWidget::setColor(const RkColor &color)
{
        return impl_ptr->setColor(color);
}

const RkFont& RkWidget::font() const
{
        return impl_ptr->font();
}

void RkWidget::setFont(const RkFont &font)
{
        impl_ptr->setFont(font);
}

void RkWidget::enableInput()
{
        setWidgetAttribute(static_cast<Rk::WidgetAttribute>(static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled)
                           | static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled)
                           | static_cast<int>(Rk::WidgetAttribute::CloseInputEnabled)));
        for (const auto &ch: children()) {
                auto widget = dynamic_cast<RkWidget*>(ch);
                if (widget)
                        widget->enableInput();
        }
}

void RkWidget::disableInput()
{
        if (modality() == Rk::Modality::ModalParent || modality() == Rk::Modality::ModalTopWidget)
                return;

        clearWidgetAttribute(static_cast<Rk::WidgetAttribute>(static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled)
                            | static_cast<int>(Rk::WidgetAttribute::MouseInputEnabled)
                            | static_cast<int>(Rk::WidgetAttribute::CloseInputEnabled)));
        for (const auto &ch: children()) {
                auto widget = dynamic_cast<RkWidget*>(ch);
                if (widget)
                        widget->disableInput();
        }
}

bool RkWidget::isInputEnabled() const
{
        return  static_cast<int>(widgetAttributes()) & static_cast<int>(Rk::WidgetAttribute::KeyInputEnabled);
}

void RkWidget::enableGrabKey(bool b)
{
        // For now only for top level window.
        if (!parentWidget())
                impl_ptr->enableGrabKey(b);
}

bool RkWidget::grabKeyEnabled() const
{
        return impl_ptr->grabKeyEnabled();
}

void RkWidget::propagateGrabKey(bool b)
{
        impl_ptr->propagateGrabKey(b);
}

bool RkWidget::propagateGrabKeyEnabled() const
{
        return impl_ptr->propagateGrabKeyEnabled();
}

void RkWidget::closeEvent([[maybe_unused]] RkCloseEvent *event)
{
        if (parentWidget()) {
                auto event = std::make_unique<RkDeleteChild>(parentWidget(), this);
                eventQueue()->postEvent(parentWidget(), std::move(event));
        }
}

void RkWidget::keyPressEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::keyReleaseEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::shortcutEvent(RkKeyEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::mouseMoveEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        RK_LOG_DEV_DEBUG(title() << ": " << event->point().x() << ", y: " << event->point().y());
        RK_UNUSED(event);
}

void RkWidget::mouseButtonReleaseEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::mouseDoubleClickEvent(RkMouseEvent *event)
{
        mouseButtonPressEvent(event);
}

void RkWidget::wheelEvent(RkWheelEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::dropEvent(RkDropEvent *event)
{
        if (parentWidget())
                getTopWidget()->dropEvent(event);
}

void RkWidget::moveEvent(RkMoveEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::resizeEvent(RkResizeEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::paintEvent(RkPaintEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::showEvent(RkShowEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::hideEvent(RkHideEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::focusEvent([[maybe_unused]] RkFocusEvent *event)
{
        update();
}

void RkWidget::hoverEvent(RkHoverEvent *event)
{
        RK_UNUSED(event);
}

void RkWidget::update()
{
        impl_ptr->update();
}

RkRect RkWidget::rect() const
{
        return impl_ptr->rect();
}

void RkWidget::close()
{
        eventQueue()->postEvent(this, std::make_unique<RkCloseEvent>());
}

bool RkWidget::isModal() const
{
        return modality() != Rk::Modality::NonModal;
}

Rk::Modality RkWidget::modality() const
{
        return impl_ptr->modality();
}

void RkWidget::setWidgetAttribute(Rk::WidgetAttribute attribute)
{
        impl_ptr->setWidgetAttribute(attribute);
}

void RkWidget::clearWidgetAttribute(Rk::WidgetAttribute attribute)
{
        impl_ptr->clearWidgetAttribute(attribute);
}

Rk::WidgetAttribute RkWidget::widgetAttributes() const
{
        return impl_ptr->getWidgetAttributes();
}

RkWidget* RkWidget::getTopWidget()
{
        if (!parentWidget())
                return this;
        return parentWidget()->getTopWidget();
}

bool RkWidget::isTopWindow() const
{
        return !parentWidget();
}

void RkWidget::setFocus(bool b)
{
        impl_ptr->setFocus(b);
}

bool RkWidget::hasFocus() const
{
        return impl_ptr->hasFocus();
}

void RkWidget::setPointerShape(Rk::PointerShape shape)
{
        impl_ptr->setPointerShape(shape);
}

Rk::PointerShape RkWidget::pointerShape() const
{
        return impl_ptr->pointerShape();
}

bool RkWidget::pointerIsOverWindow() const
{
        return impl_ptr->pointerIsOverWindow();
}

bool RkWidget::isChild(RkWidget *widget)
{
        for (const auto &child: children()) {
                if (widget == dynamic_cast<RkWidget*>(child))
                        return true;
        }
        return false;
}

RkPoint RkWidget::mapToGlobal(const RkPoint& p) const
{
        return impl_ptr->isTopWidget() ? p + position()
                : p + parentWidget()->mapToGlobal(position());
}

RkPoint RkWidget::mapToLocal(const RkPoint& p) const
{
        return impl_ptr->isTopWidget() ? p - position()
                : p - parentWidget()->mapToGlobal(position());
}

void RkWidget::setVisible(bool b)
{
        if (!name().empty())
                RK_LOG_DEV_DEBUG(": " << name() << ": called");

        impl_ptr->setExplicitHidden(!b);
        if (b && parentWidget() && !parentWidget()->isVisible()) {
                if (!name().empty())
                        RK_LOG_DEV_DEBUG(": " << name() << ": show ignored");
                return;
        }

        impl_ptr->setVisible(b);
        if (!b) {
                eventQueue()->postEvent(this,
                                        std::move(std::make_unique<RkHideEvent>()));
        } else {
                eventQueue()->postEvent(this,
                                std::move(std::make_unique<RkShowEvent>()));
        }

        if (!b && parentWidget()) {
                eventQueue()->postEvent(parentWidget(),
                                        std::move(std::make_unique<RkPaintEvent>()));
        } else {
                eventQueue()->postEvent(this,
                                        std::move(std::make_unique<RkPaintEvent>()));
        }

        RK_IMPL_PTR(this)->setChildrenVisible(b);
        if (!name().empty())
                RK_LOG_DEV_DEBUG(": " << name() << ": exit");
}

bool RkWidget::isVisible() const
{
        return impl_ptr->isVisible();
}

bool RkWidget::isAncestorOf(RkWidget *child)
{
        if (!child)
                return false;

        if (RK_IMPL_PTR(this)->isTopWidget() || child->parentWidget() == this)
                return true;

        return isAncestorOf(child->parentWidget());
}
