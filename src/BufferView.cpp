/**
 * File name: BufferView.cpp
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

#include "BufferView.h"

#include <RkPainter.h>
#include <RkEvent.h>

BufferView::BufferView(GeonkickWidget* parent, const std::vector<float> &data)
        : GeonkickWidget(parent)
        , bufferData{data}
        , updateGraph{true}
        , graphImage{nullptr}
{
        setBackgroundColor(50, 50, 50);
        setBorderColor(40, 40, 40);
        setBorderWidth(1);
}

void BufferView::setData(const std::vector<float> &data)
{
        bufferData = data;
        updateGraph = true;
        update();
}

std::vector<float> BufferView::getData() const
{
        return bufferData;
}

void BufferView::paintWidget(RkPaintEvent *event)
{
        RK_UNUSED(event);
        if (updateGraph)
                drawGraph();
        if (graphImage && !graphImage->isNull()) {
                RkPainter painter(this);
                painter.drawImage(*graphImage.get(), 0, 0);
        }
}

void BufferView::drawGraph()
{
        graphImage = std::make_unique<RkImage>(size());
        RkPainter painter(graphImage.get());
        painter.fillRect(RkRect({0, 0}, size()), background());
        if (bufferData.empty())
                return;

        RkPen pen = painter.pen();
        pen.setColor({59, 130, 4, 255});
        painter.setPen(pen);

        std::vector<RkPoint> graphPoints;
        int x = 0;
        int kY = height() / 2;
        float yScale = 1.0f;
        float max = fabs(*std::max_element(bufferData.begin(), bufferData.end(),
                                           [](float a, float b){ return fabs(a) < fabs(b); }));
        if (max > 1e-5)
                yScale = 1.0f / max;
        int graphWidth = width();
        for (const auto &val: bufferData) {
                int y =  kY * (1.0f - yScale * val);
                graphPoints.emplace_back(RkPoint(x / graphWidth, y));
                x++;
        }
        painter.drawPolyline(graphPoints);
        updateGraph = false;
}

void BufferView::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left)
                action graphPressed();
}
