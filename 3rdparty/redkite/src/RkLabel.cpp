/**
 * File name: RkLabel.cpp
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

#include "RkLabel.h"
#include "RkLabelImpl.h"
#include "RkLog.h"

RkLabel::RkLabel(RkWidget *parent, const std::string &text)
        : RkWidget(parent, std::make_unique<RkLabel::RkLabelImpl>(this, text, parent))
        , impl_ptr{static_cast<RkLabel::RkLabelImpl*>(o_ptr.get())}
{
}

void RkLabel::setText(const std::string &text)
{
        impl_ptr->setText(text);
        update();
}

std::string RkLabel::text() const
{
     return impl_ptr->text();
}

void RkLabel::setImage(const RkImage &image)
{
        impl_ptr->setImage(image);
}

void RkLabel::paintEvent(RkPaintEvent *event)
{
        RK_UNUSED(event);
        impl_ptr->drawLabel();
}
