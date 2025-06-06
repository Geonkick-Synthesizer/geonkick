/**
 * File name: geonkick_button.cpp
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

#include "geonkick_button.h"

GeonkickButton::GeonkickButton(GeonkickWidget *parent)
        : RkButton(parent)
{
        setName("GeonkickButton");
        show();
}

void GeonkickButton::setPressedImage(const RkImage &img)
{
        setImage(img, RkButton::State::Pressed);
}

void GeonkickButton::setUnpressedImage(const RkImage &img)
{
        setImage(img, RkButton::State::Unpressed);
}

void GeonkickButton::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
        RkButton::mouseButtonPressEvent(event);
}
