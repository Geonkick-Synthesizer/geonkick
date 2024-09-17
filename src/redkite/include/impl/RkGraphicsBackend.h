/**
 * File name: RkGraphicsBackend.h
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

#ifndef RK_GRAPHICS_BACKEND_H
#define RK_GRAPHICS_BACKEND_H

#include "Rk.h"
#include "RkImage.h"
#include "RkPoint.h"
#include "RkPen.h"
#include "RkRect.h"
#include "RkFont.h"

class RkCanvas;

class RkGraphicsBackend {
 public:
        RkGraphicsBackend() = default;
        virtual ~RkGraphicsBackend() = default;
        virtual void drawText(const std::string &text, int x, int y) = 0;
        virtual void drawImage(const std::string &file, int x, int y) = 0;
        virtual void drawImage(const RkImage &image, int x, int y) = 0;
        virtual void drawEllipse(const RkPoint& p, int width, int height) = 0;
        virtual void drawLine(const RkPoint &p1, const RkPoint &p2) = 0;
        virtual void drawRect(const RkRect &rect) = 0;
        virtual void drawPolyLine(const std::vector<RkPoint> &points) = 0;
        virtual void fillRect(const RkRect &rect, const RkColor &color) = 0;
        virtual void setPen(const RkPen &pen) = 0;
        virtual void setFont(const RkFont &font) = 0;
        virtual int getTextWidth(const std::string &text) const = 0;
        virtual void translate(const RkPoint &offset) = 0;
        virtual void rotate(rk_real angle) = 0;
};

#endif // RK_GRAPHICS_BACKEND_H
