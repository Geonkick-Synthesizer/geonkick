/**
 * File name: PathListModel.h
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

#ifndef GEONKICK_PATH_LIST_MODEL_H
#define GEONKICK_PATH_LIST_MODEL_H

#include "globals.h"

#include <RkModel.h>

class PathListModel: public RkModel {
  public:
        enum class PathListDataType : int {
                Path = static_cast<int>(RkModelItem::DataType::UserType)
        };
        explicit PathListModel(RkObject *parent);
        void setHomeDirectory(const std::string &path);
        RkVariant itemData(size_t index, int dataType = static_cast<int>(RkModelItem::DataType::String)) const override;
        size_t itemsNumber() const override;
        int itemSpan() const override;
        void addPath(const std::filesystem::path &path);

 private:
        std::vector<std::filesystem::path> pathList;
        RkFont stringFont;
};

#endif // GEONKICK_PATH_LIST_MODEL_H
