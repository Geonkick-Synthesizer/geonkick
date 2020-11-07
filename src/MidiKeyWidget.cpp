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

        RkImage img(size());
        RkPainter painter(&img);
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
        setBackgroundImage(img);
        show();
}

void MidiKeyWidget::drawCell(RkPainter &painter,
                             GeonkickTypes::MidiKey key,
                             int row, int col)
{
        RkRect cellRect({widgetPadding + col * cellSize.width(),
                         widgetPadding + row * cellSize.height()}, cellSize);
        auto penBk = painter.pen();
        painter.fillRect(cellRect, {60, 60, 60});
        painter.setPen(penBk);
        painter.drawRect(cellRect);
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

void MidiKeyWidget::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        if (!currentCell.empty()) {
                // Draw cell background.
                RkColor bkColor;
                if (cell.state() == KeyCell::State::Pressed)
                        bkColor = RkColor(120, 120, 120);
                else if (cell.state() == KeyCell::State::Hover)
                        bkColor = RkColor(80, 80, 80);
                else
                        return;

                // Draw background.
                painter.fillRect(currentCell.rect(), bkColor);

                // Draw cell borders.
                auto pen = painter.pen();
                if (cell.state() == KeyCell::State::Pressed)
                        pen.setColor({60, 60, 60, 230});
                else
                        pen.setColor({40, 40, 40, 230});
                painter.setPen(pen);
                painter.drawRect(currentCell.rect());

                // Draw cell text.
                RkFont font = painter.font();
                font.setSize(10);
                if (currentCell->column() == 0 || currentCell->row()) {
                        font.setWeight(RkFont::Weight::Bold);
                        pen.setColor({200, 200, 230});
                } else {
                        pen.setColor({200, 200, 200});
                        font.setWeight(RkFont::Weight::Normal);
                }
                pen.setFont(font);
                painter.setPen(pen);
                painter.drawText(currentCell.rect(), midiKeyToNote(currentCell.key()));
        }
}

void MidiKeyWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() != RkMouseEvent::ButtonType::Right) {
                auto cell = getCurrentCell(event->x(), event->y());
                if (cell.empty() && cell != currentCell) {
                        currentCell = cell;
                        currentCell->setState(KeyCell::State::Pressed);
                        update();
                }
        }
                
}

MidiKeyWidget::KeyCell MidiKeyWidget::getCurrentCell(int x, int y)
{
        KeyCell cell;
        int row = (y - widgetPadding) / cellSize->height();
        if (row > midiRows - 1)
                return KeyCell();
        int col = (x - widgetPadding) / cellSize->width();
        if (col > midiColumns - 1)
                return KeyCell();
        GeonkickTypes::MidiKey key = row * midiColumns + col + 21;
        if (key < 21 || key > 109)
                return KeyCell();
        cell.setColumn(col);
        cell.setRow(raw);
        cell.setKey(key);
        cell.setRect(RkRect({widgetPadding + col * cellSize.width(),
                             widgetPadding + raw * cellSize.height()},
                        cellSize));
        return cell;
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

