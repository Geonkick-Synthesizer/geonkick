/**
 * File name: effects_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://iuriepage.wordpress.com)
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

#ifndef GEONKICK_EFFECTS_GROUP_BOX_H
#define GEONKICK_EFFECTS_GROUP_BOX_H

#include "geonkick_groupbox.h"

class GeonkickApi;
class EnvelopeWidget;
class DistortionGroupBox;

class EffectsGroupBox: public GeonkickGroupBox
{
 public:
        EffectsGroupBox(GeonkickApi *api, GeonkickWidget *parent);
        ~EffectsGroupBox() = default;
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());
        void setEnvelopeWidget(EnvelopeWidget* widget);

 private:
        GeonkickApi *geonkickApi;
        DistortionGroupBox *distortionGroupBox;
};

#endif // GKICK_GENERAL_GROUP_BOX_H
