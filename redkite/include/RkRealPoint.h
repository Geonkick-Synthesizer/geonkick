/**
 * File name: RkRealPoint.h
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

#ifndef  RK_REAL_POINT_H
#define  RK_REAL_POINT_H

#include "RkPoint.h"

class RK_EXPORT RkRealPoint {
 public:

       constexpr RkRealPoint() : xValue{0}, yValue{0}
       {
       }

       constexpr RkRealPoint(const RkPoint &p) : xValue(p.x()), yValue(p.y())
       {
       }

       constexpr RkRealPoint(rk_real x, rk_real y) : xValue{x}, yValue{y}
       {
       }

       constexpr rk_real x() const
       {
               return xValue;
       }

       constexpr void setX(rk_real x)
       {
               xValue = x;
       }

       constexpr rk_real y() const
       {
               return yValue;
       }

       constexpr void setY(rk_real y)
       {
               yValue = y;
       }

 private:
        rk_real xValue;
        rk_real yValue;
};

#endif // RK_REAL_POINT_H
