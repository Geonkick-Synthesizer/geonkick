/**
 * File name: oscillator_group_box.h
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
 *
 * This file is part of MashaFile.
 *
 * MashaFile is free software; you can redistribute it and/or modify
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

#include "gkick_oscillator.h"
#include "control_group_box.h"

class GkickOscillator;
class QComboBox;
class GKickApi;

class GeneralGroupBox: public ControlGroupBox
{
        Q_OBJECT
 public:
        GeneralGroupBox(QWidget *widget, std::shared_ptr<GKickApi> &api);
        ~GeneralGroupBox();

 protected:
        void createFilterGroupBox();

 private:
        std::shared_ptr<GKickApi> kickApi;
        QComboBox *filterTypeCb;
};

#endif
