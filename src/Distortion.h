/**
 * File name: Distortion.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef DISTORTION_H
#define DISTORTION_H

#include "GeonkickWidget.h"

class DistortionModel;

class Distortion: public GeonkickWidget
{
 public:
        Distortion(GeonkickWidget* parent, DistortionModel *parent);
        void createView();
        void updateView();

 private:
        DistortionModel *distortionModel;
};

#endif // DISTORTION_H
