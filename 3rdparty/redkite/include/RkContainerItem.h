/**
 * File name: RkContainerItem.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#ifndef RK_CONTAINER_ITEM_H
#define RK_CONTAINER_ITEM_H

#include <RkWidget.h>

class RK_EXPORT RkContainerItem: public RkObject {
 public:
        enum class ItemType : int {
                ItemContainer = 0,
                ItemWidget    = 1,
                ItemSpace     = 2,
        };
         RkContainerItem(RkObject *parent = nullptr,
                        ItemType type = ItemType::ItemContainer,
                        Rk::Alignment align = Rk::Alignment::AlignLeft)
                 : RkObject(parent)
                 , itemType{type}
                 , itemAlignment{align}
                 , itemHidden{false} {}
        virtual ~RkContainerItem() = default;
        ItemType type() const { return itemType; }
        virtual void setPosition(const RkPoint &point) { itemPosition = point; }
        virtual RkPoint position() const { return itemPosition; }
        virtual void setX(int val) { itemPosition.setX(val); }
        virtual int x() const { return itemPosition.x(); }
        virtual void setY(int val) { itemPosition.setY(val); }
        virtual int y() const { return itemPosition.y(); }
        virtual void setSize(const RkSize &size) { itemSize = size; }
        virtual RkSize size() const { return itemSize; }
        virtual void setWidth(int val) { itemSize.setWidth(val); }
        virtual int width() const { return itemSize.width(); }
        virtual void setHeight(int val) { itemSize.setHeight(val); }
        virtual int height() const { return itemSize.height(); }
        void setAlignment(Rk::Alignment align) { itemAlignment = align; }
        Rk::Alignment alignment() const { return itemAlignment; }
        virtual void hide(bool b) { itemHidden = b; }
        virtual bool isHidden() const { return itemHidden; }

 private:
        RK_DISABLE_COPY(RkContainerItem);
        RK_DISABLE_MOVE(RkContainerItem);
        RkSize itemSize;
        RkPoint itemPosition;
        ItemType itemType;
        Rk::Alignment itemAlignment;
        bool itemHidden;
};

#endif // RK_CONTAINER_ITEM_H
