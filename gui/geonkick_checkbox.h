/**
 * File name: geonkick_checkbox.h
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

#ifndef GEONKICK_CHECKBOX_H
#define GEONKICK_CHECKBOX_H

#include "geonkick_widget.h"

class GeonkickButton;
class GeonkickLabel;
class QHBoxLayout;

class GeonkickCheckbox: public GeonkickWidget
{
 Q_OBJECT

 public:
        GeonkickCheckbox(GeonkickWidget *parent = nullptr);
        virtual ~GeonkickCheckbox();
        void setCheckboxLabelImage(const QPixmap &pixmap);
        void setCheckboxLabelImage(const QString &file);
        void setCheckedImage(const QPixmap &pixmap);
        void setUncheckedImage(const QPixmap &pixmap);
        void setCheckedImage(const QString &file);
        void setUncheckedImage(const QString &file);
        void setPadding(int left, int top, int right, int buttom);

 public slots:
        void setChecked(bool checked);

 private:
        GeonkickButton *checkboxButton;
        GeonkickButton *checkboxLabel;
        QHBoxLayout *mainLayout;
};

#endif // GEONKICK_CHECKBOX_H
