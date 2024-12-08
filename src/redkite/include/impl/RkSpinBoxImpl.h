/**
 * File name: RkSpinBoxImpl.h
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

#ifndef RK_LABEL_IMPL_H
#define RK_LABEL_IMPL_H

#include "RkWidgetImpl.h"
#include "RkSpinBox.h"
#include "RkLabel.h"

class RkButton;
class RkLabel;

class SpinBoxLabel: public RkLabel
{
public:
        SpinBoxLabel(RkWidget* parent);
protected:
        void wheelEvent(RkWheelEvent *event) override;
};

class RkSpinBox::RkSpinBoxImpl : public RkWidget::RkWidgetImpl {
 public:
    RkSpinBoxImpl(RkSpinBox *interface, RkWidget *parent = nullptr);
    virtual ~RkSpinBoxImpl();
    void init();
    void setCurrentIndex(int index);
    int currentIndex() const;
    void addItem(const RkVariant& item);
    void clear();
    RkVariant currentItem() const;
    RkButton* upControl() const;
    RkButton* downControl() const;
    void updateControls();

 private:
    RK_DECALRE_INTERFACE_PTR(RkSpinBox);
    int currentItemIndex;
    std::vector<RkVariant> spinBoxItems;
    RkButton *upButton;
    RkButton *downButton;
    SpinBoxLabel *displayLabel;
};

#endif // RK_LABEL_IMPL_H
