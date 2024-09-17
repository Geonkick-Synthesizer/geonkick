/**
 * File name: RkList.h
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

#ifndef RK_LIST_H
#define RK_LIST_H

#include "RkWidget.h"
#include "RkImage.h"

class RkModel;

class RkList : public RkWidget {
 public:
        RkList(RkWidget *parent, RkModel *model = nullptr);
        virtual ~RkList() = default;
        RkModel* getModel() const;

 protected:
        RK_DECLARE_IMPL_PTR(RkList);
        virtual void paintEvent(RkPaintEvent *event) override;
        virtual void keyPressEvent(RkKeyEvent *event) override;
        virtual void keyReleaseEvent(RkKeyEvent *event) override;
        virtual void mouseMoveEvent(RkMouseEvent *event) override;
        virtual void mouseButtonPressEvent(RkMouseEvent *event) override;
        virtual void mouseButtonReleaseEvent(RkMouseEvent *event) override;
        virtual void mouseDoubleClickEvent(RkMouseEvent *event) override;
        virtual void wheelEvent(RkWheelEvent *event) override;

 private:
    RK_DISABLE_COPY(RkList);
    RK_DISABLE_MOVE(RkList);
};

#endif // RK_LIST_H
