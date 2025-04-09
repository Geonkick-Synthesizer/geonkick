/**
 * File name: kit_widget.h
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

#ifndef KIT_WIDGET_H
#define KIT_WIDGET_H

#include "geonkick_widget.h"
#include "FileBrowser.h"

#include "RkPainter.h"

class RkLineEdit;
class RkButton;
class KitModel;
class RkContainer;
class PercussionModel;
class KitPercussionView;
class RkTimer;

class KitWidget: public GeonkickWidget
{
 public:
        using PercussionIndex = int;
        KitWidget(GeonkickWidget *parent, KitModel *model);
        KitModel* getModel() const;

 protected:
        void updateView();
        void addPercussion(PercussionModel *model);
        void updatePercussion(PercussionIndex index, PercussionModel *model);
        void removePercussion(PercussionIndex index);
        void copyPercussion(int index);
        void keyPressEvent(RkKeyEvent *event) override;
        void onUpdateLevelers();

 private:
        KitModel *kitModel;
        RkButton *addButton;
        RkContainer *percussionsContainer;
        std::vector<KitPercussionView*> percussionViewList;
        RkTimer *levelersTimer;
};

class KitChannelsView: public GeonkickWidget
{
 public:
        explicit KitChannelsView(KitWidget *parent, KitModel *model);

 protected:
        void paintWidget(RkPaintEvent *event) override;

 private:
        KitModel *kitModel;
        int channelWidth;
        int channelHeight;
};

#endif // KIT_WIDGET_H
