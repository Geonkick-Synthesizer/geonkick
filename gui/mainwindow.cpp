/**
 * File name: mainwindow.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

#define GEONKICK_MAINWINDOW_WIDTH  940
#define GEONKICK_MAINWINDOW_HEIGHT 765

MainWindow::MainWindow(GeonkickWidget *parent) :
        GeonkickWidget(parent),
        geonkickApi(new GeonkickApi(this))
{
        setFixedSize(GEONKICK_MAINWINDOW_WIDTH, GEONKICK_MAINWINDOW_HEIGHT);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::init(void)
{
        if (geonkickApi->init()) {
	  //return false;
        }

        setTheme(new GeonkickTheme("Geontime"));

        geonkickApi->setKickLength(300);
        geonkickApi->setKickAmplitude(0);
        geonkickApi->setKickFilterFrequency(0);
        oscillators = geonkickApi->oscillators();

        // Oscillator 1
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)];
        oscillator->setFrequency(1000);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);
	oscillator->setFunction(Oscillator::FunctionType::Sine);
        oscillator->setAmplitude(0.05);

        // Oscillator 2
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)];
        oscillator->setFrequency(1000);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);
	oscillator->setFunction(Oscillator::FunctionType::Sine);
        oscillator->setAmplitude(0.05);

        // Noise
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)];
	oscillator->setFunction(Oscillator::FunctionType::Noise);
	oscillator->setAmplitude(0.05);
	oscillator->setFilterType(Oscillator::FilterType::LowPass);
	oscillator->setFilterFrequency(5000);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 10);
        mainLayout->setSpacing(5);
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
        faderWidget->setFixedSize(65, 330);
        GEONKICK_LOG_DEBUG("Fader level: " << faderWidget->getFaderLevel());
        connect(faderWidget, SIGNAL(levelUpdated(int)), this, SLOT(setLimiterValue(int)));
        faderWidget->setFaderLevel(100 * geonkickApi->limiterValue());
        hBoxLayout->addWidget(faderWidget, 0, Qt::AlignTop);
        hBoxLayout->addSpacing(5);
        mainLayout->addLayout(hBoxLayout);

        // Create control area.
        ControlArea *controlAreaWidget = new ControlArea(this, geonkickApi, oscillators);
        mainLayout->addWidget(controlAreaWidget);

        return true;
}

void MainWindow::openExportDialog()
{
        ExportWidget exportDialog(this, geonkickApi);
        exportDialog.exec();
}

void MainWindow::setLimiterValue(int value)
{
        GEONKICK_LOG_DEBUG("value: " << value);
        geonkickApi->setLimiterValue(static_cast<double>(value) / 100);
}

void MainWindow::paintWidget(QPaintEvent *event)
{
        QPainter painter(this);
        painter.drawRect(0, 0, width() - 1, height() - 1);
}
