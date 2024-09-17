/**
 * File name: geonkick_slider.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#ifndef GEONKICK_SLIDER_H
#define GEONKICK_SLIDER_H

#include "geonkick_widget.h"

class RkMouseEvent;

class GeonkickSlider: public GeonkickWidget
{
 public:
        enum class Orientation:char {
               Horizontal,
               Vertical
        };

        GeonkickSlider(GeonkickWidget *parent,
                       Orientation orientation = Orientation::Horizontal);
        ~GeonkickSlider() = default;
        int getValue() const;
        void onSetValue(int value, int defaultVal = 0);
        RK_DECL_ACT(valueUpdated, valueUpdated(int value),
                    RK_ARG_TYPE(int), RK_ARG_VAL(value));
        Orientation getOrientation() const;

 protected:
        int calculateValue(int x, int y);
        virtual void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseMoveEvent(RkMouseEvent *event) override;
        void mouseButtonReleaseEvent(RkMouseEvent *event) override;
        void mouseDoubleClickEvent(RkMouseEvent *event) override;
        int pixelsFromValue() const;

 private:
        Orientation sliderOrientation;
        bool isSelected;
        int sliderValue;
        int defaultValue;
        int sliderPixels;
};

#endif // GEONKICK_SLIDER_H
