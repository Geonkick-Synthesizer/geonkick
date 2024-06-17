/**
 * File name: geonkick_slider.cpp
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

#include "geonkick_slider.h"

#include <RkEvent.h>
#include <RkPainter.h>

GeonkickSlider::GeonkickSlider(GeonkickWidget *parent, Orientation orientation)
        : GeonkickWidget(parent)
        , sliderOrientation{orientation}
        , isSelected{false}
        , sliderValue{0}
        , defaultValue{0}
        , sliderPixels{0}
{
        setBackgroundColor(100, 100, 100);
        show();
}

void GeonkickSlider::paintWidget(RkPaintEvent *event)
{
        RK_UNUSED(event);
        RkImage img(width(), height());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        painter.setPen(RkPen(RkColor(40, 40, 40)));
        painter.drawRect({0, 0, width() - 1, height() - 1});
        if (sliderOrientation == Orientation::Horizontal)
                painter.fillRect(RkRect(1, 1, sliderPixels, height() - 1 - 1),
                                 RkColor(200, 200, 200));
        else
                painter.fillRect(RkRect(1, height() - 1 - sliderPixels, width() - 2, sliderPixels),
                                 RkColor(200, 200, 200));
        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void GeonkickSlider::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                sliderValue += event->button() == RkMouseEvent::ButtonType::WheelUp ? 2 : -2;
                sliderValue = std::clamp(sliderValue, 0, 100);
                onSetValue(sliderValue);
                sliderPixels = pixelsFromValue();
                action valueUpdated(sliderValue);
                update();
                return;
        }

        if (event->x() >= 0 && event->x() < width()
            && event->y() >= 0 && event->y() < height()) {
                int value = calculateValue(event->x(), event->y());
                isSelected = true;
                if (value != sliderValue) {
                        sliderValue = value;
                        action valueUpdated(sliderValue);
                        update();
                }
         }
}

void GeonkickSlider::mouseMoveEvent(RkMouseEvent *event)
{
        if (isSelected) {
                int value = calculateValue(event->x(), event->y());
                sliderValue = value;
                action valueUpdated(sliderValue);
                update();
        }
}

void GeonkickSlider::mouseButtonReleaseEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
        isSelected = false;
}

void GeonkickSlider::mouseDoubleClickEvent([[maybe_unused]] RkMouseEvent *event)
{
        onSetValue(defaultValue, defaultValue);
        action valueUpdated(defaultValue);
}

int GeonkickSlider::calculateValue(int x, int y)
{
        int value = 0;
        if (sliderOrientation == Orientation::Horizontal) {
                if (x < 1)
                        value = 0;
                else if (x > width() - 1 - 1)
                        value = width() - 2 * 1;
                else
                        value = x;
                sliderPixels = value;
                value = 100 * ((double)sliderPixels / (width() - 2));
        } else {
                if (y < 1)
                        value = height() - 2;
                else if (y > height() - 2)
                        value = 0;
                else
                        value = height() - y;
                sliderPixels = value;
                value = 100 * ((double)sliderPixels / (height() - 2));
        }
        return value;
}

int GeonkickSlider::getValue() const
{
        return sliderValue;
}

int GeonkickSlider::pixelsFromValue() const
{
        int pixels;
        if (sliderOrientation == Orientation::Horizontal)
                pixels = (static_cast<double>(sliderValue) / 100) * (width() - 2);
        else
                pixels = (static_cast<double>(sliderValue) / 100) * (height() - 2);
        return pixels;
}

void GeonkickSlider::onSetValue(int value, int defaultVal)
{
        defaultValue = std::clamp(defaultVal, 0, 100);
        value = std::clamp(value, 0, 100);
        if (value != sliderValue) {
                sliderValue = value;
                sliderPixels = pixelsFromValue();
                update();
        }
}

GeonkickSlider::Orientation GeonkickSlider::getOrientation() const
{
        return sliderOrientation;
}
