/**
 * File name: channels_widget.cpp
 * Project: Geonkick (a percussion synthesizer)
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
#include "geonkick_state.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>

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
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
{
	RK_ACT_BIND(editChannel, editingFinished, RK_ACT_ARGS(), this, updateChannelName());
        createKeys();
        createChannels();

        addButton = new RkButton(this);
	addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setPosition({10, 10});
        addButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_channel_button)));
        //        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), this, addChannel());
        addButton->show();

        openKitButton = new RkButton(this);
	openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setPosition({5 + addButton->x() + addButton->width(), addButton->y()});
        openKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_channel_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Open));
        openKitButton->show();

        saveKitButton = new RkButton(this);
	saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setPosition({5 + openKitButton->x() + openKitButton->width(), addButton->y()});
        saveKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_channel_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Save));
        saveKitButton->show();

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
		if (event->x() < channesNameWidth) {
			geonkickApi->setCurrentPercussion(channel->id);
			return;
		}

                const auto *key = getKey(event->x(), event->y());
                if (key) {
                        if (channel->id < connectionMatrix.size()
                            && key->id < connectionMatrix[channel->id].size()) {
                                if (connectionMatrix[channel->id][key->id]) {
                                        connectionMatrix[channel->id][key->id] = false;
                                } else {
                                        connectionMatrix[channel->id].fill(false);
                                        connectionMatrix[channel->id][key->id] = true;
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

void ChannelsWidget::addChannel(const Percussion &per)
{
	if (channelsList.size() + 1 > midiKeys.size() - 1)
		return;

        Channel channel;
        channel.id = per.id;
        channel.name = per.name;
        channel.rect = RkRect(0, keyWidth + channelsList.size() * channelHeight,
                              channesNameWidth + keyWidth * 17, channelHeight);
        channelsList.push_back(channel);
        connectionMatrix.push_back({false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false,
                                            false, false, false, false, true});
	for (auto &key: midiKeys)
                key.rect.setHeight(channelHeight * channelsList.size() + keyWidth);
        update();
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

void ChannelsWidget::openFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Kit" : "Save Kit");
        fileDialog->setFilters({".gkit", ".GKIT"});
        if (type == FileDialog::Type::Open) {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("OpenKit"));
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, openKit(file));
        } else {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("SaveKit"));
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, saveKit(file));
        }
}

void ChannelsWidget::openKit(const std::string &file)
{
        if (file.size() < 6) {
                RK_LOG_ERROR("can't open preset. File name empty or wrong format.");
                return;
        }

        std::filesystem::path filePath(file);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkit"
            && filePath.extension() != ".GKIT")) {
                RK_LOG_ERROR("can't open kit. Wrong file format.");
                return;
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(filePath));
        if (!sfile.is_open()) {
                RK_LOG_ERROR("can't open kit.");
                return;
        }
        std::string fileData((std::istreambuf_iterator<char>(sfile)), (std::istreambuf_iterator<char>()));
        sfile.close();

        auto kit = parseKit(fileData, filePath.parent_path());
        if (!kit.list.empty()) {
                for (const auto &per: kit.list)
                        addPercussion(per);
        }

        geonkickApi->setCurrentWorkingPath("OpenKit", filePath.has_parent_path() ? filePath.parent_path() : filePath);
        updateGui();
}

ChannelsWidget::Kit
ChannelsWidget::parseKit(std::string &fileData,
                         const std::filesystem::path &path)
{
        rapidjson::Document document;
        Kit kit;
        document.Parse(fileData.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                kit.name = m.value.GetString();
                        if (m.name == "author" && m.value.IsString())
                                kit.author = m.value.GetString();
                        if (m.name == "url" && m.value.IsString())
                                kit.url = m.value.GetString();
                        if (m.name == "percussions" && m.value.IsArray())
                                kit.list = parsePercussions(m.value, path);
                }
        }
        return kit;
}

std::vector<ChannelsWidget::Percussion>
ChannelsWidget::parsePercussions(const rapidjson::Value &envelopeArray,
                                 const std::filesystem::path &path)
{
        std::vector<Percussion> percussions;
        for (const auto &el: envelopeArray.GetArray()) {
                if (el.IsObject()) {
                        Percussion per;
                        for (const auto &m: el.GetObject()) {
                                if (m.name == "id" && m.value.IsInt())
                                        per.id = m.value.GetInt();
                                if (m.name == "name" && m.value.IsString())
                                        per.name = m.value.GetString();
                                if (m.name == "file" && m.value.IsString())
                                        per.file = path / std::filesystem::path(m.value.GetString());
                                if (m.name == "key" && m.value.IsInt())
                                        per.key = m.value.GetInt();
                                if (m.name == "enabled" && m.value.IsBool())
                                        per.enabled = m.value.GetBool();
                                if (m.name == "limiter" && m.value.IsDouble())
                                        per.limiter = m.value.GetDouble();
                        }
                        percussions.push_back(per);
                }
        }

        return percussions;

}

void ChannelsWidget::addPercussion(const Percussion &per)
{
        if (per.id > geonkickApi->getPercussionsNumber()) {
                RK_LOG_ERROR("invalid percussion id: " << per.id);
                return;
        }

        auto state = std::make_shared<GeonkickState>();
        if (!state->loadFile(per.file)) {
                RK_LOG_ERROR("can't load percussion file: " << per.file);
                return;
        }
        geonkickApi->setState(state, per.id, per.key);
        addChannel(per);
}

void ChannelsWidget::saveKit(const std::string &file)
{
        RK_LOG_INFO("file:" << file);
}

