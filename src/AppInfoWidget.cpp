/**
 * File name: AppInfoWidget.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#include "AppInfoWidget.h"
#include "GeonkickModel.h"

#include "RkContainer.h"
#include "RkLabel.h"

AppInfoWidget::AppInfoWidget(GeonkickWidget* parent, GeonkickModel *model)
        : GeonkickWidget(parent)
{
        setFixedSize(110, 65);
        auto contianer = new RkContainer(this, Rk::Orientation::Vertical);
        contianer->addSpace(26);

        std::string geonkickInfo;
        if (model->instanceType() == GeonkickModel::InstanceType::Lv2)
                geonkickInfo = "LV2 plugin";
        else if (model->instanceType() == GeonkickModel::InstanceType::Vst3)
                geonkickInfo = "VST3 plugin";
        else
                geonkickInfo = "Standalone";
        geonkickInfo += std::string(" ") + Geonkick::applicationVersionStr;

        // Instance type and version
        auto label = new RkLabel(this, geonkickInfo);
        label->setSize(width(), 14);
        label->setBackgroundColor(background());
        label->setTextColor({150, 150, 150});
        auto labelFont = font();
        labelFont.setSize(10);
        labelFont.setWeight(RkFont::Weight::Bold);
        label->setFont(labelFont);
        label->show();
        contianer->addWidget(label);

        // Multi, single, basic
        std::string kitType;
        if (model->kitType() == GeonkickModel::KitType::SingleInstrument)
                kitType = "Single instrument";
        else
                kitType = "Multi instrument";

        if (model->isBasic())
                kitType += " (basic)";

        label = new RkLabel(this, kitType);
        label->setSize(width(), 14);
        label->setBackgroundColor(background());
        label->setTextColor({150, 150, 150});
        label->setFont(labelFont);
        label->show();
        contianer->addWidget(label);

        show();
}
