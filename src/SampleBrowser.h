/**
 * File name: SampleBrowser.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#ifndef GEONKICK_SAMPLE_BROWSER_H
#define GEONKICK_SAMPLE_BROWSER_H

#include "geonkick_widget.h"
#include "geonkick_api.h"

class FileBrowser;
class BufferView;
class GeonkickButton;
class RkContainer;
class GeonkickSlider;
class KitModel;
class RkSpinBox;

class SampleBrowser: public GeonkickWidget {
 public:
        SampleBrowser(GeonkickWidget *parent, KitModel* model);

  protected:
        void createBottomMenu(RkContainer* container);
        void setOscillator(GeonkickApi::OscillatorType osc);
        void loadSample(const fs::path &file);
        void doExport(const fs::path &filePath) const;

 private:
        KitModel *kitModel;
        FileBrowser *fileBrowser;
        GeonkickButton *osc1Button;
        GeonkickButton *osc2Button;
        GeonkickButton *osc3Button;
        RkSpinBox *exportFormatSpinBox;
};

#endif // GEONKICK_SAMPLE_BROWSER_H
