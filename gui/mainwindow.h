/**
 * File name: mainwindow.h
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
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

#ifndef GKICK_MAINWINDOW_H
#define GKICK_MAINWINDOW_H

#include "gkickapi.h"
#include "gkickoscillator.h"
#include "oscillator_widget.h"

#include <QMainWindow>
//#include "ui_MainWindow.h"

#define GKICK_UI_MAX_TIME (1.0)

class QPushButton;
class QCloseEvent;
class QVBoxLayout;
class QGroupBox;
class QRadioButton;
class QComboBox;
class QLabel;

class MainWindow : public QMainWindow
{
      Q_OBJECT

 public:
      	enum OscillatorType {
	  OSC_1 = 0,
          OSC_2 = 1,
	  OSC_NOISE = 2,
	};

      MainWindow(void);
      ~MainWindow();
      bool init(void);

      protected slots:

 signals:
	void quit(void);

 protected:
	void closeEvent(QCloseEvent *event);
	bool setupMenuBar(void);
	bool setupToolBar(void);
        void createBottomControlArea(void);
        void createEnvelopesGroupBox(QWidget *controlAreaWidget);
	void createOscillatorBox(QWidget *controlAreaWidget, MainWindow::OscillatorType type);
	void createNoiseBox(QWidget *controlAreaWidget);
	void createGeneralSettingsBox(QWidget *controlAreaWidget);

 protected slots:

        void viewOsc(bool b, MainWindow::OscillatorType type);
        void viewOsc1(bool b);
        void viewOsc2(bool b);
        void viewNoiseOsc(bool b);
        void viewGeneral(bool b);
	void setAmplitudeEnvelope(bool b);
	void setFrequencyEnvelope(bool b);
	void setKickLength(double v);
	void oscillatorFunctionChanged(int index, MainWindow::OscillatorType type);

 private:
	//	QPushButton *uploadButton;
	std::unique_ptr<GKickApi> gkickApi;
	std::vector<std::unique_ptr<GKickOscillator>> oscillators;
      	QLabel *oscillatorWidgetLabel;
	OscillatorWidget *oscillatorWidget;
	QVBoxLayout *centralWidgetLayout;
	QGroupBox *envelopeGroupBox;
	QRadioButton *amplitudeRb;
	QRadioButton *frequencyRb;
	QComboBox *waveFunctionCb;
};

#endif
