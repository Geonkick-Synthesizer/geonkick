/**
 * File name: MainWindow.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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
#include "kit_model.h"

class Oscillator;
class GeonkickApi;
class TopBar;
class EnvelopeWidget;
class ControlArea;
class Limiter;

class MainWindow : public GeonkickWidget
{
 public:
      explicit MainWindow(RkMain* app,
                          GeonkickApi *api,
                          const std::string &preset = std::string());
      explicit MainWindow(RkMain* app,
                          GeonkickApi *api,
                          const RkNativeWindowInfo &info);
      ~MainWindow();
      bool init(void);
      RK_DECL_ACT(onScaleFactor, onScaleFactor(double factor), RK_ARG_TYPE(double), RK_ARG_VAL(factor));
      static RkRect getWindowSize();

 protected:
      void shortcutEvent(RkKeyEvent *event) override;
      void dropEvent(RkDropEvent *event) override;
      void openFileDialog(FileDialog::Type type);
      void openPreset(const std::string &fileName);
      void savePreset(const std::string &fileName);
      void setPreset(const std::string &fileName);
#ifndef  GEONKICK_OS_WINDOWS
      void openExportDialog();
#endif // GEONKICK_OS_WINDOWS
      void openPreset();
      void resetToDefault();
      void updateLimiter(KitModel::PercussionIndex index);
      RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());
      void setSample(const std::string &file);

 private:
      void createViewState();
      void createShortcuts();
      GeonkickApi *geonkickApi;
      std::vector<std::unique_ptr<Oscillator>> oscillators;
      TopBar *topBar;
      EnvelopeWidget* envelopeWidget;
      ControlArea* controlAreaWidget;
      std::string presetName;
      std::string currentWorkingPath;
      Limiter *limiterWidget;
      KitModel *kitModel;
};

#endif // GEONKICK_MAINWINDOW_H
