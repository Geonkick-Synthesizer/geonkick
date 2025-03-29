/**
 * File name: PathBookmarksView.h
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

class RkFlowContainer;
class PathButton;

class PathBookmarksModel;

class PathBookmarksView: public AbstractView
{
 public:
        explicit PathBookmarksView(GeonkickWidget *parent,
                                   PathBookmarksModel* model = nullptr);
        void createView() override;
        void updateView() override;
        RK_DECL_ACT(pathSelected,
                    pathSelected(const fs::path& path),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(sizeUpdated,
                    sizeUpdated(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

 protected:
        void bindModel() override;
        void unbindModel() override;

 private:
        RkFlowContainer *flowContainer;
        std::vector<PathButton*> pathButtons;
};

#endif // PATH_BOOKMARKS_VIEW_H
