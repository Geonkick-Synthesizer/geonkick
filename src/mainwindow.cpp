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
//#include "geonkick_state.h"
//#include "about.h"

MainWindow::MainWindow(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickWidget(parent)
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

MainWindow::MainWindow(GeonkickApi *api, const RkNativeWindowInfo &info)
        : GeonkickWidget(info)
        , geonkickApi{api}
        , topBar{nullptr}
        , envelopeWidget{nullptr}
          //        presetName(preset)
{
        setTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
        setFixedWidth(GEONKICK_MAINWINDOW_WIDTH);
        setFixedHeight(GEONKICK_MAINWINDOW_HEIGHT);
//        setWindowIcon(QPixmap(":/app_icon.png"));
}

MainWindow::~MainWindow()
{
        if (geonkickApi)
                geonkickApi->registerCallbacks(false);
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
        topBar->setX(15);
        topBar->show();

        // Create envelope widget.
        envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        envelopeWidget->setX(topBar->x());
        envelopeWidget->setY(topBar->y() + topBar->height());
        envelopeWidget->setFixedSize(RkSize(850, 340));
        envelopeWidget->show();
        //        connect(this, SIGNAL(updateGui()), envelopeWidget, SIGNAL(update()));

        //        auto limiterWidget = new Limiter(geonkickApi, this);
        //        connect(this, SIGNAL(updateGui()), limiterWidget, SLOT(updateLimiter()));
        //        limiterWidget->setFixedSize(65, 340);

        auto controlAreaWidget = new ControlArea(this, RkSize(width(), height() - topBar->height() - envelopeWidget->height()),
                                                 geonkickApi, oscillators);
        controlAreaWidget->setPosition(0, envelopeWidget->y() + envelopeWidget->height());
        RK_ACT_BIND(this, updateGui(), RK_ARGS(), controlAreaWidget, update());
        controlAreaWidget->show();
        //        if (!presetName.isEmpty()) {
        //        setPreset(presetName);
        //        emit updateGui();
        //        }

        return true;
}

/*void MainWindow::openExportDialog()
{
        ExportWidget exportDialog(this, geonkickApi);
        exportDialog.exec();
}

void MainWindow::savePreset()
{
        QFileDialog fileDialog(this, tr("Save Preset") + std::string(" - ") + std::string(GEOKICK_APP_NAME),
                               "",
                               tr("Geonkick preset (*.gkick)"));
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        if (fileDialog.exec() == QDialog::Rejected)
                return;

        std::stringList files = fileDialog.selectedFiles();
        if (files.isEmpty() || files.first().isEmpty()) {
                QMessageBox::critical(this,
                                      "Error | Save Preset",
                                      "Can't save preset. Empty file name. File name format example: 'mykick.gkick'");
                return;
        }

        QFileInfo fileInfo(files.first());
        if (fileInfo.baseName().isEmpty()) {
                QMessageBox::critical(this,
                                      "Error | Save Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME),
                                      "Can't save preset. Wrong file format. File name format example: 'mykick.gkick'");
                return;
        }

        std::string fileName;
        if (fileInfo.suffix().isEmpty() || fileInfo.suffix().toLower() != "gkick")
                fileName = fileInfo.filePath() + ".gkick";
        else
                fileName = fileInfo.filePath();

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Save Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME), "Can't save preset");
                return;
        }

        file.write(geonkickApi->getState()->toJson());
        file.close();
        topBar->setPresetName(QFileInfo(file).baseName());
}

void MainWindow::openPreset()
{
        QFileDialog fileDialog(this, tr("Open Preset")  + std::string(" - ") + std::string(GEOKICK_APP_NAME), "",
                               tr("Geonkick preset (*.gkick)"));
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (fileDialog.exec() == QDialog::Rejected)
                return;

        std::stringList files = fileDialog.selectedFiles();
        if (files.isEmpty() || files.first().isEmpty()) {
                QMessageBox::critical(this, "Error | Open Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME), "Can't open preset");
                return;
        }

        setPreset(files.first());
        emit updateGui();
}

void MainWindow::setPreset(const std::string &fileName)
{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Open Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME), "Can't open preset");
                return;
        }

        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if (document.isNull()) {
                QMessageBox::critical(this, "Error | Open Preset" + std::string(" - ") + std::string(GEOKICK_APP_NAME), "Wrong file contents");
                return;
        } else {
                geonkickApi->setState(std::make_shared<GeonkickState>(document.toBinaryData()));
                topBar->setPresetName(QFileInfo(file).baseName());
        }
        file.close();
}

void MainWindow::openAboutDialog()
{
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
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

