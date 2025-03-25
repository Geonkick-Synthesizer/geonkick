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
#include "AbstractModel.h"
#include "ItemList.h"

class PathBookmarksModel: public AbstractModel {
  public:
        explicit PathBookmarksModel(RkObject *parent, const std::string &name);
        bool addPath(const std::filesystem::path &path);
        bool removePath(const std::filesystem::path &path);
        const std::vector<std::filesystem::path>& getPaths() const;
        bool containsPath(const std::filesystem::path &path) const;
        RK_DECL_ACT(pathAdded,
                    pathAdded(const std::filesystem::path& path),
                    RK_ARG_TYPE(const std::filesystem::path&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(pathRemoved,
                    pathRemoved(const std::filesystem::path& path),
                    RK_ARG_TYPE(const std::filesystem::path&),
                    RK_ARG_VAL(path));

 private:
        ItemList<std::filesystem::path> pathList;
};

#endif // PATH_BOOKMARKS_MODEL_H
