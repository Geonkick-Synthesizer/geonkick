/**
 * File name: DistortionView.h
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

#ifndef DISTORTION_VIEW_H
#define DISTORTION_VIEW_H

#include "AbstractView.h"

class DistortionModel;

class DistortionView: public AbstractView
{
 public:
        explicit DistortionView(GeonkickWidget* parent, DistortionModel *model);
        void createView() override;
        void updateView() override;

protected:
        void bindModel() override;
        void unbindModel() override;
};

#endif // DISTORTION_VEW_H