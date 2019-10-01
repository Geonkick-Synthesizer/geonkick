/**
 * File name: mainwindow.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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
#include "geonkick_state.h"
#include "about.h"

#include <RkPlatform.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

#include <RkEvent.h>

MainWindow::MainWindow(RkMain *app, GeonkickApi *api, std::string preset)
        : GeonkickWidget(app)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{preset}
{
        setFixedSize(940, 760);
        setTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
        show();
}

MainWindow::MainWindow(RkMain *app, GeonkickApi *api, const RkNativeWindowInfo &info)
        : GeonkickWidget(app, info)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{std::string()}
{
        setFixedSize(940, 760);
        setTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
        RK_ACT_BIND(geonkickApi, stateChanged, RK_ACT_ARGS(), this, updateGui());
        show();
}

MainWindow::~MainWindow()
{
        if (geonkickApi) {
                geonkickApi->registerCallbacks(false);
                geonkickApi->setEventQueue(nullptr);
                // Since for plugins the DSP/api is not destroyed there
                // is a need to unbind from the GUI that is being destroyed.
                RK_ACT_UNBIND_ALL(geonkickApi, kickLengthUpdated);
                RK_ACT_UNBIND_ALL(geonkickApi, kickAmplitudeUpdated);
                RK_ACT_UNBIND_ALL(geonkickApi, kickUpdated);
                RK_ACT_UNBIND_ALL(geonkickApi, newKickBuffer);
                RK_ACT_UNBIND_ALL(geonkickApi, currentPlayingFrameVal);
                RK_ACT_UNBIND_ALL(geonkickApi, stateChanged);
                if (geonkickApi->isStandalone())
                        delete geonkickApi;
        }
}

bool MainWindow::init(void)
{
        oscillators = geonkickApi->oscillators();
        if (geonkickApi->isStandalone() && !geonkickApi->isJackEnabled())
                GEONKICK_LOG_INFO("Jack is not installed or not running. "
                                  << "There is a need for jack server running "
                                  << "in order to have audio output.");

        topBar = new TopBar(this, geonkickApi);
        topBar->setX(10);
        topBar->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), topBar, updateGui());
        RK_ACT_BIND(topBar, openFile, RK_ACT_ARGS(), this, openFileDialog(FileDialog::Type::Open));
        RK_ACT_BIND(topBar, saveFile, RK_ACT_ARGS(), this, openFileDialog(FileDialog::Type::Save));
        RK_ACT_BIND(topBar, openAbout, RK_ACT_ARGS(), this, openAboutDialog());
        RK_ACT_BIND(topBar, openExport, RK_ACT_ARGS(), this, openExportDialog());
        RK_ACT_BIND(topBar, layerSelected, RK_ACT_ARGS(GeonkickApi::Layer layer, bool b), geonkickApi, enbaleLayer(layer, b));

        // Create envelope widget.
        envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        envelopeWidget->setX(10);
        envelopeWidget->setY(topBar->y() + topBar->height());
        envelopeWidget->setFixedSize(850, 340);
        envelopeWidget->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), envelopeWidget, updateGui());
        RK_ACT_BIND(envelopeWidget, requestUpdateGui, RK_ACT_ARGS(), this, updateGui());
        auto limiterWidget = new Limiter(geonkickApi, this);
        limiterWidget->setPosition(envelopeWidget->x() + envelopeWidget->width() + 8, envelopeWidget->y());
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), limiterWidget, onUpdateLimiter());
        limiterWidget->show();
        controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
        controlAreaWidget->setPosition(10, envelopeWidget->y() + envelopeWidget->height() + 3);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());
        controlAreaWidget->show();

        // TODO: Key shortcut feature will be implemented in the next version of Redkite.
        auto info = nativeWindowInfo();
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_o), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_O), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_h), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_H), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_k), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_K), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_a), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_A), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_e), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_E), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_r), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_R), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_s), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XGrabKey(info->display, XKeysymToKeycode(info->display, XK_S), ControlMask, info->window, False, GrabModeAsync, GrabModeAsync);
        XFlush(info->display);

        if (geonkickApi->isStandalone() && !presetName.empty())
                openPreset(presetName);
        updateGui();
        return true;
}

void MainWindow::openExportDialog()
{
        new ExportWidget(this, geonkickApi);
}

void MainWindow::savePreset(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("Save Preset: " << "Can't save preset. File name empty or wrong format. Format example: 'mykick.gkick'");
                return;
        }

        std::filesystem::path filePath(fileName);
        std::locale loc;
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK"))
                filePath.replace_extension(".gkick");

        std::ofstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("Error | Save Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME) << ". Can't save preset");
                return;
        }
        file << geonkickApi->getState()->toJson();
        file.close();
        topBar->setPresetName(filePath.stem());
        geonkickApi->setCurrentWorkingPath("SavePreset", filePath.has_parent_path() ? filePath.parent_path() : filePath);
}

void MainWindow::openPreset(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("Open Preset: " << "Can't save preset. File name empty or wrong format. Format example: 'mykick.gkick'");
                return;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK")) {
                RK_LOG_ERROR("Open Preset: " << "Can't open preset. Wrong file format.");
                return;
        }

        std::ifstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("Open Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME) << ". Can't open preset.");
                return;
        }

        std::string fileData((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        auto state = std::make_shared<GeonkickState>();
        state->loadData(fileData);
        geonkickApi->setState(state);
        topBar->setPresetName(filePath.stem());
        file.close();
        geonkickApi->setCurrentWorkingPath("OpenPreset", filePath.has_parent_path() ? filePath.parent_path() : filePath);
        updateGui();
}

void MainWindow::openFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Preset" : "Save Preset");
        fileDialog->setFilters({".gkick", ".GKICK"});
        if (type == FileDialog::Type::Open) {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("OpenPreset"));
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, openPreset(file));
        } else {
                fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("SavePreset"));
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, savePreset(file));
        }
}

void MainWindow::openAboutDialog()
{
        new AboutDialog(this);
}

void MainWindow::keyPressEvent(const std::shared_ptr<RkKeyEvent> &event)
{
        if (event->key() == Rk::Key::Key_k || event->key() == Rk::Key::Key_K) {
                /* Key press: note number A4, velocity maximum. */
                // geonkickApi->setKeyPressed(true, 69, 127); TODO: make this call thread-safe.
                // for now this function is called only from the audio thread.
        } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                   && (event->key() == Rk::Key::Key_r || event->key() == Rk::Key::Key_R)) {
                geonkickApi->setState(geonkickApi->getDefaultState());
                topBar->setPresetName("");
                updateGui();
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
        }
}

void MainWindow::keyReleaseEvent(const std::shared_ptr<RkKeyEvent> &event)
{
        if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
            && (event->key() == Rk::Key::Key_h || event->key() == Rk::Key::Key_H)) {
                envelopeWidget->hideEnvelope(false);
        }
}

