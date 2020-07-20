/**
 * File name: controls_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "controls_widget.h"
#include "oscillator.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "effects_group_box.h"
#include "geonkick_api.h"

ControlsWidget::ControlsWidget(GeonkickWidget *parent,
                               GeonkickApi* api,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
        : GeonkickWidget(parent)
        , geonkickApi{api}
        , effectsWidget{nullptr}
{
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)].get();
        auto widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(0, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(8 + 224, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(2 * (8 + 224), 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        auto generalWidget = new GeneralGroupBox(this, geonkickApi);
        generalWidget->setPosition(3 * (8 + 224), 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), generalWidget, updateGui());
        generalWidget->show();

        effectsWidget = new EffectsGroupBox(geonkickApi, this);
        effectsWidget->setFixedSize(500, 82);
        effectsWidget->setPosition(2 * (8 + 224), 285);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), effectsWidget, updateGui());
        effectsWidget->show();
}

void ControlsWidget::setEnvelopeWidget(EnvelopeWidget *widget)
{
        effectsWidget->setEnvelopeWidget(widget);
}
