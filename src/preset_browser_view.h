/**
 * File name: preset_browser_view.h
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

#ifndef PRESET_BROWSER_H
#define PRESET_BROWSER_H

#include "globals.h"
#include "geonkick_widget.h"

class PresetBrowserModel;
class RkButton;
class RkContainer;

class PresetBrowserView: public GeonkickWidget {
 public:
        PresetBrowserView(GeonkickWidget *parent, PresetBrowserModel* model);

 protected:
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseMoveEvent(RkMouseEvent *event) override;
        void paintWidget(RkPaintEvent *event) override;
        void updatePageButtons();

 private:
        PresetBrowserModel *browserModel;
        int topPadding;
        int leftPadding;
        int columnWidth;
        int rowHeight;
        int overRow;
        int overColumn;
        RkButton* nextPresetPageButton;
        RkButton* prevPresetPageButton;
        RkButton* nextFolderPageButton;
        RkButton* prevFolderPageButton;
        RkContainer *bottomContainer;
};

#endif // PRESET_BROWSER_H
