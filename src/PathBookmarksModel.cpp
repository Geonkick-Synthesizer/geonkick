/**
 * File name: PathBookmarksModel.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "PathBookmarksModel.h"
#include "DesktopPaths.h"

PathBookmarksModel::PathBookmarksModel(RkObject *parent)
        : AbstractModel(parent)
{
        DesktopPaths desktopPaths;
        pathList.addItem(desktopPaths.getHomePath());
        pathList.addItem(desktopPaths.getDesktopPath());
        pathList.addItem(desktopPaths.getDownloadsPath());
        auto drives = desktopPaths.getDrivesList();
        for (auto& d: drives)
                pathList.addItem(std::move(d));
}

bool PathBookmarksModel::addPath(const std::filesystem::path &path)
{
        if (!pathList.hasItem(path)) {
                pathList.addItem(path);
                action pathAdded(path);
                action modelUpdated();
                return true;
        }

        return false;
}

bool PathBookmarksModel::removePath(const std::filesystem::path &path)
{
        if (!pathList.hasItem(path)) {
                pathList.removeItem(path);
                action pathRemoved(path);
                action modelUpdated();
                return true;
        }
        return false;
}

const std::vector<std::filesystem::path>& PathBookmarksModel::getPaths() const
{
        return pathList.getItems();
}

bool PathBookmarksModel::containsPath(const std::filesystem::path &path) const
{
        return pathList.hasItem(path);
}
