/**
 * File name: RkWidget.h
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

#ifndef RK_WIDGET_H
#define RK_WIDGET_H

#include "Rk.h"
#include "RkObject.h"
#include "RkEventQueue.h"
#include "RkCanvas.h"
#include "RkRect.h"
#include "RkColor.h"
#include "RkFont.h"

class RkEvent;
class RkCloseEvent;
class RkKeyEvent;
class RkMouseEvent;
class RkWheelEvent;
class RkDropEvent;
class RkMoveEvent;
class RkResizeEvent;
class RkPaintEvent;
class RkShowEvent;
class RkHideEvent;
class RkFocusEvent;
class RkHoverEvent;
class RkMain;
class RkNativeWindowInfo;

class RkWidget: public RkObject, public RkCanvas {
 public:
        explicit RkWidget(RkMain &mainApp, Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit RkWidget(RkMain &mainApp,
                          const RkNativeWindowInfo &parent,
                          Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit RkWidget(RkWidget *parent,
                          Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        virtual ~RkWidget();
        RkCanvasInfo* getCanvasInfo() const override;
        void freeCanvasInfo() override;
        Rk::WidgetFlags widgetFlags() const;
        void show(bool b = true);
        void hide();
        void setTitle(const std::string &title);
        const std::string& title() const;
        bool isClose() const;
        RkWidget* parentWidget() const;
        void setSize(int w, int h);
        void setSize(const RkSize &size);
        const RkSize& size() const;
        void setMinimumSize(const RkSize &size);
        const RkSize& minimumSize() const;
        void setMaximumSize(const RkSize & size);
        const RkSize& maximumSize() const;
        void setWidth(int w);
        int width() const;
        int minimumWidth() const;
        int maximumWidth() const;
        void setMinimumWidth(int width);
        void setMaximumWidth(int width);
        void setHeight(int h);
        int height() const;
        int minimumHeight() const;
        int maximumHeight() const;
        void setMinimumHeight(int height);
        void setMaximumHeight(int height);
        void setFixedWidth(int width);
        void setFixedHeight(int height);
        void setFixedSize(const RkSize &size);
        void setFixedSize(int width, int height);
        int x() const;
        void setX(int x);
        int y() const;
        void setY(int y);
        void setPosition(int x, int y);
        void setPosition(const RkPoint &p);
        const RkPoint& position() const;
        void setBorderWidth(int width);
        int borderWidth() const;
        void setBackgroundColor(int red, int green, int blue);
        void setBackgroundColor(const RkColor &color);
        const RkColor& background() const;
        void setBorderColor(int red, int green, int blue);
        const RkColor& borderColor() const;
        void setTextColor(const RkColor &color);
        const RkColor& textColor() const;
        const RkColor& color() const;
        void setColor(const RkColor &color);
        const RkFont& font() const;
        void setFont(const RkFont &font);
        RkRect rect() const;
        void update();
        void close();
        Rk::Modality modality() const;
        bool isModal() const;
        void setWidgetAttribute(Rk::WidgetAttribute attribute);
        void clearWidgetAttribute(Rk::WidgetAttribute attribute);
        Rk::WidgetAttribute widgetAttributes() const;
        void enableInput();
        void disableInput();
        bool isInputEnabled() const;
        RkWidget* getTopWidget();
        bool isTopWindow() const;
        void enableGrabKey(bool b);
        bool grabKeyEnabled() const;
        void propagateGrabKey(bool b);
        bool propagateGrabKeyEnabled() const;
        void setFocus(bool b = true);
        bool hasFocus() const;
        void setPointerShape(Rk::PointerShape shape);
        Rk::PointerShape pointerShape() const;
        bool pointerIsOverWindow() const;
        bool isChild(RkWidget *widget);
        RkPoint mapToGlobal(const RkPoint& p) const;
        RkPoint mapToLocal(const RkPoint& p) const;
        void setVisible(bool b = true);
        bool isVisible() const;
        bool isAncestorOf(RkWidget *child);
        void setScaleFactor(double factor);
        double scaleFactor() const;

 protected:
        RK_DECLARE_IMPL_PTR(RkWidget);
        RkWidget(RkWidget *parent, std::unique_ptr<RkWidgetImpl> impl);
        virtual void closeEvent(RkCloseEvent *event);
        virtual void keyPressEvent(RkKeyEvent *event);
        virtual void keyReleaseEvent(RkKeyEvent *event);
        virtual void shortcutEvent(RkKeyEvent *event);
        virtual void mouseMoveEvent(RkMouseEvent *event);
        virtual void mouseButtonPressEvent(RkMouseEvent *event);
        virtual void mouseButtonReleaseEvent(RkMouseEvent *event);
        virtual void mouseDoubleClickEvent(RkMouseEvent *event);
        virtual void wheelEvent(RkWheelEvent *event);
        virtual void dropEvent(RkDropEvent *event);
        virtual void moveEvent(RkMoveEvent *event);
        virtual void resizeEvent(RkResizeEvent *event);
        virtual void paintEvent(RkPaintEvent *event);
        virtual void showEvent(RkShowEvent *event);
        virtual void hideEvent(RkHideEvent *event);
        virtual void focusEvent(RkFocusEvent *event);
        virtual void hoverEvent(RkHoverEvent *event);

 private:
        friend class RkEventQueue;
        RK_DISABLE_COPY(RkWidget);
        RK_DISABLE_MOVE(RkWidget);
};

#endif // RK_WIDGET_H
