/**
 * File name: RkWidget.cpp
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

#include "RkLog.h"
#include "RkWidget.h"
#include "RkEvent.h"
#include "RkWidgetImpl.h"
#include "RkPlatform.h"
#include "RkMain.h"

RkWidget::RkWidget(RkMain *mainApp, Rk::WindowFlags flags)
        : RkObject(nullptr, std::make_unique<RkWidgetImpl>(this, nullptr, flags, true))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        mainApp->setTopLevelWidget(this);
}

RkWidget::RkWidget(RkMain *mainApp, const RkNativeWindowInfo &parent, Rk::WindowFlags flags)
        : RkObject(nullptr, std::make_unique<RkWidgetImpl>(this, parent, flags, true))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        mainApp->setTopLevelWidget(this);
}

RkWidget::RkWidget(RkWidget *parent, Rk::WindowFlags flags)
        : RkObject(parent, std::make_unique<RkWidgetImpl>(this, parent, flags))
        , impl_ptr{static_cast<RkWidgetImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called: " << this);
        if (modality() == Rk::Modality::ModalTopWidget) {
                auto topWidget = getTopWidget();
                if (topWidget)
                        topWidget->disableInput();
                else if (parentWidget() && modality() == Rk::Modality::ModalParent)
                        parentWidget()->disableInput();
        }
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
        }
}

Rk::WindowFlags RkWidget::windowFlags() const
{
        return impl_ptr->windowFlags();
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
        impl_ptr->show(b);
}

bool RkWidget::isShown() const
{
	return impl_ptr->isShown();
}

void RkWidget::hide()
{
        impl_ptr->show(false);
}

const RkNativeWindowInfo* RkWidget::nativeWindowInfo() const
{
        return impl_ptr->nativeWindowInfo();
}

RkWindowId RkWidget::id() const
{
        return impl_ptr->id();
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
        if (w > maximumWidth())
                w = maximumWidth();
        else if (w < minimumWidth())
                w = minimumWidth();

        if (h > maximumHeight())
                h = maximumHeight();
        else if (h < minimumHeight())
                h = minimumHeight();

        impl_ptr->setSize(RkSize(w, h));
}

void RkWidget::setSize(const RkSize &size)
{
        impl_ptr->setSize(size);
}

RkSize RkWidget::size() const
{
        return impl_ptr->size();
}

void RkWidget::setWidth(int w)
{
        if (w > maximumWidth())
                impl_ptr->setSize(RkSize(maximumWidth(), impl_ptr->size().height()));
        else if (w < minimumWidth())
                impl_ptr->setSize(RkSize(minimumWidth(), impl_ptr->size().height()));
        else
                impl_ptr->setSize(RkSize(w, impl_ptr->size().height()));
}

int RkWidget::width() const
{
        return impl_ptr->size().width();
}

int RkWidget::minimumWidth() const
{
        return impl_ptr->minimumWidth();
}

int RkWidget::maximumWidth() const
{
        return impl_ptr->maximumWidth();
}

void RkWidget::setHeight(int h)
{
        if (h > maximumHeight())
                impl_ptr->setSize(RkSize(impl_ptr->size().width(),  maximumHeight()));
        else if (h < minimumHeight())
                impl_ptr->setSize(RkSize(impl_ptr->size().width(),  minimumHeight()));
        else
                impl_ptr->setSize(RkSize(impl_ptr->size().width(),  h));
}

int RkWidget::height() const
{
        return impl_ptr->size().height();
}

int RkWidget::minimumHeight() const
{
        return impl_ptr->minimumHeight();
}

int RkWidget::maximumHeight() const
{
        return impl_ptr->maximumHeight();
}

void RkWidget::setMinimumWidth(int width)
{
        impl_ptr->setMinimumWidth(width);
}

void RkWidget::setMinimumHeight(int height)
{
        impl_ptr->setMinimumHeight(height);
}

void RkWidget::setMaximumWidth(int width)
{
        impl_ptr->setMaximumWidth(width);
}

void RkWidget::setMaximumHeight(int height)
{
        impl_ptr->setMaximumHeight(height);
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

RkPoint RkWidget::position() const
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
        if (red > 255)
                red = 255;
        if (red < 0)
                red = 0;

        if (green > 255)
                green = 255;
        if (green < 0)
                green = 0;

        if (blue > 255)
                blue = 255;
        if (blue < 0)
                blue = 0;
        impl_ptr->setBorderColor(RkColor(red, green, blue));
}

const RkColor& RkWidget::borderColor() const
{
        return impl_ptr->borderColor();
}

void RkWidget::setBackgroundColor(int red, int green, int blue)
{
        if (red > 255)
                red = 255;
        if (red < 0)
                red = 0;

        if (green > 255)
                green = 255;
        if (green < 0)
                green = 0;

        if (blue > 255)
                blue = 255;
        if (blue < 0)
                blue = 0;

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

void RkWidget::event(RkEvent *event)
{
        impl_ptr->event(event);
        RkObject::event(event);
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

const RkCanvasInfo* RkWidget::getCanvasInfo() const
{
        return impl_ptr->getCanvasInfo();
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

void RkWidget::setScaleFactor(double factor)
{
        impl_ptr->setScaleFactor(factor);
        for (const auto &ch: children()) {
                auto widget = dynamic_cast<RkWidget*>(ch);
                if (widget)
                        widget->setScaleFactor(factor);
        }

        if (this == getTopWidget())
            eventQueue()->setScaleFactor(factor);
        update();
}

double RkWidget::scaleFactor() const
{
        return impl_ptr->scaleFactor();
}


bool RkWidget::pointerIsOverWindow() const
{
        return impl_ptr->pointerIsOverWindow();
}
