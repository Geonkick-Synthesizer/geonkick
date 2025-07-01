/**
 * File name: FilesView.cpp
 * Project: Geonkick (A precussive synthesizer)
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

#include "FilesView.h"
#include "geonkick_button.h"
#include "geonkick_slider.h"
#include "PathBookmarksModel.h"

#include <RkLineEdit.h>
#include <RkEvent.h>
#include <RkPainter.h>

RK_DECLARE_IMAGE_RC(scrollbar_button_up);
RK_DECLARE_IMAGE_RC(scrollbar_button_down);
RK_DECLARE_IMAGE_RC(bookmark_16x16_unpressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_pressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_hover);

FilesView::FilesView(GeonkickWidget *parent)
        : GeonkickWidget(parent)
        , selectedFileIndex{-1}
        , hightlightLine{-1}
        , hightlightBookmarkIcon{false}
        , offsetIndex{-1}
        , currentPath{std::filesystem::current_path()}
        , lineHeight{14}
        , lineSacing{lineHeight / 2}
        , visibleLines{0}
        , topScrollBarButton{nullptr}
        , bottomScrollBarButton{nullptr}
        , scrollBar{nullptr}
        , scrollBarWidth{12}
        , isScrollBarVisible{false}
        , bookmarksModel{nullptr}
        , newPathEdit{nullptr}
        , isShowFolders{true}
{
        setSize(parent->size());
        setBackgroundColor(40, 40, 40);
        createScrollBar();
        showScrollBar(false);
        show();
}

void FilesView::setSize(const RkSize &s)
{
        GeonkickWidget::setSize(s - RkSize{0, 21});
        visibleLines = height() / (lineHeight + lineSacing);
        showScrollBar(filesList.size() > visibleLines);
        updateScrollBarView();
                if (size().isEmpty())
                        std::abort();
}

void FilesView::setCurrentPath(const fs::path &path)
{
        if (currentPath == path)
                return;

        currentPath = path;
        selectedFileIndex = -1;
        loadCurrentDirectory();
        update();
}

const fs::path& FilesView::getCurrentPath() const
{
        return currentPath;
}

void FilesView::createScrollBar()
{
        topScrollBarButton = new GeonkickButton(this);
        topScrollBarButton->setCheckable(true);
        topScrollBarButton->setUnpressedImage(RkImage(12, 17, RK_IMAGE_RC(scrollbar_button_up)));
        RK_ACT_BIND(topScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineUp());

        bottomScrollBarButton = new GeonkickButton(this);
        bottomScrollBarButton->setCheckable(true);
        bottomScrollBarButton->setUnpressedImage(RkImage(12, 17, RK_IMAGE_RC(scrollbar_button_down)));
        RK_ACT_BIND(bottomScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineDown());

        scrollBar = new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical);
        RK_ACT_BIND(scrollBar, valueUpdated, RK_ACT_ARGS(int val), this, scrollBarChanged(val));

        updateScrollBarView();
}

void FilesView::updateScrollBarView()
{
        if (!scrollBar || !topScrollBarButton || !bottomScrollBarButton)
                return;

        topScrollBarButton->setSize(scrollBarWidth, (static_cast<double>(3) / 2) * scrollBarWidth);
        topScrollBarButton->setPosition(width() - scrollBarWidth, 0);

        bottomScrollBarButton->setSize(scrollBarWidth, (static_cast<double>(3) / 2) * scrollBarWidth);
        bottomScrollBarButton->setPosition(width() - scrollBarWidth, height() -  bottomScrollBarButton->height());

        scrollBar->setSize(scrollBarWidth, bottomScrollBarButton->y() - topScrollBarButton->y() - topScrollBarButton->height());
        scrollBar->setPosition(topScrollBarButton->x(), topScrollBarButton->y() + topScrollBarButton->height());
}

void FilesView::showScrollBar(bool b)
{
        if (!scrollBar)
                return;

        isScrollBarVisible = b;
        if (isScrollBarVisible) {
                topScrollBarButton->show();
                bottomScrollBarButton->show();
                scrollBar->show();
                scrollBar->onSetValue(0);
                updateScrollBar();
        } else {
                topScrollBarButton->hide();
                bottomScrollBarButton->hide();
                scrollBar->hide();
        }
}

void FilesView::updateScrollBar()
{
        if (isScrollBarVisible) {
                if (filesList.empty() || offsetIndex < 0)
                        scrollBar->onSetValue(100);
                else
                        scrollBar->onSetValue(100 - static_cast<double>(offsetIndex)
                                              / (filesList.size() - visibleLines) * 100);
        }
}

void FilesView::scrollBarChanged(int val)
{
        val = 100 - val;
        if (filesList.size() > visibleLines) {
                offsetIndex = (static_cast<double>(val) / 100) * (filesList.size() - visibleLines);
                update();
        }
}

void FilesView::loadCurrentDirectory()
{
        if (selectedFileIndex >= 0
            && static_cast<size_t>(selectedFileIndex) < filesList.size()) {
                currentPath = filesList[selectedFileIndex];
        }

        if (!std::filesystem::is_directory(currentPath))
                return;

        try {
                auto pathEntries = fs::directory_iterator(currentPath);
                auto isDir = [](const auto& entry) { return entry.is_directory(); };
                auto isFile = [](const auto& entry) { return entry.is_regular_file(); };
                auto isFilteredFile = [&](const auto& entry) {
                        auto ext = entry.path().extension().string();
                        std::transform(ext.begin(), ext.end(), ext.begin(), [](auto c) {
                                return std::tolower(c);
                        });
                        return std::ranges::any_of(fileFilters, [&](const auto& filter) {
                                return std::equal(ext.begin(),
                                                  ext.end(),
                                                  filter.begin(),
                                                  filter.end(),
                                                  [](auto a, auto b) {
                                                          return std::tolower(a) == std::tolower(b);
                                                  });
                        });
                };

                filesList.clear();
                if (getIsShowFolders()) {
                        auto dirs = pathEntries | std::views::filter(isDir)
                                | std::views::transform(&fs::directory_entry::path);
                        // TODO: use std::ranges::to C++23
                        std::vector<fs::path> sortedDirs;
                        for (const auto& path : dirs)
                                sortedDirs.push_back(path);
                        std::sort(sortedDirs.begin(), sortedDirs.end());
                        filesList = std::move(sortedDirs);
                }

                pathEntries = fs::directory_iterator(currentPath);
                auto files = pathEntries | std::views::filter(isFile)
                        | std::views::filter(isFilteredFile)
                        | std::views::transform(&fs::directory_entry::path);
                std::vector<fs::path> sortedFiles;
                for (const auto& path : files)
                        sortedFiles.push_back(path);
                std::sort(sortedFiles.begin(), sortedFiles.end());
                filesList.insert(filesList.end(), sortedFiles.begin(), sortedFiles.end());
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading directory");
                filesList.clear();
        }

        offsetIndex = 0;
        selectedFileIndex = 0;
        showScrollBar(filesList.size() > visibleLines);
        action currentPathChanged(currentPath);
}

void FilesView::drawBookmarkIcon(RkPainter& painter, int line, int yPos)
{
        auto index = offsetIndex + line;
        if (!bookmarksModel || index < 0 || static_cast<size_t>(index + 1) > filesList.size()
             || !fs::is_directory(filesList[index]))
                return;

        RkImage bookmarkIcon;
        if (bookmarksModel->containsPath(filesList[index]))
                bookmarkIcon = RK_RC_IMAGE(bookmark_16x16_pressed);
        else if (hightlightBookmarkIcon && line == hightlightLine)
                bookmarkIcon = RK_RC_IMAGE(bookmark_16x16_hover);
        else
                bookmarkIcon = RK_RC_IMAGE(bookmark_16x16_unpressed);
        painter.drawImage(bookmarkIcon, width() - 40, yPos);
}

std::string FilesView::truncateFileName(RkPainter &painter,
                                        const std::string& text,
                                        int maxWidth)
{
    const std::string ellipsis = "...";
    if (painter.getTextWidth(text) <= maxWidth)
        return text;

    std::string truncatedText = text;
    while (!truncatedText.empty() && painter.getTextWidth(ellipsis + truncatedText) > maxWidth)
        truncatedText.erase(0, 1);

    return ellipsis + truncatedText;
}

void FilesView::paintWidget(RkPaintEvent *event)
{
        RkImage img(width(), height());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        RkPen normalPen = painter.pen();
        auto hightlightPen = normalPen;
        hightlightPen.setColor({200, 200, 200});
        normalPen.setColor({150, 150, 150});
        painter.setPen(normalPen);
        RkPen selectedPen = normalPen;
        selectedPen.setColor({255, 255, 255});
        auto font = painter.font();
        font.setSize(lineHeight);
        painter.setFont(font);

        int lineYPos = 0;
        auto index = offsetIndex;
        int line = 0;
        while(index >= 0 && (static_cast<decltype(filesList.size())>(index) < filesList.size())
              && (static_cast<decltype(visibleLines)>(index - offsetIndex) < visibleLines)) {
                auto fileName = filesList[index].filename().string();
                if (!fileName.empty()) {
                        auto font = painter.font();
                        if (std::filesystem::is_directory(filesList[index]))
                                font.setWeight(RkFont::Weight::Bold);
                        else
                                font.setWeight(RkFont::Weight::Normal);
                        painter.setFont(font);

                        if (selectedFileIndex == index)
                                painter.setPen(selectedPen);
                        else if (hightlightLine == line)
                                painter.setPen(hightlightPen);
                        else
                                painter.setPen(normalPen);

                        RkRect textRect(10, lineYPos, width() - 40, lineHeight);
                        fileName = truncateFileName(painter,
                                                    fileName,
                                                    textRect.width() - 25);
                        painter.drawText(textRect, fileName, Rk::Alignment::AlignLeft);
                        drawBookmarkIcon(painter, line, lineYPos);
                }
                lineYPos += lineHeight + lineSacing;
                index++;
                line++;
        }
        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void FilesView::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp) {
                onLineUp();
                return;
        } else if (event->button() == RkMouseEvent::ButtonType::WheelDown) {
                onLineDown();
                return;
        }

        auto line = getLine(event->x(), event->y());
        if (line < 0)
                return;

        selectedFileIndex = offsetIndex + line;
        auto filePath = selectedFile();
        auto bookmarkWidth = RK_RC_IMAGE(bookmark_16x16_hover).width();
        if (bookmarksModel && isBookmarkArea(event, bookmarkWidth)
            && std::filesystem::is_directory(filePath)) {
                if (bookmarksModel->containsPath(filePath))
                        bookmarksModel->removePath(filePath);
                else
                        bookmarksModel->addPath(filePath);
        } else if (!std::filesystem::is_directory(filePath)) {
                action fileSelected(filePath);
        } if (std::filesystem::is_directory(filePath)) {
                action folderSelected(filePath);
        }
        update();
}

bool FilesView::isBookmarkArea(RkMouseEvent *event, int bookmarkWidth) const
{
        return (event->x() >= width() - 40)
                && event->x() < (width() - 40 + bookmarkWidth);
}

void FilesView::mouseDoubleClickEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp) {
                onLineUp();
                return;
        } else if(event->button() == RkMouseEvent::ButtonType::WheelDown) {
                onLineDown();
                return;
        }

        auto line = getLine(event->x(), event->y());
        if (line > -1) {
                selectedFileIndex = offsetIndex + line;
                activateSelectedFile();
        }
}

void FilesView::mouseMoveEvent(RkMouseEvent *event)
{
        if (event->x() > width() - scrollBarWidth) {
                hightlightLine = -1;
                update();
                return;
        }

        auto lineTemp = hightlightLine;
        hightlightLine = getLine(event->x(), event->y());
        auto bookmarkWidth = RK_RC_IMAGE(bookmark_16x16_unpressed).width();
        auto bookmarkTemp = hightlightBookmarkIcon;
        if (isBookmarkArea(event, bookmarkWidth))
                hightlightBookmarkIcon = true;
        else
                hightlightBookmarkIcon = false;

        if (hightlightLine != lineTemp || bookmarkTemp != hightlightBookmarkIcon)
                update();
}

void FilesView::keyPressEvent(RkKeyEvent *event)
{
        if (filesList.empty())
                return;

        if (event->key() == Rk::Key::Key_Down || event->key() == Rk::Key::Key_Up) {
                event->key() == Rk::Key::Key_Down ? selectedFileIndex++ : selectedFileIndex--;
                if (selectedFileIndex < 0)
                        selectedFileIndex = filesList.size() - 1;
                else if (static_cast<size_t>(selectedFileIndex) > filesList.size() - 1)
                        selectedFileIndex = 0;
                if (selectedFileIndex < offsetIndex
                    || static_cast<size_t>(selectedFileIndex) > offsetIndex + visibleLines - 1) {
                        offsetIndex = selectedFileIndex;
                }
                update();
                updateScrollBar();
                action fileSelected(selectedFile());
                return;
        }

        if (event->key() == Rk::Key::Key_Return)
                activateSelectedFile();
}

fs::path FilesView::selectedFile() const
{
        if (filesList.empty())
                return {};

        if (selectedFileIndex >= 0
            && static_cast<decltype(filesList.size())>(selectedFileIndex) < filesList.size()) {
                return filesList[selectedFileIndex];
        }
        return {};
}

void FilesView::activateSelectedFile()
{
        auto file = selectedFile();
        if (!file.empty()) {
                if (!std::filesystem::is_directory(file))
                        action fileActivated(file);
                else
                        loadCurrentDirectory();
                update();
        }
}

int FilesView::getLine(int x, int y) const
{
        if (x > 0 && x < width() - scrollBarWidth && y > 0 && y < height()) {
                decltype(filesList.size()) line = y / (lineHeight + lineSacing);
                if (line <= filesList.size() - static_cast<decltype(filesList.size())>(offsetIndex + 1))
                        return line;
        }

        return -1;
}

void FilesView::onLineUp()
{
        offsetIndex--;
        if (offsetIndex < 0)
                offsetIndex = 0;
        updateScrollBar();
        update();
}

void FilesView::onLineDown()
{
        if (offsetIndex + visibleLines < filesList.size())
                offsetIndex++;
        updateScrollBar();
        update();
}

void FilesView::setFilters(const std::vector<std::string> &filters)
{
        fileFilters = filters;
        if (!fileFilters.empty())
                setCurrentFileExtension(fileFilters.front());
}

void FilesView::showFolders(bool b)
{
        isShowFolders = b;
}

bool FilesView::getIsShowFolders() const
{
        return isShowFolders;
}

void FilesView::setCurrentFileExtension(const fs::path &ext)
{
        currentFileExtension = ext;
}

const fs::path& FilesView::getCurrentFileExtension() const
{
        return currentFileExtension;
}

void FilesView::setBookmarksModel(PathBookmarksModel *model)
{
        if (bookmarksModel)
                unbindObject(bookmarksModel);
        bookmarksModel = model;
        RK_ACT_BIND(bookmarksModel, modelUpdated, RK_ACT_ARGS(), this, update());
}

PathBookmarksModel* FilesView::getBookmarksModel() const
{
        return bookmarksModel;
}

void FilesView::addNewPath()
{
        createEditPathControl(FileActions::CreateDirectory);
}

void FilesView::createFile()
{
        createEditPathControl(FileActions::CreateFile);
}

bool FilesView::createPath(const fs::path &path)
{
        try {
                if (fs::exists(path))
                        return true;

                if (fs::create_directory(path))
                        return true;
        } catch (const fs::filesystem_error& e) {
                return false;
        }

        return false;
}

void FilesView::createEditPathControl(FilesView::FileActions act)
{
        if (newPathEdit)
                newPathEdit->close();

        newPathEdit = new RkLineEdit(this);
        newPathEdit->setPosition(4, 2);
        newPathEdit->setSize(width() - 40, 20);
        newPathEdit->show();
        newPathEdit->setFocus();

        auto processEditEvent = [act, this]() -> void {
                if (newPathEdit->text().empty()) {
                        if (!filesList.empty())
                                filesList.erase(filesList.begin());
                        newPathEdit->close();
                        newPathEdit = nullptr;
                        return;
                }
                auto newPath = getCurrentPath() / fs::path(newPathEdit->text());
                if (act == FileActions::CreateFile && !hasValidExtension(newPath))
                        newPath += getCurrentFileExtension();

                if (act == FilesView::FileActions::CreateFile)
                        action onCreateFile(newPath);
                else
                        createPath(newPath);

                newPathEdit->close();
                newPathEdit = nullptr;
                selectedFileIndex = -1;
                loadCurrentDirectory();
        };

        RK_ACT_BINDL(newPathEdit,
                     escapePressed,
                     RK_ACT_ARGS(),
                     [=,this](){
                             newPathEdit->close();
                             newPathEdit = nullptr;
                             if (!filesList.empty())
                                     filesList.erase(filesList.begin());
                     });

        RK_ACT_BINDL(newPathEdit,
                     editingFinished,
                     RK_ACT_ARGS(),
                     processEditEvent);

        filesList.insert(filesList.begin(), fs::path());
        selectedFileIndex = 0;
        update();
}

bool FilesView::hasValidExtension(const fs::path file) const
{
        auto ext = file.extension().string();
        return !ext.empty() &&
                std::ranges::any_of(fileFilters, [&](const std::string& validExt) {
                        return ext == validExt;
                });
}

