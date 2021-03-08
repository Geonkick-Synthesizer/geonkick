/**
 * File name: RkPoint.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor <http://quamplex.com>
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

#ifndef  RK_POINT_H
#define  RK_POINT_H

#include "Rk.h"

class RK_EXPORT RkPoint {
 public:

       constexpr RkPoint() : xValue{0}, yValue{0}
       {
       }

       constexpr RkPoint(int x, int y) : xValue{x}, yValue{y}
       {
       }

       friend constexpr bool operator==(const RkPoint &p1, const RkPoint &p2)
       {
               return p1.xValue == p2.xValue && p1.yValue == p2.yValue;
       }

       friend constexpr bool operator!=(const RkPoint &p1, const RkPoint &p2)
       {
               return p1.xValue != p2.xValue || p1.yValue != p2.yValue;
       }

       constexpr int x() const
       {
               return xValue;
       }

       constexpr void setX(int x)
       {
               xValue = x;
       }

       constexpr int y() const
       {
               return yValue;
       }

       constexpr void setY(int y)
       {
               yValue = y;
       }

 private:
        int xValue;
        int yValue;
};

#endif // RK_POINT_H
