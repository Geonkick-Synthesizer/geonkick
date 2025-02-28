/**
 * File name: FilesView.cpp
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

#include "FilesView.h"
#include "geonkick_button.h"
#include "geonkick_slider.h"
#include "PathBookmarksModel.h"

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
{
        setSize(parent->size() - RkSize{0, 21});
        setBackgroundColor(40, 40, 40);
        createScrollBar();
        showScrollBar(false);
        show();
}

void FilesView::setSize(const RkSize &size)
{
        GeonkickWidget::setSize(size - RkSize{0, 21});
        visibleLines = height() / (lineHeight + lineSacing);
        showScrollBar(filesList.size() > visibleLines);
        updateScrollBarView();
}

std::string FilesView::getCurrentPath() const
{
        return currentPath.string();
}

void FilesView::setCurrentPath(const std::string &path)
{
        if (currentPath == path)
                return;

        currentPath = path;
        selectedFileIndex = -1;
        loadCurrentDirectory();
        update();
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
        if (selectedFileIndex > -1
            && static_cast<size_t>(selectedFileIndex) < filesList.size()) {
                currentPath = filesList[selectedFileIndex];
        }

        if (!std::filesystem::is_directory(currentPath))
                return;

        decltype(filesList) files;
        decltype(filesList) dirs;
        try {
                for (const auto &entry : std::filesystem::directory_iterator(currentPath)) {
			if (entry.path().empty())
				continue;
                        if (std::filesystem::is_directory(entry.path())) {
                                dirs.emplace_back(entry.path());
                        } else if (std::find(fileFilters.begin(), fileFilters.end(), entry.path().extension())
                                   != fileFilters.end()) {
                                files.emplace_back(entry.path());
                        }
                }
                filesList = files;
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading directory");
                files.clear();
        }

        if (!files.empty()) {
                std::sort(files.begin(), files.end(),
                          [] (decltype(files)::value_type &a, decltype(files)::value_type &b) -> bool
                        {
                                        return a < b;
                        });
        }

        if (!dirs.empty()) {
                std::sort(dirs.begin(), dirs.end(),
                          [] (decltype(dirs)::value_type &a, decltype(dirs)::value_type &b) -> bool
                        {
                                        return a < b;
                        });
        }

        filesList = std::move(dirs);
        filesList.insert(filesList.end(), files.begin(), files.end());

        offsetIndex = 0;
        selectedFileIndex = 0;
        showScrollBar(filesList.size() > visibleLines);
        action currentPathChanged(currentPath.string());
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
        std::string path = getSelectedFile();
        auto bookmarkWidth = RK_RC_IMAGE(bookmark_16x16_hover).width();
        if (bookmarksModel && isBookmarkArea(event, bookmarkWidth)
            && std::filesystem::is_directory(path)) {
                if (bookmarksModel->containsPath(path))
                        bookmarksModel->removePath(path);
                else
                        bookmarksModel->addPath(path);
        } else if (!std::filesystem::is_directory(path)) {
                action currentFileChanged(path);
                update();
        }
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
                openSelectedFile();
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
        if (!filesList.empty() && (event->key() == Rk::Key::Key_Down || event->key() == Rk::Key::Key_Up)) {
                event->key() == Rk::Key::Key_Down ? selectedFileIndex++ : selectedFileIndex--;
                if (selectedFileIndex > -1 && static_cast<decltype(filesList.size())>(selectedFileIndex) > filesList.size() - 1)
                        selectedFileIndex = filesList.size() - 1;
                else if (selectedFileIndex < 0)
                        selectedFileIndex = 0;
                if (selectedFileIndex < offsetIndex
                    || static_cast<decltype(filesList.size())>(selectedFileIndex) > offsetIndex + visibleLines - 1) {
                        offsetIndex = selectedFileIndex;
                }
                update();
                updateScrollBar();
                return;
        }

        if (event->key() == Rk::Key::Key_Return)
                openSelectedFile();
}

std::string FilesView::getSelectedFile() const
{
        if (!filesList.empty() && selectedFileIndex > -1
            && static_cast<decltype(filesList.size())>(selectedFileIndex) < filesList.size()
	    && !filesList[selectedFileIndex].empty()) {
                return filesList[selectedFileIndex].string();
        }
        return "";
}

void FilesView::openSelectedFile()
{
        std::string file = getSelectedFile();
        if (!file.empty()) {
                if (!std::filesystem::is_directory(file))
                        action openFile(filesList[selectedFileIndex].string());
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

std::string FilesView::selectedFile() const
{
        if (selectedFileIndex > -1)
                return filesList[selectedFileIndex].string();
        return std::string();
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
