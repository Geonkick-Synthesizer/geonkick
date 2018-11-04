/**
 * File name: geonkick_mainwindow.h
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

#ifndef GEONGKICK_MAINWINDOW_H
#define GEONGKICK_MAINWINDOW_H

#include "geonkick_widget.h"
#include "oscillator.h"
#include "envelope_widget.h"

#include <QMainWindow>

class OscillatorWidget;

class QPushButton;
class QCloseEvent;
class QVBoxLayout;
class QGroupBox;
class QRadioButton;
class QComboBox;
class QLabel;
class GeonkickApi;

class MainWindow : public GeonkickWidget
{
      Q_OBJECT

 public:

      MainWindow(GeonkickApi *api = nullptr, GeonkickWidget *parent = nullptr);
      ~MainWindow();
      bool init(void);

 public slots:
      void openExportDialog();
      void setLimiterValue(int value);

 private:
      void loadApiDefaultSettings();
      void loadApiSettings();

      GeonkickApi *geonkickApi;
      std::vector<Oscillator*> oscillators;
      OscillatorWidget *oscillatorWidget;
      QVBoxLayout *centralWidgetLayout;
      QVector<QGroupBox*> envelopeGroupBox;
};

#endif // GEONKICK_MAINWINDOW_H
