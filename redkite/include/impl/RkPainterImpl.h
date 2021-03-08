/**
 * File name: RkPainterImpl.cpp
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

#ifndef RK_PAINTER_IMPL_H
#define RK_PAINTER_IMPL_H

#include "RkPainter.h"

#ifdef RK_GRAPHICS_CAIRO_BACKEND
class RkCairoGraphicsBackend;
#else
#error No graphics backend defined
#endif

class RkPainter::RkPainterImpl {
 public:
        RkPainterImpl(RkPainter* interface, RkCanvas* canvas);
        ~RkPainterImpl();
        void drawText(const std::string &text, int x, int y);
        void drawImage(const std::string &file, int x, int y);
        void drawImage(const RkImage &image, int x, int y);
        void drawEllipse(const RkPoint& p, int width, int height);
        void drawLine(const RkPoint &p1, const RkPoint &p2);
        void drawRect(const RkRect &rect);
        void drawPolyline(const std::vector<RkPoint> &points);
        void fillRect(const RkRect &rect, const RkColor &color);
        void applyAlpha(int alpha);
        const RkPen& pen() const;
        void setPen(const RkPen &pen);
        const RkFont& font() const;
        void setFont(const RkFont &font);
        void translate(const RkPoint &offset);
        void rotate(rk_real angle);
        int getTextWidth(const std::string &text) const;

 private:
        RK_DECALRE_INTERFACE_PTR(RkPainter);
#ifdef RK_GRAPHICS_CAIRO_BACKEND
        std::unique_ptr<RkCairoGraphicsBackend> backendGraphics;
#else
#error No graphics backend defined
#endif
        RkPen painterPen;
        RkFont painterFont;
};

#endif // RK_PAINTER_IMPL_H
