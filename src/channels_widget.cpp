/**
 * File name: channels_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#include "channels_widget.h"

ChannelsWidget::ChannelsWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
{
}

void ChannelsWidget::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
	RK_UNUSED(event);
        RkPainter painter(this);
        auto font = painter.font();
        font.setSize(14);
        painter.setFont(font);
        drawKeys(painter);
        //        drawChannels(painter);
}

void ChannelsWidget::drawKeys(RkPainter &painter)
{
        constexpr std::array<std::string_view, 16> keys {"A4", "A#4", "B4", "C4",
                        "C#4", "D4", "D#4","E4",
                        "F4", "F#4", "G4", "G#4",
                        "A5", "A#5", "B5", "C5"};

        int gridWidth = 10;
        int x = 180;
        for (const auto &key: keys) {
                painter.drawText(x, 30, std::string(key));
                x += painter.getTextWidth(std::string(key)) + gridWidth;
        }

        x = 130;
        int y = 30 + painter.font().size();
        for (int i = 0;  i < 16; i++) {
                painter.drawText(x, y, "Kick " + std::to_string(i));
                y += painter.font().size() + 5;
        }

}

void ChannelsWidget::mouseMoveEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	RK_UNUSED(event);
}

void ChannelsWidget::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	RK_UNUSED(event);
}

void ChannelsWidget::mouseButtonReleaseEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	RK_UNUSED(event);
}

void ChannelsWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	RK_UNUSED(event);
}
