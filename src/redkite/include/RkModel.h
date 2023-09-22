/**
 * File name: RkModel.h
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

#ifndef RK_MODEL_H
#define RK_MODEL_H

#include "Rk.h"
#include "RkObject.h"
#include "RkVariant.h"

class RK_EXPORT RkModel;

class RK_EXPORT RkModelItem {
 public:
        enum class DataType : int {
                String   = 0,
                Color    = 1,
                Size     = 2,
                Font     = 4,
                UserType = 5,
        };

        RkModelItem(RkModel *model, size_t index)
                : itemModel{model}
                , itemIndex{index}
        {
        }

        RkVariant data(int dataType = static_cast<int>(RkModelItem::DataType::String)) const;
        size_t getIndex() const { return itemIndex; }
        RkModel *model() const { return itemModel; }

 private:
        RkModel *itemModel;
        size_t itemIndex;
};

class RK_EXPORT RkModel: public RkObject {
  public:
        explicit RkModel(RkObject *parent);
        virtual ~RkModel() = default;
        RK_DECL_ACT(modelChanged, modelChanged(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(itemSelected, itemSelected(RkModelItem item), RK_ARG_TYPE(RkModelItem), RK_ARG_VAL(item));
        virtual RkVariant itemData(size_t index, int dataType = static_cast<int>(RkModelItem::DataType::String)) const = 0;
        virtual size_t itemsNumber() const = 0;
        virtual int itemSpan() const = 0;
        void selectItem(size_t index);
        bool isItemSelected(size_t index) const;
        bool isValidIndex(size_t index) const;

 protected:
        RK_DELCATE_IMPL_PTR(RkModel);

 private:
        RK_DISABLE_COPY(RkModel);
        RK_DISABLE_MOVE(RkModel);
};

#endif // RK_MODEL_H
