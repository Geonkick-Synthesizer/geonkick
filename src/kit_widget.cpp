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
#include "geonkick_state.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>

RK_DECLARE_IMAGE_RC(add_percussion_button);
RK_DECLARE_IMAGE_RC(remove_per_button);
RK_DECLARE_IMAGE_RC(copy_per_button);

KitWidget::KitWidget(GeonkickWidget *parent, GeonkickApi* api)
	: GeonkickWidget(parent)
	, geonkickApi{api}
	, keyWidth{30}
	, percussionHeight{20}
	, percussionNameWidth{100}
	, editPercussion{new RkLineEdit(this)}
	, editedPercussion{nullptr}
        , addButton{nullptr}
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
        , percussionWidth{0}
{
	RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(), this, updatePercussionName());

        createKeys();
        addNewPercussion();

        addButton = new RkButton(this);
	addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setPosition({10, 10});
        addButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_percussion_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), this, addNewPercussion());
        addButton->show();

        openKitButton = new RkButton(this);
	openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setPosition({5 + addButton->x() + addButton->width(), addButton->y()});
        openKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_percussion_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b), this, openFileDialog(FileDialog::Type::Open));
        openKitButton->show();

        saveKitButton = new RkButton(this);
	saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setPosition({5 + openKitButton->x() + openKitButton->width(), addButton->y()});
        saveKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(add_percussion_button)));
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
        percussionWidth = percussionNameWidth + midikeys.size() * keyWidth;
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
                RkRect rect(x, 0, keyWidth, keyWidth);
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
        for (const auto &percussion: percussionsList) {
                RkRect rect(percussionNameWidth, y,
                            percussionWidth - percussionNameWidth,
                            percussionHeight);
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
                if (percussionsList.size() > 1) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(remove_per_button)),
                                          x, rect.top() + 2);
                        x += 16 + 3;
                }
                if (percussionsList.size() < n) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(copy_per_button)),
                                          x, rect.top() + 2);
                }

                i++;
                y += percussionHeight;
        }
}

void KitWidget::drawConnections(RkPainter &painter)
{
        for(const auto &per : percussionsList.size()) {
                auto point = getIntersectionPoint(k, per);
                        if (connectionMatrix[per][k])
                                drawConnection(painter, point);
                }
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

	const auto *percussion = getPercussion(event->x(), event->y());
        if (percussion) {
		if (event->x() < channesNameWidth) {
			geonkickApi->setCurrentPercussion(percussion->id);
			return;
		} else if ((event->x() > percussionWidth + 5)
                           && (event->x() < percussionWidth + 5 + 16)) {
                        if (percussionsList.size() == 1)
                                copyPercussion(percussion->id);
                        else
                                removePercussion(percussion->id);
                        update();
                        return;
                } else if (percussionsList.size() > 1
                           && percussionsList.size() < geonkickApi->getPercussionsNumber()
                           && (event->x() > percussionWidth + 5 + 16 + 3)
                           && (event->x() < percussionWidth + 5 + 16 + 3 + 16)) {
                        copyPercussion(percussion->id);
                        update();
                        return;
                }

                const auto *key = getKey(event->x());
                if (key) {
                        geonkickApi->setPercussionPlayingKey(percussion->id, key.key);
                        update();
                }
        }
}

void KitWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
	if (event->button() != RkMouseEvent::ButtonType::Left)
		return;

	editedPercussion = getPercussion(event->x(), event->y());
	if (editedPercussion && event->x() < channesNameWidth) {
                editPercussion->setText(editedPercussion->name);
		editPercussion->setSize({channesNameWidth, percussionHeight});
		editPercussion->setPosition(editedPercussion->rect.topLeft());
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

void KitWidget::addPercussion(const Percussion &per)
{
	if (percussionsList.size() + 1 > midiKeys.size() - 1)
		return;

        auto state = std::make_shared<GeonkickState>();
        if (!state->loadFile(per.file)) {
                RK_LOG_ERROR("can't load percussion file: " << per.file);
                return;
        }
        geonkickApi->setState(state, per.id, per.key);

        percussionsList.push_back(per);
	for (auto &key: midiKeys)
                key.rect.setHeight(percussionHeight * percussionsList.size() + keyWidth);
        update();
}

KitWidget::Percussion* KitWidget::getPercussion(int x, int y)
{
        int ypos = keyWidth;
        for (auto &per: percussionsList) {
                if (x > 0 && x < percussionWidth + 5 + 16 + 3 + 16
                    && y > ypos && y < ypos + percussionHeight) {
                        return &ch;
                }
                ypos += percussionHeight;
        }
                return nullptr;
}

const KitWidget::PercussionKey* KitWidget::getKey(int x) const
{
        int xpos = percussionNameWidth;
        for (const auto &key: midiKeys) {
                if (x > xpos && x < xpos + keyWidth)
                        return &key;
                xpos += keyWidth;
        }
        return nullptr;
}

RkPoint KitWidget::getIntersectionPoint(const PercussionKey &key, const Percussion &percussion) const
{
        return {key.rect.left() + key.rect.width() / 2,
                percussion.rect.top() + percussion.rect.height() / 2};
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
        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));
        sfile.close();

        auto kit = parseKit(fileData, filePath.parent_path());
        if (!kit.list.empty()) {
                connectionMatrix.clear();
                percussionsList.clear();
                auto n = geonkickApi->getPercussionsNumber();
                for (decltype(n) i = 0; i < n; i++)
                        geonkickApi->enablePercussion(i, false);
                for (const auto &per: kit.list)
                        addPercussion(per);
        }

        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setCurrentWorkingPath("OpenKit", path);
        updateGui();
}

KitWidget::Kit
KitWidget::parseKit(std::string &fileData,
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

std::vector<KitWidget::Percussion>
KitWidget::parsePercussions(const rapidjson::Value &envelopeArray,
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

void KitWidget::addNewPercussion()
{
        int id = geonkickApi->getUnusedPercussion();
        if (id < 0)
                return;

        Percussion per;
        per.id = id;
        per.name = "Default";
        per.key = -1;
        per.enabled = true;
        per.limiter = 0.01;

        geonkickApi->enablePercussion(per.id, true);
        geonkickApi->setCurrentPercussion(per.id);
        geonkickApi->setState(geonkickApi->getDefaultState());
        geonkickApi->setPercussionPlayingKey(per.id, -1);
        addPercussion(per);
}

void KitWidget::removePercussion(int id)
{
        geonkickApi->enablePercussion(id, false);
        for (auto it = percussionsList.cbegin(); it != percussionsList.cend(); ++i) {
                if (it->id == static_cast<decltype(it->id)>(id)) {
                        percussionsList.erase(it);
                        break;
                }
        }
}

void KitWidget::copyPercussion(int id)
{
        GEONKICK_LOG_INFO("copyPercussion called");
}

void KitWidget::saveKit(const std::string &file)
{
        RK_LOG_INFO("file:" << file);
}

