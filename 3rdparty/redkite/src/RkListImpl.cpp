/**
 * File name: RkListImpl.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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

#include "RkListImpl.h"
#include "RkLog.h"
#include "RkModel.h"

RkList::RkListImpl::RkListImpl(RkList *interface,
                               RkWidget *parent, RkModel *model)
    : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
    , inf_ptr{interface}
    , listModel{model}
    , topMargin{0}
    , leftMargin{10}
{
}

void RkList::RkListImpl::drawList(RkPainter &painter)
{
        int y = topMargin;
        size_t i = 0;
        RkPen pen = painter.pen();
        while (i < listModel->itemsNumber()) {
                RkVariant itemData = listModel->itemData(i, static_cast<int>(RkModelItem::DataType::Color));
                if (std::holds_alternative<RkColor>(itemData)) {
                        pen.setColor(std::get<RkColor>(itemData));
                        painter.setPen(pen);
                }

                itemData = listModel->itemData(i, static_cast<int>(RkModelItem::DataType::Font));
                if (std::holds_alternative<RkFont>(itemData))
                        painter.setFont(std::get<RkFont>(itemData));

                if  (listModel->isItemSelected(i)) {
                        pen.setColor({255, 255, 255});
                        painter.setPen(pen);
                }

                itemData = listModel->itemData(i, static_cast<int>(RkModelItem::DataType::String));
                if (std::holds_alternative<std::string>(itemData)) {
                        RkFont font = painter.font();
                        painter.drawText(RkRect(leftMargin, y, inf_ptr->width() - leftMargin, listModel->itemSpan()),
                                         std::get<std::string>(itemData),
                                         Rk::Alignment::AlignLeft);
                }

                y += listModel->itemSpan();
                i++;
        }
}

RkModel* RkList::RkListImpl::getModel() const
{
        return listModel;
}

size_t RkList::RkListImpl::getIndex(int y) const
{
        if (listModel->itemSpan() > 0)
                return (y - topMargin) / listModel->itemSpan();
        return -1;
}
