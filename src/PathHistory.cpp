/**
 * File name: PathHistory.cpp
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

#include "PathHistory.h"

PathHistory::PathHistory(RkObject *parent)
        : RkObject(parent)
        , currentIndex(-1)
{
}

void PathHistory::goTo(const fs::path& newPath)
{
        if (!pathHistory.empty()
            && (pathHistory.back() == newPath
            || (currentIndex < pathHistory.size()
            && pathHistory[currentIndex] == newPath)))
                return;

        if (currentIndex + 1 < pathHistory.size()) {
                // Clear forward history.
                pathHistory.erase(pathHistory.begin() + currentIndex + 1,
                                  pathHistory.end());
        }

        pathHistory.push_back(newPath);
        currentIndex = pathHistory.size() - 1;
        action pathChanged(newPath);
        action backwardHistoryUpdated(hasBackwardHistory());
        action forwardHistoryUpdated(hasForwardHistory());
}

void PathHistory::goBack()
{
        if (hasBackwardHistory()) {
                action pathChanged(pathHistory[--currentIndex]);
        }

        action backwardHistoryUpdated(hasBackwardHistory());
        action forwardHistoryUpdated(hasForwardHistory());
}

void PathHistory::goForward()
{
        if (hasForwardHistory()) {
                action pathChanged(pathHistory[++currentIndex]);
        }

        action backwardHistoryUpdated(hasBackwardHistory());
        action forwardHistoryUpdated(hasForwardHistory());
}

bool PathHistory::hasBackwardHistory() const
{
        return !pathHistory.empty() && currentIndex > 0;
}

bool PathHistory::hasForwardHistory() const
{
        return currentIndex + 1 < pathHistory.size();
}

std::optional<fs::path> PathHistory::getCurrentPath() const
{
        if (!pathHistory.empty() && currentIndex < pathHistory.size())
                return pathHistory[currentIndex];
        return std::nullopt;
}
