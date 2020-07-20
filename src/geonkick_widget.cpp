/**
 * File name: geonkick_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://iuriepage.wordpress.com>
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

GeonkickWidget::GeonkickWidget(RkMain *app,
                               const RkNativeWindowInfo &info,
                               Rk::WindowFlags flags)
        : RkWidget(app, info, flags)
{
        setBackgroundColor(68, 68, 70);
        setTextColor({210, 226, 226, 140});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

GeonkickWidget::GeonkickWidget(RkMain *app, Rk::WindowFlags flags)
        : RkWidget(app, flags)
{
        setBackgroundColor({68, 68, 70});
        setTextColor({210, 226, 226, 140});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

GeonkickWidget::GeonkickWidget(GeonkickWidget *parent, Rk::WindowFlags flags)
        : RkWidget(parent, flags)
{
        setBackgroundColor(68, 68, 70);
        setTextColor({210, 226, 226, 140});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

void GeonkickWidget::paintEvent(RkPaintEvent *event)
{
        if (!backgroundImage.isNull()) {
                RkPainter painter(this);
                painter.fillRect(rect(), background());
                painter.drawImage(backgroundImage, 0, 0);
        }

        paintWidget(event);
}

void GeonkickWidget::paintWidget(RkPaintEvent *event)
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

void GeonkickWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
}
