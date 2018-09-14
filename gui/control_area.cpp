/**
 * File name: control_area.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include "gkick_oscillator.h"
#include "control_area.h"
#include "envelopes_group_box.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "effects_group_box.h"
#include "gkickapi.h"

#include <QHBoxLayout>
#include <QGridLayout>

ControlArea::ControlArea(GeonkickWidget *parent, std::shared_ptr<GKickApi> &api,
                         std::vector<std::shared_ptr<GKickOscillator>> &oscillators)
                         : GeonkickWidget(parent)
{
        auto mainLayout = new QHBoxLayout(this);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(10, 0, 10, 0);
        setLayout(mainLayout);
        auto widget = new OscillatorGroupBox(this, oscillators[GKickOscillator::OSC_1]);
        mainLayout->addWidget(widget);
        mainLayout->setAlignment(widget, Qt::AlignTop);
        widget = new OscillatorGroupBox(this, oscillators[GKickOscillator::OSC_2]);
        mainLayout->addWidget(widget);
        mainLayout->setAlignment(widget, Qt::AlignTop);

        auto layoutGBox = new QGridLayout;
        layoutGBox->setSpacing(0);
        layoutGBox->setContentsMargins(0, 0, 0, 0);
        mainLayout->addLayout(layoutGBox);
        layoutGBox->addWidget(new OscillatorGroupBox(this, oscillators[GKickOscillator::OSC_NOISE]), 0, 0);
        layoutGBox->addWidget(new GeneralGroupBox(this, api), 0, 1);
        layoutGBox->addWidget(new EffectsGroupBox(this), 1, 0, 1, 2);
}

ControlArea::~ControlArea()
{
}

