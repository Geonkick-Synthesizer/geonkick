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
#include "filter.h"
#include "Distortion.h"

#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(osc_effects_bk);

OscillatorEffects::OscillatorEffects(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , mainLayout{new RkContainer(this)}
        , filterTabButton{nullptr}
        , distortionTabButton{nullptr}
        , filterTab{nullptr}
        , distortionTab{nullptr}
        , showFilterTab{true}
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
        filterTab->setModel(model);
        distortionTab->setModel(model);
        updateView();
}

Oscillator* OscillatorEffects::getModel() const
{
        return oscillatorModel;
}

void OscillatorEffects::createView()
{
        auto tabButtonsLayout = new RkContainer(this);
        tabButtonsLayout->setSize(width(), 20);
        filterTabButton = new EffectTabButton(this, RkImage(30, 20, RK_IMAGE_RC(osc_effects_tab_filter)));
        tabButtonsLayout->addWidget(filterTabButton);
        RK_ACT_BIND(filterTabButton, enabled,
                    RK_ACT_ARGS(bool b),
                    oscillator,
                    enableFilter(b));

        distortionTabButton = new EffectTabButton(this, RkImage(30, 20, RK_IMAGE_RC(osc_effects_tab_distortion)));
        tabButtonsLayout->addWidget(distortionTabButton);
        mainLayout->addContainer(tabButtonsLayou);
        RK_ACT_BIND(distorionTabButton, enabled,
                    RK_ACT_ARGS(bool b),
                    oscillatorModel,
                    enableDistortion(b));

        auto effectTabLayout = new RkContainer(this);
        effectTabLayout->setSize(width(), height() - tabButtonsLayout->height());
        filterTab = new Filter(this, Envelope::Category::General);
        filterTab->setCutOffRange(20, 20000);
        filterTab->setResonanceRange(1, 1000);
        filterTab->setPosition(0, 125);
        RK_ACT_BIND(filterTab, enabled, RK_ACT_ARGS(bool b),
                    oscillatorModel, enableFilter(b));
        RK_ACT_BIND(filterTab, cutOffChanged, RK_ACT_ARGS(double val),
                    oscillatorModel, setFilterFrequency(val));
        RK_ACT_BIND(filterTab, resonanceChanged, RK_ACT_ARGS(double val),
                    oscillatorModel, setFilterQFactor(val));
        RK_ACT_BIND(filterTab, typeChanged,
                    RK_ACT_ARGS(GeonkickApi::FilterType type),
                    oscillatorModel, setFilterType(type));
        effectTabLayout->addWidget(filterTab);
        distortionTab = new Distortion(this, oscillatorModel->getDistortion());
        effectTabLayout->addWidget(filterDistortion);
        mainLayout->addContainer(effectTabLayout);
        updateView();
}

void OscillatorEffects::updateView()
{
        filterTabButton->enable(oscillatorModel->isFilterEnabled());
        distortionTabButton->enable(oscillatorModel->isDistortionEnabled());
        if (showFilterTab) {
                if (!filterTab->isVisible()) {
                        filterTab->show();
                        mainLayout->update();
                }
                filterTab->updateView();
        } else {
                if (!distortionTab->isVisible()) {
                        filterTab->show();
                        mainLayout->update();
                }
                distortionTab->updateView();
        }
}
