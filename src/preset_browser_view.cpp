/**
 * File name: preset_browser_view.h
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

#include "preset_browser_view.h"
#include "preset_browser_model.h"
#include "FileBrowser.h"
#include "preset_folder.h"
#include "geonkick_api.h"

#include <RkPainter.h>
#include <RkImage.h>
#include <RkEvent.h>
#include <RkButton.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(next_page);
RK_DECLARE_IMAGE_RC(next_page_hover);
RK_DECLARE_IMAGE_RC(next_page_on);
RK_DECLARE_IMAGE_RC(prev_page);
RK_DECLARE_IMAGE_RC(prev_page_hover);
RK_DECLARE_IMAGE_RC(prev_page_on);

PresetBrowserView::PresetBrowserView(GeonkickWidget *parent,
                                     PresetBrowserModel* model)
        : GeonkickWidget(parent)
        , browserModel{model}
        , topPadding{15}
        , leftPadding{5}
        , columnWidth{150}
        , rowHeight{20}
        , overRow{-1}
        , overColumn{-1}
        , nextPresetPageButton{nullptr}
        , prevPresetPageButton{nullptr}
        , nextFolderPageButton{nullptr}
        , prevFolderPageButton{nullptr}
        , bottomContainer{nullptr}
{
        setSize(800, 350);
        RK_ACT_BIND(browserModel, presetFolderAdded, RK_ACT_ARGS(PresetFolder*), this, updatePageButtons());
        RK_ACT_BIND(browserModel, presetFolderRemoved, RK_ACT_ARGS(), this, updatePageButtons());
        RK_ACT_BIND(browserModel, presetPageChanged, RK_ACT_ARGS(), this, updatePageButtons());
        RK_ACT_BIND(browserModel, folderPageChanged, RK_ACT_ARGS(), this, updatePageButtons());
        RK_ACT_BIND(browserModel, folderSelected, RK_ACT_ARGS(PresetFolder*), this, updatePageButtons());
        RK_ACT_BIND(browserModel, presetSelected, RK_ACT_ARGS(Preset*), this, updatePageButtons());

        bottomContainer = new RkContainer(this);
        bottomContainer->setSize({width(), 24});
        bottomContainer->setPosition({0, height() - 24  - 5});
        bottomContainer->setHiddenTakesPlace();
        bottomContainer->addSpace((leftPadding + columnWidth) - 2 * 24 - 10);
        nextFolderPageButton = new RkButton(this);
        nextFolderPageButton->setType(RkButton::ButtonType::ButtonPush);
        nextFolderPageButton->setSize(24, 24);
        nextFolderPageButton->setImage(RkImage(nextFolderPageButton->size(), RK_IMAGE_RC(next_page)),
                           RkButton::State::Unpressed);
        nextFolderPageButton->setImage(RkImage(nextFolderPageButton->size(), RK_IMAGE_RC(next_page_hover)),
                           RkButton::State::UnpressedHover);
        nextFolderPageButton->setImage(RkImage(nextFolderPageButton->size(), RK_IMAGE_RC(next_page_on)),
                           RkButton::State::Pressed);
        RK_ACT_BIND(nextFolderPageButton, pressed, RK_ACT_ARGS(), browserModel, folderNextPage());

        prevFolderPageButton = new RkButton(this);
        prevFolderPageButton->setType(RkButton::ButtonType::ButtonPush);
        prevFolderPageButton->setSize(24, 24);
        prevFolderPageButton->setImage(RkImage(prevFolderPageButton->size(), RK_IMAGE_RC(prev_page)),
                           RkButton::State::Unpressed);
        prevFolderPageButton->setImage(RkImage(prevFolderPageButton->size(), RK_IMAGE_RC(prev_page_hover)),
                           RkButton::State::UnpressedHover);
        prevFolderPageButton->setImage(RkImage(prevFolderPageButton->size(), RK_IMAGE_RC(prev_page_on)),
                           RkButton::State::Pressed);
        RK_ACT_BIND(prevFolderPageButton, pressed, RK_ACT_ARGS(), browserModel, folderPreviousPage());

        bottomContainer->addWidget(prevFolderPageButton);
        bottomContainer->addSpace(5);
        bottomContainer->addWidget(nextFolderPageButton);

        bottomContainer->addSpace(10, Rk::Alignment::AlignRight);
        nextPresetPageButton = new RkButton(this);
        nextPresetPageButton->setType(RkButton::ButtonType::ButtonPush);
        nextPresetPageButton->setSize(24, 24);
        nextPresetPageButton->setImage(RkImage(nextPresetPageButton->size(), RK_IMAGE_RC(next_page)),
                           RkButton::State::Unpressed);
        nextPresetPageButton->setImage(RkImage(nextPresetPageButton->size(), RK_IMAGE_RC(next_page_hover)),
                           RkButton::State::UnpressedHover);
        nextPresetPageButton->setImage(RkImage(nextPresetPageButton->size(), RK_IMAGE_RC(next_page_on)),
                           RkButton::State::Pressed);
        RK_ACT_BIND(nextPresetPageButton, pressed, RK_ACT_ARGS(), browserModel, nextPresetPage());
        bottomContainer->addWidget(nextPresetPageButton, Rk::Alignment::AlignRight);
        bottomContainer->addSpace(5, Rk::Alignment::AlignRight);

        prevPresetPageButton = new RkButton(this);
        prevPresetPageButton->setType(RkButton::ButtonType::ButtonPush);
        prevPresetPageButton->setSize(24, 24);
        prevPresetPageButton->setImage(RkImage(prevPresetPageButton->size(), RK_IMAGE_RC(prev_page)),
                           RkButton::State::Unpressed);
        prevPresetPageButton->setImage(RkImage(prevPresetPageButton->size(), RK_IMAGE_RC(prev_page_hover)),
                           RkButton::State::UnpressedHover);
        prevPresetPageButton->setImage(RkImage(prevPresetPageButton->size(), RK_IMAGE_RC(prev_page_on)),
                           RkButton::State::Pressed);
        RK_ACT_BIND(prevPresetPageButton, pressed, RK_ACT_ARGS(), browserModel, previousPresetPage());
        bottomContainer->addWidget(prevPresetPageButton, Rk::Alignment::AlignRight);
        updatePageButtons();
        setBackgroundColor({60, 60, 60});
}

void PresetBrowserView::paintWidget(RkPaintEvent *event)
{
        RkImage img(size());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        painter.fillRect({0, 0, leftPadding + columnWidth, height()}, {40, 40, 40, 70});
        painter.setPen(RkColor(50, 50, 50));
        painter.drawLine({leftPadding + columnWidth, 0}, {leftPadding + columnWidth, height()});
        auto font = painter.font();
        font.setSize(12);
        painter.setFont(font);
        if (browserModel->columns()) {
                int xColumn = leftPadding;
                int yRow = topPadding;
                for (size_t col = 0; col < browserModel->columns(); col++) {
                        for (size_t row = 0; row < browserModel->rows(); row++) {
                                auto presetName = browserModel->presetName(row, col);
                                auto font = painter.font();
                                RkRect textRect(xColumn + 15, yRow, columnWidth, rowHeight);
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
                                constexpr size_t maxChars = 20;
                                if (col == 0 && presetName.size() > maxChars)
                                        presetName = presetName.substr(0, maxChars - 3) + "...";
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

void PresetBrowserView::updatePageButtons()
{
        prevFolderPageButton->show(browserModel->folderPages() > 1);
        nextFolderPageButton->show(browserModel->folderPages() > 1);
        prevPresetPageButton->show(browserModel->presetPages() > 1);
        nextPresetPageButton->show(browserModel->presetPages() > 1);
        bottomContainer->update();
        update();
}
