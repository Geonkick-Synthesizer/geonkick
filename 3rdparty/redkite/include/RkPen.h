/**
 * File name: RkPen.h
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

#ifndef  RK_PEN_H
#define  RK_PEN_H

#include "Rk.h"
#include "RkColor.h"

class RK_EXPORT RkPen {
 public:

        enum class PenStyle {
                NoLine    = 0,
                SolidLine = 1,
                DashLine  = 2,
                DotLine   = 3
        };

        explicit constexpr RkPen(PenStyle style = PenStyle::SolidLine)
                : penStyleVal{style}
                , penWidthVal{1}
                , penColorVal{RkColor()}
       {
       }

       constexpr RkPen(const RkColor &color)
               :  penStyleVal{PenStyle::SolidLine}
                , penWidthVal{1}
                , penColorVal{color}
       {
       }

       friend constexpr bool operator==(const RkPen &pen1, const RkPen &pen2)
       {
               return pen1.penStyleVal == pen2.penStyleVal
                      && pen1.penColorVal == pen2.penColorVal
                      && pen1.penWidthVal == pen2.penWidthVal;
       }

       friend constexpr bool operator!=(const RkPen &pen1, const RkPen &pen2)
       {
               return pen1.penStyleVal != pen2.penStyleVal
                      || pen1.penColorVal != pen2.penColorVal
                      || pen1.penWidthVal != pen2.penWidthVal;
       }

       constexpr PenStyle style() const
       {
               return penStyleVal;
       }

       constexpr void setStyle(PenStyle style)
       {
               penStyleVal = style;
       }

       constexpr const RkColor& color() const
       {
               return penColorVal;
       }

       constexpr void setColor(const RkColor &color)
       {
               penColorVal = color;
       }

       constexpr int width() const
       {
               return penWidthVal;
       }

       constexpr void setWidth(int width)
       {
               penWidthVal = width;
       }

 private:
       PenStyle penStyleVal;
       int penWidthVal;
       RkColor penColorVal;
};

#endif // RK_PEN_H
