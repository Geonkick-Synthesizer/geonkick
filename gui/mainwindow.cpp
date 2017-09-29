/**
 * File name: mainwindow.cpp
 * Project: MashaFile (desktop client)
 *
 * Copyright (C) 2015 Iurie Nistor (http://sv-ti.com)
 *
 * This file is part of MashaFile.
 *
 * MashaFile is free software; you can redistribute it and/or modify
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
#include "gkick_knob.h"
#include "control_area.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow() :
        gkickApi(std::make_unique<GKickApi>()),
        oscillatorWidgetLabel(NULL),
        oscillatorWidget(NULL),
        centralWidgetLayout(NULL),
        amplitudeRb(NULL),
        frequencyRb(NULL),
        waveFunctionCb(NULL)
{
}

bool MainWindow::init(void)
{
        if (gkickApi.get()->hasErrors()) {
                return false;
        }

        gkickApi.get()->setKickLength(0.25);
        oscillators = gkickApi.get()->getOscillators();
        oscillators[GKickOscillator::OSC_1].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
        oscillators[GKickOscillator::OSC_2].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
        oscillators[GKickOscillator::OSC_NOISE].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
        oscillators[GKickOscillator::OSC_1].get()->setOscAmplitudeValue(1);
        oscillators[GKickOscillator::OSC_1].get()->setOscFrequencyValue(10000);
        oscillators[GKickOscillator::OSC_2].get()->setOscAmplitudeValue(0);
        oscillators[GKickOscillator::OSC_2].get()->setOscFrequencyValue(10000);
        oscillators[GKickOscillator::OSC_NOISE].get()->setOscAmplitudeValue(1);

        // Create central Widget.
        setCentralWidget(new QWidget(this));
        centralWidgetLayout = new QVBoxLayout();
        centralWidget()->setLayout(centralWidgetLayout);

        // Create and envelope widget to the central widget layout.
        oscillatorWidget = new OscillatorWidget(centralWidget(), oscillators[MainWindow::OSC_1].get());
        oscillatorWidget->setAmplitudeEnvelope();
        connect(gkickApi.get(), SIGNAL(kickLengthUpdated(double)), oscillatorWidget, SLOT(updateKickLength(double)));
        centralWidgetLayout->addWidget(oscillatorWidget);

        // Create and control area to central layout.
        QWidget *controlAreaWidget = new ControlArea(centralWidget());
        centralWidgetLayout->addWidget(controlAreaWidget);
        centralWidgetLayout->setStretchFactor(controlAreaWidget, 1);
        centralWidgetLayout->setStretchFactor(oscillatorWidget, 2);

        return true;
}

MainWindow::~MainWindow()
{
}

void MainWindow::createEnvelopesGroupBox(QWidget *controlAreaWidget)
{
        QGroupBox *oscillatorsGroupBox = new QGroupBox(tr("Envelopes"), controlAreaWidget);
        controlAreaWidget->layout()->addWidget(oscillatorsGroupBox);
        oscillatorsGroupBox->setLayout(new QVBoxLayout());
        QRadioButton *osc1Rb    = new QRadioButton(tr("OSC 1"), oscillatorsGroupBox);
        QRadioButton *osc2Rb    = new QRadioButton(tr("OSC 2"), oscillatorsGroupBox);
        QRadioButton *noiseRb   = new QRadioButton(tr("Noise"), oscillatorsGroupBox);
        QRadioButton *generalRb = new QRadioButton(tr("General"), oscillatorsGroupBox);
        osc1Rb->setChecked(true);
        connect(osc1Rb, SIGNAL(clicked(bool)), this, SLOT(viewOsc1(bool)));
        connect(osc2Rb, SIGNAL(clicked(bool)), this, SLOT(viewOsc2(bool)));
        connect(noiseRb, SIGNAL(clicked(bool)), this, SLOT(viewNoiseOsc(bool)));
        connect(generalRb, SIGNAL(clicked(bool)), this, SLOT(viewGeneral(bool)));
        oscillatorsGroupBox->layout()->addWidget(osc1Rb);
        oscillatorsGroupBox->layout()->addWidget(osc2Rb);
        oscillatorsGroupBox->layout()->addWidget(noiseRb);
        oscillatorsGroupBox->layout()->addWidget(generalRb);
}

void MainWindow::createControlArea(QWidget *controlAreaWidget)
{
        controlAreaWidget->layout()->addWidget(new EnvelopesGroupBox(controlAreaWidget));
        controlAreaWidget->layout()->addWidget(new OscillatorGroupBox(controlAreaWidget, MainWindow::OSC_1));
        controlAreaWidget->layout()->addWidget(new OscillatorGroupBox(controlAreaWidget, MainWindow::OSC_2));
        controlAreaWidget->layout()->addWidget(new NoiseGroupBox(controlAreaWidget));
        controlAreaWidget->layout()->addWidget(new GenralSettingGroupBox(controlAreaWidget));
        /*        QString name = (type == MainWindow::OSC_1) ? tr("OSC 1") : tr("OSC 2");
        QGroupBox *oscillatorGroupBox = new QGroupBox(name, controlAreaWidget);
        controlAreaWidget->layout()->addWidget(oscillatorGroupBox);
        QGridLayout *oscillatorGroupBoxLayout = new QGridLayout();
        oscillatorGroupBox->setLayout(oscillatorGroupBoxLayout);
        controlAreaWidget->layout()->addWidget(oscillatorGroupBox);

        // Wave function.
        waveFunctionCb = new QComboBox(oscillatorGroupBox);
        waveFunctionCb->addItem(tr("Sine"));
        waveFunctionCb->addItem(tr("Sqare"));
        waveFunctionCb->addItem(tr("Triangle"));
        waveFunctionCb->addItem(tr("Sawtooth"));
        waveFunctionCb->addItem(tr("Noise"));
        connect(waveFunctionCb, SIGNAL(currentIndexChanged(int)),
                this, SLOT(oscillatorFunctionChanged(int)));
        oscillatorGroupBoxLayout->addWidget(new QLabel(tr("Wave function")), 0, 0);
        oscillatorGroupBoxLayout->addWidget(waveFunctionCb, 0, 1);

       // Select enevelope.
        envelopeGroupBox << new QGroupBox(tr("Envelope"), oscillatorGroupBox);
        envelopeGroupBox[type]->setLayout(new QHBoxLayout());
        amplitudeRb << = new QRadioButton(tr("Amplitude"), envelopeGroupBox);
        frequencyRb  = new QRadioButton(tr("Frquency"), envelopeGroupBox);
        amplitudeRb->setChecked(true);
        connect(amplitudeRb, SIGNAL(clicked(bool)), this, SLOT(setAmplitudeEnvelope(bool)));
        connect(frequencyRb, SIGNAL(clicked(bool)), this, SLOT(setFrequencyEnvelope(bool)));
        envelopeGroupBox->layout()->addWidget(amplitudeRb);
        envelopeGroupBox->layout()->addWidget(frequencyRb);
        oscillatorGroupBoxLayout->addWidget(envelopeGroupBox, 1, 0);

        GKickKnob *kickAmplitudeKnob = new GKickKnob(oscillatorGroupBox);
        kickAmplitudeKnob->setValue(oscillators[type].get()->getOscAmplitudeValue());
        oscillatorGroupBoxLayout->addWidget(kickAmplitudeKnob, 2, 0);

        GKickKnob *kickFrequencyKnob = new GKickKnob(oscillatorGroupBox);
        kickFrequencyKnob->setValue(oscillators[type].get()->getOscFrequencyValue());
        oscillatorGroupBoxLayout->addWidget(kickFrequencyKnob, 2, 1);

        connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillators[type].get(), SLOT(setOscAmplitudeValue(double)));
        connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)),
                oscillators[type].get(), SLOT(setOscFrequencyValue(double)));

        if (type == MainWindow::OSC_2) {
                kickAmplitudeKnob->setPosition(0);
        } else {
                kickAmplitudeKnob->setPosition(0.25);
        }
        kickFrequencyKnob->setPosition(0.25);*/
}

