/**
 * File name: geonkick_slider.cpp
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

#include "geonkick_slider.h"

#include <QMouseEvent>

GeonkickSlider::GeonkickSlider(GeonkickWidget *parent, Orientation orientation)
        : GeonkickWidget(parent),
          sliderOrientation(orientation),
          isSelected(false),
          sliderValue(0),
          sliderPixels(0)
{
}

GeonkickSlider::~GeonkickSlider()
{
}

void GeonkickSlider::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        int padding = 1;
        painter.setPen(QPen(QColor(40, 40, 40)));
        painter.drawRect(0, 0, width() - 1, height() - 1);
        if (sliderOrientation == Orientation::Horizontal) {
                painter.fillRect(1, 1, sliderPixels, height() - 1 - 1,
                                 QBrush(QColor(200, 200, 200)));
        } else {
                painter.fillRect(1, height() - 1 - sliderPixels,
                                 width() - 2, sliderPixels,
                                 QBrush(QColor(200, 200, 200)));
        }
}

void GeonkickSlider::mousePressEvent(QMouseEvent *event)
{
        if (event->x() >= 0 && event->x() < width()
            && event->y() >= 0 && event->y() < height()) {
                sliderPixels = calculateValue(event->x(), event->y());
                if (sliderOrientation == Orientation::Horizontal) {
                        setValue(100 * (sliderPixels / width() - 2));
                } else {
                        setValue(100 * (sliderPixels / height() - 2));
                }
                isSelected = true;
                update();
         }
}

void GeonkickSlider::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                sliderPixels = calculateValue(event->x(), event->y());
                if (sliderOrientation == Orientation::Horizontal) {
                        setValue(100 * (sliderPixels / width() - 2));
                } else {
                        setValue(100 * (sliderPixels / height() - 2));
                }
                update();
        }
}

void GeonkickSlider::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        isSelected = false;
}


int GeonkickSlider::calculateValue(int x, int y) const
{
        int value = 0;
        if (sliderOrientation == Orientation::Horizontal) {
                if (x < 1) {
                        value = 0;
                } else if (x > width() - 1 - 1) {
                        value = width() - 2 * 1;
                } else {
                        value = x;
                }
        } else {
                if (y < 1) {
                        value = height() - 2;
                } else if (y > height() - 2) {
                        value = 0;
                } else {
                        value = height() - y;
                }
        }
        return value;
}

int GeonkickSlider::getValue() const
{
        return sliderValue;
}

void GeonkickSlider::setValue(int value)
{
        if (value > 100) {
                value = 100;
        } else if (value < 0) {
                value = 0;
        }

        if (value != sliderValue) {
                sliderValue = value;
                emit valueUpdated(sliderValue);
        }
}
