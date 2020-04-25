/**
 * File name: geonkick_button.cpp
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

#include "geonkick_button.h"

GeonkickButton::GeonkickButton(GeonkickWidget *parent)
        : GeonkickWidget(parent)
        , is_pressed{false}
        , is_checkable{false}
{
        show();
}

void GeonkickButton::mouseButtonPressEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
        if (isCheckable()) {
                setPressed(!isPressed());
                toggled(isPressed());
        } else if (!isPressed()) {
                setPressed(true);
                toggled(true);
        }
}

void GeonkickButton::setPressedImage(const RkImage &img)
{
        pressedImage = img;
        if (isPressed()) {
                setFixedSize(img.width(), img.height());
                setBackgroundImage(pressedImage);
        }
}

void GeonkickButton::setUnpressedImage(const RkImage &img)
{
        unpressedImage = img;
        if (!isPressed()) {
                setFixedSize(img.width(), img.height());
                setBackgroundImage(unpressedImage);
        }
}

void GeonkickButton::setPressed(bool pressed)
{
        is_pressed = pressed;
        if (is_pressed)
                setBackgroundImage(pressedImage);
        else
                setBackgroundImage(unpressedImage);
        update();
}

bool GeonkickButton::isPressed() const
{
        return is_pressed;
}

bool GeonkickButton::isCheckable()
{
        return is_checkable;
}

void GeonkickButton::setCheckable(bool checkable)
{
        is_checkable = checkable;
}
