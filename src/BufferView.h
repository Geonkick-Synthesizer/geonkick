/**
 * File name: BufferView.h
 * Project: Geonkick (A percussion synthesizer)
F *
 * Copyright (C) 2020 Iurie Nistor 
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#ifndef GEONKICK_BUFFER_VIEW
#define GEONKICK_BUFFER_VIEW

#include "geonkick_widget.h"

#include <RkImage.h>

class BufferView : public GeonkickWidget {
  public:
        BufferView(GeonkickWidget* parent, const std::vector<float> &data = std::vector<float>());
        virtual ~BufferView() = default;
        void setData(const std::vector<float> &data);
        std::vector<float> getData() const;
        RK_DECL_ACT(graphPressed, graphPressed(), RK_ARG_TYPE(), RK_ARG_VAL());

  protected:
     void mouseButtonPressEvent(RkMouseEvent *event) override;
     void paintWidget(RkPaintEvent *event) override;
     void drawGraph();

  private:
     std::vector<float> bufferData;
     bool updateGraph;
     std::unique_ptr<RkImage> graphImage;
};

#endif // GEONKICK_BUFFER_VIEW
