/**
 * File name: RkWidgetContainer.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#include "RkContainer.h"
#include "RkContainerWidgetItem.h"

RkContainer::RkContainer(RkWidget *parent, Rk::Orientation orientation)
	: RkContainerItem(parent, ItemType::ItemContainer)
	, containerOrientation{orientation}
	, itemSpacing{0}
        , isHiddenTakesPlace{true}
{
	setSize(parent->size());
}

void RkContainer::addContainer(RkContainer *container, Rk::Alignment align)
{
        container->setAlignment(align);
	containerItems.push_back(container);
	update();
}

void RkContainer::addWidget(RkWidget *widget, Rk::Alignment align)
{
        auto item = new RkContainerWidgetItem(widget, align);
	containerItems.push_back(item);
	update();
}

void RkContainer::removeWidget(RkWidget *widget)
{
        std::erase_if(containerItems, [widget](const auto *i) {
                return i->type() == RkContainerItem::ItemType::ItemWidget
                        && dynamic_cast<const RkContainerWidgetItem*>(i)->widget() == widget; });
}

void RkContainer::addSpace(int space, Rk::Alignment align)
{
        auto item = new RkContainerItem(this, RkContainerItem::ItemType::ItemSpace, align);
        item->setSize({space, space});
        containerItems.push_back(item);
	update();
}

void RkContainer::removeAt(size_t index)
{
        if (index < containerItems.size()) {
                if (containerItems[index]->parent() == this)
                        delete containerItems[index];
                containerItems.erase(containerItems.begin() + index);
        }
}

RkContainerItem* RkContainer::at(size_t index) const
{
        if (index < containerItems.size())
                return containerItems[index];
        return nullptr;
}

void RkContainer::update()
{
	int posLeft = initPosition(Rk::Alignment::AlignLeft);
	int posRight = initPosition(Rk::Alignment::AlignRight);
	for (const auto &item: containerItems) {
                if (!hiddenTakesPlace() && item->isHidden())
                        continue;
                auto align = item->alignment();
                if (orientation() == Rk::Orientation::Horizontal) {
                        (align == Rk::Alignment::AlignLeft || align == Rk::Alignment::AlignTop) ?
                                item->setX(posLeft) : item->setX(posRight - item->width());
                        item->setY(y() + (height() - item->height()) / 2);
                } else {
                        (align == Rk::Alignment::AlignLeft || align == Rk::Alignment::AlignTop) ?
                                item->setY(posLeft) : item->setY(posRight - item->height());
                        item->setX(x() + (width() - item->width()) / 2);
                }

                auto w = (orientation() == Rk::Orientation::Horizontal) ? item->width() : item->height();
                if (align == Rk::Alignment::AlignLeft || align == Rk::Alignment::AlignTop)
                        posLeft += w + itemSpacing;
                else
                        posRight -= w + itemSpacing;
	}
}

int RkContainer::initPosition(Rk::Alignment alignment)
{
	int pos = 0;
	if (alignment == Rk::Alignment::AlignLeft) {
		if (orientation() == Rk::Orientation::Horizontal)
			pos = x();
		else
			pos = y();
	} else {
		if (orientation() == Rk::Orientation::Horizontal)
			pos = x() + width();
		else
			pos = y() + height();
	}
	return pos;
}

void RkContainer::clear()
{
        for (auto &item: containerItems) {
                if (item->parent() == this)
                        delete item;
        }
        containerItems.clear();
}

Rk::Orientation RkContainer::orientation() const
{
	return containerOrientation;
}

void RkContainer::setSize(const RkSize &size)
{
        RkContainerItem::setSize(size);
	update();
}

void RkContainer::setWidth(int width)
{
	RkContainerItem::setWidth(width);
	update();
}

void RkContainer::setHeight(int height)
{
	RkContainerItem::setHeight(height);
	update();
}

void RkContainer::setPosition(const RkPoint &position)
{
        RkContainerItem::setPosition(position);
	update();
}

void RkContainer::setX(int val)
{
        RkContainerItem::setX(val);
        update();
}

void RkContainer::setY(int val)
{
        RkContainerItem::setY(val);
        update();
}

void RkContainer::setSpacing(size_t space)
{
	itemSpacing = space;
	update();
}

size_t RkContainer::spacing() const
{
	return itemSpacing;
}

void RkContainer::setHiddenTakesPlace(bool b)
{
        isHiddenTakesPlace = b;
        update();
}

bool RkContainer::hiddenTakesPlace() const
{
        return isHiddenTakesPlace;
}
