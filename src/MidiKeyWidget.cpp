/**
 * File name: MidiKeyWidget.cpp
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

#include "MidiKeyWidget.h"
#include "percussion_model.h"

MidiKeyWidget::MidiKeyWidget(GeonkickWidget *parent,
                             PercussionModel *model,
                             Rk::WindowFlags flag)
        : GeonkickWidget(parent, flag)
        , percussionModel{model}
        , cellSize{32, 32}
        , widgetPadding{10}
        , midiRows{8}
        , midiColumns{12}
{
        setFixedSize(midiColumns * cellSize.width() + 2 * widgetPadding,
                     midiRows * cellSize.height()  +  2 * widgetPadding);
        setBackgroundColor({68, 68, 70, 240});
        show();
}

void MidiKeyWidget::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        RkPainter painter(this);
        auto font = painter.font();
        font.setSize(10);
        painter.setFont(font);
        auto pen = painter.pen();
        pen.setColor({10, 10, 10, 230});
        pen.setWidth(2);
        painter.setPen(pen);
        RkRect borderRect = rect();
        borderRect.setSize({borderRect.width() - 1, borderRect.height() - 1});
        painter.drawRect(borderRect);
        pen.setColor({40, 40, 40, 230});
        pen.setWidth(1);
        painter.setPen(pen);
        GeonkickTypes::MidiKey key = 21;
        for (int row = 0; row < midiRows && key <= 109; row++) {
                for (int col = 0; col < midiColumns && key <= 109; col++)
                        drawCell(painter, key++, row, col);
        }
}

void MidiKeyWidget::drawCell(RkPainter &painter,
                             GeonkickTypes::MidiKey key,
                             int row, int col)
{
        RkRect cellRect({widgetPadding + col * cellSize.width(),
                         widgetPadding + row * cellSize.height()}, cellSize);
        painter.drawRect(cellRect);
        auto penBk = painter.pen();
        auto pen = penBk;
        RkFont font;
        if (col == 0 || row == 0) {
                font = painter.font();
                font.setWeight(RkFont::Weight::Bold);
                pen.setColor({240, 240, 240});
        } else {
                pen.setColor({200, 200, 200});
                font = painter.font();
                font.setWeight(RkFont::Weight::Normal);
        }
        painter.setPen(pen);
        painter.setFont(font);
        painter.drawText(cellRect, midiKeyToNote(key));
        painter.setPen(penBk);
}

// std::pair<int, int> MidiKeyWidget::getCell(int x, int y)
// {
        
// }

void MidiKeyWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
}

void MidiKeyWidget::mouseButtonReleaseEvent(RkMouseEvent *event)
{
}

void MidiKeyWidget::mouseMoveEvent(RkMouseEvent *event)
{
        
}

RkString MidiKeyWidget::midiKeyToNote(GeonkickTypes::MidiKey key)
{
        if (key < 21 || key > 108)
                return RkString("Any");

        constexpr std::array<const char*, 17> notes =
                  {"C",  "C#", "D",
                   "D#", "E",  "F",
                   "F#", "G",  "G#",
                   "A",  "A#", "B"};
        return RkString(notes[(key - 20) % 12]) + std::to_string((key - 20) / 12);
}

