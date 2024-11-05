/**
 * File name: OscillatorFunctionView.cpp
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

#include "OscillatorFunctionView.h"
#include "Filter.h"
#include "Distortion.h"

#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(effect_tab_control_filter);
RK_DECLARE_IMAGE_RC(effect_tab_control_dist);

OscillatorEffectView::OscillatorEffectView(GeonkickWidget *parent, Oscillator* model)
        : GeonkickWidget(parent)
        , oscillatorModel{model}
        , filterControl{new EffectTabControl(this, RK_IMAGE_RC(effect_tab_control_filter)))}
        , distortionControl{new EffectTabControl(this, RK_IMAGE_RC(effect_tab_control_dist)))}
{
        setFixedSize(224, 125);

        auto mainLayout = RkContainer(this);
        mainLayout->setSize(size);

        auto controlsLayout = RkContainer(this);
        controlsLayout->setSize({width(), distortionControl->height()});
        controlsLayout->addWidget(filterControl);
        controlsLayout->addWidget(distortionControl);
        mainLayout->addContianer(controlsLayout);

        auto filter = new Filter(this, model->filter());
        RK_ACT_BIND(filterControl,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    model->filter(),
                    enable(b));
        RK_ACT_BIND(filterControl,
                    showControl,
                    RK_ACT_ARGS(),
                    filter,
                    show());
        mainLayout->addWidget(filter);
        auto distortion = new Distortion(this, model->distortion());
        RK_ACT_BIND(distortionControl,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    model->filter(),
                    enable(b));
        RK_ACT_BIND(distorionControl,
                    showControl,
                    RK_ACT_ARGS(),
                    distortion,
                    show());
        mainLayout->addWidget(distortion);
}
