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

#include <QHBoxLayout>
#include <QVBoxLayout>

GeonkickGroupBox::GeonkickGroupBox(const QString &title, GeonkickWidget *parent)
  : GeonkickWidget(parent),
    groupBox(new QGroupBox(title, this))
{
    // Add groupbox in the  widget layout.
    QVBoxLayout *widgetLayout = QVBoxLayout(this);
    setLyout(widgetLayout);
    widgetLayout->addWidget(groupBox);
}

GeonkickGroupBox::~GeonkickGroupBox()
{
}

void GeonkickGroupBox::setTitle(const QString &title)
{
        groupBox->setTitle(name);
}

void GeonkickGroupBox::setGroubBoxLayout(QLayout *layout)
{
        groupBox->setLayout(layout);
}
