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

KitWidget::KitWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
	, keyWidth{30}
	, percussionHeight{20}
        , percussionWidth{0}
	, percussionNameWidth{100}
	, editPercussion{new RkLineEdit(this)}
	, editedPercussion{nullptr}
        , addButton{nullptr}
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
{
	RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(), this, updatePercussionName());

        createKeys();
        addNewPercussion();

        addButton = new RkButton(this);
	addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setPosition({10, 10});
        addButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), this, addNewPercussion());
        addButton->show();

        openKitButton = new RkButton(this);
	openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setPosition({5 + addButton->x() + addButton->width(), addButton->y()});
        openKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Open));
        openKitButton->show();

        saveKitButton = new RkButton(this);
	saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setPosition({5 + openKitButton->x() + openKitButton->width(), addButton->y()});
        saveKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Save));
        saveKitButton->show();
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
        percussionWidth = percussionNameWidth + midiKeys.size() * keyWidth;
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
        for (const auto &key: midiKeys) {
                RkRect rect(x, 0, keyWidth, keyWidth + kitList.size() * percussionHeight);
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

        int i = 0;
        int y = keyWidth;
        auto n = geonkickApi->getPercussionsNumber();
        for (const auto &percussion: kitList) {
                RkRect rect(0, y, percussionWidth, percussionHeight);
		painter.setPen(pen);
                if (i % 2)
                        painter.fillRect(rect, {200, 200, 200, 80});
                else
                        painter.fillRect(rect, {160, 160, 160, 80});
		RkRect txtRect = rect;
		txtRect.setWidth(300);
		txtRect.setTopLeft(RkPoint(txtRect.left() + 5, txtRect.top()));
		painter.setPen(txtPen);
		painter.drawText(txtRect, std::string(percussion.name), Rk::Alignment::AlignLeft);
                int x = rect.right() + 5;
                if (kitList.size() > 1) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(remove_per_button)),
                                          x, rect.top() + 2);
                        x += 16 + 3;
                }
                if (kitList.size() < n) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(copy_per_button)),
                                          x, rect.top() + 2);
                }

                i++;
                y += percussionHeight;
        }
}

void KitWidget::drawConnections(RkPainter &painter)
{
        for (decltype(kitList.size()) i = 0; i < kitList.size(); i++) {
                auto keyId = midiKeys.size() - 1;
                auto key = kitList[i].key;
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

	auto *percussion = getPercussion(event->x(), event->y());
        if (percussion) {
		if (event->x() < percussionNameWidth) {
			geonkickApi->setCurrentPercussion(percussion->id);
			return;
		} else if ((event->x() > percussionWidth + 5)
                           && (event->x() < percussionWidth + 5 + 16)) {
                        if (kitList.size() == 1)
                                copyPercussion(*percussion);
                        else
                                removePercussion(percussion->id);
                        update();
                        return;
                } else if (kitList.size() > 1
                           && kitList.size() < geonkickApi->getPercussionsNumber()
                           && (event->x() > percussionWidth + 5 + 16 + 3)
                           && (event->x() < percussionWidth + 5 + 16 + 3 + 16)) {
                        copyPercussion(*percussion);
                        update();
                        return;
                }

                const auto *key = getKey(event->x());
                if (key) {
                        percussion->key = key->key;
                        geonkickApi->setPercussionPlayingKey(percussion->id, percussion->key);
                        update();
                }
        }
}

void KitWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	if (event->button() != RkMouseEvent::ButtonType::Left)
		return;

	editedPercussion = getPercussion(event->x(), event->y());
	if (editedPercussion && event->x() < percussionNameWidth) {
                editPercussion->setText(editedPercussion->name);
		editPercussion->setSize({percussionNameWidth, percussionHeight});
                int y = keyWidth + percussionHeight * ((event->y() - keyWidth) / percussionHeight);
                editPercussion->setPosition(0, y);
		editPercussion->show();
		editPercussion->setFocus();
	}
}

void KitWidget::updatePercussionName()
{
	if (editedPercussion) {
		auto name = editPercussion->text();
		if (!name.empty()) {
			editedPercussion->name = name;
			editPercussion->hide();
			editPercussion->setFocus(false);
		}
		update();
	}
}

void KitWidget::addPercussion(const std::shared_ptr<PercussionState> &per)
{
	if (kitList.size() + 1 > midiKeys.size() - 1)
		return;

        geonkickApi->setPercussionState(per);
        Percussion percussion;
        percussion.id   = per->getId();
        percussion.name = per->getName();
        percussion.key  = per->getPlayingKey();
        kitList.push_back(percussion);
        update();
}

KitWidget::Percussion* KitWidget::getPercussion(int x, int y)
{
        if (y > keyWidth && x > 0 && x < percussionWidth + 5 + 16 + 3 + 16) {
                int id = (y - keyWidth) / percussionHeight;
                if (id > -1 && static_cast<decltype(kitList.size())>(id) < kitList.size())
                        return &kitList[id];
        }
        return nullptr;
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

        kitList.clear();
        for (const auto &per: kit->percussions())
                addPercussion(per);

        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setKitState(std::move(kit));
        geonkickApi->setCurrentWorkingPath("OpenKit", path);
        editPercussion->setText("");
        updateGui();
}


void KitWidget::addNewPercussion()
{
        int id = geonkickApi->getUnusedPercussion();
        if (id < 0)
                return;

        auto state = geonkickApi->getDefaultPercussionState();
        state->setId(id);
        addPercussion(state);
}

void KitWidget::removePercussion(int id)
{
        for (auto it = kitList.cbegin(); it != kitList.cend(); ++it) {
                if (it->id == static_cast<decltype(it->id)>(id)) {
                        geonkickApi->enablePercussion(id, false);
                        kitList.erase(it);
                        break;
                }
        }
}

void KitWidget::copyPercussion(const Percussion &per)
{
        auto newId = geonkickApi->getUnusedPercussion();
        if (newId > - 1) {
                auto state = geonkickApi->getPercussionState(per.id);
                state->setId(newId);
                state->setPlayingKey(per.key);
                state->setName(per.name);
                geonkickApi->setPercussionState(state);
                kitList.push_back(per);
        }
}

void KitWidget::saveKit(const std::string &file)
{
        auto state = geonkickApi->getKitState();
        for (const auto &per: kitList) {
                auto percussion = state->getPercussion(per.id);
                if (percussion)
                        percussion->setName(per.name);
        }
        state->save(file);
}
