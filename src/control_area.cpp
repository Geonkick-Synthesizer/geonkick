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
#include "GeonkickModel.h"
#include "kit_widget.h"

ControlArea::ControlArea(GeonkickWidget *parent, GeonkickModel* model)
        : GeonkickWidget(parent)
        , geonkickModel{model}
        , oscillators{model->getOscillatorModels()}
        , currentWidget{nullptr}
        , controlsWidget{nullptr}
#ifndef GEONKICK_SINGLE
        , kitWidget{nullptr}
#endif // GEONKICK_SINGLE
{
        setFixedSize(920, 375);
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
                        controlsWidget = new ControlsWidget(this, geonkickModel);
                        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlsWidget, updateGui());
                }
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
                        kitWidget = new KitWidget(this, geonkickModel->getKitModel());
                currentWidget = kitWidget;
                currentWidget->show();
        }
}
#endif // GEONKICK_SINGLE
