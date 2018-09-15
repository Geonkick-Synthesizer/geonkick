/**
 * File name: geonkick_button.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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
#include "globals.h"

GeonkickButton::GeonkickButton(GeonkickWidget *parent)
        : GeonkickWidget(parent),
          is_pressed(false)
{
}

GeonkickButton::~GeonkickButton()
{
}

void GeonkickButton::mousePressEvent(QMouseEvent * event)
{
        Q_UNUSED(event)
        setPressed(true);
        emit toggled(is_pressed);
}

void GeonkickButton::setPressedImage(const QPixmap &pixmap)
{
        pressedImage = pixmap;
        if (is_pressed) {
                setFixedSize(pixmap.size().width(), pixmap.size().height());
                setBackgroundImage(pressedImage);
        }
}

void GeonkickButton::setUnpressedImage(const QPixmap &pixmap)
{
        unpressedImage = pixmap;
        if (!is_pressed) {
                setFixedSize(pixmap.size().width(), pixmap.size().height());
                setBackgroundImage(unpressedImage);
        }
}

void GeonkickButton::setPressed(bool pressed)
{
        is_pressed = pressed;
        if (is_pressed) {
                setBackgroundImage(pressedImage);
        } else {
                setBackgroundImage(unpressedImage);
        }
        update();
}

bool GeonkickButton::isPressed() const
{
        return is_pressed;
}

