/**
 * File name: compressor_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
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

#ifndef GEONKICK_COMPRESSOR_GROUPBOX_H
#define GEONKICK_COMPRESSOR_GROUPBOX_H

#include "geonkick_groupbox.h"

class GeonkickSlider;
class GeonkickApi;
class GeonkickButton;

class CompressorGroupBox: public GeonkickGroupBox
{
 public:
        CompressorGroupBox(GeonkickApi *api, GeonkickWidget* parent);
        void updateGui();
        void setAttack(int val);
        void setThreshold(int val);
        void setRatio(int val);
        void setMakeup(int val);

 private:
        GeonkickApi *geonkickApi;
        GeonkickSlider *attackSlider;
        GeonkickSlider *thresholdSlider;
        GeonkickSlider *ratioSlider;
        GeonkickSlider *makeupSlider;
        GeonkickButton *compressorCheckbox;
};

#endif // GEONKICK_COMPRESSOR_WIDGET_H
