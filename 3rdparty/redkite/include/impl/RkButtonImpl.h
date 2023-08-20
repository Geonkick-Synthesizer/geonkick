/**
 * File name: RkButtonImpl.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#ifndef RK_LABEL_IMPL_H
#define RK_LABEL_IMPL_H

#include "RkWidgetImpl.h"
#include "RkButton.h"
#include "RkImageImpl.h"
#include "RkPainter.h"

class RkButton::RkButtonImpl : public RkWidget::RkWidgetImpl {
 public:
        RkButtonImpl(RkButton *interface, RkWidget *parent = nullptr);
        virtual ~RkButtonImpl() = default;
        void setText(const RkString &text);
        RkString text() const;
        void setImage(const RkImage &img, RkButton::State type);
        bool isPressed() const;
        void setPressed(bool pressed);
        ButtonType type() const;
        void setType(ButtonType type);
	void drawButton(RkPainter &painter);
        void setEmphasize(bool b);

 protected:
        void applyEffect(RkImage &img);
        void updateButtonState();

 private:
        RK_DECALRE_INTERFACE_PTR(RkButton);
        ButtonType buttonType;
        bool is_pressed;
        std::array<RkImage, 4> buttonImages;
        RkButton::State buttonState;
        bool isEmphasizeEnabled;
        RkString buttonText;
};

#endif // RK_LABEL_H
