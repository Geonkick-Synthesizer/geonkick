/**
 * File name: RkLabel.h
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

#ifndef RK_LABEL_H
#define RK_LABEL_H

#include "RkWidget.h"
#include "RkImage.h"

class RkLabel : public RkWidget {
 public:
    explicit RkLabel(RkWidget *parent, const std::string &text = std::string());
    explicit RkLabel(RkWidget *parent, const RkImage &image);
    virtual ~RkLabel() = default;
    void setText(const std::string &text);
    std::string text() const;
    void setImage(const std::string &file);
    void setImage(const RkImage &image);

 protected:
    RK_DECLARE_IMPL_PTR(RkLabel);
    virtual void paintEvent(RkPaintEvent *event) override;

 private:
    RK_DISABLE_COPY(RkLabel);
    RK_DISABLE_MOVE(RkLabel);
};

#endif // RK_LABEL_H
