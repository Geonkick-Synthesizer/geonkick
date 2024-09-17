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

class RkMain;
class RkSystemWindow;
class RkPaintEvent;

class RkWidget::RkWidgetImpl : public RkObject::RkObjectImpl {
 public:
        explicit RkWidgetImpl(RkWidget* inf,
                              RkMain *mainApp,
                              const RkNativeWindowInfo* parent = nullptr,
                              Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit RkWidgetImpl(RkWidget* inf,
                              RkWidget* parent,
                              Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        virtual ~RkWidgetImpl();
        RkWidgetImpl(const RkWidget &other) = delete;
        RkWidgetImpl& operator=(const RkWidgetImpl &other) = delete;
        RkWidgetImpl(RkWidgetImpl &&other) = delete;
        RkWidgetImpl& operator=(RkWidgetImpl &&other) = delete;
        bool isTopWidget() const;
        RkCanvasInfo* getCanvasInfo() const;
        void setSystemWindow(RkSystemWindow *window);
        RkSystemWindow* getSystemWindow() const;
        void setEventQueue(RkEventQueue *queue) override;
        Rk::WidgetFlags getWidgetFlags() const;
        void setVisible(bool b);
	bool isVisible() const;
        void setTitle(const std::string &title);
        const std::string& title() const;
        bool isClose() const;
        void event(RkEvent *event) override;
        void setSize(const RkSize &size);
        const RkSize& size() const;
        void setMinimumSize(const RkSize& size);
        const RkSize& minimumSize() const;
        void setMaximumSize(const RkSize& size);
        const RkSize& maximumSize() const;
        void setPosition(const RkPoint &position);
        const RkPoint& position() const;
        void setBorderWidth(int width);
        int borderWidth() const;
        void setBorderColor(const RkColor &color);
        const RkColor &borderColor() const;
        void setBackgroundColor(const RkColor &color);
        const RkColor& background() const;
        RkRect rect() const;
        void update(bool updateChildren = false);
        static Rk::WidgetAttribute defaultWidgetAttributes();
        Rk::Modality modality() const;
        void setWidgetAttribute(Rk::WidgetAttribute attribute);
        void clearWidgetAttribute(Rk::WidgetAttribute attribute);
        Rk::WidgetAttribute getWidgetAttributes() const;
        void setFocus(bool b = true);
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
        void setScaleFactor(double factor);
        double scaleFactor() const;
        void setExplicitHidden(bool b = true);
        bool isExplicitHidden() const;
        void setChildrenVisible(bool b);

protected:
        void processPaintEvent(RkPaintEvent* event);
        void processChildrenEvents(RkEvent *event);

 private:
        RK_DECALRE_INTERFACE_PTR(RkWidget);
        bool topWidget;
        RkSystemWindow* systemWindow;
        std::string widgetTitle;
        bool widgetClosed;
        RkSize widgetMinimumSize;
        RkSize widgetMaximumSize;
        RkSize widgetSize;
        RkPoint widgetPosition;
        int widgetBorderWidth;
        RkColor widgetBorderColor;
        RkColor widgetBackground;
        Rk::WidgetAttribute widgetAttributes;
        Rk::WidgetFlags widgetFlags;
        Rk::Modality widgetModality;
        RkColor widgetTextColor;
        RkColor widgetDrawingColor;
        RkFont widgetFont;
        Rk::PointerShape widgetPointerShape;
        bool isWidgetExplicitHidden;
        bool isWidgetVisible;
        bool widgetHasFocus;
};

#endif // RK_WIDGET_IMPL_H
