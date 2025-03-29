/**
 * File name: RkImage.cpp
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

#include "RkImage.h"
#include "RkImageImpl.h"

#include "RkLog.h"

RkImage::RkImage()
        : o_ptr{std::make_unique<RkImageImpl>(this, 0, 0, nullptr)}
{
}

RkImage::RkImage(int width,
                 int height,
                 const unsigned char *data,
                 Format format)
        : o_ptr{std::make_unique<RkImageImpl>(this, width, height, data, format)}
{
}

RkImage::RkImage(const RkSize &size,
                 const unsigned char *data,
                 Format format)
        : o_ptr{std::make_unique<RkImageImpl>(this, size.width(), size.height(), data, format)}
{
}

RkImage::RkImage(std::unique_ptr<RkImageImpl> impl)
        : o_ptr{std::move(impl)}
{
}

RkImage::RkImage(const RkImage &image) :
        o_ptr{std::make_unique<RkImageImpl>(this, 0, 0, nullptr)}
{
        o_ptr->createImage({image.width(), image.height()}, image.format(), image.data());
}

RkImage::~RkImage()
{
}

RkImage& RkImage::operator=(const RkImage &other)
{
        o_ptr->createImage({other.width(), other.height()}, other.format(), other.data());
        return *this;
}

void RkImage::fill(const RkColor &color)
{
        o_ptr->fill(color);
}

void RkImage::grayscaleImage(double fadeFactor)
{
        return o_ptr->grayscaleImage(fadeFactor);
}

unsigned char* RkImage::data() const
{
        return o_ptr->data();
}

std::vector<unsigned char> RkImage::dataCopy() const
{
        return o_ptr->dataCopy();
}

RkImage::Format RkImage::format() const
{
        return o_ptr->format();
}

int RkImage::width() const
{
        return o_ptr->width();
}

int RkImage::height() const
{
        return o_ptr->height();
}

RkSize RkImage::size() const
{
        return o_ptr->size();
}

bool RkImage::isNull() const
{
        return o_ptr->isNull();
}

RkCanvasInfo* RkImage::getCanvasInfo() const
{
        return o_ptr->getCanvasInfo();
}
