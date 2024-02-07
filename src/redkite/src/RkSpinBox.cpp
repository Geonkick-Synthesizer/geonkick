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
    std::string currentText() const;
    RK_DECL_ACT(currentIndexChanged,
                currentIndexChanged(size_t index),
                RK_ARG_TYPE(size_t),
                RK_ARG_VAL(index));

 protected:
    virtual void paintEvent(RkPaintEvent *event) override;

 private:
    RK_DISABLE_COPY(RkSpinBox);
    RK_DISABLE_MOVE(RkSpinBox);
    RK_DELCATE_IMPL_PTR(RkSpinBox);
};

#endif // RK_SPINBOX_H


#include "RkSpinBox.h"
#include "RkSpinBoxImpl.h"
#include "RkLog.h"

RkSpinBox::RkSpinBox(RkWidget *parent)
        : RkWidget(parent, std::make_unique<RkSpinBox::RkSpinBoxImpl>(this, parent))
        , impl_ptr{static_cast<RkSpinBox::RkSpinBoxImpl*>(o_ptr.get())}
{
        RK_LOG_DEBUG("called");
}

void RkSpinBox::setCurrentIndex(size_t index)
{
        impl_ptr->setCurrentIndex(index);
        update();
}

size_t RkSpinBox::currentIndex() const
{
        return impl_ptr->currentIndex();
}

void RkSpinBox::addItem(const std::string& text)
{
        return impl_ptr->addItem(text);
}

std::string RkSpinBox::currentText() const
{
        return impl_ptr->currentText();
}
