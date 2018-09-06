/**
 * File name: top_bar.h
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

#ifndef GEONKICK_TOP_BAR_H
#define GEONKICK_TOP_BAR_H

#include "geonkick_widget.h"

class GeonkickButton;

class TopBar : public GeonkickWidget
{
 public:
        TopBar(GeonkickWidget *parent);
        ~TopBar();

 private:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        bool is_pressed;
        int dX;
        int dY;
        GeonkickButton *openFileButton;
        GeonkickButton *saveFileButton;
        GeonkickButton *exportFileButton;
        GeonkickButton *settingsButton;
};

#endif // GEONKICK_TOP_BAR_H
