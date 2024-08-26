/**
 * File name: RkSpinBox.h
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

#ifndef RK_SPINBOX_H
#define RK_SPINBOX_H

#include "RkWidget.h"
#include "RkVariant.h"

class RkButton;

class RkSpinBox : public RkWidget {
 public:
    RkSpinBox(RkWidget *parent);
    virtual ~RkSpinBox() = default;
    void setRange(int from, int to);
    void setCurrentIndex(int index);
    int currentIndex() const;
    void addItem(const RkVariant& item);
    RkVariant currentItem() const;
    RkButton* upControl() const;
    RkButton* downControl() const;
    RK_DECL_ACT(currentIndexChanged,
                currentIndexChanged(int index),
                RK_ARG_TYPE(size_t),
                RK_ARG_VAL(index));

protected:
    RK_DECLARE_IMPL_PTR(RkSpinBox);
    void resizeEvent(RkResizeEvent *event) override;

 private:
    RK_DISABLE_COPY(RkSpinBox);
    RK_DISABLE_MOVE(RkSpinBox);
};

#endif // RK_SPINBOX_H
