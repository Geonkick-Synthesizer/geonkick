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
#include "geonkick_button.h"

#include <RkEvent.h>
#include <RkPainter.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(topmenu_presets_active);
RK_DECLARE_IMAGE_RC(topmenu_presets_hover);
RK_DECLARE_IMAGE_RC(topmenu_presets_off);
RK_DECLARE_IMAGE_RC(topmenu_samples_active);
RK_DECLARE_IMAGE_RC(topmenu_samples_hover);
RK_DECLARE_IMAGE_RC(topmenu_samples_off);

Sidebar::Sidebar(GeonkickWidget *parent)
        : GeonkickWidget(parent)
{
        setSize(309, parent->height() - 8);
        setBackgroundColor(70, 70, 70);
        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size() - RkSize(8, 8));
        mainLayout->setPosition({8, 8});
        createTabButtons(mainLayout);

        mainLayout->addSpace(4);
        auto widget = new GeonkickWidget(this);
        widget->setSize(mainLayout->size() - RkSize(4, 4));
        widget->setBackgroundColor({55, 55, 55});
        mainLayout->addWidget(widget, Rk::Alignment::AlignLeft);
}

void Sidebar::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        RkPainter painter(this);
        auto pen = painter.pen();
        pen.setColor({55, 55, 55});
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(RkRect({0, 0}, RkSize(width(), height())));
}

void Sidebar::createTabButtons(RkContainer *mainLayout)
{
        auto buttonsLayout = new RkContainer(this, Rk::Orientation::Horizontal);
        buttonsLayout->setSize({mainLayout->size().width(), 20});

        // Presets button
        auto presetsButton = new GeonkickButton(this);
        //        presetsButton->setPressed(viewState()->getMainView() == ViewState::View::Presets);
        presetsButton->setFixedSize(45, 20);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_off)),
                               RkButton::State::Unpressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_active)),
                               RkButton::State::Pressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_hover)),
                               RkButton::State::UnpressedHover);
        presetsButton->show();
        /*        RK_ACT_BIND(presetsButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Presets));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
        presetsButton, setPressed(view == ViewState::View::Presets));*/
        buttonsLayout->addWidget(presetsButton);

        // Samples button
        buttonsLayout->addSpace(5);
        auto samplesButton = new GeonkickButton(this);
        //        samplesButton->setPressed(viewState()->getMainView() == ViewState::View::Samples);
        samplesButton->setFixedSize(54, 20);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_off)),
                                RkButton::State::Unpressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_active)),
                                RkButton::State::Pressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_hover)),
                                RkButton::State::UnpressedHover);
        samplesButton->show();
        /*        RK_ACT_BIND(samplesButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Samples));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
        samplesButton, setPressed(view == ViewState::View::Samples));*/
        buttonsLayout->addWidget(samplesButton);

        mainLayout->addContainer(buttonsLayout);
}
