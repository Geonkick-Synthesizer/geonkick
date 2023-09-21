/**
 * File name: RkRect.h
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

#ifndef  RK_RECT_H
#define  RK_RECT_H

#include "RkSize.h"
#include "RkPoint.h"

class RK_EXPORT RkRect {
 public:

        constexpr RkRect()
        {
        }

        constexpr RkRect(int x, int y, int width, int height)
                : rectTopLeft(x, y)
                , rectBottomRight(x + width, y + height)
        {
        }

        constexpr RkRect(const RkPoint &topLeft, const RkSize &size)
                : rectTopLeft{topLeft}
                , rectBottomRight(topLeft.x() + size.width(), topLeft.y() + size.height())
        {
        }

        constexpr RkRect(const RkPoint &topLeft, const RkPoint &bottomRight)
                : rectTopLeft{topLeft}
                , rectBottomRight{bottomRight}
        {
        }

        friend constexpr bool operator==(const RkRect &r1, const RkRect &r2)
        {
               return r1.rectTopLeft == r2.rectTopLeft && r1.rectBottomRight == r2.rectBottomRight;
        }

        friend constexpr bool operator!=(const RkRect &r1, const RkRect &r2)
        {
                return r1.rectTopLeft != r2.rectTopLeft || r1.rectBottomRight != r2.rectBottomRight;
        }

        constexpr int width() const
        {
               if (rectBottomRight.x() >= rectTopLeft.x())
                       return rectBottomRight.x() - rectTopLeft.x();
               else
                       return rectTopLeft.x() - rectBottomRight.x();
        }

        constexpr void setWidth(int width)
        {
                rectBottomRight.setX(rectTopLeft.x() + width);
        }

        constexpr int height() const
        {
                if (rectBottomRight.y() >= rectTopLeft.y())
                        return rectBottomRight.y() - rectTopLeft.y();
                else
                        return rectTopLeft.y() - rectBottomRight.y();
        }

        constexpr void setHeight(int height)
        {
                rectBottomRight.setY(rectTopLeft.y() + height);
        }

        constexpr RkSize size() const
        {
                return RkSize(width(), height());
        }

        constexpr void setSize(const RkSize &size)
        {
                rectBottomRight.setX(rectTopLeft.x() + size.width());
                rectBottomRight.setY(rectTopLeft.y() + size.height());
        }

        constexpr void setTopLeft(const RkPoint &p)
        {
                rectTopLeft = p;
        }

        constexpr void setTopRight(const RkPoint &p)
        {
                rectTopLeft.setY(p.y());
                rectBottomRight.setX(p.x());
        }

        constexpr void setBottomLeft(const RkPoint &p)
        {
                rectTopLeft.setX(p.x());
                rectBottomRight.setY(p.y());
        }

        constexpr void setBottomRight(const RkPoint &p)
        {
                rectBottomRight = p;
        }

        constexpr RkPoint topLeft() const
        {
                return rectTopLeft;
        }

        constexpr RkPoint topRight() const
        {
                return RkPoint(rectBottomRight.x(), rectTopLeft.y()) ;
        }

        constexpr RkPoint bottomLeft() const
        {
                return RkPoint(rectTopLeft.x(), rectBottomRight.y()) ;
        }

        constexpr RkPoint bottomRight(void) const
        {
                return rectBottomRight;
        }

        constexpr int left() const
        {
                return rectTopLeft.x();
        }

        constexpr int top() const
        {
                return rectTopLeft.y();
        }

        constexpr int bottom() const
        {
                return rectBottomRight.y();
        }

        constexpr int right() const
        {
                return rectBottomRight.x();
        }

        constexpr int area() const
        {
                return width() * height();
        }

 private:
       RkPoint rectTopLeft;
       RkPoint rectBottomRight;
};

#endif // RK_RECT_H
