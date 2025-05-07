/**
 * File name: Sidebar.cpp
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

#include "Sidebar.h"
#include "GeonkickModel.h"
#include "geonkick_button.h"
#include "PresetBrowser.h"
#include "SampleBrowser.h"

#include <RkEvent.h>
#include <RkPainter.h>
#include <RkContainer.h>
#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(topmenu_presets_active);
RK_DECLARE_IMAGE_RC(topmenu_presets_hover);
RK_DECLARE_IMAGE_RC(topmenu_presets_off);
RK_DECLARE_IMAGE_RC(topmenu_samples_active);
RK_DECLARE_IMAGE_RC(topmenu_samples_hover);
RK_DECLARE_IMAGE_RC(topmenu_samples_off);

Sidebar::Sidebar(GeonkickWidget *parent, GeonkickModel *model)
        : GeonkickWidget(parent)
        , geonkickModel{model}
        , mainLayout{new RkContainer(this, Rk::Orientation::Vertical)}
        , currentWidget{nullptr}
{
        setSize(310, parent->height() - 8);
        mainLayout->setSize(size());

        mainLayout->addSpace(4);
        createTabButtons(mainLayout);

        mainLayout->addSpace(4);
        showSidebarWidget();
}

void Sidebar::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        RkPainter painter(this);
        auto pen = painter.pen();
        pen.setColor({55, 55, 55});
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(RkRect({0, 0}, RkSize(width() - 1, height() - 1)));
}

void Sidebar::createTabButtons(RkContainer *mainLayout)
{
        auto buttonsLayout = new RkContainer(this, Rk::Orientation::Horizontal);
        buttonsLayout->setSize({mainLayout->size().width(), 20});
        buttonsLayout->addSpace(4);

        // Presets button
        presetsButton = new GeonkickButton(this);
        presetsButton->setPressed(true);
        presetsButton->setFixedSize(45, 20);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_off)),
                               RkButton::State::Unpressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_active)),
                               RkButton::State::Pressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_hover)),
                               RkButton::State::UnpressedHover);
        presetsButton->show();
        buttonsLayout->addWidget(presetsButton);

        // Separator
        buttonsLayout->addSpace(3);
        auto separator = new RkLabel(this);
        separator->setSize(2, 21);
        separator->setBackgroundColor(68, 68, 70);
        separator->setImage(RkImage(separator->size(), RK_IMAGE_RC(separator)));
        separator->show();
        buttonsLayout->addWidget(separator);
        buttonsLayout->addSpace(3);

        // Samples button
        samplesButton = new GeonkickButton(this);
        samplesButton->setFixedSize(54, 20);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_off)),
                                RkButton::State::Unpressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_active)),
                                RkButton::State::Pressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_hover)),
                                RkButton::State::UnpressedHover);
        samplesButton->show();

        RK_ACT_BINDL(presetsButton, pressed, RK_ACT_ARGS(),
                     [=,this]() {
                             samplesButton->setPressed(false);
                             showSidebarWidget(); });
        RK_ACT_BINDL(samplesButton, pressed, RK_ACT_ARGS(),
                     [=,this]() {
                             presetsButton->setPressed(false);
                             showSidebarWidget(); });

        buttonsLayout->addWidget(samplesButton);
        mainLayout->addContainer(buttonsLayout);
}

void Sidebar::showSidebarWidget()
{
        if (currentWidget) {
                mainLayout->removeWidget(currentWidget);
                delete currentWidget;
        }

        if (presetsButton->isPressed())
                currentWidget = new PresetBrowser(this, geonkickModel->getKitModel());
        else
                currentWidget = new SampleBrowser(this, geonkickModel->getKitModel());

        mainLayout->addWidget(currentWidget);
}
