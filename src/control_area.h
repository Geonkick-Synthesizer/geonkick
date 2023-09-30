/**
 * File name: control_area.h
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

#ifndef GKICK_CONTROL_AREA_H
#define GKICK_CONTROL_AREA_H

#include "envelope.h"
#include "geonkick_widget.h"
#include "ViewState.h"

class Oscillator;
class ControlsWidget;
class KitModel;
#ifndef GEONKICK_SINGLE
class KitWidget;
#endif // GEONKICK_SINGLE
class EnvelopeWidget;
class PresetBrowserView;
class PresetBrowserModel;

class ControlArea: public GeonkickWidget
{
 public:
        ControlArea(GeonkickWidget *parent,
                    KitModel* model,
                    const std::vector<std::unique_ptr<Oscillator>> &oscillators);
        ~ControlArea() = default;
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());
        void showControls();
#ifndef GEONKICK_SINGLE
        void showKit();
#endif // GEONKICK_SINGLE
        void showPresets();
        KitModel* getKitModel() const;
 private:
        void showWidget(ViewState::View view);

 private:
        KitModel *kitModel;
        const std::vector<std::unique_ptr<Oscillator>> &oscillators;
        PresetBrowserModel *presetsModel;
        RkWidget* currentWidget;
};

#endif // GKICK_CONTROL_AREA_H
