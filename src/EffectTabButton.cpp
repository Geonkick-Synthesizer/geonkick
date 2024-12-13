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

#include <RkButton.h>

RK_DECLARE_IMAGE_RC(effects_tab_enable_button);
RK_DECLARE_IMAGE_RC(effects_tab_enable_button_active);
RK_DECLARE_IMAGE_RC(effects_tab_enable_button_hover);

EffectTabButton::EffectTabButton(GeonkickWidget* parent)
        : GeonkickButton(parent)
        , enableButton{new RkButton(this)}
{
        setSize(46, 16);
        enableButton->setSize(10, 10);
        enableButton->setCheckable(true);
        enableButton->setPosition(4, (height() - enableButton->height()) / 2);
        enableButton->setImage(RkImage(enableButton->size(), RK_IMAGE_RC(effects_tab_enable_button)),
                                  RkButton::State::Unpressed);
        enableButton->setImage(RkImage(enableButton->size(), RK_IMAGE_RC(effects_tab_enable_button_active)),
                                  RkButton::State::Pressed);
        enableButton->setImage(RkImage(enableButton->size(), RK_IMAGE_RC(effects_tab_enable_button_hover)),
                                  RkButton::State::UnpressedHover);
        enableButton->setImage(RkImage(enableButton->size(), RK_IMAGE_RC(effects_tab_enable_button_hover)),
                                  RkButton::State::PressedHover);

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
