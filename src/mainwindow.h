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
#include "file_dialog.h"

class Oscillator;
class GeonkickApi;
class TopBar;
class EnvelopeWidget;
class ControlArea;
class RkKeyEvent;

class MainWindow : public GeonkickWidget
{
 public:
      explicit MainWindow(RkMain* app, GeonkickApi *api, std::string preset = std::string());
      explicit MainWindow(RkMain* app, GeonkickApi *api, const RkNativeWindowInfo &info);
      ~MainWindow();
      bool init(void);

 protected:
      void keyPressEvent(const std::shared_ptr<RkKeyEvent> &event) final;
      void keyReleaseEvent(const std::shared_ptr<RkKeyEvent> &event) final;

      void openFileDialog(FileDialog::Type type);
      void openPreset(const std::string &fileName);
      void savePreset(const std::string &fileName);
      void setPreset(const std::string &fileName);
      void openExportDialog();
      void openPreset();
      void openAboutDialog();
      RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());

 private:
      GeonkickApi *geonkickApi;
      std::vector<std::unique_ptr<Oscillator>> oscillators;
      TopBar *topBar;
      EnvelopeWidget* envelopeWidget;
      ControlArea* controlAreaWidget;
      std::string presetName;
      std::string currentWorkingPath;
};

#endif // GEONKICK_MAINWINDOW_H
