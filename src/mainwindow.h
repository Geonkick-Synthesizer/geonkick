/**
 * File name: mainwindow.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of Geonkick.
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

class Oscillator;
//class OscillatorWidget;
class GeonkickApi;
class TopBar;
class EnvelopeWidget;

class MainWindow : public GeonkickWidget
{
 public:
        explicit MainWindow(GeonkickApi *api, GeonkickWidget *parent = nullptr);
        explicit MainWindow(GeonkickApi *api, const RkNativeWindowInfo &info);
      ~MainWindow();
      bool init(void);

 protected:
      // void keyPressEvent(QKeyEvent *event);
      // void keyReleaseEvent(QKeyEvent *event);
      ///void setPreset(const QString &fileName);

      // protected slots:
         //      void openExportDialog();
         //      void openPreset();
         //      void savePreset();
         //      void openAboutDialog();
         // signals:
         //      void updateGui();

 private:
        //      void loadApiDefaultSettings();
        //      void loadApiSettings();

      GeonkickApi *geonkickApi;
      std::vector<std::unique_ptr<Oscillator>> oscillators;
      TopBar *topBar;
      EnvelopeWidget* envelopeWidget;
      std::string presetName;
};

#endif // GEONKICK_MAINWINDOW_H
