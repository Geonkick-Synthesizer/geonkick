/**
 * File name: percussion_view.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#ifndef PERCUSSION_VIEW_H
#define PERCUSSION_VIEW_H

#include "geonkick_widget.h"

class RkLineEdit;
class RkButton;
class PercussionModel;
class GeonkickSlider;
class RkProgressBar;

class KitPercussionView: public GeonkickWidget
{
 public:
        using KeyIndex = int;
        explicit KitPercussionView(GeonkickWidget *parent,
                                   PercussionModel *model = nullptr);
        void setModel(PercussionModel *model);
        void setBackground(const RkColor &color) { backgroundColor = color; }

 protected:
        void createView();
        void updateView();
        void setNameWidth(int width);
        void setKeyWidth(int width);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event);
        void mouseDoubleClickEvent(RkMouseEvent *event);
        void updatePercussionName();
        PercussionModel* getModel();
        void remove(bool b);

 private:
        PercussionModel *percussionModel;
        int nameWidth;
        int keyWidth;
        RkLineEdit *editPercussion;
        RkButton *copyButton;
        RkButton *removeButton;
        RkButton *playButton;
        RkButton *muteButton;
        RkButton *soloButton;
        GeonkickSlider *limiterSlider;
        RkProgressBar *levelerProgress;
        RkColor backgroundColor;
};

#endif // KIT_WIDGET_H
