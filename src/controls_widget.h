/**
 * File name: controls_widget.h
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

#ifndef CONTROLS_WIDGET_H
#define CONTROLS_WIDGET_H

#include "geonkick_widget.h"

class GeonkickApi;
class Oscillator;
class EnvelopeWidget;
class GeonkickModel;

class ControlsWidget: public GeonkickWidget
{
 public:
        ControlsWidget(GeonkickWidget *parent, GeonkickModel* model);
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());

 private:
        GeonkickModel *geonkickModel;
};

#endif // CONTROLS_WIDGET_H
