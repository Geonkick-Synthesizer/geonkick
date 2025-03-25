/**
 * File name: RkContainerWidgetItem.h
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

#ifndef RK_CONTAINER_WIDGET_ITEM_H
#define RK_CONTAINER_WIDGET_ITEM_H

#include <RkContainerItem.h>

class RkWidget;

class RkContainerWidgetItem: public RkContainerItem {
 public:
        RkContainerWidgetItem(RkWidget *parent,
                            Rk::Alignment align = Rk::Alignment::AlignLeft)
                 : RkContainerItem(parent, ItemType::ItemWidget, align)
                   , itemWidget{parent} {}
        virtual ~RkContainerWidgetItem() = default;
        void setPosition(const RkPoint &point) override { itemWidget->setPosition(point); }
        RkPoint position() const override { return itemWidget->position(); }
        void setX(int val) override { itemWidget->setX(val); }
        int x() const override  { return itemWidget->x(); }
        void setY(int val) override  { itemWidget->setY(val); }
        int y() const override { return itemWidget->y(); }
        void setSize(const RkSize &size) override { itemWidget->setSize(size); }
        RkSize size() const override { return itemWidget->size(); }
        void setWidth(int val) override { itemWidget->setWidth(val); }
        int width() const override { return itemWidget->width(); }
        void setHeight(int val) override { itemWidget->setHeight(val); }
        int height() const override { return itemWidget->height(); }
        RkWidget* widget() const { return itemWidget; }
        void hide(bool b) override { return b ? itemWidget->hide() : itemWidget->show(); }
        bool isHidden() const override { return !itemWidget->isVisible(); }

 private:
        RK_DISABLE_COPY(RkContainerWidgetItem);
        RK_DISABLE_MOVE(RkContainerWidgetItem);
        RkSize itemSize;
        RkWidget *itemWidget;
};

#endif // RK_CONTAINER_WIDGET_ITEM_H
