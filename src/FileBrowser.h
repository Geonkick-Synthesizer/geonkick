/**
 * File name: FileBrowser.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor
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

#ifndef GEONKICK_FILE_BROWSER_H
#define GEONKICK_FILE_BROWSER_H

#include "geonkick_widget.h"

class FilesView;
class RkLineEdit;
class RkPaintEvent;
class RkMouseEvent;
class RkCloseEvent;
class GeonkickButton;
class RkContainer;
class GeonkickButton;
class RkContainer;
class PathBookmarksModel;
class BreadcrumbBar;
class PathHistory;
class PathBookmarksView;

class FileBrowser: public GeonkickWidget {
 public:
        explicit FileBrowser(GeonkickWidget *parent,
                             const std::string &name = "FileBrowser",
                             bool saveAction = false);
        void setSize(const RkSize &size);
        fs::path currentDirectory() const;
        void setCurrentDirectoy(const fs::path &path);
        fs::path currentFile() const;
        void setFilters(const std::vector<std::string> &filters);
        void setHomeDirectory(const fs::path &path);
        bool createDirectory(const fs::path &dir);
        PathBookmarksModel* getBookmarks() const;

        RK_DECL_ACT(currentPathChanged,
                    currentPathChanged(const fs::path &path),
                    RK_ARG_TYPE(const fs::path &path),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(fileActivated,
                    fileActivated(const fs::path& file),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(folderSelected,
                    folderSelected(const fs::path& path),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(fileSelected,
                    fileSelected(const fs::path& file),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(pathBookmarked,
                    pathBookmarked(const fs::path& path),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(onCreateFile,
                    onCreateFile(const fs::path& filePath),
                    RK_ARG_TYPE(const fs::path& filePath),
                    RK_ARG_VAL(filePath));
        RK_DECL_ACT(createFile,
                    createFile(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

 protected:
        void createUi();
        void createTopMenu(RkContainer *container);
        void createBookmarkDirectoryControls(RkContainer *container);
        void createNewDirectoryControls(RkContainer *container);
        void createSaveControls(RkContainer *container);
        void updateBookmarkButton(const std::filesystem::path &path);
        void updateView();

 private:
        RkContainer *mainContainer;
        GeonkickButton *bookmarkDirectoryButton;
        PathHistory* pathHistory;
        PathBookmarksModel* pathBookmarksModel;
        PathBookmarksView* bookmarksView;
        BreadcrumbBar *breadcrumbBar;
        FilesView *filesView;
        bool saveButtonAction;
};

#endif // GEONKICK_FILE_BROWSER_H
