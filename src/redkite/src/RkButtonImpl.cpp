/**
 * File name: RkButtonImpl.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "RkButtonImpl.h"

RkButton::RkButtonImpl::RkButtonImpl(RkButton *interface, RkWidget *parent)
    : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
    , inf_ptr{interface}
    , buttonType{ButtonType::ButtonUncheckable}
    , is_pressed{false}
    , buttonState{RkButton::State::Unpressed}
    , isEmphasizeEnabled{false}
{
}

void RkButton::RkButtonImpl::setText(const RkString &text)
{
        buttonText = text;
}

RkString RkButton::RkButtonImpl::text() const
{
        return buttonText;
}

void RkButton::RkButtonImpl::setImage(const RkImage &img, RkButton::State state)
{
        buttonImages[static_cast<size_t>(state)] = img;
        if (state == RkButton::State::Unpressed)
                inf_ptr->setSize(img.size());
}

bool RkButton::RkButtonImpl::isPressed() const
{
        return is_pressed;
}

void RkButton::RkButtonImpl::setPressed(bool pressed)
{
        is_pressed = pressed;
        isEmphasizeEnabled = false;
        updateButtonState();
}

void RkButton::RkButtonImpl::setType(RkButton::ButtonType type)
{
        buttonType = type;
        updateButtonState();
}

RkButton::ButtonType RkButton::RkButtonImpl::type(void) const
{
        return buttonType;
}

void RkButton::RkButtonImpl::drawButton(RkPainter &painter)
{
        if (isPressed()) {
                if (!buttonImages[static_cast<size_t>(buttonState)].isNull())
                        painter.drawImage(buttonImages[static_cast<size_t>(buttonState)], 0, 0);
                else if (!buttonImages[static_cast<size_t>(RkButton::State::Pressed)].isNull())
                        painter.drawImage(buttonImages[static_cast<size_t>(RkButton::State::Pressed)], 0, 0);
                else if (!buttonImages[static_cast<size_t>(RkButton::State::Unpressed)].isNull())
                        painter.drawImage(buttonImages[static_cast<size_t>(RkButton::State::Unpressed)], 0, 0);
        } else if (!buttonImages[static_cast<size_t>(RkButton::State::Unpressed)].isNull()) {
                if (!buttonImages[static_cast<size_t>(buttonState)].isNull())
                        painter.drawImage(buttonImages[static_cast<size_t>(buttonState)], 0, 0);
                else
                        painter.drawImage(buttonImages[static_cast<size_t>(RkButton::State::Unpressed)], 0, 0);
        } else if (!buttonImages[static_cast<size_t>(RkButton::State::Unpressed)].isNull()) {
                painter.drawImage(buttonImages[static_cast<size_t>(RkButton::State::Unpressed)], 0, 0);
        }

        if (!buttonText.empty()) {
                auto pen = painter.pen();
                pen.setColor(textColor());
                painter.setPen(pen);
                painter.setFont(inf_ptr->font());
                painter.drawText(inf_ptr->rect(), buttonText);
        }
}

void RkButton::RkButtonImpl::setEmphasize(bool b)
{
        isEmphasizeEnabled = b;
        updateButtonState();
}

void RkButton::RkButtonImpl::updateButtonState()
{
        if (isPressed()) {
                if (isEmphasizeEnabled)
                        buttonState = RkButton::State::PressedHover;
                else
                        buttonState = RkButton::State::Pressed;
        } else {
                if (isEmphasizeEnabled)
                        buttonState = RkButton::State::UnpressedHover;
                else
                        buttonState = RkButton::State::Unpressed;
        }
}
