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

#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow() :
        gkickApi(std::make_shared<GKickApi>())
{
}

MainWindow::~MainWindow()
{
}

bool MainWindow::init(void)
{
        if (gkickApi.get()->hasErrors()) {
                return false;
        }

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

        // Create central Widget.
        setCentralWidget(new QWidget(this));
        centralWidgetLayout = new QVBoxLayout(centralWidget());
        centralWidget()->setLayout(centralWidgetLayout);

        // Create  envelope widget.
        GKickEnvelopeWidget* envelopeWidget = new GKickEnvelopeWidget(centralWidget(), gkickApi, oscillators);
        centralWidgetLayout->addWidget(envelopeWidget);

        // Create control area.
        ControlArea *controlAreaWidget = new ControlArea(centralWidget(), gkickApi, oscillators);
        centralWidgetLayout->addWidget(controlAreaWidget);
        centralWidgetLayout->setStretchFactor(controlAreaWidget, 0);
        centralWidgetLayout->setStretchFactor(envelopeWidget, 2);

        return true;
}

void MainWindow::createControlArea(QWidget *controlAreaWidget)
{
        /*        envelopeGroupBox = new EnvelopesGroupBox(controlAreaWidget);
        connect(envelopeGroupBox, SIGNAL(viewEnvelope(GKickEnvelopeWidget::EnvelopeType)),
                envelopeWidget, SLOT(viewEnvelope(GKickEnvelopeWidget::EnvelopeType)));

        controlAreaWidget->layout()->addWidget(envelopeGroupBox);
        controlAreaWidget->layout()->addWidget(new OscillatorGroupBox(controlAreaWidget,
                                                                      oscillators[GkickOscillator::OSC_1]));
        controlAreaWidget->layout()->addWidget(new OscillatorGroupBox(controlAreaWidget,
                                                                      oscillators[GkickOscillator::OSC_2]));
        controlAreaWidget->layout()->addWidget(new NoiseGroupBox(controlAreaWidget,
                                                                 oscillators[GkickOscillator::OSC_NOISE]));
                                                                 controlAreaWidget->layout()->addWidget(new GenralSettingGroupBox(controlAreaWidget, gkickApi));*/
}
