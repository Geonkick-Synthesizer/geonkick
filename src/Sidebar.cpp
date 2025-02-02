/**
 * File name: Sidebar.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#include "Sidebar.h"

#include <RkEvent.h>
#include <RkPainter.h>

Sidebar::Sidebar(GeonkickWidget *parent)
        : GeonkickWidget(parent)
{
        setSize(309, parent->height() - 8);
        setBackgroundColor(70, 70, 70);
}

void Sidebar::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        RkPainter painter(this);
        auto pen = painter.pen();
        pen.setColor({55, 55, 55});
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(RkRect({0, 0}, RkSize(width(), height())));
}

