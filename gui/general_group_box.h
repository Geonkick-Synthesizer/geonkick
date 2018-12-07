/**
 * File name: general_group_box.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#ifndef GKICK_GENERAL_GROUP_BOX_H
#define GKICK_GENERAL_GROUP_BOX_H

#include "oscillator.h"
#include "geonkick_groupbox.h"

class Oscillator;
class GeonkickApi;
class GeonkickCheckbox;
class Knob;
class GeonkickButton;

class GeneralGroupBox: public GeonkickGroupBox
{
        Q_OBJECT
 public:
        GeneralGroupBox(GeonkickWidget *parent, GeonkickApi *api);
        ~GeneralGroupBox();

public slots:
        void update();

 protected:
        void createAplitudeEnvelopeHBox();
        void createFilterHBox();
 protected slots:
        void setFilterType(bool state);

 private:
        GeonkickApi* geonkickApi;
        GeonkickCheckbox *filterCheckbox;
        Knob *kickAmplitudeKnob;
        Knob *kickLengthKnob;
        Knob *kickFrequencyKnob;
        Knob *kickQFactorKnob;
        GeonkickButton *filterType;
};

#endif
