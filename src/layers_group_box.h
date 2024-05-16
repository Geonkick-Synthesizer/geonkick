/**
 * File name: layers_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor 
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

#ifndef GEONKICK_LAYERS_GROUPBOX_H
#define GEONKICK_LAYERS_GROUPBOX_H

#include "geonkick_groupbox.h"

class GeonkickSlider;
class GeonkickApi;
class GeonkickButton;

class LayersGroupBox: public GeonkickGroupBox
{
 public:
        LayersGroupBox(GeonkickApi *api, GeonkickWidget *parent);
        void updateGui();

 protected:
        void setLayerAmplitude(int layer, int val);

 private:
        GeonkickApi *geonkickApi;
        GeonkickSlider *layerSliders[3];
};

#endif // GEONKICK_LAYERS_WIDGET_H
