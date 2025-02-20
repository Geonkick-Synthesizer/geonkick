/**
 * File name: PathBookmarksModel.h
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

#ifndef PATH_BOOKMARKS_MODEL_H
#define PATH_BOOKMARKS_MODEL_H

#include "globals.h"
#include "ItemList.h"

class PathBookmarksModel: public RkObject {
  public:
        explicit PathBookmarksModel(RkObject *parent);
        void setHomeDirectory(const std::string &path);
        void addPath(const std::filesystem::path &path);
        void removePath(const std::filesystem::path &path);
        const std::unordered_set<std::filesystem::path>& getPaths() const;

 private:
        ItemList<std::filesystem::path> pathList;
};

#endif // PATH_BOOKMARKS_MODEL_H
