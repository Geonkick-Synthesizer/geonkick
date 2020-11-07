/**
 * File name: MidiKeyWidget.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor (http://iuriepage.wordpress.com)
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

#ifndef GEONKICK_MIDIKEY_WIDGET_H
#define GEONKICK_MIDIKEY_WIDGET_H

#include "geonkick_widget.h"

#include <RkPainter.h>

class PercussionModel;

class MidiKeyWidget: public GeonkickWidget
{
 public:
        MidiKeyWidget(GeonkickWidget *parent,
                      PercussionModel *model,
                      Rk::WindowFlags flag = Rk::WindowFlags::Popup);
        /* RK_DECL_ACT(close, close() */
        /*             RK_ARG_TYPE(), */
        /*             RK_ARG_VAL()); */
        static RkString midiKeyToNote(GeonkickTypes::MidiKey key);

 protected:
        void drawCell(RkPainter &painter, GeonkickTypes::MidiKey key, int row, int col);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseButtonReleaseEvent(RkMouseEvent *event) override;
        void mouseMoveEvent(RkMouseEvent *event) override;

 private:
        /* class KeyCell { */
        /*       KeyCell() */
        /*               : state{CellState::Default} */
        /*         , note{"Any"} */
        /*         { */
        /*         } */
        /*         enum class CellState: int { */
        /*                 CellDefault, */
        /*                 CellSelected, */
        /*                 CellHover}; */
        /*         CellState state; */
        /*         std::string note; */
        /* }; */
        PercussionModel *percussionModel;
        RkSize cellSize;
        int widgetPadding;
        int midiRows;
        int midiColumns;
};

#endif // GEONKICK_MIDIKEY_WIDGET_H
