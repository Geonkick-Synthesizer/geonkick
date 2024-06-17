/**
 * File name: RkImageBackendCanvas.cpp
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

#include "RkCairoImageBackendCanvas.h"
#include "RkCanvasInfo.h"
#include "RkLog.h"

RkCairoImageBackendCanvas::RkCairoImageBackendCanvas(const RkSize &size,
                                                     RkImage::Format format,
                                                     const unsigned char *data)
        : canvasInfo{nullptr}
        , imageSize{size}
{
        RK_LOG_DEBUG("size w: " << size.width() << ", h: " << size.width());
        auto cairoFormat = toCairoFormat(format);
        if (cairoFormat != CAIRO_FORMAT_INVALID && !imageSize.isEmpty()) {
                canvasInfo = std::make_unique<RkCanvasInfo>();
                auto stride = cairo_format_stride_for_width(cairoFormat, imageSize.width());
                if (data == nullptr)
                        imageData = std::vector<unsigned char>(imageSize.width() * imageSize.height() * pixelLength(format), 0);
                else
                        imageData.assign(data, data + imageSize.width() * imageSize.height() * pixelLength(format));
                canvasInfo->cairo_surface = cairo_image_surface_create_for_data(imageData.data(),
                                                                                CAIRO_FORMAT_ARGB32,
                                                                                imageSize.width(),
                                                                                imageSize.height(),
                                                                                stride);
                if (!canvasInfo->cairo_surface) {
                        RK_LOG_ERROR("can't create cairo surface");
                }
        }
}

RkCairoImageBackendCanvas::~RkCairoImageBackendCanvas()
{
        if (canvasInfo)
                cairo_surface_destroy(canvasInfo->cairo_surface);
}

cairo_format_t RkCairoImageBackendCanvas::toCairoFormat(RkImage::Format format) const
{
        switch (format)
        {
        case RkImage::Format::ARGB32: return CAIRO_FORMAT_ARGB32;
        case RkImage::Format::RGB32:  return CAIRO_FORMAT_RGB24;
        default: return CAIRO_FORMAT_INVALID;
        }
}

int RkCairoImageBackendCanvas::pixelLength(RkImage::Format format) const
{
        switch (format)
        {
        case RkImage::Format::ARGB32:
        case RkImage::Format::RGB32:  return 4;
        default: return 1;
        }
}

const RkSize& RkCairoImageBackendCanvas::size() const
{
        return imageSize;
}

bool RkCairoImageBackendCanvas::isNull() const
{
        return canvasInfo == nullptr;
}

unsigned char* RkCairoImageBackendCanvas::data()
{
        return imageData.data();
}

std::vector<unsigned char> RkCairoImageBackendCanvas::dataCopy() const
{
        return imageData;
}

RkCanvasInfo* RkCairoImageBackendCanvas::getCanvasInfo() const
{
        return canvasInfo.get();
}

void RkCairoImageBackendCanvas::fill(const RkColor &color)
{
        unsigned char *data = imageData.data();
        for (decltype(imageData.size()) i = 0; i < imageData.size() - 4; i += 4) {
                *(data + i)     = color.red();
                *(data + i + 1) = color.green();
                *(data + i + 2) = color.blue();
                *(data + i + 3) = color.alpha();
        }
}
