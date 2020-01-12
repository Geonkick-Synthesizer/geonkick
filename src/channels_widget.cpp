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
        constexpr std::array<std::string_view, 16> keys {"A4", "A#4", "B4", "C4",
                        "C#4", "D4", "D#4", "E4",
                        "F4", "F#4", "G4", "G#4",
                        "A5", "A#5", "B5", "C5"};
}

void ChannelsWidget::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
	RK_UNUSED(event);
        RkPainter painter(this);
        auto font = painter.font();
        font.setSize(14);
        painter.setFont(font);
        drawKeys(painter);
        drawChannels(painter);
        drawConnections(painter);
}

void ChannelsWidget::drawKeys(RkPainter &painter)
{
        int i = 0;
        for (const auto &key: midiKeys) {
                if (i % 2)
                        painter.fillRect(key.rect, {200, 200, 200});
                else
                        painter.fillRect(key.rect, {180, 180, 180});
                RkRect txtRect(rect.x(), 30, key.rect.width(), painter.font().size());
                painter.drawText(txtRect, std::string(key.name));
        }
}

void ChannelsWidget::drawChannels(RkPainter &painter)
{
        int i = 0;
        for (const auto &channel: channelsList) {
                if (i % 2)
                        painter.fillRect(channel.rect, {200, 200, 200});
                else
                        painter.fillRect(channel.rect, {180, 180, 180});
                RkRect txtRect = channel.rect;
                txtRect.setWidth(180);
                painter.drawText(txtRect, std::string(channel.name));
        }
}

void ChannelsWidget::drawConnections(RkPainter &painter)
{
        for (auto k = 0; k < midiKeys.size(); k++) {
                for(auto ch = 0; ch < channelsList.size(); ch++) {
                        auto point = getIntersectionPoint(midiKeys[k], channelsList[ch]);
                        if (connectionMatrix[ch][k])
                                drawConnection(painter, point);
                }
        }
}

void ChannelsWidget::drawConnection(RkPainter &painter, const RkPoint &point)
{
        painter.drawCicle(point);
}

void ChannelsWidget::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	auto channel = getChannel(event->x(), event->y());
        if (channel) {
                auto key = getChannelKey(event->x(), event->y());
                if (key) {
                        connectionMatrix[channel->id][key->id] = !connectionMatrix[channel->id][key->id];
                        update();
                }
        }
}

Channel* ChannelsWidget::getChannel(int x, int y)
{
        for (const auto &ch: channelsList) {
                if (x > ch.rect.left() && x < ch.rect.right()
                    && y > ch.rect.top() && y < ch.rect.bottom()) {
                        return &ch;
                }
        }
                return nullptr;
}

Key* ChannelsWidget::getKey(int x, int y)
{
        for (const auto &key: midiKeys) {
                if (x > key.rect.left() && x < key.rect.right()
                    && y > key.rect.top() && y < key.rect.bottom()) {
                        return &key;
                }
        }
        return nullptr;
}

RkPoint ChannelsWidget::getIntersectionPoint(const ChannelKey &key, const Channel &channel)
{
        return {key.rect.x() + key.rect.width() / 2, channel.rect.y() + channel.rect.height() / 2};
}
