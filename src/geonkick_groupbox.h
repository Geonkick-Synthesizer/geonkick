/**
 * File name: geonkick_groupbox.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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

#ifndef GEOBKICK_GROUPBOX_H
#define GEOBKICK_GROUPBOX_H

#include "geonkick_widget.h"

class GeonkickGroupBox: public GeonkickWidget
{
 public:

     enum class Orientation:char {
         Vertical,
         Horizontal
     };

     GeonkickGroupBox(GeonkickWidget *parent = nullptr,
                  Orientation orientation = Orientation::Vertical);
     virtual ~GeonkickGroupBox() = default;
     GeonkickWidget* getGroupBoxLabel();
     void setGroupBoxLabel(GeonkickWidget *label);
     GeonkickWidget* addWidget(GeonkickWidget *widget);
     void setWidgetAlignment(GeonkickWidget *widget);
     void setPadding(int left, int top, int right, int bottom);

 private:
     Orientation groupBoxOrientation;
     GeonkickWidget *groupBoxLabel;
};

#endif // GEOBKICK_GROUPBOX_H
