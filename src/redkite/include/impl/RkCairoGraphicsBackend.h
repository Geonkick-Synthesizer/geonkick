/**
 * File name: RkCairoGraphicsBackend.h
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

#ifndef RK_CAIRO_GRAPHICS_BACKEND_H
#define RK_CAIRO_GRAPHICS_BACKEND_H

#include "Rk.h"
#include "RkImage.h"
#include "RkPen.h"
#include "RkRect.h"
#include "RkFont.h"

#include <cairo/cairo.h>

class RkPoint;
class RkRealPoint;
class RkCanvas;

class RkCairoGraphicsBackend {
 public:
        RkCairoGraphicsBackend(RkCanvas* canvas);
        ~RkCairoGraphicsBackend();
        void drawText(const std::string &text, int x, int y);
        void drawImage(const std::string &file, int x, int y);
        void drawImage(const RkImage &image, int x, int y);
        void drawEllipse(const RkPoint& p, int width, int height);
        void drawLine(const RkPoint &p1, const RkPoint &p2);
        void drawRect(const RkRect &rect);
        void drawPolyLine(const std::vector<RkPoint> &points);
        void drawCurve(const std::vector<RkPoint> &points);
        void drawPolyLine(const std::vector<RkRealPoint> &points);
        void drawCurve(const std::vector<RkRealPoint> &points);
        void fillRect(const RkRect &rect, const RkColor &color);
        void applyAlpha(int alpha);
        void setPen(const RkPen &pen);
        void setFont(const RkFont &font);
        int getTextWidth(const std::string &text) const;
        void translate(const RkPoint &offset);
        void rotate(rk_real angle);
        void scale(double x, double y);

 protected:
        cairo_t* context() const;

 private:
        RkCanvas *canvas;
};

#endif // RK_CAIRO_GRAPHICS_BACKEND_H
