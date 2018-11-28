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
#include "geonkick_theme.h"
#include "top_bar.h"
#include "fader.h"
#include "export_widget.h"
#include "geonkick_api.h"
#include "geonkick_state.h"

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
        geonkickApi(api)
{
        setFixedSize(GEONKICK_MAINWINDOW_WIDTH, GEONKICK_MAINWINDOW_HEIGHT);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::init(void)
{
        oscillators = geonkickApi->oscillators();
        setTheme(new GeonkickTheme("Geontime"));

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        setLayout(mainLayout);
        mainLayout->addWidget(new TopBar(this));

        // Create envelope widget.
        auto hBoxLayout = new QHBoxLayout;
        hBoxLayout->setSpacing(0);
        hBoxLayout->setContentsMargins(0, 0, 0, 0);
        auto envelopeWidget = new EnvelopeWidget(this, geonkickApi, oscillators);
        envelopeWidget->setFixedSize(850, 340);
        hBoxLayout->addWidget(envelopeWidget);
        auto faderWidget = new Fader(this);
        faderWidget->setFixedSize(65, 340);
        connect(faderWidget, SIGNAL(levelUpdated(int)), this, SLOT(setLimiterValue(int)));
        faderWidget->setFaderLevel(100 * geonkickApi->limiterValue());
        hBoxLayout->addWidget(faderWidget);
        mainLayout->addLayout(hBoxLayout);

        ControlArea *controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
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
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Preset"),
                                                        "./",
                                                        tr("Geonkick preset (*.gkick)"));

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Save Preset", "Can't save preset");
                return;
        }

        file.write(geonkickApi->getState()->toJson());
        file.close();
}

void MainWindow::openPreset()
{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Preset"), "./",
                                                        tr("Geonkick preset (*.gkick)"));

        GEONKICK_LOG_ERROR(fileName);
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "Error | Open Preset", "Can't open preset");
                return;
        }

        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if (document.isNull()) {
                QMessageBox::critical(this, "Error | Open Preset", "Wrong file contents");
        } else {
                geonkickApi->setState(std::make_shared<GeonkickState>(document.toBinaryData()));
        }
        file.close();
        updateGui();
}

void MainWindow::setLimiterValue(int value)
{
        geonkickApi->setLimiterValue(static_cast<double>(value) / 100);
}

void MainWindow::updateGui()
{
        /*        faderWidget.update();
        controlAreaWidget.update();
        envelopeWidget.update();*/
}
