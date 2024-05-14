/**
 * File name: RkImageImpl.h
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
#include "RkCanvas.h"

#ifdef RK_GRAPHICS_CAIRO_BACKEND
class RkCairoImageBackendCanvas;
#else
#error No graphics backend defined.
#endif

class RkImage::RkImageImpl {
 public:
        RkImageImpl(RkImage *inf,
                    int width,
                    int height,
                    const unsigned char *data,
                    RkImage::Format format = RkImage::Format::ARGB32);
        virtual ~RkImageImpl();
        RkCanvasInfo *getCanvasInfo() const;
        unsigned char* data() const;
        std::vector<unsigned char> dataCopy() const;
        Format format() const;
        int width() const;
        int height() const;
        RkSize size() const;
        bool isNull() const;
        void createImage(const RkSize &size,
                         RkImage::Format format,
                         const unsigned char *data);
        void fill(const RkColor &color);

 private:
        RK_DECALRE_INTERFACE_PTR(RkImage);
        RkImage::Format imageFormat;
#ifdef RK_GRAPHICS_CAIRO_BACKEND
        std::unique_ptr<RkCairoImageBackendCanvas> imageBackendCanvas;
#else
#error No graphics backend defined
#endif
};
