/**
 * File name: EffectTabButton.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#include "EffectTabButton.h"

EffectTabButton::EffectTabButton(GeonkickWidget* parent)
        : GeonkickButton(parent)
        , enableButton{new GeonkickButton(this)}
{
        setSize(30, 20);
        enableButton->setSize(16, 16);
        enableButton->setCheckable(true);
        enableButton->setPosition(3, (height() - enableButton->height()) / 2);
        RK_ACT_BIND(enableButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    enabled(pressed));
}

void EffectTabButton::enable(bool b)
{
        enableButton->setPressed(b);
}
