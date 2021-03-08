/**
 * File name: RkCairoGraphicsBackend.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor (http://geontime.com)
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

#include "RkCairoGraphicsBackend.h"
#include "RkCanvas.h"
#include "RkCanvasInfo.h"
#include "RkLog.h"

#include <math.h>

RkCairoGraphicsBackend::RkCairoGraphicsBackend(RkCanvas *canvas)
        : cairoContext{cairo_create(canvas->getCanvasInfo()->cairo_surface)}
{
        cairo_set_font_size(context(), 10);
        cairo_set_line_width (context(), 1);
}

cairo_t* RkCairoGraphicsBackend::context() const
{
        return cairoContext;
}

RkCairoGraphicsBackend::~RkCairoGraphicsBackend()
{
        cairo_destroy(context());
}

void RkCairoGraphicsBackend::drawText(const std::string &text, int x, int y)
{
        cairo_move_to(context(), x, y);
        cairo_show_text(context(), text.c_str());
}

void RkCairoGraphicsBackend::drawImage(const std::string &file, int x, int y)
{
        auto image = cairo_image_surface_create_from_png(file.c_str());
        cairo_set_source_surface(context(), image, x, y);
        cairo_paint(context());
        cairo_surface_destroy(image);
}

void RkCairoGraphicsBackend::drawImage(const RkImage &image, int x, int y)
{
        cairo_set_source_surface(context(),
                                 image.getCanvasInfo()->cairo_surface,
                                 x, y);
        cairo_paint(context());
}

void RkCairoGraphicsBackend::drawEllipse(const RkPoint& p, int width, int height)
{
        if (width == height) {
                cairo_move_to(context(), p.x() + width / 2, p.y());
                cairo_arc(context(), p.x(), p.y(), width / 2, 0, 2 * M_PI);
                cairo_stroke(context());
        } else {
                // TODO: implemented ellipse.
                RK_LOG_ERROR("ellipse not implemented yet");
        }
}

void RkCairoGraphicsBackend::drawLine(const RkPoint &p1, const RkPoint &p2)
{
        cairo_move_to(context(), p1.x() + 0.5, p1.y() + 0.5);
        cairo_line_to(context(), p2.x() + 0.5, p2.y() + 0.5);
        cairo_stroke(context());
}

void RkCairoGraphicsBackend::drawRect(const RkRect &rect)
{
        cairo_rectangle(context(),
                        rect.left() + 0.5,
                        rect.top() + 0.5,
                        rect.width(),
                        rect.height());
        cairo_stroke(context());
}

void RkCairoGraphicsBackend::setPen(const RkPen &pen)
{
        cairo_set_line_width(context(), pen.width());
        cairo_set_source_rgba(context(),
                              static_cast<double>(pen.color().red()) / 255,
                              static_cast<double>(pen.color().green()) / 255,
                              static_cast<double>(pen.color().blue()) / 255,
                              static_cast<double>(pen.color().alpha()) / 255);
        double dashLine[] = {12, 8};
        double dotLine[] = {1, 2};
        switch (pen.style())
        {
        case RkPen::PenStyle::DashLine:
                cairo_set_dash(context(), dashLine, 2, 0);
                break;
        case RkPen::PenStyle::DotLine:
                cairo_set_dash(context(), dotLine, 2, 0);
                break;
        case RkPen::PenStyle::NoLine:
        case RkPen::PenStyle::SolidLine:
        default:
                cairo_set_dash(context(), nullptr, 0, 0);
                break;
        }
}

void RkCairoGraphicsBackend::setFont(const RkFont &font)
{
        cairo_set_font_size(context(), font.size());
        cairo_font_slant_t slant;
        switch (font.style())
        {
        case RkFont::Style::Normal:
                slant = CAIRO_FONT_SLANT_NORMAL;
                break;
        case RkFont::Style::Italic:
                slant = CAIRO_FONT_SLANT_ITALIC;
                break;
        case RkFont::Style::Oblique:
                slant = CAIRO_FONT_SLANT_OBLIQUE;
                break;
        default:
                slant = CAIRO_FONT_SLANT_NORMAL;
        }

        cairo_font_weight_t weight;
        switch (font.weight())
        {
        case RkFont::Weight::Normal:
                weight = CAIRO_FONT_WEIGHT_NORMAL;
                break;
        case RkFont::Weight::Bold:
                weight = CAIRO_FONT_WEIGHT_BOLD;
                break;
        default:
                weight = CAIRO_FONT_WEIGHT_NORMAL;
        }

        auto face = cairo_toy_font_face_create(font.family().c_str(), slant, weight);
        cairo_set_font_face(context(), face);
        cairo_font_face_destroy(face);
        cairo_set_font_size(context(), font.size());
}

void RkCairoGraphicsBackend::drawPolyLine(const std::vector<RkPoint> &points)
{
        bool first = true;
        RkPoint currPoint;
        for (const auto &point: points) {
                if (first) {
                        cairo_move_to(context(), point.x() + 0.5, point.y() + 0.5);
                        currPoint = point;
                        first = false;
                } else if (currPoint != point) {
                        cairo_rel_line_to(context(), point.x() - currPoint.x(),
                                          point.y() - currPoint.y());
                        currPoint = point;
                }
        }
        cairo_stroke(context());
}

void RkCairoGraphicsBackend::fillRect(const RkRect &rect, const RkColor &color)
{
        cairo_rectangle(context(), rect.left(), rect.top(), rect.width(), rect.height());
        cairo_set_source_rgba(context(),
                             static_cast<double>(color.red()) / 255,
                             static_cast<double>(color.green()) / 255,
                             static_cast<double>(color.blue()) / 255,
                             static_cast<double>(color.alpha()) / 255);
        cairo_fill(context());
}

void RkCairoGraphicsBackend::applyAlpha(int alpha)
{
        cairo_paint_with_alpha(context(), (float) alpha / 255);
}

void RkCairoGraphicsBackend::translate(const RkPoint &offset)
{
        cairo_translate(context(), offset.x(), offset.y());
}

void RkCairoGraphicsBackend::rotate(rk_real angle)
{
        cairo_rotate(context(), angle);
}

int RkCairoGraphicsBackend::getTextWidth(const std::string &text) const
{
        if (text.empty())
                return 0;

        cairo_text_extents_t extents;
        cairo_text_extents (context(), text.data(), &extents);
        return extents.x_advance;
}
