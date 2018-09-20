/**
 * File name: geonkick_button.cpp
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


#include "geonkick_checkbox.h"
#include "geonkick_button.h"
#include "geonkick_label.h"

#include <QHBoxLayout>

GeonkickCheckbox::GeonkickCheckbox(GeonkickWidget *parent)
        : GeonkickWidget(parent),
          checkboxButton(new GeonkickButton(this)),
          checkboxLabel(new GeonkickButton(this)),
          mainLayout(new QHBoxLayout(this))
{
        checkboxButton->setCheckable(true);
        checkboxLabel->setCheckable(true);
        setPadding(0, 0, 0, 0);
        mainLayout->setSpacing(3);
        setLayout(mainLayout);
        mainLayout->addWidget(checkboxButton, 0, Qt::AlignLeft);
        mainLayout->addWidget(checkboxLabel, 0, Qt::AlignLeft);
        connect(checkboxButton, SIGNAL(toggled(bool)), this, SIGNAL(stateUpdated(bool)));
        connect(checkboxButton, SIGNAL(toggled(bool)), checkboxLabel, SLOT(setPressed(bool)));
        connect(checkboxLabel, SIGNAL(toggled(bool)), checkboxButton, SLOT(setPressed(bool)));
        connect(checkboxLabel, SIGNAL(toggled(bool)), this, SIGNAL(stateUpdated(bool)));
}

GeonkickCheckbox::~GeonkickCheckbox()
{
}

void GeonkickCheckbox::setCheckboxLabelImage(const QPixmap &pixmap)
{
        checkboxLabel->setUnpressedImage(pixmap);
}

void GeonkickCheckbox::setCheckboxLabelImage(const QString &file)
{
        setCheckboxLabelImage(QPixmap(file));
}

void GeonkickCheckbox::setCheckedImage(const QPixmap &pixmap)
{
        checkboxButton->setPressedImage(pixmap);
}

void GeonkickCheckbox::setUncheckedImage(const QPixmap &pixmap)
{
        checkboxButton->setUnpressedImage(pixmap);
}

void GeonkickCheckbox::setCheckedImage(const QString &file)
{
        setCheckedImage(QPixmap(file));
}

void GeonkickCheckbox::setUncheckedImage(const QString &file)
{
        setUncheckedImage(QPixmap(file));
}

void GeonkickCheckbox::setPadding(int left, int top, int right, int buttom)
{
        mainLayout->setContentsMargins(left, top, right, buttom);
}

void GeonkickCheckbox::setChecked(bool checked)
{
        checkboxButton->setPressed(checked);
}
