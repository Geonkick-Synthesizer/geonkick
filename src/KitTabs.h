/**
 * File name: KitTabs.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef KITTABS_H
#define KITTABS_H

#include "geonkick_widget.h"

class KitModel;
class RkContainer;
class RkButton;
class PercussionModel;

class KitTabs: public GeonkickWidget
{
 public:
        KitTabs(GeonkickWidget *parent, KitModel *model);
        KitModel* getModel() const;
        void updateView();

 protected:
        void paintEvent(RkPaintEvent * event) override;
        void selectCurrentTab(PercussionModel *per, RkButton* button);
        RkButton* createTabButton(PercussionModel *per);

 private:
        KitModel *kitModel;
        RkContainer* mainLayout;
        std::vector<RkButton*> tabsList;
};

#endif // KITTABS_H
