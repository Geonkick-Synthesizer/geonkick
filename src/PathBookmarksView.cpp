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

#include "PathBookmarksView.h"
#include "PathBookmarksModel.h"
#include "RkFlowContainer.h"
#include "PathButton.h"

PathBookmarksView::PathBookmarksView(GeonkickWidget* parent, PathBookmarksModel* model)
        : AbstractView(parent, model)
        , flowContainer{new RkFlowContainer(this)}
{
        setSize(parent->width() - 4, 20);
        setBackgroundColor({40, 40, 40});

        flowContainer->setSize(size());
        flowContainer->setSpacing(2);

        createView();
        bindModel();

        show();
}

void PathBookmarksView::createView()
{
        updateView();
}

void PathBookmarksView::updateView()
{
        flowContainer->clear();
        for (auto &button: pathButtons)
                delete button;
        pathButtons.clear();

        auto model = static_cast<PathBookmarksModel*>(getModel());
        for (const auto &path: model->getPaths()) {
                auto button = new PathButton(this, path);
                button->setType(RkButton::ButtonType::ButtonPush);
                flowContainer->addWidget(button);
                RK_ACT_BIND(button,
                            pressed,
                            RK_ACT_ARGS(),
                            this,
                            pathSelected(button->getPath()));
                pathButtons.push_back(button);
        }

        if (flowContainer->size() != size()) {
                setSize(flowContainer->size());
                flowContainer->update();
                action sizeUpdated();
        }
}

void PathBookmarksView::bindModel()
{
        RK_ACT_BIND(getModel(), modelUpdated, RK_ACT_ARGS(), this, updateView());
}

void PathBookmarksView::unbindModel()
{
        unbindObject(getModel());
}
