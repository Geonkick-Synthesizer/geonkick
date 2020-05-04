/**
 * File name: kit_widget.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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
#include "file_dialog.h"

#include "RkPainter.h"

class RkLineEdit;
class RkButton;
class KitModel;
class RkContainer;
class PercussionModel;

class KitWidget: public GeonkickWidget
{
 public:
        using PercussionIndex = int;
        KitWidget(GeonkickWidget *parent, KitModel *model);

 protected:
        void updateView();
        void addPercussion(PercussionModel *model);
        void updatePercussion(PercussionIndex index, PercussionModel *model);
        void removePercussion(PercussionIndex index);
        void drawKeys(RkPainter &painter);
        void showFileDialog(FileDialog::Type type);
        void openKit(const std::string &file);
        void saveKit(const std::string &file);
        void copyPercussion(int index);
        void keyPressEvent(RkKeyEvent *event);

 private:
        KitModel *kitModel;
        RkButton *addButton;
        RkButton *openKitButton;
        RkButton *saveKitButton;
        RkContainer *percussionsContainer;
};

#endif // KIT_WIDGET_H
