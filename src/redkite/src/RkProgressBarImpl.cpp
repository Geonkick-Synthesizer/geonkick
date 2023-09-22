/**
 * File name: RkProgressBarImpl.cpp
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

#include "RkProgressBarImpl.h"
#include "RkPainter.h"
#include "RkImage.h"
#include "RkLog.h"

RkProgressBar::RkProgressBarImpl::RkProgressBarImpl(RkProgressBar *interface, RkWidget *parent)
    : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
    , inf_ptr{interface}
    , beginVal{0}
    , endVal{100}
    , currentVal{beginVal}
    , progressColor{52, 116, 209}
    , progressOrientation{Rk::Orientation::Horizontal}
{
}

RkProgressBar::RkProgressBarImpl::~RkProgressBarImpl()
{
}


int RkProgressBar::RkProgressBarImpl::beginValue() const
{
        return beginVal;
}

void RkProgressBar::RkProgressBarImpl::setBeginValue(int val)
{
        if (currentVal < val)
                currentVal = val;
        beginVal = val;
}

int RkProgressBar::RkProgressBarImpl::endValue() const
{
        return endVal;
}

void RkProgressBar::RkProgressBarImpl::setEndValue(int val)
{
        if (currentVal > val)
                currentVal = val;
        endVal = val;
}

Rk::Orientation RkProgressBar::RkProgressBarImpl::orientation() const
{
        return progressOrientation;
}

void RkProgressBar::RkProgressBarImpl::setOrientation(Rk::Orientation orientation)
{
        progressOrientation  = orientation;
}

int RkProgressBar::RkProgressBarImpl::value() const
{
        return currentVal;
}

void RkProgressBar::RkProgressBarImpl::setValue(int val)
{
        if (val > endVal)
                currentVal = endVal;
        else if (val < beginVal)
                currentVal = beginVal;
        else
                currentVal = val;
}

void RkProgressBar::RkProgressBarImpl::setProgressColor(const RkColor &color)
{
        progressColor = color;
}

void RkProgressBar::RkProgressBarImpl::drawProgressBar()
{
        RkSize thisSize = size();
        RkImage img(thisSize);
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        int range = std::abs(endVal - beginVal);
        if (range > 0 && thisSize.width() > 0 && thisSize.height() > 0) {
                if (progressOrientation == Rk::Orientation::Horizontal)
                        painter.fillRect(RkRect(0, 0, currentVal * thisSize.width() / range, thisSize.height()),
                                         progressColor);
                else
                        painter.fillRect(RkRect(0, 0, thisSize.width(), currentVal * thisSize.height() / range),
                                         progressColor);
                RkPainter paint(inf_ptr);
                paint.drawImage(img, 0, 0);
         }
}
