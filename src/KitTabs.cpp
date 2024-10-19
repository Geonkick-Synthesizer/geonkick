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
#include <RkPainter.h>
#include <RkImage.h>

KitTabs::KitTabs(GeonkickWidget *parent, KitModel *model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , mainLayout{nullptr}
{
        setSize(915, 25);
        mainLayout = new RkContainer(this);
        mainLayout->setSize(size() - RkSize(5, 5));
        mainLayout->setPosition({3, 3});
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
        mainLayout->clear();
        for (auto &tab: tabsList)
                delete tab;
        tabsList.clear();
        for (const auto &per: kitModel->percussionModels()) {
                auto tab = createTabButton(per);
                mainLayout->addWidget(tab);
                mainLayout->addSpace(2);
                tabsList.push_back(tab);
                RK_ACT_BIND(tab, pressed, RK_ACT_ARGS(), this, selectCurrentTab(per, tab));
        }
}


RkButton* KitTabs::createTabButton(PercussionModel *per)
{
        auto buttonImg = [per](RkSize size, RkButton::State state)
        {
                RkImage img(size);
                RkColor color;
                switch (state) {
                case RkButton::State::Unpressed:
                        color = RkColor(50, 50, 50);
                        break;
                case RkButton::State::UnpressedHover:
                        color = RkColor(80, 80, 80);
                        break;
                case RkButton::State::Pressed:
                        color = RkColor(100, 100, 100);
                        break;
                default:
                        color = RkColor(60, 60, 60);
                }
                RkPainter painter(&img);
                painter.fillRect(RkRect({0, 0}, img.size()), color);
                auto f = painter.font();
                f.setSize(9);
                f.setWeight(RkFont::Weight::Bold);
                painter.setFont(f);
                auto pen = painter.pen();
                pen.setColor(color + RkColor(100, 100, 100));
                painter.setPen(pen);
                painter.drawText({{4, 0}, size - RkSize(4, 0)},
                                 per->name(),
                                 Rk::Alignment::AlignLeft);
                return img;
        };

        auto tab = new RkButton(this);
        tab->setSize(55, mainLayout->height());
        if (per->isSelected())
                tab->setPressed(true);
        tab->setImage(buttonImg(tab->size(), RkButton::State::Unpressed),
                      RkButton::State::Unpressed);
        tab->setImage(buttonImg(tab->size(), RkButton::State::UnpressedHover),
                      RkButton::State::UnpressedHover);
        tab->setImage(buttonImg(tab->size(), RkButton::State::Pressed),
                      RkButton::State::Pressed);
        tab->show();
        return tab;
}

void KitTabs::selectCurrentTab(PercussionModel *per, RkButton* button)
{
        per->select();
        for (auto &tab: tabsList) {
                if (tab != button)
                        tab->setPressed(false);
        }
}

void KitTabs::paintEvent(RkPaintEvent * event)
{
        RkPainter painter(this);
        auto pen = painter.pen();
        pen.setColor({55, 54, 54});
        painter.setPen(pen);
        painter.drawRect(rect());
}
