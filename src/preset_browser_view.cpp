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

PresetBrowserView::PresetBrowserView(GeonkickWidget *parent,
                                     PresetBrowserModel* model)
        : GeonkickWidget(parent, Rk::WindowFlags::Popup)
        , browserModel{model}
{
}

void PresetBrowserView::paintWidget(RkPaintEvent *event)
{
        RkPainter painter(this);
        painter.setPen(RkColor(180, 180, 180));
        if (browserModel->columns()) {
                int columnWidth = width() / browserModel->columns();
                int xColumn = 10;
                int yRow = 20;
                for (size_t col = 0; col < browserModel->columns(); col++) {
                        for (size_t row = 0; row < browserModel->rows(); row++) {
                                auto presetName = browserModel->presetName(row, col);
                                painter.drawText({xColumn, yRow, }, presetName);
                                // auto selected = browserModel->isPresetSelected(i, k);
                                //                        if (seleacted) {
                                //                                drawPresetInfo(browserModel->presetInfo(i, k));
                                //                        }
                                yRow += painter.font().size() + 5;
                        }
                        xColumn += columnWidth;
                }
        }
}
