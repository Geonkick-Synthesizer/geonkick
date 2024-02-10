/**
 * File name: RkSpinBoxImpl.cpp
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

#include "RkSpinBoxImpl.h"
#include "RkButton.h"
#include "RkLabel.h"

RkSpinBox::RkSpinBoxImpl::RkSpinBoxImpl(RkSpinBox *interface,
                                        RkWidget *parent)
        : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
        , inf_ptr{interface}
        , currentItemIndex{0}
        , upButton{nullptr}
        , downButton{nullptr}
        , displayLabel{nullptr}
{
}

RkSpinBox::RkSpinBoxImpl::~RkSpinBoxImpl()
{
}

void RkSpinBox::RkSpinBoxImpl::init()
{
        upButton = new RkButton(inf_ptr);
        upButton->setBackgroundColor(0, 0, 255);
        upButton->setSize(inf_ptr->width() / 4, inf_ptr->height() / 2);
        //        upButton->setPosition(inf_ptr->width() - upButton->width(), 0);
        upButton->show();
        
        /*downButton = new RkButton(inf_ptr);
        downButton->setBackgroundColor(45, 99, 255);
        downButton->setSize(upButton->size());
        downButton->setPosition(upButton->x(), upButton->y() + upButton->height());
        downButton->show();
        
        displayLabel = new RkLabel(inf_ptr);
        displayLabel->setBackgroundColor(0, 255, 0);
        displayLabel->setSize(inf_ptr->width() - upButton->width(), inf_ptr->height());
        displayLabel->show();*/
}


void RkSpinBox::RkSpinBoxImpl::setCurrentIndex(size_t index)
{
        if (!spinBoxItems.empty() && index < spinBoxItems.size()) {
                currentItemIndex = index;
                if (std::holds_alternative<std::string>(spinBoxItems[currentItemIndex]))
                        displayLabel->setText(std::get<std::string>(spinBoxItems[currentItemIndex]));
        }
}

size_t RkSpinBox::RkSpinBoxImpl::currentIndex() const
{
        return currentItemIndex;
}

void RkSpinBox::RkSpinBoxImpl::addItem(const RkVariant& item)
{
        spinBoxItems.push_back(item);
}

RkVariant RkSpinBox::RkSpinBoxImpl::currentItem() const
{
        if (!spinBoxItems.empty() && currentItemIndex < spinBoxItems.size())
                return spinBoxItems[currentItemIndex];
        return {};
}

RkButton* RkSpinBox::RkSpinBoxImpl::upControl() const
{
        return upButton;
}

RkButton* RkSpinBox::RkSpinBoxImpl::downControl() const
{
        return downButton;
}
