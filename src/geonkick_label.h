/**
 * File name: geonkick_label.h
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

#ifndef GEONKICK_LABEL_H
#define GEONKICK_LABEL_H

#include "geonkick_widget.h"

class QLabel;

class GeonkickLabel: public GeonkickWidget
{
 Q_OBJECT

 public:
        GeonkickLabel(GeonkickWidget *parent = nullptr);
        GeonkickLabel(const QString &title, GeonkickWidget *parent = nullptr);
        virtual ~GeonkickLabel();
        void init();
        void setImage(const QPixmap &image);
        void setImage(const QString &file);
        void setText(const QString &text);

 private:
        QLabel *widgetLabel;
};

#endif // GEONKICK_LABEL_H
