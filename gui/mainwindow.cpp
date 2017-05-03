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

#include <QFileDialog>
#include <QPushButton>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow() :
  gkickApi(std::make_unique<GKickApi>()),
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
  oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
  oscillators[MainWindow::OSC_NOISE].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
  oscillators[MainWindow::OSC_BASE].get()->setOscAmplitudeValue(1);
  oscillators[MainWindow::OSC_BASE].get()->setOscFrequencyValue(10000);
  oscillators[MainWindow::OSC_NOISE].get()->setOscAmplitudeValue(1);

  // Create central Widget.
  setCentralWidget(new QWidget(this));
  oscillatorWidget = new OscillatorWidget(centralWidget(), oscillators[MainWindow::OSC_BASE].get());
  oscillatorWidget->setAmplitudeEnvelope();
  connect(gkickApi.get(), SIGNAL(kickLengthUpdated(double)), oscillatorWidget, SLOT(updateKickLength(double)));

  centralWidgetLayout = new QVBoxLayout();
  
  centralWidgetLayout->addWidget(oscillatorWidget);
  centralWidget()->setLayout(centralWidgetLayout);

  createBottomControlArea();
  
  return true;
}

MainWindow::~MainWindow()
{
  //if (uploadButton != NULL) {
  //  delete uploadButton;
  //}
}

void MainWindow::createBottomControlArea(void)
{
  QHBoxLayout* controlAreaLayout = new QHBoxLayout;
  QWidget *controlAreaWidget = new QWidget(centralWidget());
  controlAreaWidget->setMaximumHeight(200);
  controlAreaWidget->setLayout(controlAreaLayout);

  // Add control area to central layout.
  centralWidgetLayout->addWidget(controlAreaWidget);
  centralWidgetLayout->setStretchFactor(controlAreaWidget, 1);
  centralWidgetLayout->setStretchFactor(oscillatorWidget, 2);

  createGroupOscillatorsBox(controlAreaWidget);
  createBaseOscillatorBox(controlAreaWidget);
  createNoiseBox(controlAreaWidget);
  createGeneralSettingsBox(controlAreaWidget);
}


void MainWindow::createGroupOscillatorsBox(QWidget *controlAreaWidget)
{
  QGroupBox *oscillatorsGroupBox = new QGroupBox(tr("Oscillators"), controlAreaWidget);
  controlAreaWidget->layout()->addWidget(oscillatorsGroupBox);
  oscillatorsGroupBox->setLayout(new QVBoxLayout());
  QRadioButton *baseRb  = new QRadioButton(tr("Base Oscillator"), oscillatorsGroupBox);
  QRadioButton *noiseRb = new QRadioButton(tr("Noise"), oscillatorsGroupBox);
  baseRb->setChecked(true);
  connect(baseRb, SIGNAL(clicked(bool)), this, SLOT(viewBaseOsc(bool)));
  connect(noiseRb, SIGNAL(clicked(bool)), this, SLOT(viewNoiseOsc(bool)));
  oscillatorsGroupBox->layout()->addWidget(baseRb);
  oscillatorsGroupBox->layout()->addWidget(noiseRb);
}

void MainWindow::createBaseOscillatorBox(QWidget *controlAreaWidget)
{
  QGroupBox *baseOscillatorGroupBox = new QGroupBox(tr("Base Oscillator"), controlAreaWidget);
  controlAreaWidget->layout()->addWidget(baseOscillatorGroupBox);
  QGridLayout *baseOscillatorGroupBoxLayout = new QGridLayout();
  baseOscillatorGroupBox->setLayout(baseOscillatorGroupBoxLayout);

  // Wave function.
  waveFunctionCb = new QComboBox(baseOscillatorGroupBox);
  waveFunctionCb->addItem("Sine");
  waveFunctionCb->addItem("Sqare");
  waveFunctionCb->addItem("Triangle");
  waveFunctionCb->addItem("Sawtooth");
  waveFunctionCb->addItem("Noise");
  connect(waveFunctionCb, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(oscillatorFunctionChanged(int)));
  baseOscillatorGroupBoxLayout->addWidget(new QLabel(tr("Wave function")), 0, 0);
  baseOscillatorGroupBoxLayout->addWidget(waveFunctionCb, 0, 1);

  // Select enevelope.
  envelopeGroupBox = new QGroupBox(tr("Envelope"), baseOscillatorGroupBox);
  baseOscillatorGroupBoxLayout->addWidget(envelopeGroupBox, 1, 0);
  envelopeGroupBox->setLayout(new QHBoxLayout());
  amplitudeRb  = new QRadioButton(tr("Amplitude"), envelopeGroupBox);
  frequencyRb  = new QRadioButton(tr("Frquency"), envelopeGroupBox);
  amplitudeRb->setChecked(true);
  connect(amplitudeRb, SIGNAL(clicked(bool)), this, SLOT(setAmplitudeEnvelope(bool)));
  connect(frequencyRb, SIGNAL(clicked(bool)), this, SLOT(setFrequencyEnvelope(bool)));
  envelopeGroupBox->layout()->addWidget(amplitudeRb);
  envelopeGroupBox->layout()->addWidget(frequencyRb);

  
  GKickKnob *kickAmplitudeKnob = new GKickKnob(baseOscillatorGroupBox);
  kickAmplitudeKnob->setValue(oscillators[MainWindow::OSC_BASE].get()->getOscAmplitudeValue());
  baseOscillatorGroupBoxLayout->addWidget(kickAmplitudeKnob, 2, 0);
  
  GKickKnob *kickFrequencyKnob = new GKickKnob(baseOscillatorGroupBox);
  kickFrequencyKnob->setValue(oscillators[MainWindow::OSC_BASE].get()->getOscFrequencyValue());
  baseOscillatorGroupBoxLayout->addWidget(kickFrequencyKnob, 2, 1);  
 
  connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
	  oscillators[MainWindow::OSC_BASE].get(), SLOT(setOscAmplitudeValue(double)));
  connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)),
	  oscillators[MainWindow::OSC_BASE].get(), SLOT(setOscFrequencyValue(double)));
  
  kickAmplitudeKnob->setPosition(0.25);
  kickFrequencyKnob->setPosition(0.25);
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
  //  event->ignore();
  //this->setVisible(false);
}

void MainWindow::viewBaseOsc(bool b)
{
  if (b) {
	  oscillatorWidget->setOscillator(oscillators[MainWindow::OSC_BASE].get());
	  if(frequencyRb->isChecked()) {
		  oscillatorWidget->setFrequencyEnvelope();
	  } else {
		  oscillatorWidget->setAmplitudeEnvelope();  
	  }
	  envelopeGroupBox->setEnabled(true);
  }
}

void MainWindow::viewNoiseOsc(bool b)
{
  if (b) {
    oscillatorWidget->setOscillator(oscillators[MainWindow::OSC_NOISE].get());
    oscillatorWidget->setAmplitudeEnvelope();		  
    envelopeGroupBox->setEnabled(false);
  }
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
  qDebug() << "MainWindow::setKickLength: " << v;
  gkickApi->setKickLength(GKICK_UI_MAX_TIME * v);
  //  oscillatorWidget->setKickLength(GKICK_UI_MAX_TIME * v);
}

void MainWindow::oscillatorFunctionChanged(int index)
{
	QString text = waveFunctionCb->itemText(index);
	if (text == "Sine") {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	} else if (text == "Sqare") {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SQARE);
	} else if(text == "Triangle") {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_TRIANGLE);
	} else if (text == "Sawtooth") {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SAWTOOTH);
	} else if (text == "Noise") {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
	} else {
		oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
	}
}
