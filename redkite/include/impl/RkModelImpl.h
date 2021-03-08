/**
 * File name: RkModelImpl.h
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

#ifndef RK_MODEL_IMPL_H
#define RK_MODEL_IMPL_H

#include "RkModel.h"
#include "RkObjectImpl.h"

class RkModel::RkModelImpl : public RkObject::RkObjectImpl {
 public:
        RkModelImpl(RkModel* interface, RkObject* parent);
        virtual ~RkModelImpl() = default;
        bool isItemSelected(size_t index) const;
        void selectItem(size_t index);

 private:
        RK_DECALRE_INTERFACE_PTR(RkModel);
        size_t selectedIndex;
};

#endif // RK_MODEL_IMPL_H
