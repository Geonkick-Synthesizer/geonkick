/**
 * File name: geonkick_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "geonkick_widget.h"

#include <RkPainter.h>

GeonkickWidget::GeonkickWidget(GeonkickWidget *parent)
        : RkWidget(parent)
{
        setBackgroundColor(68, 68, 70);
}

GeonkickWidget::GeonkickWidget(const RkNativeWindowInfo &info)
        : RkWidget(info)
{
        setBackgroundColor(68, 68, 70);
}

GeonkickWidget::~GeonkickWidget()
{
}

void GeonkickWidget::paintEvent(const std::shared_ptr<RkPaintEvent> &event)
{
        if (!backgroundImage.isNull()) {
                RkPainter painter(this);
                painter.fillRect(rect(), background());
                painter.drawImage(backgroundImage, 0, 0);
        }

        paintWidget(event);
}

void GeonkickWidget::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
        RK_UNUSED(event);
}

void GeonkickWidget::setBackgroundImage(const RkImage &img)
{
        if (!img.isNull()) {
                backgroundImage = img;
                update();
        }
}

