/**
 * File name: geonkick_button.h
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

#ifndef GEONKICK_BUTTON_H
#define GEONKICK_BUTTON_H

#include <geonkick_widget.h>

#include <RkButton.h>
#include <RkImage.h>

class GeonkickButton: public RkButton
{
 public:
        explicit GeonkickButton(GeonkickWidget *parent);
        virtual ~GeonkickButton() = default;
        void setPressedImage(const RkImage &img);
        void setUnpressedImage(const RkImage &img);

protected:
        void mouseButtonPressEvent(RkMouseEvent *event) override;
};

#endif // GEONKICK_BUTTON_H
