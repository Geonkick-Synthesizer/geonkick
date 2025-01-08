/**
 * File name: general_group_box.h
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

#ifndef GKICK_GENERAL_GROUP_BOX_H
#define GKICK_GENERAL_GROUP_BOX_H

#include "AbstractView.h"

class Knob;
class GeonkickButton;
class PercussionModel;
class InstrumentGlobalEffects;

class GeneralGroupBox: public AbstractView
{
 public:
        GeneralGroupBox(GeonkickWidget *parent, PercussionModel *model);
        void createView() override;
        void updateView() override;

 protected:
        void bindModel() override;
        void unbindModel() override;
        void createAplitudeEnvelopeHBox();
        void createEffects();

 private:
        Knob *instrumentAmplitudeKnob;
        Knob *instrumentLengthKnob;
        InstrumentGlobalEffects *globalEffects;
};

#endif //GKICK_GENERAL_GROUP_BOX_H
