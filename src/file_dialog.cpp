/**
 * File name: file_dialog.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://geontime.com)
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

#include <RkLabel.h>
#include <RkLineEdit.h>
#include <RkEvent.h>
#include <RkPainter.h>

extern const unsigned char rk_open_active_png[];
extern const unsigned char rk_save_active_png[];
extern const unsigned char rk_cancel_png[];
extern const unsigned char rk_scrollbar_button_up_png[];
extern const unsigned char rk_scrollbar_button_down_png[];

#define act

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
{
        setFixedSize(parent->width() - 20, parent->height() - 100);
        visibleLines = height() / (lineHeight + lineSacing);
        setPosition(10, 50);
        setBackgroundColor(50, 50, 50);
        setBorderColor(40, 40, 40);
        setBorderWidth(1);

        createScrollBar();
        showScrollBar(false);
        loadCurrentDirectory();
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
        topScrollBarButton->setUnpressedImage(RkImage(12, 17, rk_scrollbar_button_up_png));
        topScrollBarButton->setSize(scrollBarWidth, ((float)3 / 2) * scrollBarWidth);
        topScrollBarButton->setPosition(width() - scrollBarWidth, 0);
        topScrollBarButton->setCheckable(true);
        RK_ACT_BIND(topScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineUp());

        bottomScrollBarButton = new GeonkickButton(this);
        bottomScrollBarButton->setUnpressedImage(RkImage(12, 17, rk_scrollbar_button_down_png));
        bottomScrollBarButton->setSize(scrollBarWidth, ((float)3 / 2) * scrollBarWidth);
        bottomScrollBarButton->setPosition(width() - scrollBarWidth, height() -  bottomScrollBarButton->height());
        bottomScrollBarButton->setCheckable(true);
        RK_ACT_BIND(bottomScrollBarButton, toggled, RK_ACT_ARGS(bool b), this, onLineDown());
}

void FilesView::showScrollBar(bool b)
{
        if (b) {
                topScrollBarButton->show();
                bottomScrollBarButton->show();
        } else {
                topScrollBarButton->hide();
                bottomScrollBarButton->hide();
        }
}

void FilesView::loadCurrentDirectory()
{
        if (selectedFileIndex > -1)
                currentPath = filesList[selectedFileIndex];

        if (!std::filesystem::is_directory(currentPath))
                return;

        decltype(filesList) files;
        try {
                for (const auto &entry : std::filesystem::directory_iterator(currentPath)) {
                        if (std::filesystem::is_directory(entry.path())
                            || entry.path().extension() == ".gkick" || entry.path().extension() == ".GKICK")
                                files.emplace_back(entry.path());
                }
                filesList = files;
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading directory");
                files.clear();
        }

        if (!filesList.empty())
                std::sort(filesList.begin(), filesList.end(),
                          [] (decltype(filesList)::value_type &a, decltype(filesList)::value_type &b) -> bool
                        {
                                if (std::filesystem::is_directory(b)
                                    && !std::filesystem::is_directory(a))
                                        return false;
                                else if (!std::filesystem::is_directory(b)
                                             && std::filesystem::is_directory(a))
                                        return true;
                                else
                                        return a > b; // TODO: convert to lowercase and compare.
                        });

        if (currentPath.parent_path() != currentPath.root_path())
                filesList.insert(filesList.begin(), currentPath.parent_path());
        else
                filesList.insert(filesList.begin(), currentPath.root_path());

        offsetIndex = 0;
        selectedFileIndex = -1;

        if (filesList.size() > visibleLines)
                showScrollBar(true);
        else
                showScrollBar(false);

        currentPathChanged(currentPath.string());
}

void FilesView::paintWidget(const std::shared_ptr<RkPaintEvent> &event)
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

void FilesView::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
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

void FilesView::mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event)
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
                if (!std::filesystem::is_directory(filesList[selectedFileIndex]))
                        openFile(filesList[selectedFileIndex].string());
                else
                        loadCurrentDirectory();
                update();
        }
}

void FilesView::mouseMoveEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        auto line = hightlightLine;
        hightlightLine = getLine(event->x(), event->y());
        if (hightlightLine != line)
                update();
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
        update();
}

void FilesView::onLineDown()
{
        if (offsetIndex + visibleLines < filesList.size() - 1)
                offsetIndex++;
        update();
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
                acceptButton->setUnpressedImage(RkImage(90, 30, rk_save_active_png));
        else
                acceptButton->setUnpressedImage(RkImage(90, 30, rk_open_active_png));
        RK_ACT_BIND(acceptButton, toggled, RK_ACT_ARGS(bool pressed), this, onAccept());
        acceptButton->show();

        auto cancelButton = new GeonkickButton(this);
        cancelButton->setFixedSize(90, 30);
        cancelButton->setPosition(acceptButton->x() - cancelButton->width() - 5, acceptButton->y());
        cancelButton->setUnpressedImage(RkImage(90, 30, rk_cancel_png));
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
        if (dialogType == Type::Open) {
                if (!filesView->selectedFile().empty())
                        pathSelected = filesView->selectedFile();
        } else {
                if (filesView->selectedFile().empty() || !fileNameEdit->text().empty())
                        pathSelected = filesView->getCurrentPath() / std::filesystem::path(fileNameEdit->text());
                else
                        pathSelected = filesView->selectedFile();
        }

        selectedFile(pathSelected);
        close();
}

void FileDialog::onCancel()
{
        status = AcceptStatus::Cancel;
        close();
}

void FileDialog::closeEvent(const std::shared_ptr<RkCloseEvent> &event)
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

