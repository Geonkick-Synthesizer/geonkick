/**
 * File name: top_bar.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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

#ifndef GEONKICK_TOP_BAR_H
#define GEONKICK_TOP_BAR_H

#include "geonkick_widget.h"
#include "geonkick_api.h"

class RkLabel;
class GeonkickButton;
class RkContainer;
class ViewState;
class GeonkickModel;
class PresetNavigator;
class RkSpinBox;
class RkLineEdit;

class TopBar : public GeonkickWidget
{
 public:
        TopBar(GeonkickWidget *parent, GeonkickModel *model);
        void setPresetName(const std::string &name);
        RK_DECL_ACT(openAbout, openAbout(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(resetToDefault, resetToDefault(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(layerSelected,
                    layerSelected(GeonkickApi::Layer layer, bool b),
                    RK_ARG_TYPE(GeonkickApi::Layer, bool),
                    RK_ARG_VAL(layer, b));
        void updateGui();

 protected:
        RkWidget* createInstrumentNameLabel();
        void showMidiPopup();
        void showSettings();

 private:
#ifndef GEONKICK_LIMITED_VERSION
        void createLyersButtons(RkContainer *mainLayout);
#endif // GEONKICK_LIMITED_VERSION
        void addSeparator(RkContainer *mainLayout, int width = 5);
        GeonkickModel *geonkickModel;
        GeonkickButton *saveFileButton;
        GeonkickButton *exportFileButton;
        PresetNavigator* presetNavigator;
#ifndef GEONKICK_LIMITED_VERSION
        GeonkickButton *layer1Button;
        GeonkickButton *layer2Button;
        GeonkickButton *layer3Button;
#endif // GEONKICK_LIMITED_VERSION
        GeonkickButton *tuneCheckbox;
        RkLineEdit *instrumentName;
        GeonkickButton *controlsButton;
        GeonkickButton *midiKeyButton;
        RkSpinBox *midiChannelSpinBox;
        GeonkickButton *noteOffButton;
#ifndef GEONKICK_SINGLE
        GeonkickButton *kitButton;
#endif // GEONKICK_SINGLE
        GeonkickButton *presetsButton;
        GeonkickButton *samplesButton;
        GeonkickButton *settingsButton;
};

#endif // GEONKICK_TOP_BAR_H
