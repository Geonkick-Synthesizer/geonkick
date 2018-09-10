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
#include "gkick_envelope_widget.h"
#include "envelopes_group_box.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "control_area.h"
#include "geonkick_theme.h"
#include "top_bar.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

#define GEONKICK_MAINWINDOW_WIDTH  1000
#define GEONKICK_MAINWINDOW_HEIGHT 750

MainWindow::MainWindow(GeonkickWidget *parent) :
        GeonkickWidget(parent),
        gkickApi(std::make_shared<GKickApi>())
{
        QPalette pal;
        pal.setColor(QPalette::Background, QColor(68, 68, 70));
        setAutoFillBackground(true);
        setPalette(pal);
        //        setFixedSize(GEONKICK_MAINWINDOW_WIDTH, GEONKICK_MAINWINDOW_HEIGHT);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::init(void)
{
        if (gkickApi.get()->hasErrors()) {
	  //return false;
        }

        setTheme(new GeonkickTheme("Geontime"));

        gkickApi->setMaxLength(1.5);
        gkickApi->setKickLength(0.25);
        gkickApi->setAmplitude(1);
        gkickApi->setKickFilterFrequency(0);
        oscillators = gkickApi.get()->getOscillators();
	oscillators[GKickOscillator::OSC_1].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	oscillators[GKickOscillator::OSC_2].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	oscillators[GKickOscillator::OSC_NOISE].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
	oscillators[GKickOscillator::OSC_1].get()->setOscAmplitudeValue(1);
	oscillators[GKickOscillator::OSC_1].get()->setOscFrequencyValue(10000);
	oscillators[GKickOscillator::OSC_1].get()->setFilterType(GKickOscillator::FILTER_LP);
	oscillators[GKickOscillator::OSC_1].get()->setFilterFrequency(0);
	oscillators[GKickOscillator::OSC_2].get()->setOscAmplitudeValue(0);
	oscillators[GKickOscillator::OSC_2].get()->setOscFrequencyValue(10000);
	oscillators[GKickOscillator::OSC_2].get()->setFilterType(GKickOscillator::FILTER_LP);
	oscillators[GKickOscillator::OSC_2].get()->setFilterFrequency(0);
	oscillators[GKickOscillator::OSC_NOISE].get()->setOscAmplitudeValue(1);
	oscillators[GKickOscillator::OSC_NOISE].get()->setFilterType(GKickOscillator::FILTER_LP);
	oscillators[GKickOscillator::OSC_NOISE].get()->setFilterFrequency(0);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        setLayout(mainLayout);
        setContentsMargins(0, 0, 0, 0);

        mainLayout->addWidget(new TopBar(this));

        // Create envelope widget.
        GKickEnvelopeWidget* envelopeWidget = new GKickEnvelopeWidget(this, gkickApi, oscillators);
        mainLayout->addWidget(envelopeWidget);

        // Create control area.
        ControlArea *controlAreaWidget = new ControlArea(this, gkickApi, oscillators);
        mainLayout->addWidget(controlAreaWidget);

        //connect(controlAreaWidget, SIGNAL(viewEnvelope(GKickEnvelope::EnvelopeCategory)),
        //        envelopeWidget, SLOT(viewEnvelope(GKickEnvelope::EnvelopeCategory)));

        return true;
}
