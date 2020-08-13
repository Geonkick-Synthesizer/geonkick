/**
 * File name: mainwindow.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "mainwindow.h"
#include "oscillator.h"
#include "envelope_widget.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "control_area.h"
#include "top_bar.h"
#include "limiter.h"
#include "export_widget.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "about.h"
#include "right_bar.h"

#include <RkEvent.h>

MainWindow::MainWindow(RkMain *app, GeonkickApi *api, const std::string &preset)
        : GeonkickWidget(app)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{preset}
        , limiterWidget{nullptr}
        , kitModel{nullptr}
{
        setFixedSize(950, 760);
        setTitle(GEONKICK_NAME);
        geonkickApi->registerCallbacks(true);
	RK_ACT_BIND(geonkickApi, stateChanged, RK_ACT_ARGS(), this, updateGui());
        createShortcuts();
        show();
}

MainWindow::MainWindow(RkMain *app, GeonkickApi *api, const RkNativeWindowInfo &info)
        : GeonkickWidget(app, info)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{std::string()}
        , limiterWidget{nullptr}
        , kitModel{nullptr}
{
        setFixedSize(950, 760);
        setTitle(GEONKICK_NAME);
        geonkickApi->registerCallbacks(true);
        RK_ACT_BIND(geonkickApi, stateChanged, RK_ACT_ARGS(), this, updateGui());
        createShortcuts();
        show();
}

void MainWindow::createShortcuts()
{
        addShortcut(Rk::Key::Key_k, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_K, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_k, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_k, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_O, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_O, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_o, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_o, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_S, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_S, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_s, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_s, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_H, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_H, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_h, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_h, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_E, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_E, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_e, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_e, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_A, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_A, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_e, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_e, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_C, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_C, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_c, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_c, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_V, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_V, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_v, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_v, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_R, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_R, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_r, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_r, Rk::KeyModifiers::Control_Right);
}

MainWindow::~MainWindow()
{
        if (geonkickApi) {
                geonkickApi->registerCallbacks(false);
                geonkickApi->setEventQueue(nullptr);
                if (geonkickApi->isStandalone())
                        delete geonkickApi;
        }
}

bool MainWindow::init(void)
{
        oscillators = geonkickApi->oscillators();
        if (geonkickApi->isStandalone() && !geonkickApi->isJackEnabled()) {
                GEONKICK_LOG_INFO("Jack is not installed or not running. "
                                  << "There is a need for jack server running "
                                  << "in order to have audio output.");
        }
        topBar = new TopBar(this, geonkickApi);
        topBar->setX(10);
        topBar->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), topBar, updateGui());
        RK_ACT_BIND(topBar, openFile, RK_ACT_ARGS(),
                    this, openFileDialog(FileDialog::Type::Open));
        RK_ACT_BIND(topBar, saveFile, RK_ACT_ARGS(),
                    this, openFileDialog(FileDialog::Type::Save));
        RK_ACT_BIND(topBar, openAbout, RK_ACT_ARGS(),
                    this, openAboutDialog());
        RK_ACT_BIND(topBar, resetToDefault, RK_ACT_ARGS(),
                    this, resetToDefault());
        RK_ACT_BIND(topBar, openExport, RK_ACT_ARGS(),
                    this, openExportDialog());
        RK_ACT_BIND(topBar, layerSelected,
                    RK_ACT_ARGS(GeonkickApi::Layer layer, bool b),
                    geonkickApi, enbaleLayer(layer, b));

        // Create envelope widget.
        envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        envelopeWidget->setX(10);
        envelopeWidget->setY(topBar->y() + topBar->height());
        envelopeWidget->setFixedSize(850, 340);
        envelopeWidget->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), envelopeWidget, updateGui());
        RK_ACT_BIND(envelopeWidget, requestUpdateGui, RK_ACT_ARGS(), this, updateGui());
        limiterWidget = new Limiter(geonkickApi, this);
        limiterWidget->setPosition(envelopeWidget->x() + envelopeWidget->width() + 8,
                                   envelopeWidget->y());
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), limiterWidget, onUpdateLimiter());
        limiterWidget->show();

        controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
        controlAreaWidget->setEnvelopeWidget(envelopeWidget);
        controlAreaWidget->setPosition(10, envelopeWidget->y() + envelopeWidget->height() + 3);
        controlAreaWidget->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());

        kitModel = controlAreaWidget->getKitModel();
        RK_ACT_BIND(kitModel,
                    limiterUpdated,
                    RK_ACT_ARGS(KitModel::PercussionIndex index),
                    this,
                    updateLimiter(index));
        RK_ACT_BIND(limiterWidget, limiterUpdated, RK_ACT_ARGS(int val),
                    kitModel, updatePercussion(kitModel->selectedPercussion()));

        auto rightBar = new RightBar(this);
        rightBar->setPosition(width() - rightBar->width(), 0);
        rightBar->show();

        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());
        RK_ACT_BIND(rightBar, showControls, RK_ACT_ARGS(), controlAreaWidget, showControls());
        RK_ACT_BIND(rightBar, showKit, RK_ACT_ARGS(), controlAreaWidget, showKit());
        RK_ACT_BIND(rightBar, showPresets, RK_ACT_ARGS(), controlAreaWidget, showPresets());
        if (geonkickApi->isStandalone() && !presetName.empty())
                openPreset(presetName);
        topBar->setPresetName(geonkickApi->getPercussionName(geonkickApi->currentPercussion()));
        updateGui();
        return true;
}

void MainWindow::openExportDialog()
{
        new ExportWidget(this, geonkickApi);
}

void MainWindow::savePreset(const std::string &fileName)
{
        auto state = geonkickApi->getPercussionState();
        if (state->save(fileName)) {
                std::filesystem::path filePath(fileName);
                topBar->setPresetName(state->getName());
                geonkickApi->setCurrentWorkingPath("SavePreset",
                                                   filePath.has_parent_path() ? filePath.parent_path() : filePath);
        }
}

void MainWindow::openPreset(const std::string &fileName)
{
        if (fileName.size() < 7) {
                RK_LOG_ERROR("Open Preset: "
                             << "Can't open preset. File name "
                             << "empty or wrong format. Format example: 'mykick.gkick'");
                return;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || !std::filesystem::is_regular_file(filePath)
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK")) {
                RK_LOG_ERROR("Open Preset: " << "Can't open preset. Wrong file format.");
                return;
        }

        std::ifstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("Open Preset" + std::string(" - ") + std::string(GEONKICK_NAME)
                             << ". Can't open preset.");
                return;
        }

        std::string fileData((std::istreambuf_iterator<char>(file)),
                             (std::istreambuf_iterator<char>()));
        auto state = geonkickApi->getDefaultPercussionState();
        state->loadData(fileData);
        state->setId(geonkickApi->currentPercussion());
        geonkickApi->setPercussionState(state);
        action geonkickApi->percussionUpdated(state->getId());
        file.close();
        geonkickApi->setCurrentWorkingPath("OpenPreset",
                                           filePath.has_parent_path() ? filePath.parent_path() : filePath);
        updateGui();
}

void MainWindow::openFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Preset" : "Save Preset");
        fileDialog->setFilters({".gkick", ".GKICK"});
        if (type == FileDialog::Type::Open) {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("OpenPreset"));
                RK_ACT_BIND(fileDialog,
                            selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this,
                            openPreset(file));
        } else {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("SavePreset"));
                RK_ACT_BIND(fileDialog,
                            selectedFile,
                            RK_ACT_ARGS(const std::string &file),
                            this,
                            savePreset(file));
        }
}

void MainWindow::openAboutDialog()
{
        new AboutDialog(this);
}

void MainWindow::keyPressEvent(RkKeyEvent *event)
{
        if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
            && (event->key() == Rk::Key::Key_k || event->key() == Rk::Key::Key_K)) {
                geonkickApi->playKick();
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_r || event->key() == Rk::Key::Key_R)) {
                resetToDefault();
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_h || event->key() == Rk::Key::Key_H)) {
                envelopeWidget->hideEnvelope(true);
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_o || event->key() == Rk::Key::Key_O)) {
                openFileDialog(FileDialog::Type::Open);
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_s || event->key() == Rk::Key::Key_S)) {
                openFileDialog(FileDialog::Type::Save);
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_e || event->key() == Rk::Key::Key_E)) {
                openExportDialog();
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_a || event->key() == Rk::Key::Key_A)) {
                openAboutDialog();
        } else if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                   && (event->key() == Rk::Key::Key_c || event->key() == Rk::Key::Key_C)) {
                geonkickApi->copyToClipboard();
        } else if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                    && (event->key() == Rk::Key::Key_v || event->key() == Rk::Key::Key_V)) {
                geonkickApi->pasteFromClipboard();
                geonkickApi->notifyPercussionUpdated(geonkickApi->currentPercussion());
                updateGui();
        }
}

void MainWindow::resetToDefault()
{
        auto currId = geonkickApi->currentPercussion();
        auto state = geonkickApi->getDefaultPercussionState();
        state->setId(currId);
        state->setName(geonkickApi->getPercussionName(currId));
        state->setPlayingKey(geonkickApi->getPercussionPlayingKey(currId));
        state->setChannel(geonkickApi->getPercussionChannel(currId));
        geonkickApi->setPercussionState(state);
        geonkickApi->notifyPercussionUpdated(geonkickApi->currentPercussion());
        updateGui();
}

void MainWindow::keyReleaseEvent(RkKeyEvent *event)
{
        if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
            && (event->key() == Rk::Key::Key_h || event->key() == Rk::Key::Key_H)) {
                envelopeWidget->hideEnvelope(false);
        }
}

void MainWindow::updateLimiter(KitModel::PercussionIndex index)
{
        if (kitModel->isPercussionSelected(index))
                limiterWidget->onUpdateLimiter();
}
