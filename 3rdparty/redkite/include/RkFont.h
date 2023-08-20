/**
 * File name: RkFont.h
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

#ifndef  RK_FONT_H
#define  RK_FONT_H

#include "Rk.h"
#include "RkColor.h"

class RK_EXPORT RkFont {
 public:
        enum class Style : int {
                Normal = 0,
                Italic = 1,
                Oblique = 3
        };

        enum class Weight: int {
                Normal = 0,
                Bold = 1
        };

         RkFont() : fontFamily{"Arial"} // TODO: get default font family
                  , fontSize{10}
                  , fontWeight{Weight::Normal}
                  , fontStyle{Style::Normal} {}

        explicit RkFont(const std::string &family,
                                  int size = 10,
                                  Weight weight = Weight::Normal,
                                  Style style = Style::Normal)
                : fontFamily{family}
                  , fontSize{size}
                  , fontWeight{weight}
                  , fontStyle{style} {}

        RkFont(const RkFont & font) = default;
        RkFont & operator=(const RkFont & font) = default;
        RkFont & operator=(RkFont && other) = default;
        friend  bool operator==(const RkFont &font1, const RkFont &font2)
        {
                return font1.fontFamily == font2.fontFamily
                       && font1.fontStyle == font2.fontStyle
                       && font1.fontWeight == font2.fontWeight
                       && font1.fontSize == font2.fontSize;
        }

        friend  bool operator!=(const RkFont &font1, const RkFont &font2)
        {
                return font1.fontFamily != font2.fontFamily
                       || font1.fontStyle != font2.fontStyle
                       || font1.fontWeight != font2.fontWeight
                       || font1.fontSize != font2.fontSize;
        }

        Style style() const { return fontStyle; }
        void setStyle(Style style) { fontStyle = style; }
        std::string family() const { return fontFamily; }
        void setFamily(const std::string &family) { fontFamily = family; }
        int size() const { return fontSize; };
        void setSize(int size) { fontSize = size; }
        Weight weight() const { return fontWeight; }
        void setWeight(Weight weight) { fontWeight = weight; }

 private:
        std::string fontFamily;
        int fontSize;
        Weight fontWeight;
        Style fontStyle;
};

#endif // RK_FONT_H
