/**
 * File name: percussion_view.h
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

#ifndef PERCUSSION_VIEW_H
#define PERCUSSION_VIEW_H

#include "geonkick_widget.h"
#include "geonkick_slider.h"

#include <RkLabel.h>

class PercussionLimiter : public GeonkickSlider {
  public:
        PercussionLimiter(GeonkickWidget *parent);
        void setLeveler(int value);
        int getLeveler() const;

  protected:
        virtual void paintWidget(RkPaintEvent *event) override;

  private:
        int levelerValue;
};

class RkLineEdit;
class RkSpinBox;
class RkButton;
class PercussionModel;
class RkProgressBar;
class KitWidget;
class GeonkickButton;

class KitPercussionView: public GeonkickWidget
{
 public:
        using PercussionIndex = int;
        using KeyIndex = int;
        explicit KitPercussionView(KitWidget *parent,
                                   PercussionModel *model = nullptr);
        void setModel(PercussionModel *model);
        PercussionModel* getModel();
        PercussionIndex index() const;
        void updateLeveler();

 protected:
        void createView();
        void updateView();
        void setNameWidth(int width);
        void setKeyWidth(int width);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseDoubleClickEvent(RkMouseEvent *event) override;
        void updatePercussionName();
        void remove();
        void showMidiPopup();

 private:
        KitWidget *parentView;
        PercussionModel *percussionModel;
        int nameWidth;
        int channelWidth;
        RkLineEdit *editPercussion;
        RkSpinBox *midiChannelSpinBox;
        GeonkickButton *keyButton;
        RkButton *copyButton;
        RkButton *removeButton;
        RkButton *playButton;
        RkButton *muteButton;
        RkButton *soloButton;
        RkButton *noteOffButton;
        PercussionLimiter *percussionLimiter;
};

#endif // KIT_WIDGET_H
