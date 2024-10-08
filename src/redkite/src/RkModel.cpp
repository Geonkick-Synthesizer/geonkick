/**
 * File name: RkModel.cpp
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

#include "RkModel.h"
#include "RkModelImpl.h"

RkVariant RkModelItem::data(int dataType) const
{
        return itemModel->itemData(getIndex(), dataType);
}

RkModel::RkModel(RkObject *parent)
        : RkObject(parent, std::make_unique<RkModel::RkModelImpl>(this, parent))
        , impl_ptr{static_cast<RkModel::RkModelImpl*>(o_ptr.get())}
{
}

bool RkModel::isItemSelected(size_t index) const
{
        return impl_ptr->isItemSelected(index);
}

void RkModel::selectItem(size_t index)
{
        if (isValidIndex(index)) {
                impl_ptr->selectItem(index);
                action modelChanged();
                action itemSelected(RkModelItem(this, index));
        }
}

bool RkModel::isValidIndex(size_t index) const
{
        return index < itemsNumber();
}
