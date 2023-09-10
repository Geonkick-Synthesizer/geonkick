/**
 * File name: RkGDIImageBackendCanvas.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/redkite)
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

#ifndef RK_DIRECT2D_IMAGE_BACKEND_CANVAS_H
#define RK_DIRECT2D_IMAGE_BACKEND_CANVAS_H

#include "RkImage.h"

class RkGDIImageBackendCanvas {
 public:
        RkGDIImageBackendCanvas(const RkSize &size,
                                RkImage::Format format,
                                const unsigned char *data);
        ~RkGDIImageBackendCanvas();
        const RkSize& size() const;
        bool isNull() const;
        unsigned char* data();
        std::vector<unsigned char> dataCopy() const;
        std::shared_ptr<RkCanvasInfo> getCanvasInfo() const;
        void fill(const RkColor &color);

 protected:
        int pixelLength(RkImage::Format format) const;

 private:
        std::shared_ptr<RkCanvasInfo> canvasInfo;
        std::vector<unsigned char> imageData;
        RkSize imageSize;
};

#endif // RK_DIRECT2D_IMAGE_BACKEND_CANVAS_H
