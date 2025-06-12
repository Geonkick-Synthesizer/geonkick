/**
 * File name: BreadcrumbBar.cpp
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

#include "BreadcrumbBar.h"
#include "PathButton.h"
#include "DesktopPaths.h"

BreadcrumbBar::BreadcrumbBar(GeonkickWidget* parent)
        : GeonkickWidget(parent)
{
        setBackgroundColor({40, 40, 40});
        setSize(parent->width() - 4, 20);
}

void BreadcrumbBar::setPath(const fs::path &path)
{
        if (path == currentPath)
                return;

        for (auto button: pathButtons)
                delete button;
        pathButtons.clear();

        size_t partsCount = std::distance(path.begin(), path.end());
        size_t i = 0;
        fs::path tempCurrentPath;
        for (const auto& part : path) {
                tempCurrentPath /= part;
                auto button = new PathButton(this,
                                             tempCurrentPath,
                                             (i < partsCount - 1) ? " > " : "");
                RK_ACT_BIND(button, pressed, RK_ACT_ARGS(), this, pathPressed(i));
                pathButtons.emplace_back(button);
                i++;
        }

        if (!pathButtons.empty()) {
                pathButtons.back()->setPressed(true);
                currentPath = pathButtons.back()->getPath();
        }

        updateButtonView();
}

void BreadcrumbBar::pathPressed(size_t index)
{
        if (index >= pathButtons.size())
                return;

        for (auto i = index + 1; i < pathButtons.size(); i++)
                delete pathButtons[i];
        pathButtons.erase(pathButtons.begin() + index + 1, pathButtons.end());
        updateButtonView();

        if (currentPath != pathButtons.back()->getPath()) {
                currentPath = pathButtons.back()->getPath();
                action pathChanged(pathButtons.back()->getPath());
        }
}

void BreadcrumbBar::updateButtonView()
{
        if (pathButtons.empty())
                return;

        int maxButtonHeight = 0;
        for (auto button : pathButtons)
                maxButtonHeight = std::max(maxButtonHeight, button->height());

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
