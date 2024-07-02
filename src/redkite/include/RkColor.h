/**
 * File name: RkColor.h
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

#ifndef  RK_COLOR_H
#define  RK_COLOR_H

#include "Rk.h"

class RkColor {
 public:

        constexpr RkColor()
                : redValue{255}
                , greenValue{255}
                , blueValue{255}
                , alphaValue{255}
       {
       }

       constexpr RkColor(int red, int green, int blue, int alpha = 255)
               : redValue(red)
               , greenValue(green)
               , blueValue(blue)
               , alphaValue(alpha)
       {
       }

       friend constexpr bool operator==(const RkColor &c1, const RkColor &c2)
       {
               return c1.redValue == c2.redValue
               && c1.greenValue == c2.greenValue
               && c1.blueValue == c2.blueValue
               && c1.alphaValue == c2.alphaValue;
       }

        friend constexpr bool operator!=(const RkColor &c1, const RkColor &c2)
        {
                return c1.redValue != c2.redValue
                        || c1.greenValue != c2.greenValue
                        || c1.blueValue != c2.blueValue
                        || c1.alphaValue != c2.alphaValue;
        }

        constexpr RkColor operator+(int value) const
        {
                return RkColor(
                               std::min(255, redValue + value),
                               std::min(255, greenValue + value),
                               std::min(255, blueValue + value),
                               std::min(255, alphaValue + value)
                               );
        }

        constexpr RkColor operator-(int value) const
        {
                return RkColor(
                               std::max(0, redValue - value),
                               std::max(0, greenValue - value),
                               std::max(0, blueValue - value),
                               std::max(0, alphaValue - value)
                               );
        }

        constexpr RkColor operator*(int value) const
        {
                return RkColor(
                               std::min(255, redValue * value),
                               std::min(255, greenValue * value),
                               std::min(255, blueValue * value),
                               std::min(255, alphaValue * value)
                               );
        }

        constexpr RkColor operator/(int value) const
        {
                return RkColor(
                               value != 0 ? redValue / value : 255,
                               value != 0 ? greenValue / value : 255,
                               value != 0 ? blueValue / value : 255,
                               value != 0 ? alphaValue / value : 255
                               );
        }

        constexpr RkColor operator+(const RkColor& other) const
        {
                return RkColor(
                               std::min(255, redValue + other.redValue),
                               std::min(255, greenValue + other.greenValue),
                               std::min(255, blueValue + other.blueValue),
                               std::min(255, alphaValue + other.alphaValue)
                               );
        }

        constexpr RkColor operator-(const RkColor& other) const
        {
                return RkColor(
                               std::max(0, redValue - other.redValue),
                               std::max(0, greenValue - other.greenValue),
                               std::max(0, blueValue - other.blueValue),
                               std::max(0, alphaValue - other.alphaValue)
                               );
        }

        constexpr RkColor operator*(const RkColor& other) const
        {
                return RkColor(
                               std::min(255, redValue * other.redValue / 255),
                               std::min(255, greenValue * other.greenValue / 255),
                               std::min(255, blueValue * other.blueValue / 255),
                               std::min(255, alphaValue * other.alphaValue / 255)
                               );
        }

        constexpr RkColor operator/(const RkColor& other) const
        {
                return RkColor(
                               other.redValue != 0 ? std::min(255, redValue / other.redValue * 255) : 255,
                               other.greenValue != 0 ? std::min(255, greenValue / other.greenValue * 255) : 255,
                               other.blueValue != 0 ? std::min(255, blueValue / other.blueValue * 255) : 255,
                               other.alphaValue != 0 ? std::min(255, alphaValue / other.alphaValue * 255) : 255
                               );
        }

       constexpr int red() const
       {
               return redValue;
       }

       constexpr int green() const
       {
               return greenValue;
       }

       constexpr int blue() const
       {
               return blueValue;
       }

       constexpr int alpha() const
       {
               return alphaValue;
       }

       constexpr void setRed(int red)
       {
               redValue = red;
       }

       constexpr void setGreen(int green)
       {
               greenValue = green;
       }

       constexpr void setBlue(int blue)
       {
               blueValue = blue;
       }

       constexpr void setAlpha(int alpha)
       {
               alphaValue = alpha;
       }

       unsigned long int argb() const
       {
               return (((unsigned long int)alphaValue) << 24)
                       | (((unsigned long int)redValue) << 16)
                       | (((unsigned long int)greenValue) << 8)
                       | (((unsigned long int)blueValue));
       }

 private:
        unsigned short redValue;
        unsigned short greenValue;
        unsigned short blueValue;
        unsigned short alphaValue;
};

#endif // RK_COLOR_H
