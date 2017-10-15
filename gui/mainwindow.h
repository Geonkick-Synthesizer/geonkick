/**
 * File name: mainwindow.h
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
 *
 * This file is part of GeonKick.
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
#include "gkick_oscillator.h"
#include "gkick_envelope_widget.h"

#include <QMainWindow>

class OscillatorWidget;

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
        void createControlArea(QWidget *controlAreaWidget);

 private:
	std::shared_ptr<GKickApi> gkickApi;
	std::vector<std::shared_ptr<GKickOscillator>> oscillators;
      	QLabel *oscillatorWidgetLabel;
	OscillatorWidget *oscillatorWidget;
	QVBoxLayout *centralWidgetLayout;
	QVector<QGroupBox*> envelopeGroupBox;
	QVector<QRadioButton*> amplitudeRb;
	QVector<QRadioButton*> frequencyRb;
	QVector<QComboBox*> waveFunctionCb;
};

#endif
