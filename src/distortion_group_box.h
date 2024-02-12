/**
 * File name: distortion_group_box.h
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

#ifndef GEONKICK_DISTORTION_GROUPBOX_H
#define GEONKICK_DISTORTION_GROUPBOX_H

#include "geonkick_groupbox.h"
#include "envelope.h"

class GeonkickSlider;
class GeonkickApi;
class GeonkickButton;
class EnvelopeWidget;

class DistortionGroupBox: public GeonkickGroupBox
{
 public:
        DistortionGroupBox(GeonkickApi *api, GeonkickWidget *parent);
        ~DistortionGroupBox() = default;

        void updateGui();
        void setVolume(int val);
        void setDrive(int val);
        void setInLimiter(int val);
 private:
        GeonkickApi *geonkickApi;
        GeonkickSlider *volumeSlider;
        GeonkickSlider *inLimiterSlider;
        GeonkickSlider *driveSlider;
        GeonkickButton *distortionCheckbox;
        GeonkickButton *volumeEnvButton;
        GeonkickButton *driveEnvButton;
};

#endif // GEONKICK_DISTORTION_WIDGET_H
