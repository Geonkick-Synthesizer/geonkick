/**
 * File name: control_area.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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
#include "SampleBrowser.h"
#include "SettingsWidget.h"

ControlArea::ControlArea(GeonkickWidget *parent,
                         KitModel* model,
                         const std::vector<std::unique_ptr<Oscillator>> &oscillators)
        : GeonkickWidget(parent)
        , kitModel{model}
        , oscillators{oscillators}
        , presetsModel{new PresetBrowserModel(this, kitModel->api())}
        , currentWidget{nullptr}
        , controlsWidget{nullptr}
#ifndef GEONKICK_SINGLE
        , kitWidget{nullptr}
#endif // GEONKICK_SINGLE
        , presetsWidget{nullptr}
        , samplesWidget{nullptr}
        , settingsWidget{nullptr}

{
        setName("ControlArea");
        setFixedSize(920, 370);
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view), this, showWidget(view));
        showWidget(viewState()->getMainView());
}

void ControlArea::showWidget(ViewState::View view)
{
        switch (view) {
        case ViewState::View::Controls:
                showControls();
                break;
#ifndef GEONKICK_SINGLE
        case ViewState::View::Kit:
                showKit();
                break;
#endif // GEONKICK_SINGLE
        case ViewState::View::Presets:
                showPresets();
                break;
        case ViewState::View::Samples:
                if (currentWidget)
                        currentWidget->hide();
                if (!samplesWidget)
                        samplesWidget = new SampleBrowser(this, kitModel->api());
                currentWidget = samplesWidget;
                currentWidget->show();
                break;
        case ViewState::View::Settings:
                if (currentWidget)
                        currentWidget->hide();
                if (!settingsWidget)
                        settingsWidget = new SettingsWidget(this, kitModel->api());
                currentWidget = settingsWidget;
                currentWidget->show();
                break;
        default:
                showControls();
        }
}

void ControlArea::showControls()
{
        if (!dynamic_cast<ControlsWidget*>(currentWidget)) {
                if (currentWidget)
                        currentWidget->hide();
                if (!controlsWidget) {
                        controlsWidget = new ControlsWidget(this, kitModel->api(), oscillators);
                        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlsWidget, updateGui());
                }
                currentWidget = controlsWidget;
                controlsWidget->setSize({width(), height()});
                currentWidget = controlsWidget;
                currentWidget->show();
        }
}

#ifndef GEONKICK_SINGLE
void ControlArea::showKit()
{
        if (!dynamic_cast<KitWidget*>(currentWidget)) {
                if (currentWidget)
                        currentWidget->hide();
                if (!kitWidget)
                        kitWidget = new KitWidget(this, kitModel);
                currentWidget = kitWidget;
                currentWidget->show();
        }
}
#endif // GEONKICK_SINGLE

void ControlArea::showPresets()
{
        if (!dynamic_cast<PresetBrowserView*>(currentWidget)) {
                if (currentWidget)
                        currentWidget->hide();
                if (!presetsWidget)
                        presetsWidget = new PresetBrowserView(this, presetsModel);
                currentWidget = presetsWidget;
                currentWidget->show();
        }
}

KitModel* ControlArea::getKitModel() const
{
        return kitModel;
}
