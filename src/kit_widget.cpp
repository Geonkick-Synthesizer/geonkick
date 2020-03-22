/**
 * File name: percussions_widget.cpp
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

#include "kit_widget.h"
#include "geonkick_button.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "kit_state.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>

RK_DECLARE_IMAGE_RC(add_per_button);
RK_DECLARE_IMAGE_RC(remove_per_button);
RK_DECLARE_IMAGE_RC(copy_per_button);
RK_DECLARE_IMAGE_RC(save_kit_button);
RK_DECLARE_IMAGE_RC(open_kit_button);

KitWidget::KitWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
	, keyWidth{30}
	, channelWidth{keyWidth}
	, percussionHeight{20}
        , percussionWidth{0}
	, percussionNameWidth{100}
	, editPercussion{new RkLineEdit(this)}
	, editedLineId{-1}
        , addButton{nullptr}
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
{
	RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(), this, updatePercussionName());
        createKeys();
	updatePercussionsLines();
        addButton = new RkButton(this);
	addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setPosition({5, 5});
        addButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), this, addNewPercussion());
        addButton->show();

        openKitButton = new RkButton(this);
	openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setPosition({5 + addButton->x() + addButton->width(), addButton->y()});
        openKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(open_kit_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Open));
        openKitButton->show();

        saveKitButton = new RkButton(this);
	saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setPosition({5 + openKitButton->x() + openKitButton->width(), addButton->y()});
        saveKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(save_kit_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Save));
        saveKitButton->show();
}

void KitWidget::updatePercussionsLines()
{
	percussionsLines.clear();
	auto n = geonkickApi->getPercussionsNumber();
	for (decltype(n) i = 0; i < n; i++) {
		if (geonkickApi->isPercussionEnabled(i))
			percussionsLines.push_back(i);
	}
	std::sort(percussionsLines.begin(), percussionsLines.end());
}

void KitWidget::createKeys()
{
        constexpr std::array<std::string_view, 17> keys {"A4", "A#4", "B4", "C4",
                        "C#4", "D4", "D#4", "E4",
                        "F4", "F#4", "G4", "G#4",
                        "A5", "A#5", "B5", "C5", "Any"};

        char n = 0;
        for (const auto &key: keys) {
                KeyInfo midiKey;
                midiKey.name = key;
                midiKey.key = 69 + n;
                midiKeys.push_back(midiKey);
                n++;
        }

        percussionWidth = percussionNameWidth + midiKeys.size() * keyWidth + channelWidth;
}

void KitWidget::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
{
	RK_UNUSED(event);
        RkImage img(size());
        RkPainter paint(&img);
        paint.fillRect(rect(), background());
        drawKeys(paint);
        drawPercussions(paint);
        drawConnections(paint);
        RkPainter painter(this);
        painter.drawImage(img, 0, 0);
}

void KitWidget::drawKeys(RkPainter &painter)
{
        auto pen = painter.pen();
        pen.setColor({200, 200, 200});

        auto font = painter.font();
        font.setSize(12);
        painter.setFont(font);

        int i = 0;
        int x = percussionNameWidth;
        auto n = geonkickApi->enabledPercussions();
        for (const auto &key: midiKeys) {
                RkRect rect(x, 0, keyWidth, keyWidth + n * percussionHeight);
                if (i % 2)
                        painter.fillRect(rect, {60, 60, 60});
                else
                        painter.fillRect(rect, {50, 50, 50});
                RkRect txtRect(rect.left(), 10, rect.width(), painter.font().size());
                painter.setPen(pen);
                painter.drawText(txtRect, std::string(key.name));
                i++;
                x += keyWidth;
        }
}

void KitWidget::drawPercussions(RkPainter &painter)
{
        auto pen = painter.pen();
        pen.setColor({60, 60, 60});
        auto txtPen = painter.pen();
        txtPen.setColor({200, 200, 200});
        painter.setPen(pen);

        int y = keyWidth;
        auto n = geonkickApi->getPercussionsNumber();
        auto currentId = geonkickApi->currentPercussion();
        for (decltype(percussionsLines.size()) i = 0; i < percussionsLines.size(); i++) {
                RkRect rect(0, y, percussionWidth, percussionHeight);
		painter.setPen(pen);
                if (i % 2)
                        painter.fillRect(rect, {200, 200, 200, 80});
                else
                        painter.fillRect(rect, {160, 160, 160, 80});
                if (static_cast<int>(currentId) == idFromLine(i))
                        painter.fillRect(RkRect(rect.left(), rect.top(), 4, rect.height()), {255, 255, 255, 90});
		RkRect txtRect = rect;
		txtRect.setWidth(300);
		txtRect.setTopLeft(RkPoint(txtRect.left() + 7, txtRect.top()));
		painter.setPen(txtPen);
		painter.drawText(txtRect,
                                 std::string(geonkickApi->getPercussionName(idFromLine(i))),
                                 Rk::Alignment::AlignLeft);
                auto channel = geonkickApi->getPercussionChannel(idFromLine(i));
		painter.drawText(RkRect(percussionWidth - channelWidth, y, channelWidth, percussionHeight),
				 "#" + std::to_string(channel));

                int x = rect.right() + 5;
                if (percussionsLines.size() > 1) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(remove_per_button)),
                                          x, rect.top() + 2);
                        x += 16 + 3;
                }
                if (percussionsLines.size() < n) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(copy_per_button)),
                                          x, rect.top() + 2);
                }
                y += percussionHeight;
        }
}

void KitWidget::drawConnections(RkPainter &painter)
{
        for (decltype(percussionsLines.size()) i = 0; i < percussionsLines.size(); i++) {
                if (!geonkickApi->isPercussionEnabled(idFromLine(i)))
                        continue;
                auto keyId = midiKeys.size() - 1;
                auto key = geonkickApi->getPercussionPlayingKey(idFromLine(i));
                if (key < 69 && key != -1)
                        continue;
                else if (key - 69 >= 0 && static_cast<decltype(midiKeys.size())>(key - 69) < midiKeys.size() - 1)
                        keyId = key - 69;
                else if ( key == -1 || static_cast<decltype(midiKeys.size())>(key - 69) > midiKeys.size() - 1)
                        keyId = midiKeys.size() - 1;

                // Define intersection point;
                RkPoint p {percussionNameWidth + static_cast<int>(keyId) * keyWidth + keyWidth / 2,
                          keyWidth + static_cast<int>(i) * percussionHeight + percussionHeight / 2};
                drawConnection(painter, p);
        }
}

void KitWidget::drawConnection(RkPainter &painter, const RkPoint &point)
{
        auto pen = painter.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        painter.setPen(pen);
        painter.drawCircle(point,  4);
}

void KitWidget::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        updatePercussionName();
        if (event->button() != RkMouseEvent::ButtonType::Left)
                return;

	auto id = idFromLine(getLineId(event->x(), event->y()));
        auto n = geonkickApi->getPercussionsNumber();
        if (id > -1 && static_cast<decltype(n)>(id) < n) {
                auto enabledPercussions = geonkickApi->enabledPercussions();
		if (event->x() < percussionNameWidth) {
			geonkickApi->setCurrentPercussion(id);
                        currnetPercussionChanged(id);
                        update();
			return;
		} else if ((event->x() > percussionWidth + 5)
                           && (event->x() < percussionWidth + 5 + 16)) {
                        if (enabledPercussions == 1)
                                copyPercussion(id);
                        else
                                removePercussion(id);
                        update();
                        return;
                } else if (enabledPercussions > 1
                           && enabledPercussions < geonkickApi->getPercussionsNumber()
                           && (event->x() > percussionWidth + 5 + 16 + 3)
                           && (event->x() < percussionWidth + 5 + 16 + 3 + 16)) {
                        copyPercussion(id);
                        update();
                        return;
                }

                const auto *key = getKey(event->x());
                if (key) {
                        geonkickApi->setPercussionPlayingKey(id, key->key);
                        update();
                }
        }
}

void KitWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	if (event->button() != RkMouseEvent::ButtonType::Left)
		return;

	editedLineId = getLineId(event->x(), event->y());
	if (editedLineId > -1 && event->x() < percussionNameWidth) {
                editPercussion->setText(geonkickApi->getPercussionName(idFromLine(editedLineId)));
		editPercussion->setSize({percussionNameWidth, percussionHeight});
                int y = keyWidth + percussionHeight * ((event->y() - keyWidth) / percussionHeight);
                editPercussion->setPosition(0, y);
		editPercussion->show();
		editPercussion->setFocus();
	}
}

void KitWidget::updatePercussionName()
{
	if (editedLineId > -1) {
		auto name = editPercussion->text();
		if (!name.empty()) {
			geonkickApi->setPercussionName(idFromLine(editedLineId), name);
			editPercussion->hide();
			editPercussion->setFocus(false);
		}
		update();
	}
}

void KitWidget::updatePercussionName(size_t id, const std::string &name)
{
        geonkickApi->setPercussionName(id, name);
        update();
}

std::string KitWidget::percussionName(size_t id) const
{
        return geonkickApi->getPercussionName(id);
}

void KitWidget::addPercussion(const std::shared_ptr<PercussionState> &per)
{
	if (geonkickApi->enabledPercussions() + 1 > midiKeys.size() - 1)
		return;

        editedLineId = -1;
        geonkickApi->setPercussionState(per);
	percussionsLines.push_back(per->getId());
	std::sort(percussionsLines.begin(), percussionsLines.end());
        update();
}

int KitWidget::getLineId(int x, int y) const
{
        if (y > keyWidth && x > 0 && x < percussionWidth + 5 + 16 + 3 + 16)
                return (y - keyWidth) / percussionHeight;
        return -1;
}

const KitWidget::KeyInfo* KitWidget::getKey(int x) const
{
        int xpos = percussionNameWidth;
        for (const auto &key: midiKeys) {
                if (x > xpos && x < xpos + keyWidth)
                        return &key;
                xpos += keyWidth;
        }
        return nullptr;
}

void KitWidget::openFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Kit" : "Save Kit");
        fileDialog->setFilters({".gkit", ".GKIT"});
        if (type == FileDialog::Type::Open) {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("OpenKit"));
                RK_ACT_BIND(fileDialog, selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this, openKit(file));
        } else {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("SaveKit"));
                RK_ACT_BIND(fileDialog, selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this, saveKit(file));
        }
}

void KitWidget::openKit(const std::string &file)
{
        auto kit = std::make_unique<KitState>();
        if (kit->open(file)) {
                GEONKICK_LOG_ERROR("can't open kit");
                return;
        }

        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setKitState(std::move(kit));
        geonkickApi->setCurrentWorkingPath("OpenKit", path);
        editPercussion->setText("");
        currnetPercussionChanged(0);
	updatePercussionsLines();
        updateGui();
        update();
}

void KitWidget::addNewPercussion()
{
        editedLineId = -1;
        int id = geonkickApi->getUnusedPercussion();
        if (id < 0)
                return;

        auto state = geonkickApi->getDefaultPercussionState();
        state->setId(id);
        addPercussion(state);
}

void KitWidget::removePercussion(int id)
{
        editedLineId = -1;
        geonkickApi->enablePercussion(id, false);
	for (auto it = percussionsLines.begin(); it != percussionsLines.end(); ++it) {
		if (*it == id) {
			percussionsLines.erase(it);
			std::sort(percussionsLines.begin(), percussionsLines.end());
			break;
		}
	}
}

void KitWidget::copyPercussion(int id)
{
        editedLineId = -1;
        auto newId = geonkickApi->getUnusedPercussion();
        if (newId > - 1) {
                auto state = geonkickApi->getPercussionState(id);
                state->setId(newId);
                geonkickApi->setPercussionState(state);
		percussionsLines.push_back(newId);
		std::sort(percussionsLines.begin(), percussionsLines.end());
        }
}

void KitWidget::saveKit(const std::string &file)
{
        auto state = geonkickApi->getKitState();
        state->save(file);
}

int KitWidget::idFromLine(int lineId) const
{
	if (lineId > - 1 && static_cast<decltype(percussionsLines.size())>(lineId) < percussionsLines.size())
		return percussionsLines[lineId];
	return -1;
}

