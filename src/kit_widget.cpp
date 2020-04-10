/**
 * File name: kit_widget.cpp
 * Project: Geonkick (A percussion synthesizer)
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
#include "kit_model.h"
#include "geonkick_button.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>

RK_DECLARE_IMAGE_RC(add_per_button);
RK_DECLARE_IMAGE_RC(remove_per_button);
RK_DECLARE_IMAGE_RC(copy_per_button);
RK_DECLARE_IMAGE_RC(save_kit_button);
RK_DECLARE_IMAGE_RC(open_kit_button);

KitWidget::KitWidget(GeonkickWidget *parent, KitModel *model)
	: GeonkickWidget(parent)
        , kitModel{model}
	, keyWidth{30}
	, channelWidth{keyWidth}
	, percussionHeight{20}
        , percussionWidth{0}
	, percussionNameWidth{100}
	, editPercussion{new RkLineEdit(this)}
	, editedLineIndex{-1}
        , addButton{nullptr}
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
{
	RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(),
                    this, updatePercussionName());
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
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, showFileDialog(FileDialog::Type::Open));
        openKitButton->show();

        saveKitButton = new RkButton(this);
	saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setPosition({5 + openKitButton->x() + openKitButton->width(), addButton->y()});
        saveKitButton->setUnpressedImage(RkImage(16, 16, RK_IMAGE_RC(save_kit_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, openFileDialog(FileDialog::Type::Save));
        saveKitButton->show();
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

        int x = percussionNameWidth;
        auto n = kitModel->percussionNumber();
        auto nKeys = kitModel->keysNumber();
        for (decltype(nKeys) i = 0; i < nKeys; i++) {
                RkRect rect(x, 0, keyWidth, keyWidth + n * percussionHeight);
                if (i % 2)
                        painter.fillRect(rect, {60, 60, 60});
                else
                        painter.fillRect(rect, {50, 50, 50});
                RkRect txtRect(rect.left(), 10, rect.width(), painter.font().size());
                painter.setPen(pen);
                painter.drawText(txtRect, kitModel->keyName(i));
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
        auto n = kitModel->percussionNumber();
        for (decltype(n) i = 0; i < n; i++) {
                RkRect rect(0, y, percussionWidth, percussionHeight);
		painter.setPen(pen);
                if (i % 2)
                        painter.fillRect(rect, {200, 200, 200, 80});
                else
                        painter.fillRect(rect, {160, 160, 160, 80});
                if (kitModel->isSelected(i)) {
                        painter.fillRect(RkRect(rect.left(),
                                                rect.top(), 4,
                                                rect.height()),
                                         {255, 255, 255, 90});
                }
		RkRect txtRect = rect;
		txtRect.setWidth(300);
		txtRect.setTopLeft(RkPoint(txtRect.left() + 7, txtRect.top()));
		painter.setPen(txtPen);
		painter.drawText(txtRect,
                                 std::string(kitModel->percussionName(i)),
                                 Rk::Alignment::AlignLeft);
                auto channel = kitModel->percussionChannel(i);
		painter.drawText(RkRect(percussionWidth - channelWidth,
                                        y, channelWidth,
                                        percussionHeight),
				 "#" + std::to_string(channel));

                int x = rect.right() + 5;
                if (kitModel->canRemove()) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(remove_per_button)),
                                          x, rect.top() + 2);
                        x += 16 + 3;
                }
                if (kitModel->canCopy()) {
                        painter.drawImage(RkImage(16, 16, RK_IMAGE_RC(copy_per_button)),
                                          x, rect.top() + 2);
                }
                y += percussionHeight;
        }
}

void KitWidget::drawConnections(RkPainter &painter)
{
        auto n = kitModel->percussionNumber();
        for (decltype(n) i = 0; i < n; i++) {
                // Define intersection point;
                RkPoint p {percussionNameWidth + kitModel->percussionKey(i) * keyWidth + keyWidth / 2,
                                keyWidth + i * percussionHeight + percussionHeight / 2};
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
        if (event->button() != RkMouseEvent::ButtonType::Left
            && event->button() != RkMouseEvent::ButtonType::WheelUp
            && event->button() != RkMouseEvent::ButtonType::WheelDown)
                return;

        updatePercussionName();
	auto index = getLine(event->x(), event->y());
        if (kitModel->percussionValid(index)) {
		if (event->x() < percussionNameWidth) {
                        kitModel->selectPecussion(index);
			return;
		} else if ((event->x() > percussionWidth + 5)
                           && (event->x() < percussionWidth + 5 + 16)) {
                        if (kitModel->canCopy())
                                copyPercussion(index);
                        else
                                removePercussion(index);
                        return;
                } else if (event->x() > (percussionWidth - channelWidth)
                           && event->x() < percussionWidth) {
                        if(event->button() == RkMouseEvent::ButtonType::Left
                           || event->button() == RkMouseEvent::ButtonType::WheelUp) {
                                kitModel->increasePercussionChannel(index);
                        } else if (event->button() == RkMouseEvent::ButtonType::WheelDown) {
                                kitModel->decreasePercussionChannel(index);
                        }
                        return;
                } else if (kitModel->canCopy()
                           && (event->x() > percussionWidth + 5 + 16 + 3)
                           && (event->x() < percussionWidth + 5 + 16 + 3 + 16)) {
                        copyPercussion(index);
                        return;
                } else if (key = getKey(event->x()); key) {
                        kitModel->setPercussionKey(id, key);
                }
        }
}

void KitWidget::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                mouseButtonPressEvent(event);
                return;
        }

	if (event->button() != RkMouseEvent::ButtonType::Left)
		return;

	editedLineIndex = getLine(event->x(), event->y());
	if (editedLineIndex > -1 && event->x() < percussionNameWidth) {
                editPercussion->setText(kitModel->percussionName(editedLineIndex));
		editPercussion->setSize({percussionNameWidth, percussionHeight});
                int y = keyWidth + percussionHeight * ((event->y() - keyWidth) / percussionHeight);
                editPercussion->setPosition(0, y);
		editPercussion->show();
		editPercussion->setFocus();
	}
}

void KitWidget::updatePercussionName()
{
	if (editedLineIndex > -1) {
		auto name = editPercussion->text();
		if (!name.empty()) {
			kitModel->setPercussionName(editedLineIndex, name);
			editPercussion->hide();
			editPercussion->setFocus(false);
		}
	}
}

int KitWidget::getLine(int x, int y) const
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

void KitWidget::showFileDialog(FileDialog::Type type)
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
        kitModel->open(file);
}

void KitWidget::saveKit(const std::string &file)
{
        kitModel->save();
}

void KitWidget::addNewPercussion()
{
        kitModel->addNewPercussion();
}

void KitWidget::removePercussion(int index)
{
        kitModel->removePercussion(index);
}

void KitWidget::copyPercussion(int index)
{
        kitModel->copyPercussion(index);
}

void KitWidget::updateGui()
{
        update();
}

KitModel* KitWidget::kitModel()
{
        return kitModel;
}
