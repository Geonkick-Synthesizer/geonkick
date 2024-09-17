
/**
 * File name: RkImage.h
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

#ifndef RK_IMAGE_H
#define RK_IMAGE_H

#include "RkCanvas.h"
#include "RkSize.h"
#include "RkColor.h"

class RkImage : public RkCanvas {
 public:
        enum class Format : int {
                ARGB32 = 0,
                RGB32  = 1
        };

        RkImage();
        explicit RkImage(int width,
                         int height,
                         const unsigned char *data = nullptr,
                         Format format = Format::ARGB32);

        explicit RkImage(const RkSize &size,
                         const unsigned char *data = nullptr,
                         Format format = Format::ARGB32);

        virtual ~RkImage();
        RkImage(const RkImage &image);
        RkImage& operator=(const RkImage &other);
        friend  bool operator==(const RkImage &im1, const RkImage &im2)
        {
                return im1.width() == im2.width()
                        && im1.height() == im2.height()
                        && im1.format() == im2.format()
                        && im1.dataCopy() == im2.dataCopy();

        }

        friend  bool operator!=(const RkImage &im1, const RkImage &im2)
        {
                return im1.width() != im2.width()
                        || im1.height() != im2.height()
                        || im1.format() != im2.format()
                        || im1.dataCopy() != im2.dataCopy();

         }
        void fill(const RkColor &color);
        unsigned char* data() const;
        std::vector<unsigned char> dataCopy() const;
        Format format() const;
        int width() const;
        int height() const;
        RkSize size() const;
        bool isNull() const;
        RkCanvasInfo *getCanvasInfo() const override;

 protected:
        RK_DECLARE_O_PTR(RkImage);
        explicit RkImage(std::unique_ptr<RkImageImpl> impl);
};

#endif // RK_IMAGE_H
