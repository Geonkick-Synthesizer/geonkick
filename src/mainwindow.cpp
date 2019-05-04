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
//#include "limiter.h"
//#include "export_widget.h"
#include "geonkick_api.h"
#include "geonkick_state.h"
//#include "about.h"

MainWindow::MainWindow(RkMain *app, GeonkickApi *api)
        : GeonkickWidget(app)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
        //        presetName(preset)
{
        setFixedSize(940, 760);
        setTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
//        setWindowIcon(QPixmap(":/app_icon.png"));
}

MainWindow::MainWindow(RkMain *app, GeonkickApi *api, const RkNativeWindowInfo &info)
        : GeonkickWidget(app, info)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
          //        presetName(preset)
{
        setTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
        setFixedSize(940, 760);
//        setWindowIcon(QPixmap(":/app_icon.png"));
}

MainWindow::~MainWindow()
{
        if (geonkickApi) {
                geonkickApi->registerCallbacks(false);
                RK_ACT_UNBIND_ALL(geonkickApi, kickUpdated);
                if (geonkickApi->isStandalone())
                        delete geonkickApi;
        }
}

bool MainWindow::init(void)
{
        oscillators = geonkickApi->oscillators();
        /*        if (geonkickApi->isStandalone() && !geonkickApi->isJackEnabled())
                QMessageBox::warning(this, "Warning - Geonkick", tr("Jack is not installed" \
                                     " or not running. There is a need for jack server running " \
                                     "in order to have audio output."),
                                     QMessageBox::Ok);
        */

        auto topBar = new TopBar(this);
        topBar->setX(10);
        topBar->show();
        RK_ACT_BIND(topBar, openFile, RK_ACT_ARGS(), this, openFileDialog(FileDialog::Type::Open));
        RK_ACT_BIND(topBar, saveFile, RK_ACT_ARGS(), this, openFileDialog(FileDialog::Type::Save));

        // Create envelope widget.
        envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        envelopeWidget->setX(10);
        envelopeWidget->setY(topBar->y() + topBar->height());
        envelopeWidget->setFixedSize(RkSize(850, 340));
        envelopeWidget->show();
        //        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), envelopeWidget, update());

        //        auto limiterWidget = new Limiter(geonkickApi, this);
        //        connect(this, SIGNAL(updateGui()), limiterWidget, SLOT(updateLimiter()));
        //        limiterWidget->setFixedSize(65, 340);

        controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
        controlAreaWidget->setPosition(10, envelopeWidget->y() + envelopeWidget->height() + 3);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlAreaWidget, updateGui());
        controlAreaWidget->show();
        //        if (!presetName.isEmpty()) {
        //                setPreset(presetName);
        //                updateGui();
        //         }

        return true;
}

void MainWindow::openExportDialog()
{
        //        ExportWidget exportDialog(this, geonkickApi);
        //        exportDialog.exec();
}

void MainWindow::savePreset(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("Save Preset: " << "Can't save preset. File name empty or wrong format. Format example: 'mykick.gkick'");
                return;
        }

        std::experimental::filesystem::path filePath(fileName);
        std::locale loc;
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK"))
                filePath.replace_extension(".gkick");

        std::ofstream file;
        file.open(std::experimental::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("Error | Save Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME) << ". Can't save preset");
                return;
        }
        file << geonkickApi->getState()->toJson();
        file.close();
        topBar->setPresetName(filePath.filename());
}

void MainWindow::openPreset(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("Open Preset: " << "Can't save preset. File name empty or wrong format. Format example: 'mykick.gkick'");
                return;
        }

        std::experimental::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkick"
            && filePath.extension() != ".GKICK")) {
                RK_LOG_ERROR("Open Preset: " << "Can't open preset. Wrong file format.");
                return;
        }

        std::ifstream file;
        file.open(std::experimental::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("Open Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME) << ". Can't open preset.");
                return;
        }

        std::string fileData((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        auto state = std::make_shared<GeonkickState>(fileData);
        geonkickApi->setState(state);
        //                topBar->setPresetName(QFileInfo(file).baseName());
        file.close();
        updateGui();
}

void MainWindow::openFileDialog(FileDialog::Type type)
{
        auto fileDialog = new FileDialog(this, type, type == FileDialog::Type::Open ? "Open Preset" : "Save Preset");
        if (type == FileDialog::Type::Open)
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, openPreset(file));
        else
                RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), this, savePreset(file));
}

/*void MainWindow::openAboutDialog()
{
        //        AboutDialog aboutDialog(this);
        //        aboutDialog.exec();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_K) {
                geonkickApi->setKeyPressed(true, 127);
        } else if (event->modifiers() ==  Qt::ControlModifier
                   && event->key() == Qt::Key_R) {
                geonkickApi->setState(geonkickApi->getDefaultState());
                topBar->setPresetName("");
                emit updateGui();
        } else if (event->modifiers() ==  Qt::ControlModifier
                   && event->key() == Qt::Key_H) {
                envelopeWidget->hideEnvelope(true);
                }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
        if (event->modifiers() ==  Qt::ControlModifier
            && event->key() == Qt::Key_H) {
                envelopeWidget->hideEnvelope(false);
        }
}
*/

