/**
 * File name: RkFlowContainer.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#ifndef RK_FLOW_CONTAINER_H
#define RK_FLOW_CONTAINER_H

#include "RkContainer.h"
#include "RkWidget.h"

class RkFlowContainer : public RkContainer {
 public:
        explicit RkFlowContainer(RkWidget *parent);
        virtual ~RkFlowContainer() = default;

 protected:
        void layout() override;

 private:
        RK_DISABLE_COPY(RkFlowContainer);
        RK_DISABLE_MOVE(RkFlowContainer);
};

#endif // RK_FLOW_CONTAINER_H
