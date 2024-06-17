/**
 * File name: KitTabs.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#include "KitTabs.h"
#include "kit_model.h"
#include "percussion_model.h"

#include <RkContainer.h>
#include <RkButton.h>

KitTabs::KitTabs(GeonkickWidget *parent, KitModel *model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , mainLayout{nullptr}
{
        setSize(910, 25);
        //        setBackgroundColor({0, 255, 0});
        mainLayout = new RkContainer(this);
        mainLayout->setSize(size());
        RK_ACT_BIND(kitModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        updateView();
        show();
}

KitModel* KitTabs::getModel() const
{
        return kitModel;
}

void KitTabs::updateView()
{
        GEONKICK_LOG_INFO("KitTabs::updateView()");
        mainLayout->clear();
        for (auto &tab: tabsList)
                delete tab;
        tabsList.clear();
        for (const auto &per: kitModel->percussionModels()) {
                GEONKICK_LOG_INFO("per->name():" << per->name());
                auto tab = new RkButton(this);
                tab->setBackgroundColor({100, 100, 100});
                tab->setSize(55, mainLayout->height());
                tab->setText(per->name());
                tab->show();
                mainLayout->addWidget(tab);
                mainLayout->addSpace(2);
                tabsList.push_back(tab);
        }
}

