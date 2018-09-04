/**
 * File name: geonkick_groupbox.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include "geonkick_groupbox.h"
#include "geonkick_label.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

GeonkickGroupBox::GeonkickGroupBox(GeonkickWidget *parent, Orientation orientation)
        : GeonkickWidget(parent),
          groupBoxOrientation(orientation),
          goupBoxLabel(new GeonkickLabel(this))
{
        init();
}

GeonkickGroupBox::GeonkickGroupBox(const QString &title,
                                   GeonkickWidget *parent,
                                   Orientation orientation)
        : GeonkickWidget(parent),
          groupBoxOrientation(orientation),
          goupBoxLabel(new GeonkickLabel(this))
{
        init();
}

GeonkickGroupBox::~GeonkickGroupBox()
{
}

void GeonkickGroupBox::init()
{
        QPalette pal;
        pal.setColor(QPalette::Background, QColor(68, 68, 70));
        setAutoFillBackground(true);
        setPalette(pal);

        if (groupBoxOrientation == Orientation::Vertical) {
                setLayout(new QVBoxLayout(this));
        } else {
                setLayout(new QHBoxLayout(this));
        }

        layout()->addWidget(goupBoxLabel);
}

GeonkickLabel* GeonkickGroupBox::getGroupBoxLabel()
{
        return goupBoxLabel;
}

void GeonkickGroupBox::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        int padding = 2;
        painter.setPen(QPen(QColor(40, 40, 40, 100)));
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
        painter.drawRoundedRect(padding, padding, width() - 2 * padding, height() - 2 * padding, 7, 7);
}

