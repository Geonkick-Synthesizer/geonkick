/**
 * File name: RkFlowContainer.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#include "RkFlowContainer.h"

RkFlowContainer::RkFlowContainer(RkWidget *parent)
        : RkContainer(parent, Rk::Orientation::Flow)
{
}

void RkFlowContainer::layout()
{
        int maxItemHeight = 0;
        for (const auto& item : getItems())
                maxItemHeight = std::max(maxItemHeight, item->height());

        int x = spacing() + padding();
        int y = spacing() + padding();
        maxItemHeight += spacing();
        for (auto& item : getItems()) {
                if (x + item->width() > width() - padding()) {
                        x = spacing() + padding();
                        y += maxItemHeight;
                }
                item->setPosition({x, y});
                x += item->width() + spacing();
        }

        setSize(RkSize{width(), y + maxItemHeight + padding()});
}
