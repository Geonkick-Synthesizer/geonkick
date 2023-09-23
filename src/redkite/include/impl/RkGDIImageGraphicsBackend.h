/**
 * File name: RkGDIGraphicsBackend.h
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

#ifndef RK_GDI_IMAGE_GRAPHICS_BACKEND_H
#define RK_GDI_IMAGE_GRAPHICS_BACKEND_H

#include "RkGraphicsBackend.h"

class RkGDIImageGraphicsBackend final : public RkGraphicsBackend {
 public:
        RkGDIImageGraphicsBackend(RkCanvas* canvas);
        ~RkGDIImageGraphicsBackend();
        void drawText(const std::string &text, int x, int y);
        void drawImage(const std::string &file, int x, int y);
        void drawImage(const RkImage &image, int x, int y);
        void drawEllipse(const RkPoint& p, int width, int height);
        void drawLine(const RkPoint &p1, const RkPoint &p2);
        void drawRect(const RkRect &rect);
        void drawPolyLine(const std::vector<RkPoint> &points);
        void fillRect(const RkRect &rect, const RkColor &color);
        void setPen(const RkPen &pen);
        void setFont(const RkFont &font);
        int getTextWidth(const std::string &text) const final;
        void translate(const RkPoint &offset);
        void rotate(rk_real angle);
};

#endif // RK_GDI_IMAGE_GRAPHICS_BACKEND_H
