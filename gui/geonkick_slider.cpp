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
          sliderValue(50)
{
}

GeonkickSlider::~GeonkickSlider()
{
}

void GeonkickSlider::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setPen(QPen(QColor(40, 40, 40)));
        painter.drawRect(0, 0, width() - 1, height() - 1);
        if (sliderOrientation == Orientation::Horizontal) {
                painter.fillRect(1, 1, (width() - 2) * ((double)sliderValue / 100), height() - 2,
                                 QBrush(QColor(200, 200, 200)));
        } else {
                painter.fillRect(1, 1, (width() - 2), (height() - 2) * ((double)sliderValue / 100),
                                 QBrush(QColor(200, 200, 200)));
        }
}

void GeonkickSlider::mousePressEvent(QMouseEvent *event)
{
        if (event->x() >= 0 && event->x() <= width() && event->y() >= 0 && event->y() <= height())
        {
                GKICK_LOG_INFO("here");
                if (event->x() < 1) {
                        setValue(0);
                } else if (event->x() > width() - 1) {
                        setValue(100);
                } else {
                        setValue((double)((event->x() - 1) * 100) / (width() - 2));
                }
                isSelected = true;
        }
}

void GeonkickSlider::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                if (event->x() < 1) {
                        setValue(0);
                } else if (event->x() > width() - 1) {
                        setValue(100);
                } else {
                        setValue((double)((event->x() - 1) * 100) / (width() - 2));
                }
        }
}

void GeonkickSlider::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        isSelected = false;
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
                update();
        }
}
