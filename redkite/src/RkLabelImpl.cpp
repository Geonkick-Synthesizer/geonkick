/**
 * File name: RkLabelImpl.cpp
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

#include "RkLabelImpl.h"
#include "RkPainter.h"
#include "RkLog.h"

RkLabel::RkLabelImpl::RkLabelImpl(RkLabel *interface,
                                  const std::string &text,
                                  RkWidget *parent)
    : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
    , inf_ptr{interface}
    , labelText{text}
{
}

RkLabel::RkLabelImpl::~RkLabelImpl()
{
}

void RkLabel::RkLabelImpl::setText(const std::string &text)
{
        labelText = text;
}

std::string RkLabel::RkLabelImpl::text() const
{
        return labelText;
}

void RkLabel::RkLabelImpl::setImage(const RkImage &image)
{
        labelImage = image;
}

void RkLabel::RkLabelImpl::drawLabel()
{
        if (labelText.empty() && labelImage.isNull())
                return;

        RkImage img(size());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        if (!labelImage.isNull())
                painter.drawImage(labelImage, 0, 0);
        if (!labelText.empty()) {
                auto pen = painter.pen();
                pen.setColor(textColor());
                painter.setPen(pen);
                painter.setFont(font());
                painter.drawText(inf_ptr->rect(), labelText);
        }

        RkPainter paint(inf_ptr);
        paint.drawImage(img, 0, 0);
}

