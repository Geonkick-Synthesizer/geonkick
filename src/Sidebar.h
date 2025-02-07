/**
 * File name: Sidebar.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#ifndef GEONKICK_SIDEBAR_H
#define GEONKICK_SIDEBAR_H

#include "geonkick_widget.h"

class RkPaintEvent;
class RkContainer;
class GeonkickButton;
class GeonkickModel;

class Sidebar: public GeonkickWidget {
 public:
        Sidebar(GeonkickWidget *parent, GeonkickModel *model);
 protected:
        void paintWidget([[maybe_unused]]RkPaintEvent *event) override;
        void createTabButtons(RkContainer *mainLayout);
        void showSidebarWidget();

 private:

        GeonkickModel *geonkickModel;
        RkContainer *mainLayout;
        GeonkickWidget* currentWidget;
        GeonkickButton* presetsButton;
        GeonkickButton* samplesButton;
};

#endif // GEONKICK_SIDEBAR_H
