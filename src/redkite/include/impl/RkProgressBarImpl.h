/**
 * File name: RkProgressBarImpl.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor 
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#ifndef RK_PROGRESS_BAR_IMPL_H
#define RK_PROGRESS_BAR_IMPL_H

#include "RkWidgetImpl.h"
#include "RkProgressBar.h"

class RkProgressBar::RkProgressBarImpl : public RkWidget::RkWidgetImpl {
 public:
        RkProgressBarImpl(RkProgressBar *interface, RkWidget *parent = nullptr);
        virtual ~RkProgressBarImpl();

        int beginValue() const;
        void setBeginValue(int val);
        int endValue() const;
        void setEndValue(int val);
        Rk::Orientation orientation() const;
        void setOrientation(Rk::Orientation orientation);
        int value() const;
        void setValue(int val);
        void setProgressColor(const RkColor &color);
        void drawProgressBar();

 private:
        RK_DECALRE_INTERFACE_PTR(RkProgressBar);
        int beginVal;
        int endVal;
        int currentVal;
        RkColor progressColor;
        Rk::Orientation progressOrientation;
};

#endif // RK_PROGRESS_BAR_H
