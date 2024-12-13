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
#include "oscillator.h"
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


OscillatorEffects::OscillatorEffects(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , mainLayout{new RkContainer(this, Rk::Orientation::Vertical)}
        , filterTabButton{nullptr}
        , distortionTabButton{nullptr}
        , filterTab{nullptr}
        , distortionTab{nullptr}
        , showFilterTab{true}
{
        setSize(224, 125);
        mainLayout->setSize(size());
        mainLayout->addSpace(4);
        setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(osc_effects_bk)));
        //        setBackgroundColor({0xff, 0xff, 0xff});
        createView();
        show();
}

void OscillatorEffects::setModel(Oscillator *model)
{
        oscillatorModel = model;
        filterTab->setModel(model->getFilter());
        //        distortionTab->setModel(model->getDistortion());
        updateView();
}

/*Oscillator* OscillatorEffects::getModel() const
{
        return oscillatorModel;
        }*/

void OscillatorEffects::createView()
{
        auto tabButtonsLayout = new RkContainer(this);
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
        mainLayout->addContainer(tabButtonsLayout);
        RK_ACT_BINDL(distortionTabButton,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    [=,this](bool b){oscillatorModel->getDistortion()->enable(b);});

        RK_ACT_BINDL(filterTabButton,
                     pressed,
                     RK_ACT_ARGS(),
                     [=,this](){distortionTabButton->setPressed(false);
                             updateView();});
        RK_ACT_BINDL(distortionTabButton,
                     pressed,
                     RK_ACT_ARGS(),
                     [=,this](){filterTabButton->setPressed(false);
                             updateView();});

        auto effectTabLayout = new RkContainer(this);
        effectTabLayout->setSize({width(), height() - tabButtonsLayout->height()});
        filterTab = new FilterView(this, oscillatorModel->getFilter());
        effectTabLayout->addWidget(filterTab);
        distortionTab = new DistortionView(this, oscillatorModel->getDistortion());
        effectTabLayout->addWidget(distortionTab);
        mainLayout->addContainer(effectTabLayout);
        updateView();
}

void OscillatorEffects::updateView()
{
        filterTabButton->enable(oscillatorModel->getFilter()->isEnabled());
        //        filterTabButton->enable(oscillatorModel->getDistortion()->isEnabled());
        if (filterTabButton->isPressed()) {
                distortionTab->hide();
                filterTab->show();
                filterTab->updateView();
                mainLayout->update();
        } else {
                filterTab->hide();
                distortionTab->show();
                distortionTab->updateView();
                mainLayout->update();
        }
}
