/**
 * File name: RkSpinBox.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2024Iurie Nistor 
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
#define RK_LABEL_H

#include "RkWidget.h"

class RkSpinBox : public RkWidget {
 public:
    RkSpinBox(RkWidget *parent);
    virtual ~RkSpinBox() = default;
    void setCurrentIndex(size_t index);
    size_t currentIndex() const;
    void addItem(const std::string& text);
    void setUpControl(RkWidget* button) const;
    void setDownControl(RkWidget* button) const;
    RkWidget* upControl() const;
    RkWidget* downControl() const;
    std::string currentText() const;
    RK_DECL_ACT(currentIndexChanged,
                currentIndexChanged(size_t index),
                RK_ARG_TYPE(size_t),
                RK_ARG_VAL(index));

 protected:
    virtual void setupLayout();
    
 private:
    RK_DISABLE_COPY(RkSpinBox);
    RK_DISABLE_MOVE(RkSpinBox);
    RK_DELCATE_IMPL_PTR(RkSpinBox);
};

#endif // RK_SPINBOX_H