void MainWindow::createNoiseBox(QWidget *controlAreaWidget)
{
        QGroupBox *noiseGroupBox = new QGroupBox(tr("Noise"), controlAreaWidget);
        QGridLayout *noiseOscillatorGroupBoxLayout = new QGridLayout();
        noiseGroupBox->setLayout(noiseOscillatorGroupBoxLayout);
        controlAreaWidget->layout()->addWidget(noiseGroupBox);

        GKickKnob *noiseAmplitudeKnob = new GKickKnob(noiseGroupBox);
        noiseAmplitudeKnob->setValue(0.25);
        noiseOscillatorGroupBoxLayout->addWidget(noiseAmplitudeKnob, 0, 0);
        connect(noiseAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillators[MainWindow::OSC_NOISE].get(),
                SLOT(setOscAmplitudeValue(double)));
        noiseAmplitudeKnob->setPosition(0.25);
}

void MainWindow::createGeneralSettingsBox(QWidget *controlAreaWidget)
{
        QGroupBox *generalSettingsGroupBox = new QGroupBox(tr("General Settings"), controlAreaWidget);
        controlAreaWidget->layout()->addWidget(generalSettingsGroupBox);
        QGridLayout *generalSettingsGroupBoxLayout = new QGridLayout();
        generalSettingsGroupBox->setLayout(generalSettingsGroupBoxLayout);
        GKickKnob *kickLengthKnob = new GKickKnob(NULL);
        kickLengthKnob->setValue(1.0);
        connect(kickLengthKnob, SIGNAL(valueUpdated(double)), this, SLOT(setKickLength(double)));
        generalSettingsGroupBoxLayout->addWidget(kickLengthKnob, 0, 0);
        kickLengthKnob->setPosition(0.25);
}

