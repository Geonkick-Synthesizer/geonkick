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
#include "geonkick_button.h"
#include "geonkick_api.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>

// TODO: load from file.
constexpr std::array<std::string_view, 5> channels {"Kick",
		"Snare", "Hit-Hat", "Macaras", "Crash"};

RK_DECLARE_IMAGE_RC(add_channel_button);


ChannelsWidget::ChannelsWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
	, keyWidth{30}
	, channelHeight{20}
	, channesNameWidth{100}
	, editChannel{new RkLineEdit(this)}
	, editedChannel{nullptr}
        , addButton{nullptr}
{
	RK_ACT_BIND(editChannel, editingFinished, RK_ACT_ARGS(), this, updateChannelName());
        createKeys();
        createChannels();

        addButton = new GeonkickButton(this);
	addButton->setCheckable(true);
        addButton->setSize(30, 30);
        addButton->setPosition({0, 0});
        addButton->setUnpressedImage(RkImage(30, 30, RK_IMAGE_RC(add_channel_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), this, addChannel());
}

void ChannelsWidget::createKeys()
{
        constexpr std::array<std::string_view, 17> keys {"A4", "A#4", "B4", "C4",
                        "C#4", "D4", "D#4", "E4",
                        "F4", "F#4", "G4", "G#4",
                        "A5", "A#5", "B5", "C5", "Any"};

	int x = channesNameWidth;
        int i = 0;
        for (const auto &key: keys) {
                ChannelKey midiKey;
                midiKey.id = i++;
                midiKey.name = key;
                midiKey.rect = RkRect(x, 0, keyWidth, channelHeight * 5 + keyWidth);
                midiKeys.push_back(midiKey);
                x += keyWidth;
        }
}

void ChannelsWidget::createChannels()
{
        /*	int y = keyWidth;
        for (const auto &ch: channels) {
                Channel channel;
                channel.id = geonkickApi->getUnusedPercussion();
                GEONKICK_LOG_INFO("channel: " << channel.id);
                if (channel.id < 1)
                        return;
                geonkickApi->setPercussionPlayingKey(channel.id, 0);
                geonkickApi->enablePercussion(channel.id);
                channel.name = ch;
                channel.rect = RkRect(0, y, channesNameWidth + keyWidth * 17, channelHeight);
                channelsList.push_back(channel);
                connectionMatrix.push_back({false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false, true});
                y += channelHeight;
                }*/
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
		painter.setPen(pen);
                if (i % 2)
                        painter.fillRect(channel.rect, {200, 200, 200, 80});
                else
                        painter.fillRect(channel.rect, {160, 160, 160, 80});
		RkRect txtRect = channel.rect;
		txtRect.setWidth(300);
		txtRect.setTopLeft(RkPoint(txtRect.left() + 5, txtRect.top()));
		painter.setPen(txtPen);
		painter.drawText(txtRect, std::string(channel.name), Rk::Alignment::AlignLeft);
                i++;
        }
}

void ChannelsWidget::drawConnections(RkPainter &painter)
{
        for (decltype(midiKeys.size()) k = 0; k < midiKeys.size(); k++) {
                for(decltype(channelsList.size()) ch = 0; ch < channelsList.size(); ch++) {
                        auto point = getIntersectionPoint(midiKeys[k], channelsList[ch]);
                        if (connectionMatrix[ch][k])
                                drawConnection(painter, point);
                }
        }
}

void ChannelsWidget::drawConnection(RkPainter &painter, const RkPoint &point)
{
        auto pen = painter.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        painter.setPen(pen);
        painter.drawCircle(point,  4);
}

void ChannelsWidget::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	updateChannelName();
        if (event->button() != RkMouseEvent::ButtonType::Left)
                return;

	const auto *channel = getChannel(event->x(), event->y());
        if (channel) {
                GEONKICK_LOG_INFO("channel");
		if (event->x() < channesNameWidth) {
			geonkickApi->setCurrentPercussion(channel->id);
			return;
		}

                GEONKICK_LOG_INFO("channel1");
                const auto *key = getKey(event->x(), event->y());
                if (key) {
                        GEONKICK_LOG_INFO("channel1.1: " << channel->id);
                        if (channel->id < connectionMatrix.size()
                            && key->id < connectionMatrix[channel->id].size()) {
                                if (connectionMatrix[channel->id][key->id]) {
                                        connectionMatrix[channel->id][key->id] = false;
                                        GEONKICK_LOG_INFO("channel2.1");
                                } else {
                                        connectionMatrix[channel->id].fill(false);
                                        connectionMatrix[channel->id][key->id] = true;
                                        GEONKICK_LOG_INFO("channel2.1");
                                }
                                update();
                        }
                }
        }
}

void ChannelsWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	if (event->button() != RkMouseEvent::ButtonType::Left)
		return;

	editedChannel = getChannel(event->x(), event->y());
	if (editedChannel && event->x() < channesNameWidth) {
                editChannel->setText(editedChannel->name);
		editChannel->setSize({channesNameWidth, channelHeight});
		editChannel->setPosition(editedChannel->rect.topLeft());
		editChannel->show();
		editChannel->setFocus();
	}
}

void ChannelsWidget::updateChannelName()
{
	if (editedChannel) {
		auto name = editChannel->text();
		if (!name.empty()) {
			editedChannel->name = name;
			editChannel->hide();
			editChannel->setFocus(false);
		}
		update();
	}
}

void ChannelsWidget::addChannel()
{
	if (channelsList.size() + 1 > midiKeys.size() - 1)
		return;

        Channel channel;
        channel.id = geonkickApi->getUnusedPercussion();
        if (channel.id < 1)
                return;
        channel.name = "Unknown";
        channel.rect = RkRect(0, keyWidth + channelsList.size() * channelHeight,
                              channesNameWidth + keyWidth * 17, channelHeight);
        channelsList.push_back(channel);
        connectionMatrix.push_back({false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false, false});
	for (auto &key: midiKeys)
                key.rect.setHeight(channelHeight * channelsList.size() + keyWidth);
        geonkickApi->setPercussionPlayingKey(channel.id, 0);
        geonkickApi->enablePercussion(channel.id);

	update();
	GEONKICK_LOG_INFO("addChannell");
}

ChannelsWidget::Channel* ChannelsWidget::getChannel(int x, int y)
{
        for (auto &ch: channelsList) {
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
