/**
 * File name: MidiKeyWidget.cpp
 * Project: Geonkick (A kick synthesizer)
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

#include "MidiKeyWidget.h"

#include <RkEvent.h>

MidiKeyWidget::MidiKeyWidget(GeonkickWidget *parent,
                             PercussionModel *model,
                             Rk::WidgetFlags flag)
        : GeonkickWidget(parent, flag)
        , percussionModel{model}
        , cellSize{51, 32}
        , widgetPadding{8}
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
        pen.setWidth(1);
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
        selectedCell = getCell(percussionModel->key());
        RK_ACT_BIND(percussionModel, keyUpdated,
                    RK_ACT_ARGS(PercussionModel::KeyIndex key),
                    this,
                    onUpdateKey(key));
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
        painter.drawText(cellRect, midiKeyToNote(key) + midiKeyToString(key));
        painter.setPen(penBk);
}

void MidiKeyWidget::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        RkPainter painter(this);
        if (hoverCell.isValid()) {
                // Draw cell background.
                RkColor bkColor(80, 80, 80);
                RkRect innerRect({hoverCell.rect().left() + 1, hoverCell.rect().top() + 1},
                                 RkSize(hoverCell.rect().width() - 1,
                                        hoverCell.rect().height() - 1));
                painter.fillRect(innerRect, bkColor);

                // Draw cell text.
                RkFont font = painter.font();
                font.setSize(10);
                if (hoverCell.column() == 0 || hoverCell.row() == 0)
                        font.setWeight(RkFont::Weight::Bold);
                else
                        font.setWeight(RkFont::Weight::Normal);
                auto pen = painter.pen();
                pen.setColor({230, 230, 230});
                painter.setPen(pen);
                painter.setFont(font);
                painter.drawText(hoverCell.rect(),
                                 midiKeyToNote(hoverCell.key())
                                 + midiKeyToString(hoverCell.key()));
        }

        if (selectedCell.isValid()) {
                // Draw cell background.
                RkColor bkColor;
                bkColor = RkColor(100, 100, 100);
                RkRect innerRect({selectedCell.rect().left() + 1, selectedCell.rect().top() + 1},
                                 RkSize(selectedCell.rect().width() - 1,
                                        selectedCell.rect().height() - 1));
                painter.fillRect(innerRect, bkColor);

                // Draw cell text.
                RkFont font = painter.font();
                font.setSize(10);
                if (selectedCell.column() == 0 || selectedCell.row() == 0)
                        font.setWeight(RkFont::Weight::Bold);
                else
                        font.setWeight(RkFont::Weight::Normal);
                auto pen = painter.pen();
                pen.setColor({230, 230, 230});
                painter.setPen(pen);
                painter.setFont(font);
                painter.drawText(selectedCell.rect(),
                                 midiKeyToNote(selectedCell.key())
                                 + midiKeyToString(selectedCell.key()));
        }
}

KeyCell MidiKeyWidget::getCell(int x, int y) const
{
        KeyCell cell;
        int row = (y - widgetPadding) / cellSize.height();
        if (row > midiRows - 1)
                return KeyCell();
        int col = (x - widgetPadding) / cellSize.width();
        if (col > midiColumns - 1)
                return KeyCell();
        GeonkickTypes::MidiKey key = row * midiColumns + col + 21;
        if (key < 21 || key > 109)
                return KeyCell();
        cell.setColumn(col);
        cell.setRow(row);
        cell.setKey(key);
        cell.setRect(RkRect({widgetPadding + col * cellSize.width(),
                             widgetPadding + row * cellSize.height()},
                        RkSize(cellSize.width(), cellSize.height())));
        return cell;
}

KeyCell MidiKeyWidget::getCell(GeonkickTypes::MidiKey key) const
{
        int row;
        int col;
        if (key < 21 || key > 108) {
                // Any cell.
                row = 7;
                col = 4;
        } else {
                row = (key - 21) / midiColumns;
                col = (key - 21) % 12;
        }

        KeyCell cell;
        cell.setColumn(col);
        cell.setRow(row);
        cell.setKey(key);
        cell.setRect(RkRect({widgetPadding + col * cellSize.width(),
                             widgetPadding + row * cellSize.height()},
                        RkSize(cellSize.width(), cellSize.height())));
        return cell;
}

void MidiKeyWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left) {
		if (auto cell = getCell(event->x(), event->y());
		    cell.isValid() && cell != selectedCell) {
			selectedCell = cell;
			percussionModel->setKey(selectedCell.key());
		}
	}
}

void MidiKeyWidget::mouseButtonReleaseEvent(RkMouseEvent *event)
{
}

void MidiKeyWidget::mouseMoveEvent(RkMouseEvent *event)
{
        if (auto cell = getCell(event->x(), event->y()); cell != hoverCell) {
                hoverCell = cell;
		update();
        }
}

std::string MidiKeyWidget::midiKeyToString(GeonkickTypes::MidiKey key)
{
        if (key < 21 || key > 108)
                return {};

        return " (" + std::to_string(key) + ")";
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
        return RkString(notes[(key - 21 + 9) % 12]) + std::to_string((key - 20 + 9) / 12);
}

void MidiKeyWidget::onUpdateKey(PercussionModel::KeyIndex key)
{
        selectedCell = getCell(key);
        update();
}

void MidiKeyWidget::closeEvent(RkCloseEvent *event)
{
        action isAboutToClose();
        RkWidget::closeEvent(event);
}

void MidiKeyWidget::hoverEvent(RkHoverEvent *event)
{
        if (!event->isHover()) {
                hoverCell = KeyCell();
                update();
        }
}
