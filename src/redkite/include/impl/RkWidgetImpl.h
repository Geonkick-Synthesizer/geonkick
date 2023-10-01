/**
 * File name: RkWidgetImpl.h
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

#ifndef RK_WIDGET_IMPL_H
#define RK_WIDGET_IMPL_H

#include "RkWidget.h"
#include "RkObjectImpl.h"

#ifdef RK_OS_WIN
class RkWindowWin;
#elif RK_OS_MAC
#else
class RkWindowX;
#endif // RK_WIN_OS

class RkWidget::RkWidgetImpl : public RkObject::RkObjectImpl {
 public:
        explicit RkWidgetImpl(RkWidget* interface,
                              RkWidget* parent = nullptr,
                              Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                              bool isTopWindow = false);
        explicit RkWidgetImpl(RkWidget* interface,
                              const RkNativeWindowInfo &parent,
                              Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                              bool isTopWindow = false);
        RkWidgetImpl(const RkWidget &other) = delete;
        RkWidgetImpl& operator=(const RkWidgetImpl &other) = delete;
        RkWidgetImpl(RkWidgetImpl &&other) = delete;
        RkWidgetImpl& operator=(RkWidgetImpl &&other) = delete;
        virtual ~RkWidgetImpl();
        void setEventQueue(RkEventQueue *queue) override;
        Rk::WindowFlags windowFlags() const;
        void show(bool b);
	bool isShown() const;
        void setTitle(const std::string &title);
        const std::string& title() const;
        const RkNativeWindowInfo* nativeWindowInfo() const;
        bool isClose() const;
        RkWindowId id() const;
        void event(RkEvent *event);
        void setSize(const RkSize &size);
        RkSize size() const;
        int minimumWidth() const;
        int maximumWidth() const;
        int minimumHeight() const;
        int maximumHeight() const;
        void setMinimumWidth(int width);
        void setMaximumWidth(int widht);
        void setMinimumHeight(int heigth);
        void setMaximumHeight(int heigth);
        void setPosition(const RkPoint &position);
        RkPoint position() const;
        void setBorderWidth(int width);
        int borderWidth() const;
        void setBorderColor(const RkColor &color);
        const RkColor &borderColor() const;
        void setBackgroundColor(const RkColor &color);
        const RkColor& background() const;
        RkRect rect() const;
        const RkCanvasInfo* getCanvasInfo() const;
        void freeCanvasInfo();
        void update();
        static Rk::WidgetAttribute defaultWidgetAttributes();
        Rk::Modality modality() const;
        void setWidgetAttribute(Rk::WidgetAttribute attribute);
        void clearWidgetAttribute(Rk::WidgetAttribute attribute);
        Rk::WidgetAttribute getWidgetAttributes() const;
        void setFocus(bool b);
        bool hasFocus() const;
        void setTextColor(const RkColor &color);
        const RkColor& textColor() const;
        const RkColor& color() const;
        void setColor(const RkColor &color);
        const RkFont& font() const;
        void setFont(const RkFont &font);
        void setPointerShape(Rk::PointerShape shape);
        Rk::PointerShape pointerShape() const;
        void enableGrabKey(bool b);
        bool grabKeyEnabled() const;
        void propagateGrabKey(bool b);
        bool propagateGrabKeyEnabled() const;
        bool pointerIsOverWindow() const;
        void setScaleFactor(double factor);
        double scaleFactor() const;

 private:
        RK_DECALRE_INTERFACE_PTR(RkWidget);
#ifdef RK_OS_WIN
        std::unique_ptr<RkWindowWin> platformWindow;
#elif RK_OS_MAC
#else
        std::unique_ptr<RkWindowX> platformWindow;
#endif // RK_WIN_OS
        std::string widgetTitle;
        //        std::list<RkWidget*> widgetChildren;
        bool widgetClosed;
        RkSize widgetMinimumSize;
        RkSize widgetMaximumSize;
        RkSize widgetSize;
        RkColor widgetBackground;
        Rk::WidgetAttribute widgetAttributes;
        Rk::Modality widgetModality;
        RkColor widgetTextColor;
        RkColor widgetDrawingColor;
        RkFont widgetFont;
        Rk::PointerShape widgetPointerShape;
	bool isWidgetSown;
        bool isGrabKeyEnabled;
        bool isPropagateGrabKey;
};

#endif // RK_WIDGET_IMPL_H
