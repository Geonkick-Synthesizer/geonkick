/**
 * File name: RkList.cpp
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

#include "RkList.h"
#include "RkModel.h"
#include "RkListImpl.h"
#include "RkLog.h"
#include "RkPainter.h"
#include "RkEvent.h"

RkList::RkList(RkWidget *parent, RkModel *model)
        : RkWidget(parent, std::make_unique<RkList::RkListImpl>(this, parent, model))
        , impl_ptr{static_cast<RkList::RkListImpl*>(o_ptr.get())}
{
        RK_ACT_BIND(getModel(), modelChanged, RK_ACT_ARGS(), this, update());
}

RkModel* RkList::getModel() const
{
        return impl_ptr->getModel();
}

void RkList::paintEvent(RkPaintEvent *event)
{
        RkImage img(size());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        impl_ptr->drawList(painter);
        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void RkList::keyPressEvent(RkKeyEvent *event)
{
}

void RkList::keyReleaseEvent(RkKeyEvent *event)
{
}

void RkList::mouseMoveEvent(RkMouseEvent *event)
{
}

void RkList::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left)
                getModel()->selectItem(impl_ptr->getIndex(event->y()));
}

void RkList::mouseButtonReleaseEvent(RkMouseEvent *event)
{
}

void RkList::mouseDoubleClickEvent(RkMouseEvent *event)
{
}

void RkList::wheelEvent(RkWheelEvent *event)
{
}
