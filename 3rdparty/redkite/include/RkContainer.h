/**
 * File name: RkWidgetContainer.h
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

#ifndef RK_CONTAINER_H
#define RK_CONTAINER_H

#include "RkWidget.h"
#include "RkContainerItem.h"

class RK_EXPORT RkContainer: public RkContainerItem {
 public:
        RkContainer(RkWidget *parent  = nullptr, Rk::Orientation orientation = Rk::Orientation::Horizontal);
        virtual ~RkContainer() = default;
        void addContainer(RkContainer *contaier, Rk::Alignment align = Rk::Alignment::AlignLeft);
	void addWidget(RkWidget *widget, Rk::Alignment align = Rk::Alignment::AlignLeft);
	void removeWidget(RkWidget *widget);
	void addSpace(int space, Rk::Alignment align = Rk::Alignment::AlignLeft);
	void removeAt(size_t index);
        RkContainerItem* at(size_t index) const;
	void update();
	void clear();
	Rk::Orientation orientation() const;
	Rk::Alignment alignment(RkWidget *widget) const;
	Rk::Alignment alignment(size_t index) const;
	void setSize(const RkSize &size) override;
	void setWidth(int width) override;
	void setHeight(int height) override;
	void setPosition(const RkPoint &position) override;
        void setX(int val) override;
        void setY(int val) override;
	void setSpacing(size_t space);
	size_t spacing() const;
        void setHiddenTakesPlace(bool b = true);
        bool hiddenTakesPlace() const;

 protected:
	int initPosition(Rk::Alignment alignment);

 private:
        RK_DISABLE_COPY(RkContainer);
        RK_DISABLE_MOVE(RkContainer);
        std::vector<RkContainerItem*> containerItems;
	Rk::Orientation containerOrientation;
	size_t itemSpacing;
        bool isHiddenTakesPlace;
};

#endif // RK_CONTAINER_H
