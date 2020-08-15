/**
 * File name: control_area.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "control_area.h"
#include "controls_widget.h"
#include "kit_model.h"
#include "kit_widget.h"
#include "preset_browser_model.h"
#include "preset_browser_view.h"

ControlArea::ControlArea(GeonkickWidget *parent,
                         GeonkickApi* api,
                         const std::vector<std::unique_ptr<Oscillator>> &oscillators)
        : GeonkickWidget(parent)
        , geonkickApi{api}
        , oscillators{oscillators}
        , kitModel{new KitModel(this, geonkickApi)}
        , presetsModel{new PresetBrowserModel(this, api)}
        , envelopeWidget{nullptr}
        , currentWidget{nullptr}
{
        setFixedSize(920, 370);
        auto viewState = static_cast<ViewState*>(findObject("ViewState"));
        RK_ACT_BIND(viewState, stateChanged, RK_ACT_ARGS(ViewState::Type state), this, showWidget(state));
        showWidget(viewState->state());
}

void ControlArea::showWidget(ViewState::Type state)
{
        switch (state) {
        case ViewState::Type::Controls:
                showControls();
                break;
        case ViewState::Type::Kit:
                showKit();
                break;
        case ViewState::Type::Presets:
                showPresets();
                break;
        case ViewState::Type::Samples:
                //                showSamplesWidget();
                break;
        default:
                showControls();
        }
}

void ControlArea::showControls()
{
        if (!dynamic_cast<ControlsWidget*>(currentWidget)) {
                delete currentWidget;
                auto controlsWidget = new ControlsWidget(this, geonkickApi, oscillators);
                controlsWidget->setEnvelopeWidget(envelopeWidget);
                RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlsWidget, updateGui());
                controlsWidget->setSize({width(), height()});
                currentWidget = controlsWidget;
                currentWidget->show();
        }
}

void ControlArea::showKit()
{
        if (!dynamic_cast<KitWidget*>(currentWidget)) {
                delete currentWidget;
                currentWidget = new KitWidget(this, kitModel);
                currentWidget->show();
        }
}

void ControlArea::showPresets()
{
        if (!dynamic_cast<PresetBrowserView*>(currentWidget)) {
                delete currentWidget;
                currentWidget = new PresetBrowserView(this, presetsModel);
                currentWidget->show();
        }
}

KitModel* ControlArea::getKitModel() const
{
        return kitModel;
}

void ControlArea::setEnvelopeWidget(EnvelopeWidget *widget)
{
        envelopeWidget = widget;
        auto controlsWidget = dynamic_cast<ControlsWidget*>(currentWidget);
        if (controlsWidget)
                controlsWidget->setEnvelopeWidget(envelopeWidget);
}
