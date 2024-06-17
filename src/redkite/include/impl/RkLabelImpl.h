/**
 * File name: RkLabelImpl.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor 
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
#include "RkLabel.h"
#include "RkImage.h"

class RkLabel::RkLabelImpl : public RkWidget::RkWidgetImpl {
 public:
    RkLabelImpl(RkLabel *interface,
                const std::string &text,
                RkWidget *parent = nullptr);

    virtual ~RkLabelImpl();
    void setText(const std::string &text);
    std::string text() const;
    void setImage(const RkImage &image);
    void drawLabel();

 private:
    RK_DECALRE_INTERFACE_PTR(RkLabel);
    std::string labelText;
    RkImage labelImage;
};

#endif // RK_LABEL_IMPL_H
