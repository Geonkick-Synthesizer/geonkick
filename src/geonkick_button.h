/**
 * File name: geonkick_button.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://geontime.com>
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

#ifndef GEONKICK_BUTTON_H
#define GEONKICK_BUTTON_H

#include "geonkick_widget.h"

#include <RkImage.h>

class GeonkickButton: public GeonkickWidget
{
 public:
        explicit GeonkickButton(GeonkickWidget *parent);
        virtual ~GeonkickButton() = default;
        void setPressedImage(const RkImage &img);
        void setUnpressedImage(const RkImage &img);
        bool isPressed() const;
        bool isCheckable();

        RK_DECL_ACT(toggled,
                    toggled(bool pressed),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(pressed));
        void setPressed(bool pressed);
        void setCheckable(bool checkable);

 protected:
        void mouseButtonPressEvent(RkMouseEvent *event) override;

 private:
        bool is_pressed;
        bool is_checkable;
        RkImage pressedImage;
        RkImage unpressedImage;
};

#endif // GEONKICK_BUTTON_H
