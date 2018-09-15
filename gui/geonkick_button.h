/**
 * File name: geonkick_button.h
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

#ifndef GEONKICK_BUTTON_H
#define GEONKICK_BUTTON_H

#include "geonkick_widget.h"

class GeonkickButton: public GeonkickWidget
{
 Q_OBJECT

 public:
        GeonkickButton(GeonkickWidget *parent = nullptr);
        virtual ~GeonkickButton();
        void setPressedImage(const QPixmap &pixmap);
        void setUnpressedImage(const QPixmap &pixmap);
        bool isPressed() const;

public slots:
        void setPressed(bool pressed);

 signals:
        void toggled(bool pressed);

 protected:
        void mousePressEvent(QMouseEvent * event) override;

 private:
        bool is_pressed;
        QPixmap pressedImage;
        QPixmap unpressedImage;
};

#endif // GEONKICK_BUTTON_H
