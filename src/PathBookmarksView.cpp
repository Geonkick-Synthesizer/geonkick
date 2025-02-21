/**
 * File name: PathBookmarksView.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#ifndef PATH_BOOKMARKS_VIEW_H
#define PATH_BOOKMARKS_VIEW_H

#include "AbstractView.h"
#include "RkFlowContainer.h"

PathBookmarksView::PathBookmarksView(GeonkickWidget* parent, PathBookmarksModel* model)
        : AbstractView(parent, model)
        , viewModel{model}
        , flowContainer{new RkFlowContainer(this)}
{
}

void PathBookmarksView::createView()
{
}

void PathBookmarksView::updateView()
{
        for (auto &button: buttonsList)
                delete button;
        buttonsList.clear();

        auto pathList = static_cast<PathBookmarksModel*>(getModel());
        int maxButtonHeight = 0;
        for (const auto &path: model->getPaths()) {
                auto button = new BookmarkButton(this, path);
                RK_ACT_BIND(button,
                            pressed,
                            RK_ACT_ARGS(),
                            this,
                            pathSelected(button->getPath()));
                buttonsList.push_back(button);
                maxButtonHeight = std::max(maxButtonHeight, button->height());
        }

        int padding = 2;
        int x = padding;
        int y = padding;
        maxButtonHeight += padding;
        for (auto button : pathButtons) {
                if (x + button->width() > width()) {
                        x = padding;
                        y += padding + maxButtonHeight;
                }
                button->setPosition(x, y);
                x += button->width() + padding;
        }

        auto newSize = RkSize{width(), y + maxButtonHeight};
        if (newSize != size()) {
                setSize(newSize);
                action sizeUpdated();
        }
}

void PathBookmarksView::bindModel()
{
        auto model = static_cast<PathBookmarksModel*>(getModel());
        RK_ACT_BIND(model, modelUpdated, RK_ACT_ARGS(), this, updateView());
}

PathBookmarksView::unbindModel()
{
        unbindObject(getModel());
}
