/**
 * File name: EffectsTabView.cpp
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

#include "EffectsTabView.h"
#include "oscillator.h"

#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(osc_effects_bk);

EffectTabButton::EffectTabButton(GeonkickWidget *parent, EffectModel *model)
        : GeonkickWidget(parent)
{
        setSize(20, 40);
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());
        auto enableButton = new GeonkickButton(this);
        enableButton->setCheckable(true);
        enableButton->setSize(20, 20);
        enableButton->setImage(RkImage(oscillatorCheckbox->size(),
                                       RK_IMAGE_RC(controls_osc1_on)),
                               RkButton::State::Pressed);
        enableButton->setImage(RkImage(oscillatorCheckbox->size(),
                                       RK_IMAGE_RC(controls_osc1_off)),
                               RkButton::State::Unpressed);
        enableButton->setImage(RkImage(oscillatorCheckbox->size(),
                                       RK_IMAGE_RC(controls_osc1_hover)),
                               RkButton::State::UnpressedHover);
        enableButton->setImage(RkImage(oscillatorCheckbox->size(),
                                       RK_IMAGE_RC(controls_osc1_hover)),
                               RkButton::State::PressedHover);
        mainLayout->addWidget(enableButton);
        auto label = new RkLabel(this);
        label->setImage(model->getIcon());
        mainLayout->addWidget(label);
}

EffectsTabView::EffectsTabView(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , tabButtonsLayout{new RkContainer(this)}
        , effectTabViewLayout{new RkContainer(this)}
{
        setSize(224, 125);
        setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(osc_effects_bk)));
        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size());
        tabButtonsLayout->setSize({mainLayout->size().width(), 20});
        effectTabViewLayout->setSize(width(), tabButtonsLayout->height());
        mainLayout->addLayout(tabButtonsLayout);
        mainLayout->addLayout(effectTabViewLayout);
        updateView();
        show();
}

void EffectsTabView::setModel(Oscillator *model)
{
        oscillatorModel = model;
        updateView();
}

Oscillator* EffectsTabView::getModel(Oscillator *model) const
{
        return oscillatorModel;
}

void EffectsTabView::updateView()
{
        tabButtonsLayout->clear();
        effectTabViewLayout->clear();
        for (auto &button: effectViewButtons)
                delete button;
        effectViewButtons.erase();
        for (auto &view: effectViews)
                delete view;
        effectViews.erase();
        for (auto &effect: effectsListModel) {
                auto button = new EffectTabButton(this, effect);
                tabButtonsLayout->addWidget(button);
                effectViewButtons->bush_back(button);
                auto view = new EffectView(this, effect);
                effectTabViewLayout->addWidget(tab);
                effectViews->push_back(view);
        }
}
