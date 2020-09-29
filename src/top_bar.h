/**
 * File name: top_bar.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://iuriepage.wordpress.com>
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

class TopBar : public GeonkickWidget
{
 public:
        TopBar(GeonkickWidget *parent, GeonkickApi *api);
        void setPresetName(const std::string &name);
        RK_DECL_ACT(saveFile, saveFile(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(openFile, openFile(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(openAbout, openAbout(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(resetToDefault, resetToDefault(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(openExport, openExport(), RK_ARG_TYPE(), RK_ARG_VAL());
        RK_DECL_ACT(layerSelected,
                    layerSelected(GeonkickApi::Layer layer, bool b),
                    RK_ARG_TYPE(GeonkickApi::Layer, bool),
                    RK_ARG_VAL(layer, b));
        void updateGui();

 private:
        void createLyersButtons(RkContainer *mainLayout);
        void addSeparator(RkContainer *mainLayout, int width = 5);
        GeonkickButton *openFileButton;
        GeonkickButton *saveFileButton;
        GeonkickButton *exportFileButton;
        RkLabel* presetNameLabel;
        GeonkickButton *layer1Button;
        GeonkickButton *layer2Button;
        GeonkickButton *layer3Button;
        GeonkickApi *geonkickApi;
        GeonkickButton *tuneCheckbox;
        GeonkickButton *controlsButton;
        GeonkickButton *kitButton;
        GeonkickButton *presetsButton;
        GeonkickButton *samplesButton;
};

#endif // GEONKICK_TOP_BAR_H
