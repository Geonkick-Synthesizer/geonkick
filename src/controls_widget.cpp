/**
 * File name: controls_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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
#ifndef GEONKICK_SINGLE
#include "KitTabs.h"
#endif // GEONKICK_SINGLE
#include "GeonkickModel.h"
#include "kit_model.h"
#include "percussion_model.h"

ControlsWidget::ControlsWidget(GeonkickWidget *parent,
                               GeonkickModel* model,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
        : GeonkickWidget(parent)
        , geonkickModel{model}
{
        setFixedSize({parent->width(), parent->height()});
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)].get();
        auto widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(0, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(8 + 223, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator3)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(2 * (8 + 223), 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        auto kitModel = geonkickModel->getKitModel();
        auto globalWidget = new GeneralGroupBox(this, kitModel->currentPercussion());
        globalWidget->setPosition(3 * (8 + 223), 0);
        RK_ACT_BIND(kitModel,
                    percussionSelected,
                    RK_ACT_ARGS(PercussionModel *model),
                    globalWidget,
                    setModel(model));
        RK_ACT_BIND(kitModel,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    globalWidget,
                    setModel(kitModel->currentPercussion()));
        globalWidget->show();

        effectsWidget = new EffectsGroupBox(geonkickModel->api(), this);
        effectsWidget->setFixedSize(380, 74);
        effectsWidget->setPosition(3 * (8 + 223), 260);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), effectsWidget, updateGui());
        effectsWidget->show();

#ifndef GEONKICK_SINGLE
        auto kitTabs = new KitTabs(this, geonkickModel->getKitModel());
        kitTabs->setPosition(0, height() - kitTabs->height() - 1);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), kitTabs, updateView());
#endif // GEONKICK_SINGLE
}
