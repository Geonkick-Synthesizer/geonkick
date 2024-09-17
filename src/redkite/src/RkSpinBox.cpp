/**
 * File name: RkSpinBox.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#include "RkSpinBox.h"
#include "RkSpinBoxImpl.h"
#include "RkButton.h"
#include "RkLog.h"
#include "RkEvent.h"

RkSpinBox::RkSpinBox(RkWidget *parent)
        : RkWidget(parent, std::make_unique<RkSpinBox::RkSpinBoxImpl>(this, parent))
        , impl_ptr{static_cast<RkSpinBox::RkSpinBoxImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called");
        impl_ptr->init();
        RK_ACT_BIND(upControl(),
                    pressed,
                    RK_ACT_ARGS(),
                    this, setCurrentIndex(currentIndex() + 1));
        RK_ACT_BIND(downControl(),
                    pressed,
                    RK_ACT_ARGS(),
                    this, setCurrentIndex(currentIndex() - 1));
}

void RkSpinBox::setRange(int from, int to)
{
        for (auto i = from; i <= to; i++)
                addItem(std::to_string(i));
        setCurrentIndex(0);
}

void RkSpinBox::setCurrentIndex(int index)
{
        if (index == impl_ptr->currentIndex())
                return;

        impl_ptr->setCurrentIndex(index);
        action currentIndexChanged(impl_ptr->currentIndex());
}

int RkSpinBox::currentIndex() const
{
        return impl_ptr->currentIndex();
}

void RkSpinBox::addItem(const RkVariant& item)
{
        return impl_ptr->addItem(item);
}

RkVariant RkSpinBox::currentItem() const
{
        return impl_ptr->currentItem();
}

RkButton* RkSpinBox::upControl() const
{
        return impl_ptr->upControl();
}

RkButton* RkSpinBox::downControl() const
{
        return impl_ptr->downControl();
}

void RkSpinBox::resizeEvent([[maybe_unused]]RkResizeEvent *event)
{
        impl_ptr->updateControls();
}

void RkSpinBox::wheelEvent(RkWheelEvent *event)
{
        if (event->direction() == RkWheelEvent::WheelDirection::DirectionUp)
                setCurrentIndex(currentIndex() + 1);
        else
                setCurrentIndex(currentIndex() - 1);
}
