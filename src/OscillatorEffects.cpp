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

RK_DECLARE_IMAGE_RC(osc_effects_bk);

OscillatorEffects::OscillatorEffects(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , mainLayout{new RkContainer(this)}
        , filterTabButton{nullptr}
        , distortionTabButton{nullptr}
{
        setSize(224, 125);
        mainLayout->setSize(size());
        setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(osc_effects_bk)));
        createView();
        show();
}

void OscillatorEffects::setModel(Oscillator *model)
{
        oscillatorModel = model;
        updateView();

}

Oscillator* OscillatorEffects::getModel(Oscillator *model) const
{
        return oscillatorModel;
}

void OscillatorEffects::createView(Oscillator *model)
{
        auto tabButtonsLayout = new RkContainer(this);
        tabButtonsLayout->setSize(width(), 20);
        filterTabButton = new EffectTabButton(this, RkImage(30, 20, RK_IMAGE_RC(osc_effects_tab_filter)));
        tabButtonsLayout->addWidget(filterTabButton);
        RK_ACT_BIND(filterTabButton, stateChanged, RK_ACT_ARGS(), this, updateGui());
        filterTabButton = new EffectTabButton(this, RkImage(30, 20, RK_IMAGE_RC(osc_effects_tab_distortion)));
        tabButtonsLayout->addWidget(filterTabButton);
        mainLayout->addContainer(tabButtonsLayout);

        auto effectTabLayout = new RkContainer(this);
        effectTabLayout->setSize(width(), height() - tabButtonsLayout->height());
        filterTab = new Filter(this, static_cast<Envelope::Category>(oscillatorModel->type()));
        effectTabLayout->addWidget(filterTab);
        distortionTab = new Distortion(this, static_cast<Envelope::Category>(oscillatorModel->type()));
        effectTabLayout->addWidget(filterDistortion);
        mainLayout->addContainer(effectTabLayout);
        updateView();
}

void OscillatorEffects::updateView()
{
        
}
