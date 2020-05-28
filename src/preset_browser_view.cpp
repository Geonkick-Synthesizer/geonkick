/**
 * File name: preset_browser_view.h
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

#include "preset_browser_view.h"
#include "preset_browser_model.h"

#include <RkPainter.h>
#include <RkImage.h>
#include <RkEvent.h>

PresetBrowserView::PresetBrowserView(GeonkickWidget *parent,
                                     PresetBrowserModel* model)
        : GeonkickWidget(parent, Rk::WindowFlags::Popup)
        , browserModel{model}
        , topPadding{20}
        , leftPadding{10}
        , columnWidth{100}
        , rowHeight{20}
        , overRow{-1}
        , overColumn{-1}
{
        RK_ACT_BIND(browserModel, folderSelected, RK_ACT_ARGS(PresetFolder*), this, update());
        RK_ACT_BIND(browserModel, presetSelected, RK_ACT_ARGS(Preset*), this, update());
}

void PresetBrowserView::paintWidget(RkPaintEvent *event)
{
        RkImage img(size());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        painter.fillRect({0, 0, leftPadding + columnWidth, height()}, {40, 40, 40, 70});
        painter.setPen(RkColor(65, 87, 130));
        painter.drawLine({leftPadding + columnWidth, 0}, {leftPadding + columnWidth, height()});
        auto font = painter.font();
        font.setSize(12);
        painter.setFont(font);
        if (browserModel->columns()) {
                int columnWidth = width() / browserModel->columns();
                int xColumn = leftPadding;
                int yRow = topPadding;
                for (size_t col = 0; col < browserModel->columns(); col++) {
                        for (size_t row = 0; row < browserModel->rows(); row++) {
                                auto presetName = browserModel->presetName(row, col);
                                auto font = painter.font();
                                RkRect textRect(xColumn + 5, yRow, columnWidth, rowHeight);
                                painter.setPen(RkColor(60, 60, 60));
                                if (browserModel->isSelected(row, col)) {
                                        font.setWeight(RkFont::Weight::Bold);
                                        painter.setFont(font);
                                        painter.setPen(RkColor(230, 230, 230));
                                } else {
                                        font.setWeight(RkFont::Weight::Normal);
                                        painter.setFont(font);
                                        painter.setPen(RkColor(180, 180, 180));
                                }

                                if (static_cast<decltype(row)>(overRow) == row
                                    && static_cast<decltype(col)>(overColumn) == col) {
                                        painter.setPen(RkColor(255, 255, 255));
                                }
                                painter.drawText(textRect, presetName, Rk::Alignment::AlignLeft);
                                yRow += rowHeight;
                        }
                        xColumn += columnWidth;
                        yRow = topPadding;
                }
        }
        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void PresetBrowserView::mouseButtonPressEvent(RkMouseEvent *event)
{
        browserModel->select((event->y() - topPadding) / rowHeight,
                             (event->x() - leftPadding) / columnWidth);
}

void PresetBrowserView::mouseMoveEvent(RkMouseEvent *event)
{
        auto overColumnTmp = (event->x() - leftPadding) / columnWidth;
        bool toUpdate = false;
        if (overColumnTmp != overColumn) {
                overColumn = overColumnTmp;
                toUpdate = true;
        }

        auto overRowTmp = (event->y() - topPadding) / rowHeight;
        if (overRowTmp != overRow) {
                overRow = overRowTmp;
                toUpdate = true;
        }

        if (toUpdate)
                update();
}
