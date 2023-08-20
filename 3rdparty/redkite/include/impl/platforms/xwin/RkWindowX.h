/**
 * File name: RkWindowX.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/redkite)
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

#ifndef RK_WINDOW_X_H
#define RK_WINDOW_X_H

#include "Rk.h"
#include "RkPlatform.h"
#include "RkSize.h"
#include "RkPoint.h"
#include "RkColor.h"

#include <X11/Xutil.h>

struct RkCanvasInfo;

class RkWindowX {
 public:
        explicit RkWindowX(const RkNativeWindowInfo *parent,
                           Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                           bool isTop = false);
        explicit RkWindowX(const RkNativeWindowInfo &parent,
                           Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                           bool isTop = false);
        ~RkWindowX();
        Rk::WindowFlags flags() const;
        bool init();
        void show(bool b);
        const RkNativeWindowInfo* nativeWindowInfo() const;
        void setTitle(const std::string &title);
        Display* display() const;
        RkSize size() const;
        void setSize(const RkSize &size);
        RkPoint position() const;
        void setPosition(const RkPoint &position);
        RkWindowId id() const;
        void setBorderWidth(int width);
        int borderWidth() const;
        void setBorderColor(const RkColor &color);
        const RkColor& borderColor() const;
        void setBackgroundColor(const RkColor &color);
        const RkColor& background() const;
        void resizeCanvas();
        const RkCanvasInfo* getCanvasInfo() const;
        void update();
        void setFocus(bool b);
        bool hasFocus() const;
        void setPointerShape(Rk::PointerShape shape);
        bool pointerIsOverWindow() const;
        void setScaleFactor(double factor);
        double getScaleFactor() const;

 protected:
        bool openDisplay();
        bool isWindowCreated() const;
        bool hasParent() const;
        void createCanvasInfo();
        void freeCanvasInfo();

 private:
        RK_DISABLE_COPY(RkWindowX);
        RK_DISABLE_MOVE(RkWindowX);
        RkNativeWindowInfo parentWindowInfo;
        Rk::WindowFlags windowFlags;
        Display *xDisplay;
        int screenNumber;
        Window xWindow;
        Atom deleteWindowAtom;
        mutable RkPoint windowPosition;
        mutable RkSize windowSize;
        int winBorderWidth;
        RkColor winBorderColor;
        RkColor winBackgroundColor;
        std::unique_ptr<RkCanvasInfo> canvasInfo;
        std::unique_ptr<RkNativeWindowInfo> windowInfo;
        XVisualInfo visualInfo;
        double scaleFactor;
        bool isTopWindow;
};

#endif // RK_WIDGET_XWIN_H
