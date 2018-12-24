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

GeonkickWidget::GeonkickWidget(GeonkickWidget *parent)
        : QWidget(parent)
{
        // Later this kind of settings will be taken from from theme/skin.
        QPalette pal;
        pal.setColor(QPalette::Background, QColor(68, 68, 70));
        pal.setColor(QPalette::WindowText, QColor(210, 226, 226, 140));
        setAutoFillBackground(true);
        setPalette(pal);
}

GeonkickWidget::~GeonkickWidget()
{
}

void GeonkickWidget::paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        if (!backgroundImage.isNull()) {
                painter.drawPixmap(0, 0, backgroundImage);
        }

        paintWidget(event);
}

void GeonkickWidget::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
}

void GeonkickWidget::setBackgroundImage(const QString &file)
{
        setBackgroundImage(QPixmap(file));
}

void GeonkickWidget::setBackgroundImage(const QPixmap &pixmap)
{
        if (!pixmap.isNull()) {
                backgroundImage = pixmap;
                update();
        }
}
