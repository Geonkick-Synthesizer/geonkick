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

#include <RkEvent.h>
#include <RkImage.h>

ChannelsWidget::ChannelsWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
{
        createKeys();
        createChannels();
}

void ChannelsWidget::createKeys()
{
        constexpr std::array<std::string_view, 17> keys {"A4", "A#4", "B4", "C4",
                        "C#4", "D4", "D#4", "E4",
                        "F4", "F#4", "G4", "G#4",
                        "A5", "A#5", "B5", "C5", "Any"};

        int x = 255;
        constexpr int keyWidth = 30;
        for (const auto &key: keys) {
                ChannelKey midiKey;
                midiKey.id = 0;
                midiKey.name = key;
                midiKey.rect = RkRect(x, 0, keyWidth, height() - 50);
                midiKeys.push_back(midiKey);
                x += keyWidth;
        }
}

void ChannelsWidget::createChannels()
{
        constexpr std::array<std::string_view, 5> channels {"Kick",
                        "Snare", "Hit-Hat", "Macaras", "Crash"};

        int y = 30;
        constexpr int channelHeight = 30;
        for (const auto &ch: channels) {
                Channel channel;
                channel.id = 0;
                channel.name = ch;
                channel.rect = RkRect(180, y, 600, channelHeight);
                channelsList.push_back(channel);
                y += channelHeight;
        }
}

void ChannelsWidget::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
	RK_UNUSED(event);
        RkImage img(size());
        RkPainter paint(&img);
        paint.fillRect(rect(), background());
        drawKeys(paint);
        drawChannels(paint);
        drawConnections(paint);
        RkPainter painter(this);
        painter.drawImage(img, 0, 0);
}

void ChannelsWidget::drawKeys(RkPainter &painter)
{
        auto pen = painter.pen();
        pen.setColor({200, 200, 200});

        auto font = painter.font();
        font.setSize(12);
        painter.setFont(font);

        int i = 0;
        for (const auto &key: midiKeys) {
                if (i % 2)
                        painter.fillRect(key.rect, {60, 60, 60});
                else
                        painter.fillRect(key.rect, {50, 50, 50});
                RkRect txtRect(key.rect.left(), 10, key.rect.width(), painter.font().size());
                painter.setPen(pen);
                painter.drawText(txtRect, std::string(key.name));
                i++;
        }
}

void ChannelsWidget::drawChannels(RkPainter &painter)
{
        auto pen = painter.pen();
        pen.setColor({60, 60, 60});
        auto txtPen = painter.pen();
        txtPen.setColor({200, 200, 200});

        painter.setPen(pen);

        int i = 0;
        for (const auto &channel: channelsList) {
                //                painter.setPen(pen);
                if (i % 2)
                        painter.fillRect(channel.rect, {200, 200, 200, 80});
                else
                        painter.fillRect(channel.rect, {160, 160, 160, 80});
                //                RkRect txtRect = channel.rect;
                //                txtRect.setWidth(300);
                //                painter.setPen(txtPen);
                //                painter.drawText(txtRect, std::string(channel.name), Rk::Alignment::AlignLeft);
                i++;
        }
}

void ChannelsWidget::drawConnections(RkPainter &painter)
{
        // for (decltype(midiKeys.size()) k = 0; k < midiKeys.size(); k++) {
        //         for(decltype(channelsList.size()) ch = 0; ch < channelsList.size(); ch++) {
        //                 auto point = getIntersectionPoint(midiKeys[k], channelsList[ch]);
        //                 if (connectionMatrix[ch][k])
        //                         drawConnection(painter, point);
        //         }
        // }
}

void ChannelsWidget::drawConnection(RkPainter &painter, const RkPoint &point)
{
        painter.drawCircle(point,  10);
}

void ChannelsWidget::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	const auto *channel = getChannel(event->x(), event->y());
        if (channel) {
                const auto *key = getKey(event->x(), event->y());
                if (key) {
                        //                        connectionMatrix[channel->id][key->id] = !connectionMatrix[channel->id][key->id];
                        update();
                }
        }
}

const ChannelsWidget::Channel* ChannelsWidget::getChannel(int x, int y) const
{
        for (const auto &ch: channelsList) {
                if (x > ch.rect.left() && x < ch.rect.right()
                    && y > ch.rect.top() && y < ch.rect.bottom()) {
                        return &ch;
                }
        }
                return nullptr;
}

const ChannelsWidget::ChannelKey* ChannelsWidget::getKey(int x, int y) const
{
        for (const auto &key: midiKeys) {
                if (x > key.rect.left() && x < key.rect.right()
                    && y > key.rect.top() && y < key.rect.bottom()) {
                        return &key;
                }
        }
        return nullptr;
}

RkPoint ChannelsWidget::getIntersectionPoint(const ChannelKey &key, const Channel &channel) const
{
        return {key.rect.left() + key.rect.width() / 2, channel.rect.top() + channel.rect.height() / 2};
}
