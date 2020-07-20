/**
 * File name: geonkick_groupbox.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "geonkick_groupbox.h"

GeonkickGroupBox::GeonkickGroupBox(GeonkickWidget *parent, Orientation orientation)
        : GeonkickWidget(parent)
        , groupBoxOrientation{orientation}
        , groupBoxLabel{nullptr}
{
}

GeonkickWidget* GeonkickGroupBox::getGroupBoxLabel()
{
        return groupBoxLabel;
}

void GeonkickGroupBox::setGroupBoxLabel(GeonkickWidget *label)
{
        if (label)
                groupBoxLabel = label;
}

GeonkickWidget* GeonkickGroupBox::addWidget(GeonkickWidget *widget)
{
        return widget;
}
