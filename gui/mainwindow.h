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

class MainWindow : public QMainWindow/*, public Ui_MainWindow*/
{
      Q_OBJECT

 public:
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
	void createGroupOscillatorsBox(QWidget *controlAreaWidget);
	void createBaseOscillatorBox(QWidget *controlAreaWidget);
	void createNoiseBox(QWidget *controlAreaWidget);
	void createGeneralSettingsBox(QWidget *controlAreaWidget);

 protected slots:
        void viewBaseOsc(bool b);
        void viewNoiseOsc(bool b);
	void setAmplitudeEnvelope(bool b);
	void setFrequencyEnvelope(bool b);
	void setKickLength(double v);
 private:
	enum {
	  OSC_BASE = 0,
	  OSC_NOISE = 1,
	};

	//	QPushButton *uploadButton;
	std::unique_ptr<GKickApi> gkickApi;
	std::vector<std::unique_ptr<GKickOscillator>> oscillators;
	OscillatorWidget *oscillatorWidget;
	QVBoxLayout *centralWidgetLayout;
	QGroupBox *envelopeGroupBox;
};

#endif
