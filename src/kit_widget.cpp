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
#include "geonkick_slider.h"
#include "percussion_view.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkProgressBar.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(add_per_button);
RK_DECLARE_IMAGE_RC(save_kit_button);
RK_DECLARE_IMAGE_RC(open_kit_button);

KitWidget::KitWidget(GeonkickWidget *parent, KitModel *model)
	: GeonkickWidget(parent)
        , kitModel{model}
        , addButton{nullptr}
        , openKitButton{nullptr}
        , saveKitButton{nullptr}
        , percussionsContainer{new RkContainer(this, Rk::Orientation::Vertical)}
{
        setTitle("KitWidget");
        setSize(parent->size());

        RK_ACT_BIND(kitModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        RK_ACT_BIND(kitModel, percussionAdded, RK_ACT_ARGS(PercussionModel *model),
                    this, addPercussion(model));

        percussionsContainer->setSize(size());
        percussionsContainer->setHiddenTakesPlace();
        auto topContiner = new RkContainer(this);
        topContiner->setSpacing(5);
        percussionsContainer->setHiddenTakesPlace();
        topContiner->setSize({width(), 40});
        addButton = new RkButton(this);
        addButton->setBackgroundColor(background());
        addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), kitModel, addNewPercussion());
        addButton->show();
        topContiner->addWidget(addButton);

        openKitButton = new RkButton(this);
        openKitButton->setBackgroundColor(background());
        openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setImage(RkImage(16, 16, RK_IMAGE_RC(open_kit_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, showFileDialog(FileDialog::Type::Open));
        openKitButton->show();
        topContiner->addWidget(openKitButton);

        saveKitButton = new RkButton(this);
        saveKitButton->setBackgroundColor(background());
        saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setImage(RkImage(16, 16, RK_IMAGE_RC(save_kit_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, showFileDialog(FileDialog::Type::Save));
        saveKitButton->show();
        topContiner->addWidget(saveKitButton);
        percussionsContainer->addContainer(topContiner);
        updateView();
}

void KitWidget::updateView()
{
        auto &models = kitModel->percussionModels();
        size_t n = kitModel->maxPercussionNumber();
        GEONKICK_LOG_INFO("size: " << models.size());
        for (decltype(n) i = 0; i < n; i++) {
                if (i < models.size()) {
                        if (percussionsContainer->at(i + 1))
                                updatePercussion(i, models[i]);
                        else
                                addPercussion(models[i]);
                } else if (percussionsContainer->at(i + 1)) {
                        removePercussion(i);
                }
        }
}

void KitWidget::addPercussion(PercussionModel *model)
{
        auto percussionView = new KitPercussionView(this, model);
        percussionsContainer->addWidget(percussionView, Rk::Alignment::AlignTop);
        percussionView->show();
}

void KitWidget::updatePercussion(PercussionIndex index, PercussionModel *model)
{
        auto percussionView = dynamic_cast<KitPercussionView*>(percussionsContainer->at(index));
        if (percussionView)
                percussionView->setModel(model);
}

void KitWidget::removePercussion(PercussionIndex index)
{
        if (percussionsContainer->at(index)) {
                delete percussionsContainer->at(index);
                percussionsContainer->removeAt(index);
        }
}

// void KitWidget::drawKeys(RkPainter &painter)
// {
//         auto pen = painter.pen();
//         pen.setColor({200, 200, 200});

//         auto font = painter.font();
//         font.setSize(12);
//         painter.setFont(font);

//         int x = percussionNameWidth;
//         auto n = kitModel->percussionNumber();
//         auto nKeys = kitModel->keysNumber();
//         for (decltype(nKeys) i = 0; i < nKeys; i++) {
//                 RkRect rect(x, 0, keyWidth, keyWidth + n * percussionHeight);
//                 if (i % 2)
//                         painter.fillRect(rect, {60, 60, 60});
//                 else
//                         painter.fillRect(rect, {50, 50, 50});
//                 RkRect txtRect(rect.left(), 10, rect.width(), painter.font().size());
//                 painter.setPen(pen);
//                 painter.drawText(txtRect, kitModel->keyName(i));
//                 x += keyWidth;
//         }
// }

void KitWidget::showFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Kit" : "Save Kit");
        fileDialog->setFilters({".gkit", ".GKIT"});
        if (type == FileDialog::Type::Open) {
                fileDialog->setCurrentDirectoy(kitModel->workingPath("OpenKit"));
                RK_ACT_BIND(fileDialog, selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this, openKit(file));
        } else {
                fileDialog->setCurrentDirectoy(kitModel->workingPath("SaveKit"));
                RK_ACT_BIND(fileDialog, selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this, saveKit(file));
        }
}

void KitWidget::openKit(const std::string &file)
{
        if (std::filesystem::is_regular_file(file) && kitModel->open(file))
                setFocus();
}

void KitWidget::saveKit(const std::string &file)
{
        kitModel->save(file);
}

void KitWidget::copyPercussion(int index)
{
        //kitModel->copyPercussion(index);
}

void KitWidget::keyPressEvent(RkKeyEvent *event)
{
        // if (event->key() != Rk::Key::Key_Up && event->key() != Rk::Key::Key_Down)
        //         return;

        // auto index = kitModel->selectedPercussion();
        // if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))) {
        //         kitModel->moveSelectedPercussion(event->key() == Rk::Key::Key_Down);
        // } else if (event->key() == Rk::Key::Key_Up && --index > -1) {
        //         kitModel->selectPercussion(index);
        // } else if (event->key() == Rk::Key::Key_Down
        //            && ++index < static_cast<decltype(index)>(kitModel->percussionNumber())) {
        //         //                kitModel->selectPercussion(index);
        // }
}
