/**
 * File name: file_dialog.h
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

#ifndef GEONKICK_FILE_DIALOG_H
#define GEONKICK_FILE_DIALOG_H

#include "geonkick_widget.h"

class RkLineEdit;
class RkPaintEvent;
class RkMouseEvent;
class RkCloseEvent;
class GeonkickButton;
class GeonkickSlider;
class PathListModel;
class RkList;
class RkContainer;
class GeonkickButton;

class FilesView: public GeonkickWidget {
 public:
        explicit FilesView(GeonkickWidget *parent);
        std::string selectedFile() const;
        std::string getCurrentPath() const;
        void setCurrentPath(const std::string &path);
        RK_DECL_ACT(openFile, openFile(const std::string &fileName),
                    RK_ARG_TYPE(const std::string &), RK_ARG_VAL(fileName));
        RK_DECL_ACT(currentPathChanged, currentPathChanged(const std::string &pathName),
                    RK_ARG_TYPE(const std::string &), RK_ARG_VAL(pathName));
        RK_DECL_ACT(fileSelected,
                    fileSelected(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(currentFileChanged,
                    currentFileChanged(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));

        void setFilters(const std::vector<std::string> &filters);

 protected:
        void createScrollBar();
        void showScrollBar(bool b);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        void mouseDoubleClickEvent(RkMouseEvent *event) override;
        void mouseMoveEvent(RkMouseEvent *event) override;
        void keyPressEvent(RkKeyEvent *event) override;
        void loadCurrentDirectory();
        int getLine(int x, int y) const;
        void onLineUp();
        void onLineDown();
        void openSelectedFile();
        void scrollBarChanged(int val);
        void updateScrollBar();
        std::string getSelectedFile() const;

 private:
        std::vector<std::filesystem::path> filesList;
        int selectedFileIndex;
        int hightlightLine;
        int offsetIndex;
        std::filesystem::path currentPath;
        int lineHeight;
        int lineSacing;
        decltype(filesList.size()) visibleLines;
        GeonkickButton *topScrollBarButton;
        GeonkickButton *bottomScrollBarButton;
        int scrollBarWidth;
        GeonkickSlider *scrollBar;
        bool isScrollBarVisible;
        std::vector<std::string> fileFilters;
};

class FileDialog: public GeonkickWidget {
 public:
        enum class Type: int {
                Save,
                Open,
                Browse
        };

        enum class AcceptStatus: int {
                Cancel,
                Accept
        };

        explicit FileDialog(GeonkickWidget *parent,
                            FileDialog::Type type,
                            Rk::WidgetFlags flags,
                            const std::string& title);
        explicit FileDialog(GeonkickWidget *parent,
                            FileDialog::Type type = FileDialog::Type::Open,
                            const std::string& title = std::string());
        RK_DECL_ACT(selectedFile,
                    selectedFile(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(currentFileChanged,
                    currentFileChanged(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(directoryChanged,
                    directoryChanged(const std::string &path),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(rejected,
                    rejected(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

        std::string currentDirectory() const;
        void setCurrentDirectoy(const std::string &path);
        std::string filePath() const;
        AcceptStatus acceptStatus() const;
        void setFilters(const std::vector<std::string> &filters);
        void setHomeDirectory(const std::string &path);
        bool createDirectory(const std::filesystem::path &dir);
        void bookmarkDirectory(const std::filesystem::path &dir, bool bookmark = true);
        bool isPathBookmarked(const std::filesystem::path &path) const;

 protected:
        void createUi();
        void createBookmarkDirectoryControls(RkContainer *container);
        void createNewDirectoryControls(RkContainer *container);
        void updateBookmarkButton(const std::filesystem::path &path);
        void onAccept();
        void onCancel();
        void closeEvent(RkCloseEvent *event) final;

 private:
        RkLineEdit *fileNameEdit;
        Type dialogType;
        FilesView *filesView;
        std::string pathSelected;
        AcceptStatus status;
        PathListModel* shortcutDirectoriesModel;
        RkList *shortcutDirectoriesView;
        GeonkickButton *bookmarkDirectoryButton;
};

#endif // GEONKICK_FILE_DIALOG_H
