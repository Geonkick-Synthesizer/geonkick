/**
 * File name: RkButton.h
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

#ifndef RK_BUTTON_H
#define RK_BUTTON_H

#include "RkWidget.h"
#include "RkImage.h"

class RkButton: public RkWidget {
 public:
        enum class ButtonType: int {
                ButtonUncheckable = 0,
                ButtonCheckable   = 1,
                ButtonPush        = 3
        };

        enum class State: int {
                Unpressed      = 0,
                UnpressedHover = 1,
                Pressed        = 2,
                PressedHover   = 3
        };

        explicit RkButton(RkWidget *parent);
        virtual ~RkButton() = default;
        void setText(const std::string &text);
        std::string text() const;
        void setImage(const RkImage &img, State state = State::Unpressed);
        void setPressed(bool pressed);
        bool isPressed() const;
        ButtonType type() const;
        void setCheckable(bool b);
        bool isCheckable() const;
        void setType(ButtonType type);
        RK_DECL_ACT(toggled,
                    toggled(bool pressed),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(pressed));
        RK_DECL_ACT(pressed,
                    pressed(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(released,
                    released(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

 protected:
        RK_DECLARE_IMPL_PTR(RkButton);
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseButtonReleaseEvent(RkMouseEvent *event) override;
        void hoverEvent(RkHoverEvent *event) override;
        void paintEvent(RkPaintEvent *event) override;

 private:
        RK_DISABLE_COPY(RkButton);
        RK_DISABLE_MOVE(RkButton);
};

#endif // RK_BUTTON_H
