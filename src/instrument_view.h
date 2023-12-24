/**
 * File name: instrument_view.h
 * Project: Geonkick (A percussive synthesizer)
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

class RkLineEdit;
class RkButton;
class InstrumentModel;
class RkProgressBar;
class KitWidget;
class GeonkickButton;

class InstrumentLimiter : public GeonkickSlider {
  public:
        InstrumentLimiter(GeonkickWidget *parent);
        void setLeveler(int value);
        int getLeveler() const;

  protected:
        virtual void paintWidget(RkPaintEvent *event) override;

  private:
        int levelerValue;
};

class KitInstrumentView: public GeonkickWidget
{
 public:
        using InstrumentIndex = int;
        using KeyIndex = int;
        explicit KitInstrumentView(KitWidget *parent,
                                   InstrumentModel *model = nullptr);
        void setModel(InstrumentModel *model);
        InstrumentModel* getModel();
        InstrumentIndex index() const;
        void updateLeveler();

 protected:
        void createView();
        void updateView();
        void setNameWidth(int width);
        void setKeyWidth(int width);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseDoubleClickEvent(RkMouseEvent *event) override;
        void updateInstrumentName();
        void remove();
        void showMidiPopup();

 private:
        KitWidget *parentView;
        InstrumentModel *instrumentModel;
        int nameWidth;
        int channelWidth;
        RkLineEdit *editInstrument;
        GeonkickButton *keyButton;
        RkButton *copyButton;
        RkButton *removeButton;
        RkButton *playButton;
        RkButton *muteButton;
        RkButton *soloButton;
        InstrumentLimiter *instrumentLimiter;
};

#endif // KIT_WIDGET_H
