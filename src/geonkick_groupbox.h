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

#ifndef GEOBKICK_GROUPBOX_H
#define GEOBKICK_GROUPBOX_H

#include <geonkick_widget.h>

class GeonkickWidget;
class QPaintEvent;
class QLayout;
class QBoxLayout;

class GeonkickGroupBox: public GeonkickWidget
{
 Q_OBJECT

 public:

     enum class Orientation:char {
         Vertical,
         Horizontal
     };

     GeonkickGroupBox(GeonkickWidget *parent = nullptr,
                  Orientation orientation = Orientation::Vertical);
     virtual ~GeonkickGroupBox();
     GeonkickWidget* getGroupBoxLabel();
     void setGroupBoxLabel(GeonkickWidget *label, Qt::Alignment alignment = Qt::AlignCenter);
     GeonkickWidget* addWidget(GeonkickWidget *widget, Qt::Alignment alignment = Qt::AlignTop);
     void setWidgetAlignment(GeonkickWidget *widget, Qt::Alignment alignment);
     void setPadding(int left, int top, int right, int buttom);

 private:
     Orientation groupBoxOrientation;
     GeonkickWidget *groupBoxLabel;
     QBoxLayout *mainLayout;
     QLayout *groupBoxLayout;
};

#endif // GEOBKICK_GROUPBOX_H
