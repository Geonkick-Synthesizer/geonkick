/**
 * File name: RkSize.h
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

#ifndef  RK_SIZE_H
#define  RK_SIZE_H

#include "Rk.h"

class RkSize {
 public:
       constexpr RkSize() : widthValue{0}, heightValue{0}
       {
       }

       constexpr RkSize(int width, int height) : widthValue{width}, heightValue{height}
       {
       }

       friend constexpr bool operator==(const RkSize &s1, const RkSize &s2)
       {
               return s1.widthValue == s2.widthValue && s1.heightValue == s2.heightValue;
       }

       friend constexpr bool operator!=(const RkSize &s1, const RkSize &s2)
       {
               return s1.widthValue != s2.widthValue || s1.heightValue != s2.heightValue;
       }

       constexpr int width() const
       {
               return widthValue;
       }

       constexpr void setWidth(int width)
       {
               widthValue = width;
       }

       constexpr int height() const
       {
               return heightValue;
       }

       constexpr void setHeight(int height)
       {
               heightValue = height;
       }

       constexpr bool isEmpty() const
       {
               return widthValue <= 0 || heightValue <= 0;
       }

 private:
        int widthValue;
        int heightValue;
};

#endif // RK_SIZE_H
