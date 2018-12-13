/**
 * File name: geonkick_slider.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

class QMouseEvent;

class GeonkickSlider: public GeonkickWidget
{
 Q_OBJECT

 public:
        enum class Orientation:char {
               Horizontal,
               Vertical
        };

        GeonkickSlider(GeonkickWidget *parent = nullptr,
                        Orientation orientation = Orientation::Horizontal);
        ~GeonkickSlider();
        void paintWidget(QPaintEvent *event) override;
        int getValue() const;

 public slots:
        void setValue(int value);

 signals:
        void valueUpdated(int value);

 protected:
        int  calculateValue(int x, int y);

 private:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void resizeEvent(QResizeEvent *event);
        int pixelsFromValue();
        Orientation sliderOrientation;
        bool isSelected;
        int sliderValue;
        int sliderPixels;
};

#endif // GEONKICK_SLIDER_H
