/**
 * File name: RkWindowWin.h
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

#ifndef RK_WINDOW_WIN_H
#define RK_WINDOW_WIN_H

#include "Rk.h"
#include "RkPlatform.h"
#include "RkSize.h"
#include "RkPoint.h"
#include "RkColor.h"

class RkEventQueue;
struct RkCanvasInfo;

constexpr int RK_MAIN_WINDOW_TIMER_ID = 101;

class RkWindowWin {
 public:
        explicit RkWindowWin(const RkNativeWindowInfo *parent,
                           Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                           bool isTop = false);
        explicit RkWindowWin(const RkNativeWindowInfo &parent,
                           Rk::WindowFlags flags = Rk::WindowFlags::Widget,
                           bool isTop = false);
        ~RkWindowWin();
		Rk::WindowFlags flags() const;
        RkWindowWin(const RkWindowWin &other) = delete;
        RkWindowWin& operator=(const RkWindowWin &other) = delete;
        RkWindowWin(RkWindowWin &&other) = delete;
        RkWindowWin& operator=(RkWindowWin &&other) = delete;
        bool init();
        void show(bool b = true);
        const RkNativeWindowInfo* nativeWindowInfo();
        void setTitle(const std::string &title);
        RkSize size() const;
        void setSize(const RkSize &winSize);
        RkPoint position() const;
        void setPosition(const RkPoint &pos);
        RkWindowId id() const;
        void setBorderWidth(int width);
        int borderWidth() const;
        void setBorderColor(const RkColor &color);
        const RkColor& borderColor() const;
        void setBackgroundColor(const RkColor &color);
        const RkColor& background() const;
        void resizeCanvas();
        const RkCanvasInfo* getCanvasInfo();
        void freeCanvasInfo();
        void update();
        void setFocus(bool b);
        bool hasFocus();
		bool pointerIsOverWindow() const;
        void setPointerShape(Rk::PointerShape shape);
        void setEventQueue(RkEventQueue* queue);
        void setScaleFactor(double factor);
        double getScaleFactor() const;

 protected:
        bool isWindowCreated() const;
        bool hasParent() const;
        void createCanvasInfo();

 private:
        RkNativeWindowInfo parentWindowInfo;
        std::unique_ptr<RkNativeWindowInfo> windowInfo;
        RkWindowId windowHandle;
        int winBorderWidth;
        RkColor winBorderColor;
        RkColor backgroundColor;
        RkEventQueue* eventQueue;
        mutable std::unique_ptr<RkCanvasInfo> canvasInfo;
        Rk::WindowFlags windowFlags;
        bool isTopWindow;
        double scaleFactor;
};

#endif // RK_WIDGET_WIN_H
