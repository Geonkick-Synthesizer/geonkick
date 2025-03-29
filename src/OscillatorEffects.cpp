/**
 * File name: OscillatorEffects.cpp
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

#include "OscillatorEffects.h"
#include "OscillatorModel.h"
#include "FilterModel.h"
#include "FilterView.h"
#include "DistortionModel.h"
#include "DistortionView.h"
#include "EffectTabButton.h"

#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(osc_effects_bk);
RK_DECLARE_IMAGE_RC(effects_tab_filter_button);
RK_DECLARE_IMAGE_RC(effects_tab_filter_button_active);
RK_DECLARE_IMAGE_RC(effects_tab_filter_button_hover);
RK_DECLARE_IMAGE_RC(effects_tab_distortion_button);
RK_DECLARE_IMAGE_RC(effects_tab_distortion_button_active);
RK_DECLARE_IMAGE_RC(effects_tab_distortion_button_hover);


OscillatorEffects::OscillatorEffects(GeonkickWidget *parent, OscillatorModel* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , filterTabButton{nullptr}
        , distortionTabButton{nullptr}
        , currentTabView{nullptr}
{
        setSize(224, 131);
        setBackgroundImage(RkImage(224, 115, RK_IMAGE_RC(osc_effects_bk)));
        createView();
        show();
}

void OscillatorEffects::setModel(OscillatorModel *model)
{
        oscillatorModel = model;
        if (currentTabView) {
                if (dynamic_cast<FilterView*>(currentTabView))
                        currentTabView->setModel(model->getFilter());
                else
                        currentTabView->setModel(model->getDistortion());
        }
        updateView();
}

OscillatorModel* OscillatorEffects::getModel() const
{
        return oscillatorModel;
}

void OscillatorEffects::createView()
{
        auto tabButtonsLayout = new RkContainer(this);
        tabButtonsLayout->setPosition({0, 4});
        tabButtonsLayout->setSize({width(), 16});
        tabButtonsLayout->addSpace(4);
        filterTabButton = new EffectTabButton(this);
        filterTabButton->setPressed(true);
        filterTabButton->setImage(RkImage(filterTabButton->size(), RK_IMAGE_RC(effects_tab_filter_button)),
                                  RkButton::State::Unpressed);
        filterTabButton->setImage(RkImage(filterTabButton->size(), RK_IMAGE_RC(effects_tab_filter_button_active)),
                                  RkButton::State::Pressed);
        filterTabButton->setImage(RkImage(filterTabButton->size(), RK_IMAGE_RC(effects_tab_filter_button_hover)),
                                  RkButton::State::UnpressedHover);
        filterTabButton->setImage(RkImage(filterTabButton->size(), RK_IMAGE_RC(effects_tab_filter_button_hover)),
                                  RkButton::State::PressedHover);
        tabButtonsLayout->addWidget(filterTabButton);
        RK_ACT_BINDL(filterTabButton,
                     enabled,
                    RK_ACT_ARGS(bool b),
                     [=,this](bool b){oscillatorModel->getFilter()->enable(b);});
        tabButtonsLayout->addSpace(2);
        distortionTabButton = new EffectTabButton(this);
        distortionTabButton->setImage(RkImage(distortionTabButton->size(), RK_IMAGE_RC(effects_tab_distortion_button)),
                                  RkButton::State::Unpressed);
        distortionTabButton->setImage(RkImage(distortionTabButton->size(), RK_IMAGE_RC(effects_tab_distortion_button_active)),
                                  RkButton::State::Pressed);
        distortionTabButton->setImage(RkImage(distortionTabButton->size(), RK_IMAGE_RC(effects_tab_distortion_button_hover)),
                                  RkButton::State::UnpressedHover);
        distortionTabButton->setImage(RkImage(distortionTabButton->size(), RK_IMAGE_RC(effects_tab_distortion_button_hover)),
                                  RkButton::State::PressedHover);

        tabButtonsLayout->addWidget(distortionTabButton);
        RK_ACT_BINDL(distortionTabButton,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    [=,this](bool b){oscillatorModel->getDistortion()->enable(b);});

        RK_ACT_BIND(filterTabButton,
                     pressed,
                     RK_ACT_ARGS(),
                     this,
                     showFilter());
        RK_ACT_BIND(distortionTabButton,
                     pressed,
                     RK_ACT_ARGS(),
                     this,
                     showDistortion());
        if (filterTabButton->isPressed())
                showFilter();
        else
                showDistortion();
        updateView();
}

void OscillatorEffects::showFilter()
{
        if (currentTabView)
                delete currentTabView;
        distortionTabButton->setPressed(false);
        currentTabView = new FilterView(this, oscillatorModel->getFilter());
        currentTabView->setPosition(0, 22);
}

void OscillatorEffects::showDistortion()
{
        if (currentTabView)
                delete currentTabView;
        filterTabButton->setPressed(false);
        currentTabView = new DistortionView(this, oscillatorModel->getDistortion());
        currentTabView->setPosition(0, 22);
}

void OscillatorEffects::updateView()
{
        filterTabButton->enable(oscillatorModel->getFilter()->isEnabled());
        distortionTabButton->enable(oscillatorModel->getDistortion()->isEnabled());
        currentTabView->updateView();
}
