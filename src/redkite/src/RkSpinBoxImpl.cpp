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
#include "RkImage.h"
#include "RkPainter.h"

RkSpinBox::RkSpinBoxImpl::RkSpinBoxImpl(RkSpinBox *interface,
                                        RkWidget *parent)
        : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
        , inf_ptr{interface}
        , currentItemIndex{-1}
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
        upButton->setType(RkButton::ButtonType::ButtonPush);
        upButton->show();
        downButton = new RkButton(inf_ptr);
        downButton->setType(RkButton::ButtonType::ButtonPush);
        downButton->show();       
        displayLabel = new RkLabel(inf_ptr);
        displayLabel->show();     
        updateControls();
}

void RkSpinBox::RkSpinBoxImpl::updateControls()
{
        upButton->setSize(inf_ptr->width() / 4, inf_ptr->height() / 2);
        displayLabel->setTextColor(inf_ptr->textColor());
        displayLabel->setBackgroundColor(inf_ptr->background());
        upButton->setPosition(inf_ptr->width() - upButton->width(), 0);
        {
                RkImage img(upButton->size());
                RkPainter painter(&img);
                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 upButton->background());
                auto pen = painter.pen();
                pen.setColor(upButton->textColor());
                painter.setPen(pen);
                painter.drawLine(img.width() / 2, 2, img.width() / 2, img.height() - 2);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                upButton->setImage(img, RkButton::State::Unpressed);

                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 RkColor(upButton->background().red() + 10,
                                         upButton->background().green() + 10,
                                         upButton->background().blue() + 10));
                pen = painter.pen();
                pen.setColor(RkColor(upButton->textColor().red() + 10,
                                     upButton->textColor().green() + 10,
                                     upButton->textColor().blue() + 10));
                painter.setPen(pen);
                painter.drawLine(img.width() / 2, 2, img.width() / 2, img.height() - 2);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                upButton->setImage(img, RkButton::State::UnpressedHover);
                upButton->setImage(img, RkButton::State::PressedHover);

                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 RkColor(upButton->background().red() + 60,
                                         upButton->background().green() + 60,
                                         upButton->background().blue() + 60));
                pen = painter.pen();
                pen.setColor(RkColor(upButton->textColor().red() + 20,
                                     upButton->textColor().green() + 20,
                                     upButton->textColor().blue() + 20));
                painter.setPen(pen);
                painter.drawLine(img.width() / 2, 2, img.width() / 2, img.height() - 2);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                upButton->setImage(img, RkButton::State::Pressed);
        }
        
        downButton->setSize(upButton->size());
        downButton->setPosition(upButton->x(), upButton->y() + upButton->height());
        {
                RkImage img(downButton->size());
                RkPainter painter(&img);
                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 downButton->background());
                auto pen = painter.pen();
                pen.setColor(downButton->textColor());
                painter.setPen(pen);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                downButton->setImage(img, RkButton::State::Unpressed);

                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 RkColor(downButton->background().red() + 10,
                                         downButton->background().green() + 10,
                                         downButton->background().blue() + 10));
                pen = painter.pen();
                pen.setColor(RkColor(downButton->textColor().red() + 10,
                                     downButton->textColor().green() + 10,
                                     downButton->textColor().blue() + 10));
                painter.setPen(pen);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                downButton->setImage(img, RkButton::State::UnpressedHover);
                downButton->setImage(img, RkButton::State::PressedHover);

                painter.fillRect(RkRect(1, 1, img.width() - 1, img.height()),
                                 RkColor(downButton->background().red() + 60,
                                         downButton->background().green() + 60,
                                         downButton->background().blue() + 60));
                pen = painter.pen();
                pen.setColor(RkColor(downButton->textColor().red() + 20,
                                     downButton->textColor().green() + 20,
                                     downButton->textColor().blue() + 20));
                painter.setPen(pen);
                painter.drawLine(2, img.height() / 2, img.width() - 2, img.height() / 2);
                downButton->setImage(img, RkButton::State::Pressed);
        }

        displayLabel->setSize(inf_ptr->width() - upButton->width(), inf_ptr->height());
}

void RkSpinBox::RkSpinBoxImpl::setCurrentIndex(int index)
{
        if (spinBoxItems.empty())
                return;

        currentItemIndex = std::clamp(index, 0, static_cast<int>(spinBoxItems.size() - 1));        
        if (std::holds_alternative<std::string>(spinBoxItems[static_cast<size_t>(currentItemIndex)]))
                displayLabel->setText(std::get<std::string>(spinBoxItems[static_cast<size_t>(currentItemIndex)]));
}

int RkSpinBox::RkSpinBoxImpl::currentIndex() const
{
        return currentItemIndex;
}

void RkSpinBox::RkSpinBoxImpl::addItem(const RkVariant& item)
{
        spinBoxItems.push_back(item);
}

RkVariant RkSpinBox::RkSpinBoxImpl::currentItem() const
{
        if (!spinBoxItems.empty()
            && static_cast<size_t>(currentItemIndex) < spinBoxItems.size())
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
