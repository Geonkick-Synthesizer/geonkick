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

MainWindow::MainWindow() :
  gkickApi(std::make_unique<GKickApi>()),
  oscillatorWidget(NULL)
{
}

bool MainWindow::init(void)
{
  if (gkickApi.get()->hasErrors()) {
    return false;
  }
  
  setupUi(this);

  oscillators = gkickApi.get()->getOscillators();
  oscillators[MainWindow::OSC_BASE].get()->setOscFunction(GKickOscillator::OSC_FUNC_SINE);
  oscillators[MainWindow::OSC_NOISE].get()->setOscFunction(GKickOscillator::OSC_FUNC_NOISE);
  
  oscillatorWidget = new OscillatorWidget(verticalLayoutWidget, oscillators[MainWindow::OSC_BASE].get());
  viewOscillatorLayout->addWidget(oscillatorWidget);

  GKickKnob *kickLengthKnob = new GKickKnob(NULL);
  connect(kickLengthKnob, SIGNAL(valueUpdated(double)), this, SLOT(setKickLength(double)));
  gkickApi->setKickLength(0.25);
  kickLengthKnob->setValue(0.25);
  genealSettingsLayout->addWidget(kickLengthKnob, 0,0);
  
  knobsLayout->addWidget(new GKickKnob(NULL), 1, 0);
  knobsLayout->addWidget(new GKickKnob(NULL), 1, 1);

  radioButtonBaseOsc->setChecked(true);
  connect(radioButtonBaseOsc, SIGNAL(clicked(bool)), this, SLOT(viewBaseOsc(bool)));
  connect(radioButtonNoiseOsc, SIGNAL(clicked(bool)), this, SLOT(viewNoiseOsc(bool)));

  radioButtonBaseAmplitudeEnv->setChecked(true);
  connect(radioButtonBaseAmplitudeEnv, SIGNAL(clicked(bool)), this, SLOT(setAmplitudeEnvelope(bool)));
  connect(radioButtonBaseFrequencyEnv, SIGNAL(clicked(bool)), this, SLOT(setFrequencyEnvelope(bool)));
  
  return true;
}

MainWindow::~MainWindow()
{
  //if (uploadButton != NULL) {
  //  delete uploadButton;
  //}
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
    baseOscEnvGroupBox->setEnabled(true);
  }
}

void MainWindow::viewNoiseOsc(bool b)
{
  if (b) {
    oscillatorWidget->setOscillator(oscillators[MainWindow::OSC_NOISE].get());
    baseOscEnvGroupBox->setEnabled(false);
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
}
