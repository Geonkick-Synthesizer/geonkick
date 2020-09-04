/**
 * File name: PathListModel.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "PathListModel.h"

PathListModel::PathListModel(RkObject *parent)
        : RkModel(parent)
        , pathList{"/home", "/home/Desktop", "/home/Download"}
{
}

RkVariant PathListModel::itemData(size_t index, int dataType) const
{
        if (index > itemsNumber() - 1)
                return RkVariant();

        if (static_cast<RkModelItem::DataType>(dataType) == RkModelItem::DataType::String) {
                if (index == 0)
                        return "Home";
                else if (pathList[index].has_filename())
                        return pathList[index].filename();
        } else if (static_cast<RkModelItem::DataType>(dataType) == RkModelItem::DataType::Size) {
                return RkSize(0, 20);
        } else if (static_cast<RkModelItem::DataType>(dataType) == RkModelItem::DataType::Color) {
                return RkColor(40, 100, 40);
        } else if (static_cast<PathListDataType>(dataType) == PathListDataType::Path) {
                return pathList[index];
        }
        return RkVariant();
}

size_t PathListModel::itemsNumber() const
{
        return pathList.size();
}

void PathListModel::addPath(const std::filesystem::path &path)
{
        auto it = std::find(pathList.begin(), pathList.end(), path);
        if (it != pathList.end()) {
                if (pathList.size() > 8)
                        pathList.erase(pathList.begin() + 3);
                pathList.push_back(path);
                action modelChanged();
        }
}