void
MainWindow::closeEvent(QCloseEvent *event)
{
}

void MainWindow::viewOsc1(bool b)
{
        viewOsc(b, MainWindow::OSC_1);
}

void MainWindow::viewOsc2(bool b)
{
        viewOsc(b, MainWindow::OSC_2);
}

void MainWindow::viewOsc(bool b, MainWindow::OscillatorType type)
{
        if (b) {
                oscillatorWidget->setOscillator(oscillators[type].get());
                if(frequencyRb->isChecked()) {
                        oscillatorWidget->setFrequencyEnvelope();
                } else {
                        oscillatorWidget->setAmplitudeEnvelope();
                }
        }
}

void MainWindow::viewNoiseOsc(bool b)
{
        if (b) {
                oscillatorWidget->setOscillator(oscillators[MainWindow::OSC_NOISE].get());
                oscillatorWidget->setAmplitudeEnvelope();
        }
}

void MainWindow::viewGeneral(bool b)
{
}

void MainWindow::setAmplitudeEnvelope(bool b)
{
        if (b) {
                oscillatorWidget->setAmplitudeEnvelope();
        }
}

void MainWindow::setFrequencyEnvelope(bool b)
{
        if (b) {
                oscillatorWidget->setFrequencyEnvelope();
        }
}

void MainWindow::setKickLength(double v)
{
        gkickApi->setKickLength(GKICK_UI_MAX_TIME * v);
}

void MainWindow::oscillatorFunctionChanged(int index, MainWindow::OscillatorType type)
{
	QString text = waveFunctionCb->itemText(index);
	if (text == "Sine") {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	} else if (text == "Sqare") {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_SQARE);
	} else if(text == "Triangle") {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_TRIANGLE);
	} else if (text == "Sawtooth") {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_SAWTOOTH);
	} else if (text == "Noise") {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
	} else {
		oscillators[type].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	}
}
