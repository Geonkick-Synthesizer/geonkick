/**
 * File name: geonkick_widget.cpp
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

#include "geonkick_widget.h"
#include "geonkick_theme.h"

GeonkickWidget::GeonkickWidget(GeonkickWidget *parent)
        : QWidget(parent),
          widgetTheme(nullptr)
{
        if (parent) {
                setTheme(parent->getTheme());
        }
}

GeonkickWidget::~GeonkickWidget()
{
}

void GeonkickWidget::paintEvent(QPaintEvent *event)
{
        if (!backgroundImage.isNull()) {
                QPainter painter(this);
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

void GeonkickWidget::setTheme(GeonkickTheme *theme)
{
        if (widgetTheme) {
                disconnect(widgetTheme, SIGNAL(themeChanged(GeonkickTheme*)), this, SLOT(themeChanged(GeonkickTheme*)));
        }
        widgetTheme = theme;
        connect(widgetTheme, SIGNAL(themeChanged(GeonkickTheme*)), this, SLOT(themeChanged(GeonkickTheme*)));
        themeChanged(widgetTheme);
}

GeonkickTheme* GeonkickWidget::getTheme()
{
        return widgetTheme;
}
