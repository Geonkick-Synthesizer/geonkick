/**
 * File name: file_dialog.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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
class RkLabel;
class GeonkickSlider;

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
        void setFilters(const std::vector<std::string> &filters);

 protected:
        void createScrollBar();
        void showScrollBar(bool b);
        void paintWidget(RkPaintEvent *event) final;
        void mouseButtonPressEvent(RkMouseEvent *event) final;
        void mouseDoubleClickEvent(RkMouseEvent *event) final;
        void mouseMoveEvent(RkMouseEvent *event) final;
        void keyPressEvent(RkKeyEvent *event) final;
        void loadCurrentDirectory();
        int getLine(int x, int y) const;
        void onLineUp();
        void onLineDown();
        void openSelectedFile();
        void scrollBarChanged(int val);
        void updateScrollBar();

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
                Open
        };

        enum class AcceptStatus: int {
                Cancel,
                Accept
        };

        explicit FileDialog(GeonkickWidget *parent, FileDialog::Type type, const std::string& title);
        ~FileDialog() = default;
        RK_DECL_ACT(selectedFile,
                    selectedFile(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        std::string currentDirectory() const;
        void setCurrentDirectoy(const std::string &path);
        std::string filePath() const;
        AcceptStatus acceptStatus() const;
        void setFilters(const std::vector<std::string> &filters);

 protected:
        void onAccept();
        void onCancel();
        void onPathChanged(const std::string &pathName);
        void closeEvent(RkCloseEvent *event) final;

 private:
        RkLineEdit *fileNameEdit;
        Type dialogType;
        FilesView *filesView;
        RkLabel *pathLabel;
        std::string pathSelected;
        AcceptStatus status;
};

#endif // GEONKICK_FILE_DIALOG_H
