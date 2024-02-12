/**
 * File name: SettingsWidget.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor 
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

#include "SettingsWidget.h"
#include "geonkick_api.h"

#include "RkContainer.h"
#include "RkLabel.h"
#include "RkSpinBox.h"

SettingsWidget::SettingsWidget(GeonkickWidget *parent, GeonkickApi* api)
        : GeonkickWidget(parent)
        , geonkickApi{api}
{
        setFixedSize(parent->size());
        auto mainContianer = new RkContainer(this);
        mainContianer->setSize({20, width()});
        auto label = new RkLabel(this, "Force all midi channels to ");
        label->setTextColor(textColor());
        label->setBackgroundColor(background());
        label->show();
        mainContianer->addWidget(label);
}

