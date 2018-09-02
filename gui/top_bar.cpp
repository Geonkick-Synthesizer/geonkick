/**
 * File name: top_bar.cpp
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

#include "top_bar.h"

#include <QMouseEvent>

TopBar::TopBar(GeonkickWidget *parent)
        : GeonkickWidget(parent),
          is_pressed(false),
          dX(0),
          dY(0)
{
        QPalette pal;
        pal.setColor(QPalette::Background, QColor(80, 80, 80));
        setAutoFillBackground(true);
        setPalette(pal);
        if (parent) {
                setFixedSize(parent->width(), 40);
        }
}

TopBar::~TopBar()
{
}

void TopBar::mousePressEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        is_pressed = true;
        dX = event->x();
        dY = event->y();
}

void TopBar::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        is_pressed = false;
}

void TopBar::mouseMoveEvent(QMouseEvent *event)
{
        GKICK_LOG_INFO("here");
        if (is_pressed) {
                auto parent = parentWidget();
                parent->move(event->globalX() - dX, event->globalY() - dY);
        }
}
