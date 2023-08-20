/**
 * File name: RkPainter.h
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

#ifndef RK_PAINTER_H
#define RK_PAINTER_H

#include "Rk.h"
#include "RkImage.h"
#include "RkPoint.h"
#include "RkPen.h"
#include "RkRect.h"
#include "RkFont.h"

class RkCanvas;

class RK_EXPORT RkPainter {
 public:
        RkPainter(RkCanvas *canvas);
        ~RkPainter();
        void drawText(int x, int y, const std::string &text);
        void drawText(const RkPoint &p,  const std::string &text);
        void drawText(const RkRect &rect,
                      const std::string &text,
                      Rk::Alignment alignment = Rk::Alignment::AlignCenter);
        void drawImage(const RkImage &image, int x, int y);
        void drawCircle(int x, int y, int radius);
        void drawCircle(const RkPoint &p, int radius);
        void drawLine(int x1, int y1, int x2, int y2);
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
        RK_DISABLE_COPY(RkPainter);
        RK_DISABLE_MOVE(RkPainter);
        RK_DECLARE_IMPL(RkPainter);
};

#endif // RK_PAINTER_H
