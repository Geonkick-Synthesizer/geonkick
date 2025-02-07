/**
 * File name: MidiKeyWidget.h
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

#ifndef GEONKICK_MIDIKEY_WIDGET_H
#define GEONKICK_MIDIKEY_WIDGET_H

#include "geonkick_widget.h"
#include "percussion_model.h"

#include <RkPainter.h>

class KeyCell {
  public:
        constexpr KeyCell() : midiKey{0}, cellColumn{-1}, cellRow{-1} {}
        friend constexpr bool operator==(const KeyCell &c1, const KeyCell &c2)
        {
                return c1.rect() == c2.rect() && c1.key() == c2.key()
                        && c1.column() == c2.column() && c1.row() == c2.row();
        }

        friend constexpr bool operator!=(const KeyCell &c1, const KeyCell &c2)
        {
                return c1.rect() != c2.rect() || c1.key() != c2.key()
                        || c1.column() != c2.column() || c1.row() != c2.row();
        }
        constexpr void setColumn(int col) { cellColumn = col; }
        constexpr int column() const { return cellColumn; }
        constexpr void setRow(int row) { cellRow = row; }
        constexpr int row() const { return cellRow; }
        constexpr void setRect(const RkRect &r) { cellRect = r; }
        constexpr const RkRect& rect() const {return cellRect; }
        constexpr void setKey(GeonkickTypes::MidiKey k) { midiKey = k; }
        constexpr GeonkickTypes::MidiKey key() const { return midiKey; }
        constexpr bool isValid() const
        {
                return (cellColumn >= 0 && cellRow >= 0
                        && cellColumn <= 12 && cellRow <= 7)
                        && ((midiKey >= 21 && midiKey <= 109)
                            || (midiKey == GeonkickTypes::geonkickAnyKey));
        }

  private:
        RkRect cellRect;
        GeonkickTypes::MidiKey midiKey;
        int cellColumn;
        int cellRow;
};

class MidiKeyWidget: public GeonkickWidget
{
 public:
        MidiKeyWidget(GeonkickWidget *parent,
                      PercussionModel *model,
                      Rk::WidgetFlags flag = Rk::WidgetFlags::Popup);
        RK_DECL_ACT(keySelected,
                    keySelected(GeonkickTypes::MidiKey key),
                    RK_ARG_TYPE(GeonkickTypes::MidiKey),
                    RK_ARG_VAL(key));
        RK_DECL_ACT(isAboutToClose,
                    isAboutToClose(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        static RkString midiKeyToNote(GeonkickTypes::MidiKey key);

 protected:
        void drawCell(RkPainter &painter, GeonkickTypes::MidiKey key, int row, int col);
        KeyCell getCell(int x, int y) const;
        KeyCell getCell(GeonkickTypes::MidiKey key) const;
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseButtonReleaseEvent(RkMouseEvent *event) override;
        void mouseMoveEvent(RkMouseEvent *event) override;
        void onUpdateKey(PercussionModel::KeyIndex key);
        void closeEvent(RkCloseEvent *event) override;
        void hoverEvent(RkHoverEvent *event) override;
        std::string midiKeyToString(GeonkickTypes::MidiKey key);

 private:
        PercussionModel *percussionModel;
        RkSize cellSize;
        int widgetPadding;
        int midiRows;
        int midiColumns;
        KeyCell selectedCell;
        KeyCell hoverCell;
};

#endif // GEONKICK_MIDIKEY_WIDGET_H
