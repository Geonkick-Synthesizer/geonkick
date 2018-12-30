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

#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>

#define GEONKICK_MAINWINDOW_WIDTH  940
#define GEONKICK_MAINWINDOW_HEIGHT 760

MainWindow::MainWindow(GeonkickApi *api, GeonkickWidget *parent) :
        GeonkickWidget(parent),
        geonkickApi(api),
        topBar(nullptr),
        envelopeWidget(nullptr)
{
        setWindowTitle(GEOKICK_APP_NAME);
        geonkickApi->registerCallbacks(true);
        setFixedSize(GEONKICK_MAINWINDOW_WIDTH, GEONKICK_MAINWINDOW_HEIGHT);
        setWindowIcon(QPixmap(":/app_icon.png"));
}

MainWindow::~MainWindow()
{
        if (geonkickApi)
                geonkickApi->registerCallbacks(false);
}

bool MainWindow::init(void)
{
        oscillators = geonkickApi->oscillators();

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        setLayout(mainLayout);
        topBar = new TopBar(this);
        mainLayout->addWidget(topBar);

        // Create envelope widget.
        auto hBoxLayout = new QHBoxLayout;
        hBoxLayout->setSpacing(0);
        hBoxLayout->setContentsMargins(0, 0, 0, 0);
        envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        connect(this, SIGNAL(updateGui()), envelopeWidget, SIGNAL(update()));
        envelopeWidget->setFixedSize(850, 340);
        hBoxLayout->addWidget(envelopeWidget);
        auto limiterWidget = new Limiter(geonkickApi, this);
        connect(this, SIGNAL(updateGui()), limiterWidget, SLOT(updateLimiter()));
        limiterWidget->setFixedSize(65, 340);
        hBoxLayout->addWidget(limiterWidget);
        mainLayout->addLayout(hBoxLayout);

        auto controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
        connect(this, SIGNAL(updateGui()), controlAreaWidget, SIGNAL(update()));
        mainLayout->addSpacing(5);
        mainLayout->addWidget(controlAreaWidget);
        return true;
}

void MainWindow::openExportDialog()
{
        ExportWidget exportDialog(this, geonkickApi);
        exportDialog.exec();
}

void MainWindow::savePreset()
{
        QFileDialog fileDialog(this, tr("Save Preset") + QString(" - ") + QString(GEOKICK_APP_NAME),
                               "./",
                               tr("Geonkick preset (*.gkick)"));
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        if (fileDialog.exec() == QDialog::Rejected)
                return;

        QStringList files = fileDialog.selectedFiles();
        if (files.isEmpty() || files.first().isEmpty()) {
                QMessageBox::critical(this,
                                      "Error | Save Preset",
                                      "Can't save preset. Empty file name. File name format example: 'mykick.gkick'");
                return;
        }

        QFileInfo fileInfo(files.first());
        if (fileInfo.baseName().isEmpty()) {
                QMessageBox::critical(this,
                                      "Error | Save Preset" + QString(" - ") + QString(GEOKICK_APP_NAME),
                                      "Can't save preset. Wrong file format. File name format example: 'mykick.gkick'");
                return;
        }

        QString fileName;
        if (fileInfo.suffix().isEmpty() || fileInfo.suffix().toLower() != "gkick")
                fileName = fileInfo.filePath() + ".gkick";
        else
                fileName = fileInfo.filePath();

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Save Preset" + QString(" - ") + QString(GEOKICK_APP_NAME), "Can't save preset");
                return;
        }

        file.write(geonkickApi->getState()->toJson());
        file.close();
        topBar->setPresetName(QFileInfo(file).baseName());
}

void MainWindow::openPreset()
{
        QFileDialog fileDialog(this, tr("Open Preset")  + QString(" - ") + QString(GEOKICK_APP_NAME), "./",
                               tr("Geonkick preset (*.gkick)"));
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (fileDialog.exec() == QDialog::Rejected)
                return;

        QStringList files = fileDialog.selectedFiles();
        if (files.isEmpty() || files.first().isEmpty()) {
                QMessageBox::critical(this, "Error | Open Preset" + QString(" - ") + QString(GEOKICK_APP_NAME), "Can't save preset");
                return;
        }


        QFile file(files.first());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Open Preset" + QString(" - ") + QString(GEOKICK_APP_NAME), "Can't open preset");
                return;
        }

        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if (document.isNull()) {
                QMessageBox::critical(this, "Error | Open Preset" + QString(" - ") + QString(GEOKICK_APP_NAME), "Wrong file contents");
                return;
        } else {
                geonkickApi->setState(std::make_shared<GeonkickState>(document.toBinaryData()));
                topBar->setPresetName(QFileInfo(file).baseName());
        }
        file.close();
        emit updateGui();
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
