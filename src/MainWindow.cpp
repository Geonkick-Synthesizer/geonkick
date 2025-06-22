
/**
 * File name: MainWindow.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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

#include "MainWindow.h"
#include "GeonkickModel.h"
#include "OscillatorModel.h"
#include "envelope_widget.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "control_area.h"
#include "TopBar.h"
#include "Sidebar.h"
#include "limiter.h"
#include "export_widget.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "ViewState.h"
#include "UiSettings.h"
#include "GeonkickConfig.h"

#include <RkEvent.h>

constexpr int MAIN_WINDOW_WIDTH  = 940;
#ifdef GEONKICK_SINGLE
constexpr int MAIN_WINDOW_HEIGHT = 690;
#else
constexpr int MAIN_WINDOW_HEIGHT = 715;
#endif // GEONKICK_SINGLE

MainWindow::MainWindow(RkMain& app, GeonkickApi *api, const std::string &preset)
        : GeonkickWidget(app)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{preset}
        , limiterWidget{nullptr}
        , geonkickModel{new GeonkickModel(this, geonkickApi)}
{
        setTitle(Geonkick::appName);
        setName("MainWindow");
        setScaleFactor(geonkickApi->getScaleFactor());
        createViewState();
        setFixedSize(MAIN_WINDOW_WIDTH + (GeonkickConfig().isShowSidebar() ? 313 : 0),
                     MAIN_WINDOW_HEIGHT);
        setTitle(GEONKICK_NAME);
        geonkickApi->registerCallbacks(true);
        RK_ACT_BIND(geonkickApi, stateChanged, RK_ACT_ARGS(), this, updateGui());
        createShortcuts();
}

MainWindow::MainWindow(RkMain& app, GeonkickApi *api, const RkNativeWindowInfo &info)
        : GeonkickWidget(app, info)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        , presetName{std::string()}
        , limiterWidget{nullptr}
        , geonkickModel{new GeonkickModel(this, geonkickApi)}
{
        setScaleFactor(geonkickApi->getScaleFactor());
        createViewState();
        setFixedSize(MAIN_WINDOW_WIDTH + (GeonkickConfig().isShowSidebar() ? 313 : 0),
                     MAIN_WINDOW_HEIGHT);
        setTitle(GEONKICK_NAME);
        geonkickApi->registerCallbacks(true);
        RK_ACT_BIND(geonkickApi, stateChanged, RK_ACT_ARGS(), this, updateGui());
        createShortcuts();
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

void MainWindow::createViewState()
{
        auto viewState = new ViewState(this);
        viewState->setName("ViewState");
        UiSettings *uiSettings = geonkickApi->getUiSettings();
        viewState->setMainView(uiSettings->getMainView());
        viewState->setSamplesBrowserPath(uiSettings->samplesBrowserPath());
        RK_ACT_BIND(viewState, mainViewChanged,
                    RK_ACT_ARGS(ViewState::View view),
                    geonkickApi, getUiSettings()->setMainView(view));
        RK_ACT_BIND(viewState, samplesBrowserPathChanged,
                    RK_ACT_ARGS(const std::string &path),
                    geonkickApi, getUiSettings()->setSamplesBrowserPath(path));
        setViewState(viewState);
}

void MainWindow::createShortcuts()
{
        addShortcut(Rk::Key::Key_K, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_K, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_k, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_k, Rk::KeyModifiers::Control_Right);

        addShortcut(Rk::Key::Key_H, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_H, Rk::KeyModifiers::Control_Right);
        addShortcut(Rk::Key::Key_h, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_h, Rk::KeyModifiers::Control_Right);

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

        addShortcut(Rk::Key::Key_f, Rk::KeyModifiers::Control_Left);
        addShortcut(Rk::Key::Key_F, Rk::KeyModifiers::Control_Left);

        addShortcut(Rk::Key::Key_Control_Left, Rk::KeyModifiers::Control_Left);
}

bool MainWindow::init(void)
{
        if (geonkickApi->isStandalone() && !geonkickApi->isJackEnabled()) {
                GEONKICK_LOG_INFO("Jack is not installed or not running. "
                                  << "There is a need for jack server running "
                                  << "in order to have audio output.");
        }
        topBar = new TopBar(this, geonkickModel);
        topBar->setX(10);
        topBar->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), topBar, updateGui());
        RK_ACT_BIND(topBar, resetToDefault, RK_ACT_ARGS(),
                    this, resetToDefault());
        RK_ACT_BIND(topBar, layerSelected,
                    RK_ACT_ARGS(GeonkickApi::Layer layer, bool b),
                    geonkickApi, enbaleLayer(layer, b));

        // Create Sidebar
        if (GeonkickConfig().isShowSidebar()) {
                auto sidebar = new Sidebar(this, geonkickModel);
                sidebar->setPosition({MAIN_WINDOW_WIDTH, 4});
        }

        // Create envelope widget.
        envelopeWidget = new EnvelopeWidget(this, geonkickModel);
        envelopeWidget->setX(10);
        envelopeWidget->setY(topBar->y() + topBar->height());
        envelopeWidget->setFixedSize(850, 305);
        envelopeWidget->show();

        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), envelopeWidget, updateGui());
        RK_ACT_BIND(envelopeWidget, requestUpdateGui, RK_ACT_ARGS(), this, updateGui());
        limiterWidget = new Limiter(geonkickApi, this);
        limiterWidget->setPosition(envelopeWidget->x() + envelopeWidget->width() + 8,
                                   envelopeWidget->y());
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), limiterWidget, onUpdateLimiter());
        limiterWidget->show();
        controlAreaWidget = new ControlArea(this, geonkickModel);
        controlAreaWidget->setPosition(10, envelopeWidget->y() + envelopeWidget->height());
        controlAreaWidget->show();
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());

        RK_ACT_BIND(geonkickModel->getKitModel(),
                    limiterUpdated,
                    RK_ACT_ARGS(KitModel::PercussionIndex index),
                    this,
                    updateLimiter(index));
        RK_ACT_BIND(limiterWidget, limiterUpdated, RK_ACT_ARGS(int val),
                    geonkickModel->getKitModel(),
                    updatePercussion(geonkickModel->getKitModel()->selectedPercussion()));

        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());
        if (geonkickApi->isStandalone() && !presetName.empty())
                openPreset(presetName);
        topBar->setPresetName(geonkickApi->getPercussionName(geonkickApi->currentPercussion()));
        updateGui();
        show();
        return true;
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
        if (state->getName().empty() || state->getName() == "Default")
                state->setName(filePath.stem().string());
        state->setId(geonkickApi->currentPercussion());
        geonkickApi->setPercussionState(state);
        action geonkickApi->percussionUpdated(state->getId());
        file.close();
        geonkickApi->setCurrentWorkingPath("OpenPreset",
                                           filePath.has_parent_path() ? filePath.parent_path().string() : filePath.string());
        updateGui();
}

void MainWindow::shortcutEvent(RkKeyEvent *event)
{
        if (event->type() == RkEvent::Type::KeyPressed) {
                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                    && (event->key() == Rk::Key::Key_k || event->key() == Rk::Key::Key_K)) {
                        geonkickApi->playKick();
                } else if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                           && (event->key() == Rk::Key::Key_r || event->key() == Rk::Key::Key_R)) {
                        resetToDefault();
                } else if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                           && (event->key() == Rk::Key::Key_c || event->key() == Rk::Key::Key_C)) {
                        geonkickApi->copyToClipboard();
                } else if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                           && (event->key() == Rk::Key::Key_v || event->key() == Rk::Key::Key_V)) {
                        geonkickApi->pasteFromClipboard();
                        geonkickApi->notifyPercussionUpdated(geonkickApi->currentPercussion());
                        updateGui();
                } else if ((event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                           && (event->key() == Rk::Key::Key_F || event->key() == Rk::Key::Key_f)) {
                        geonkickApi->setScaleFactor((geonkickApi->getScaleFactor() + 0.5 > 2.1) ? 1 : scaleFactor() + 0.5);
                        setScaleFactor(geonkickApi->getScaleFactor());
                        setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
                        updateGui();
                        action onScaleFactor(geonkickApi->getScaleFactor());
                }

                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control_Left))
                        envelopeWidget->setPointEditingMode(true);

                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)
                    && (event->key() == Rk::Key::Key_h || event->key() == Rk::Key::Key_H))
                        envelopeWidget->hideEnvelope(true);
        } else {
                if (!(event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control_Left)))
                        envelopeWidget->setPointEditingMode(false);

                if (!(event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control))
                    || (event->key() == Rk::Key::Key_h || event->key() == Rk::Key::Key_H))
                        envelopeWidget->hideEnvelope(false);
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

void MainWindow::dropEvent(RkDropEvent *event)
{
        std::string fileExtention;
        try {
                std::filesystem::path path(event->getFilePath());
                fileExtention = Geonkick::toLower(path.extension().string());
        } catch (const std::exception& e) {
                GEONKICK_LOG_ERROR("can't create path " << e.what());
                return;
        }

        std::string file = event->getFilePath();
        if (fileExtention == ".gkit") {
                geonkickModel->getKitModel()->open(file);
        } else if  (fileExtention == ".gkick") {
                openPreset(file);
        } else if (fileExtention == ".wav"
                 || fileExtention == ".flac"
                 || fileExtention == ".ogg") {
                setSample(file);
        }
}

void MainWindow::setSample(const std::string &file)
{
        auto osc = envelopeWidget->getCurrentOscillator();
        if (osc) {
                osc->setFunction(OscillatorModel::FunctionType::Sample);
                geonkickApi->setOscillatorSample(file, osc->index());
                geonkickApi->notifyPercussionUpdated(geonkickApi->currentPercussion());
                updateGui();
        }
}

void MainWindow::updateLimiter(KitModel::PercussionIndex index)
{
        if (geonkickModel->getKitModel()->isPercussionSelected(index))
                limiterWidget->onUpdateLimiter();
}

RkSize MainWindow::getWindowSize()
{
        return {MAIN_WINDOW_WIDTH + (GeonkickConfig().isShowSidebar() ? 313 : 0),
                MAIN_WINDOW_HEIGHT};
}
