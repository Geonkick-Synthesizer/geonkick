/**
 * File name: kit_widget.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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
#include "percussion_model.h"

#include <RkEvent.h>
#include <RkImage.h>
#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkProgressBar.h>
#include <RkContainer.h>
#include <RkTimer.h>

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
        , levelersTimer{new RkTimer(this, 30)}
{
        RK_ACT_BIND(levelersTimer, timeout, RK_ACT_ARGS(), this, onUpdateLevelers());
        percussionsContainer->setHiddenTakesPlace();
        setSize(parent->size());

        RK_ACT_BIND(kitModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        RK_ACT_BIND(kitModel, percussionAdded, RK_ACT_ARGS(PercussionModel *model),
                    this, addPercussion(model));
        RK_ACT_BIND(kitModel, percussionRemoved, RK_ACT_ARGS(PercussionIndex index),
                    this, removePercussion(index));

        addShortcut(Rk::Key::Key_Up);
        addShortcut(Rk::Key::Key_Down);
        addShortcut(Rk::Key::Key_Up, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_Up, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_Down, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_Down, Rk::KeyModifiers::Control_Right);

        auto kitContainer = new RkContainer(this, Rk::Orientation::Vertical);
        kitContainer->setHiddenTakesPlace();
        kitContainer->setSize(size());

        auto topContainer = new RkContainer(this);
        topContainer->setSpacing(5);
        percussionsContainer->setHiddenTakesPlace();
        topContainer->setSize({width(), 30});

        addButton = new RkButton(this);
        addButton->setBackgroundColor(background());
        addButton->setCheckable(true);
        addButton->setSize(16, 16);
        addButton->setImage(RkImage(16, 16, RK_IMAGE_RC(add_per_button)));
        RK_ACT_BIND(addButton, toggled, RK_ACT_ARGS(bool b), kitModel, addNewPercussion());
        addButton->show();
        topContainer->addWidget(addButton);

        openKitButton = new RkButton(this);
        openKitButton->setBackgroundColor(background());
        openKitButton->setCheckable(true);
        openKitButton->setSize(16, 16);
        openKitButton->setImage(RkImage(16, 16, RK_IMAGE_RC(open_kit_button)));
        RK_ACT_BIND(openKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, showFileDialog(FileDialog::Type::Open));
        openKitButton->show();
        topContainer->addWidget(openKitButton);

        saveKitButton = new RkButton(this);
        saveKitButton->setBackgroundColor(background());
        saveKitButton->setCheckable(true);
        saveKitButton->setSize(16, 16);
        saveKitButton->setImage(RkImage(16, 16, RK_IMAGE_RC(save_kit_button)));
        RK_ACT_BIND(saveKitButton, toggled, RK_ACT_ARGS(bool b),
                    this, showFileDialog(FileDialog::Type::Save));
        saveKitButton->show();
        topContainer->addWidget(saveKitButton);
        percussionsContainer->setHeight(kitContainer->height() - topContainer->height());

        auto kitKeysView = new KitKeysView(this, kitModel);
        kitKeysView->show();
        topContainer->addSpace(100 - 3 * 16 - 4 * 5);
        topContainer->addWidget(kitKeysView);

        kitContainer->addContainer(topContainer);
        kitContainer->addContainer(percussionsContainer);

        updateView();
        levelersTimer->start();
}

void KitWidget::updateView()
{
        percussionsContainer->clear();
        for (auto &percussionView: percussionViewList)
                delete percussionView;
        percussionViewList.clear();

        auto &models = kitModel->percussionModels();
        for (const auto &m: models)
                addPercussion(m);
}

void KitWidget::addPercussion(PercussionModel *model)
{
        auto percussionView = new KitPercussionView(this, model);
        percussionsContainer->addWidget(percussionView, Rk::Alignment::AlignTop);
        percussionViewList.push_back(percussionView);
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
        for (auto it = percussionViewList.begin(); it != percussionViewList.end(); ++it) {
                if ((*it)->getModel()->index() == index) {
                        percussionsContainer->removeAt(index);
                        delete *it;
                        percussionViewList.erase(it);
                        percussionsContainer->update();
                        break;
                }
        }
}

void KitWidget::copyPercussion(int index)
{
        kitModel->copyPercussion(index);
}

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

void KitWidget::keyPressEvent(RkKeyEvent *event)
{
        if (event->key() != Rk::Key::Key_Up && event->key() != Rk::Key::Key_Down)
                return;

        auto index = kitModel->selectedPercussion();
        if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))) {
                kitModel->moveSelectedPercussion(event->key() == Rk::Key::Key_Down);
        } else if (event->key() == Rk::Key::Key_Up) {
                kitModel->selectPercussion(--index);
        } else if (event->key() == Rk::Key::Key_Down) {
                kitModel->selectPercussion(++index);
        }
}

KitKeysView::KitKeysView(KitWidget *parent, KitModel *model)
                : GeonkickWidget(parent)
                , kitModel{model}
                , keyWidth{30}
        {
                setSize(kitModel->keysNumber() * keyWidth, keyWidth);
        }

void KitKeysView::paintWidget(RkPaintEvent *event)
{
        RkImage img(size());
        RkPainter paint(&img);
        paint.fillRect(rect(), background());

        auto pen = paint.pen();
        pen.setColor({200, 200, 200});

        auto font = paint.font();
        font.setSize(12);
        paint.setFont(font);

        auto nKeys = kitModel->keysNumber();
        for (decltype(nKeys) i = 0; i < nKeys; i++) {
                auto rect = RkRect(i * keyWidth, 0, keyWidth, keyWidth);
                if (i % 2)
                        paint.fillRect(rect, {60, 60, 60});
                else
                        paint.fillRect(rect, {50, 50, 50});
                RkRect txtRect(rect.left(), (rect.height() - paint.font().size()) / 2,
                               rect.width(), paint.font().size());
                paint.setPen(pen);
                paint.drawText(txtRect, kitModel->keyName(i));
        }
        RkPainter painter(this);
        painter.drawImage(img, 0, 0);
}

KitModel* KitWidget::getModel() const
{
        return kitModel;
}

void KitWidget::onUpdateLevelers()
{
        for (const auto &per: percussionViewList)
                per->updateLeveler();
}
