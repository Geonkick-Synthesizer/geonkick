/**
 * File name: file_dialog.cpp
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

#include "file_dialog.h"
#include "geonkick_button.h"
#include "geonkick_slider.h"

#include <RkLabel.h>
#include <RkLineEdit.h>
#include <RkEvent.h>
#include <RkPainter.h>

RK_DECLARE_IMAGE_RC(open_active);
RK_DECLARE_IMAGE_RC(save_active);
RK_DECLARE_IMAGE_RC(cancel);
RK_DECLARE_IMAGE_RC(scrollbar_button_up);
RK_DECLARE_IMAGE_RC(scrollbar_button_down);

FilesView::FilesView(GeonkickWidget *parent)
        : GeonkickWidget(parent)
        , selectedFileIndex{-1}
        , hightlightLine{-1}
        , offsetIndex{-1}
        , currentPath{std::filesystem::current_path()}
        , lineHeight{15}
        , lineSacing{lineHeight / 2}
        , visibleLines{0}
        , topScrollBarButton{nullptr}
        , bottomScrollBarButton{nullptr}
        , scrollBarWidth{12}
        , scrollBar{nullptr}
        , isScrollBarVisible{false}
{
        setFixedSize(parent->width() - 20, parent->height() - 100);
        visibleLines = height() / (lineHeight + lineSacing);
        setPosition(10, 50);
        setBackgroundColor(50, 50, 50);
        setBorderColor(40, 40, 40);
        setBorderWidth(1);
        createScrollBar();
        showScrollBar(false);
        show();
}

std::string FilesView::getCurrentPath() const
{
        return currentPath.string();
}

void FilesView::setCurrentPath(const std::string &path)
{
        currentPath = path;
        loadCurrentDirectory();
}

void FilesView::createScrollBar()
{
        topScrollBarButton = new GeonkickButton(this);
        topScrollBarButton->setUnpressedImage(RkImage(12, 17, RK_IMAGE_RC(scrollbar_button_up)));
        topScrollBarButton->setSize(scrollBarWidth, ((float)3 / 2) * scrollBarWidth);
        topScrollBarButton->setPosition(width() - scrollBarWidth, 0);
        topScrollBarButton->setCheckable(true);
        RK_ACT_BIND(topScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineUp());

        bottomScrollBarButton = new GeonkickButton(this);
        bottomScrollBarButton->setUnpressedImage(RkImage(12, 17, RK_IMAGE_RC(scrollbar_button_down)));
        bottomScrollBarButton->setSize(scrollBarWidth, ((float)3 / 2) * scrollBarWidth);
        bottomScrollBarButton->setPosition(width() - scrollBarWidth, height() -  bottomScrollBarButton->height());
        bottomScrollBarButton->setCheckable(true);
        RK_ACT_BIND(bottomScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineDown());

        scrollBar = new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical);
        scrollBar->setSize(scrollBarWidth, bottomScrollBarButton->y() - topScrollBarButton->y() - topScrollBarButton->height());
        scrollBar->setPosition(topScrollBarButton->x(), topScrollBarButton->y() + topScrollBarButton->height());
        RK_ACT_BIND(scrollBar, valueUpdated, RK_ACT_ARGS(int val), this, scrollBarChanged(val));
}

void FilesView::showScrollBar(bool b)
{
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
                        scrollBar->onSetValue(100 - (double)(offsetIndex) / (filesList.size() - visibleLines) * 100);
        }
}

void FilesView::scrollBarChanged(int val)
{
        val = 100 - val;
        if (filesList.size() > visibleLines) {
                offsetIndex = ((double)val / 100) * (filesList.size() - visibleLines);
                update();
        }
}

void FilesView::loadCurrentDirectory()
{
        if (selectedFileIndex > -1)
                currentPath = filesList[selectedFileIndex];

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
        if (currentPath.parent_path() != currentPath.root_path()
	    && !currentPath.parent_path().empty())
                filesList.insert(filesList.begin(), currentPath.parent_path());
        else if (!currentPath.root_path().empty())
                filesList.insert(filesList.begin(), currentPath.root_path());

        offsetIndex = 0;
        selectedFileIndex = 0;
        showScrollBar(filesList.size() > visibleLines);
        currentPathChanged(currentPath.string());
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

                if (index == 0)
                        fileName = "[ " + fileName + ".. ]";
                painter.drawText(RkRect(10, lineYPos, width() - 5, lineHeight), fileName, Rk::Alignment::AlignLeft);
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
        } else if(event->button() == RkMouseEvent::ButtonType::WheelDown) {
                onLineDown();
                return;
        }

        auto line = getLine(event->x(), event->y());
        if (line > -1) {
                selectedFileIndex = offsetIndex + line;
                update();
        }
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

        auto line = hightlightLine;
        hightlightLine = getLine(event->x(), event->y());
        if (hightlightLine != line)
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

void FilesView::openSelectedFile()
{
        if (!filesList.empty() && selectedFileIndex > -1
            && static_cast<decltype(filesList.size())>(selectedFileIndex) < filesList.size()
	    && !filesList[selectedFileIndex].empty()) {
                if (!std::filesystem::is_directory(filesList[selectedFileIndex]))
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

FileDialog::FileDialog(GeonkickWidget *parent, FileDialog::Type type, const std::string& title)
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)
        , dialogType{type}
        , filesView{nullptr}
        , pathLabel{nullptr}
        , status{AcceptStatus::Cancel}
{
        setTitle(title);

        setFixedSize(600, 400);
        filesView = new FilesView(this);
        RK_ACT_BIND(filesView, openFile, RK_ACT_ARGS(const std::string &), this, onAccept());
        RK_ACT_BIND(filesView, currentPathChanged, RK_ACT_ARGS(const std::string &pathName),
                    this, onPathChanged(pathName));

        pathLabel = new RkLabel(this, "Path: " + filesView->getCurrentPath());
        pathLabel->setBackgroundColor(background());
        pathLabel->setFixedSize(filesView->width(), 20);
        pathLabel->setPosition(filesView->x(), 15);
        pathLabel->setFont(font());
        pathLabel->setTextColor(textColor());
        pathLabel->show();

        auto acceptButton = new GeonkickButton(this);
        acceptButton->setFixedSize(90, 30);
        acceptButton->setPosition(width() - acceptButton->width() - 10,
                                  height() - acceptButton->height() - 5);
        if (dialogType == Type::Save)
                acceptButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(save_active)));
        else
                acceptButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(open_active)));
        RK_ACT_BIND(acceptButton, toggled, RK_ACT_ARGS(bool pressed), this, onAccept());
        acceptButton->show();

        auto cancelButton = new GeonkickButton(this);
        cancelButton->setFixedSize(90, 30);
        cancelButton->setPosition(acceptButton->x() - cancelButton->width() - 5, acceptButton->y());
        cancelButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(cancel)));
        RK_ACT_BIND(cancelButton, toggled, RK_ACT_ARGS(bool pressed), this, onCancel());
        cancelButton->show();

        if (dialogType == Type::Save) {
                fileNameEdit = new RkLineEdit(this);
                fileNameEdit->setFont(font());
                fileNameEdit->setSize(cancelButton->x() - 20, 20);
                fileNameEdit->setX(filesView->x());
                fileNameEdit->setY(cancelButton->y() + (cancelButton->height() - fileNameEdit->height()) / 2);
                fileNameEdit->show();
                RK_ACT_BIND(fileNameEdit, enterPressed, RK_ACT_ARGS(), this, onAccept());
        }

        show();
}

void FileDialog::onPathChanged(const std::string &pathName)
{
        if (!pathName.empty())
                pathLabel->setText("Path: " + pathName);
}

void FileDialog::onAccept()
{
        status = AcceptStatus::Accept;
        if (dialogType == Type::Open && !filesView->selectedFile().empty()) {
                pathSelected = filesView->selectedFile();
                action selectedFile(pathSelected);
                close();
        } else {
                if (!fileNameEdit->text().empty()) {
                        pathSelected = filesView->getCurrentPath() / std::filesystem::path(fileNameEdit->text());
                        action selectedFile(pathSelected);
                        close();
                } else if (!filesView->selectedFile().empty() && !std::filesystem::is_directory(filesView->selectedFile())) {
                        pathSelected = filesView->selectedFile();
                        action selectedFile(pathSelected);
                        close();
                }
        }
}

void FileDialog::onCancel()
{
        status = AcceptStatus::Cancel;
        close();
}

void FileDialog::closeEvent(RkCloseEvent *event)
{
        status = AcceptStatus::Cancel;
        close();
}

std::string FileDialog::currentDirectory() const
{
        return filesView->getCurrentPath();
}

void FileDialog::setCurrentDirectoy(const std::string &path)
{
        filesView->setCurrentPath(path);
}

std::string FileDialog::filePath() const
{
        return pathSelected;
}

FileDialog::AcceptStatus FileDialog::acceptStatus() const
{
        return status;
}

void FileDialog::setFilters(const std::vector<std::string> &filters)
{
        filesView->setFilters(filters);
}
