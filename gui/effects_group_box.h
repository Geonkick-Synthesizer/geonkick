/**
 * File name: effects_group_box.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

class EffectsGroupBox: public GeonkickGroupBox
{
        Q_OBJECT
 public:
        EffectsGroupBox(GeonkickWidget *parent);
        ~EffectsGroupBox();
};

#endif // GKICK_GENERAL_GROUP_BOX_H
