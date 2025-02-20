/**
 * File name: FilesView.h
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

#ifndef GEONKICK_FILES_VIEW_H
#define GEONKICK_FILES_VIEW_H

#include "geonkick_widget.h"

class GeonkickButton;
class GeonkickSlider;
class RkPainter;
class PathBookmarksModel;

class FilesView: public GeonkickWidget {
 public:
        explicit FilesView(GeonkickWidget *parent);
        void setSize(const RkSize &size);
        std::string selectedFile() const;
        void setCurrentPath(const std::string &path);
        std::string getCurrentPath() const;
        void setFilters(const std::vector<std::string> &filters);
        void setBookmarksModel(PathBookmarksModel *model);
        PathBookmarksModel* getBookmarksModel() const;
        RK_DECL_ACT(openFile, openFile(const std::string &fileName),
                    RK_ARG_TYPE(const std::string &), RK_ARG_VAL(fileName));
        RK_DECL_ACT(currentPathChanged, currentPathChanged(const std::string &path),
                    RK_ARG_TYPE(const std::string&), RK_ARG_VAL(path));
        RK_DECL_ACT(fileSelected,
                    fileSelected(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(currentFileChanged,
                    currentFileChanged(const std::string &file),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(file));
        RK_DECL_ACT(pathBookmarked,
                    pathBookmarked(const fs::path& path),
                    RK_ARG_TYPE(const fs::path& path),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(pathUnbookmarked,
                    pathUnbookmarked(const fs::path& path),
                    RK_ARG_TYPE(const fs::path& path),
                    RK_ARG_VAL(path));

 protected:
        void createScrollBar();
        void updateScrollBarView();
        void showScrollBar(bool b);
        void drawBookmarkIcon(RkPainter &painter, int line, int yPos);
        std::string truncateFileName(RkPainter &painter,
                                     const std::string& text,
                                     int maxWidth);
        void paintWidget(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;
        bool isBookmarkArea(RkMouseEvent *event, int bookmarkWidth) const;
        void toggleBookmark(const std::string& path);
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
        bool hightlightBookmarkIcon;
        int offsetIndex;
        std::filesystem::path currentPath;
        int lineHeight;
        int lineSacing;
        decltype(filesList.size()) visibleLines;
        GeonkickButton *topScrollBarButton;
        GeonkickButton *bottomScrollBarButton;
        GeonkickSlider *scrollBar;
        int scrollBarWidth;
        bool isScrollBarVisible;
        std::vector<std::string> fileFilters;
};

#endif // GEONKICK_FILES_VIEW_H
