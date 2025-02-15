/**
 * File name: FileBrowser.cpp
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

#include "FileBrowser.h"
#include "BreadcrumbBar.h"
#include "FilesView.h"
#include "geonkick_button.h"
#include "PathListModel.h"
#include "GeonkickConfig.h"
#include "PathHistory.h"

#include <RkLabel.h>
#include <RkLineEdit.h>
#include <RkEvent.h>
#include <RkPainter.h>
#include <RkList.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(file_browser_back);
RK_DECLARE_IMAGE_RC(file_browser_back_pressed);
RK_DECLARE_IMAGE_RC(file_browser_back_hover);
RK_DECLARE_IMAGE_RC(file_browser_forward);
RK_DECLARE_IMAGE_RC(file_browser_forward_pressed);
RK_DECLARE_IMAGE_RC(file_browser_forward_hover);
RK_DECLARE_IMAGE_RC(save_active);
RK_DECLARE_IMAGE_RC(open_active);
RK_DECLARE_IMAGE_RC(save_active);
RK_DECLARE_IMAGE_RC(cancel);
RK_DECLARE_IMAGE_RC(add_button_16x16);
RK_DECLARE_IMAGE_RC(bookmark_16x16_unpressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_pressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_hover);

FileBrowser::FileBrowser(GeonkickWidget *parent,
                       FileBrowser::Type type,
                       Rk::WidgetFlags flags,
                       const std::string& title)
        : GeonkickWidget(parent, flags)
        , mainContainer{nullptr}
        , dialogType{type}
        , fileNameEdit{nullptr}
        , pathHistory{new PathHistory(this)}
        , breadcrumbBar{nullptr}
        , filesView{nullptr}
        , status{AcceptStatus::Cancel}
        , shortcutDirectoriesModel{new PathListModel(this)}
          //        , shortcutDirectoriesView{new RkList(this, shortcutDirectoriesModel)}
        , backButton{nullptr}
        , forwardButton{nullptr}
        , bookmarkDirectoryButton{nullptr}
{
        setSize(parent->size());
        setTitle(title);
        createUi();
        show();
}

FileBrowser::FileBrowser(GeonkickWidget *parent,
                       FileBrowser::Type type,
                       const std::string& title)
        : GeonkickWidget(parent, type == FileBrowser::Type::Browse ? Rk::WidgetFlags::Widget : Rk::WidgetFlags::Popup)
        , mainContainer{nullptr}
        , dialogType{type}
        , fileNameEdit{nullptr}
        , pathHistory{new PathHistory(this)}
        , breadcrumbBar{nullptr}
        , filesView{nullptr}
        , status{AcceptStatus::Cancel}
        , shortcutDirectoriesModel{new PathListModel(this)}
        //        , shortcutDirectoriesView{new RkList(this, shortcutDirectoriesModel)}
        , bookmarkDirectoryButton{nullptr}
{
        setSize(parent->size());
        setTitle(title);
        //        setBackgroundColor(88, 0, 0xff);
        createUi();
        show();
}

void FileBrowser::setSize(const RkSize &size)
{
        GeonkickWidget::setSize(size);
        if (filesView)
                filesView->setSize(size);
}

void FileBrowser::createUi()
{
        /*if (widgetFlags() == Rk::WidgetFlags::Popup) {
                setBorderWidth(2);
                setBorderColor(80, 80, 80);
        }
        GeonkickConfig cfg;
        for(const auto &path: cfg.getBookmarkedPaths())
        shortcutDirectoriesModel->addPath(path);*/

        mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->setSize(size());
        mainContainer->addSpace(8);

        auto topContainer = new RkContainer(this);
        topContainer->setSize({mainContainer->width(), 20});
        mainContainer->addContainer(topContainer);

        // Create top menu.
        createTopMenu(topContainer);

        // Create breadcrumb bar.
        breadcrumbBar = new BreadcrumbBar(this);
        mainContainer->addWidget(breadcrumbBar);
        RK_ACT_BIND(breadcrumbBar,
                    sizeUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());

        // Create files view.
        mainContainer->addSpace(5);
        filesView = new FilesView(this);
        RK_ACT_BIND(fileHistory,
                    pathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    fileView,
                    setCurrentPath(path));
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const std::string &path),
                    pathHistory,
                    goTo(path));
        RK_ACT_BIND(breadcrumbBar,
                    pathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const std::string &path),
                    breadcrumbBar,
                    setPath(path));
        mainContainer->addWidget(filesView);

        pathHistory->gotTo(filesView->getCurrentPath());

        /*RK_ACT_BIND(filesView, openFile, RK_ACT_ARGS(const std::string &), this, onAccept());
        RK_ACT_BIND(filesView, currentFileChanged, RK_ACT_ARGS(const std::string &file),
                    this, currentFileChanged(file));
        RK_ACT_BIND(filesView, currentPathChanged, RK_ACT_ARGS(const std::string &pathName),
                    this, directoryChanged(pathName));
        RK_ACT_BIND(shortcutDirectoriesModel,
                    itemSelected,
                    RK_ACT_ARGS(RkModelItem item),
                    filesView,
                    setCurrentPath(std::get<std::string>(item.data(static_cast<int>(PathListModel::PathListDataType::Path)))));*/

        /*        shortcutDirectoriesView->setBackgroundColor({50, 50, 50});
        shortcutDirectoriesView->setPosition(2, filesView->y());
        shortcutDirectoriesView->setSize(100 ,filesView->height());
        shortcutDirectoriesView->show();*/

        /*        createBookmarkDirectoryControls(topContainer);
        createNewDirectoryControls(topContainer);

        auto buttomContainer = new RkContainer(this);
        buttomContainer->setSize({mainContainer->width(), 30});
        mainContainer->addSpace(5, Rk::Alignment::AlignBottom);
        mainContainer->addContainer(buttomContainer, Rk::Alignment::AlignBottom);

        if (dialogType != Type::Browse) {
                auto acceptButton = new GeonkickButton(this);
                acceptButton->setFixedSize(90, 30);
                if (dialogType == Type::Save)
                        acceptButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(save_active)));
                else
                        acceptButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(open_active)));
                RK_ACT_BIND(acceptButton, toggled, RK_ACT_ARGS(bool pressed), this, onAccept());
                acceptButton->show();
                buttomContainer->addSpace(10, Rk::Alignment::AlignRight);
                buttomContainer->addWidget(acceptButton, Rk::Alignment::AlignRight);
                auto cancelButton = new GeonkickButton(this);
                cancelButton->setFixedSize(90, 30);
                cancelButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(cancel)));
                RK_ACT_BIND(cancelButton, toggled, RK_ACT_ARGS(bool pressed), this, onCancel());
                cancelButton->show();
                buttomContainer->addSpace(5);
                buttomContainer->addWidget(cancelButton, Rk::Alignment::AlignRight);
        }

        if (dialogType == Type::Save) {
                fileNameEdit = new RkLineEdit(this);
                fileNameEdit->setFont(font());
                fileNameEdit->setSize(width() - 200, 20);
                fileNameEdit->show();
                RK_ACT_BIND(fileNameEdit, enterPressed, RK_ACT_ARGS(), this, onAccept());
                buttomContainer->addWidget(fileNameEdit);
                }*/
        updateView();
}

void FileBrowser::createTopMenu(RkContainer *container)
{
        // Backward button.
        auto backwardButton = new GeonkickButton(this);
        backwardButton->setSize(48, 20);
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_browser_back)),
                         RkButton::State::Unpressed);
        backwardButton->setImage(RkImage(bookmarkDirectoryBackButton->size(),
                         RK_IMAGE_RC(file_browser_back_pressed)),
                         RkButton::State::Pressed);
        backwardButton->setImage(RkImage(bookmarkDirectoryBackButton->size(),
                         RK_IMAGE_RC(file_browser_back_hover)),
                         RkButton::State::PressedHover);
        backwardButton->setImage(RkImage(bookmarkDirectoryBackButton->size(),
                         RK_IMAGE_RC(file_browser_back_hover)),
                         RkButton::State::UnpressedHover);
        RK_ACT_BIND(backwardButton, pressed, RK_ACT_ARGS(), pathHistory, goBack());
        RK_ACT_BIND(pathHistory,
                    backwardHistoryUpdated,
                    RK_ACT_ARGS(bool hasHistory),
                    backwardButton, backwardButton->setEnabled(hasHIstory));
        container->addWidget(backwardButton);

        // Forward button.
        auto forwardButton = new GeonkickForwardButton(this);
        forwardButton->setSize(48, 20);
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_browser_)),
                         RkButton::State::Unpressed);
        forwardButton->setImage(RkImage(bookmarkDirectoryForwardButton->size(),
                         RK_IMAGE_RC(file_browser_forward_pressed)),
                         RkButton::State::Pressed);
        forwardButton->setImage(RkImage(bookmarkDirectoryForwardButton->size(),
                         RK_IMAGE_RC(file_browser_forward_hover)),
                         RkButton::State::PressedHover);
        forwardButton->setImage(RkImage(bookmarkDirectoryForwardButton->size(),
                         RK_IMAGE_RC(file_browser_forward_hover)),
                         RkButton::State::UnpressedHover);
        RK_ACT_BIND(forwardButton, pressed, RK_ACT_ARGS(), pathHistory, goForward());
        RK_ACT_BIND(pathHistory,
                    forwardHistoryUpdated,
                    RK_ACT_ARGS(bool hasHistory),
                    forwardButton, backButton->setEnabled(hasHIstory));
        container->addWidget(forwardButton);
}

void FileBrowser::createBookmarkDirectoryControls(RkContainer *container)
{
        /*        container->addSpace(10);
        bookmarkDirectoryButton = new GeonkickButton(this);
        bookmarkDirectoryButton->setCheckable(true);
        bookmarkDirectoryButton->setSize(16, 16);
        bookmarkDirectoryButton->setImage(RkImage(bookmarkDirectoryButton->size(),
                                                  RK_IMAGE_RC(bookmark_16x16_unpressed)),
                                          RkButton::State::Unpressed);
        bookmarkDirectoryButton->setImage(RkImage(bookmarkDirectoryButton->size(),
                                                  RK_IMAGE_RC(bookmark_16x16_pressed)),
                                          RkButton::State::Pressed);
        bookmarkDirectoryButton->setImage(RkImage(bookmarkDirectoryButton->size(),
                                                  RK_IMAGE_RC(bookmark_16x16_hover)),
                                          RkButton::State::PressedHover);
        bookmarkDirectoryButton->setImage(RkImage(bookmarkDirectoryButton->size(),
                                                  RK_IMAGE_RC(bookmark_16x16_hover)),
                                          RkButton::State::UnpressedHover);
        bookmarkDirectoryButton->show();
        container->addWidget(bookmarkDirectoryButton);
        RK_ACT_BIND(bookmarkDirectoryButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    bookmarkDirectory(currentDirectory(), b));

        RK_ACT_BIND(filesView, currentPathChanged, RK_ACT_ARGS(const std::string &pathName),
        this, updateBookmarkButton(std::filesystem::path(pathName)));*/
}

void FileBrowser::createNewDirectoryControls(RkContainer *container)
{
        /*        container->addSpace(5);
        // Create directory button
        auto createDirectoryButton = new GeonkickButton(this);
        createDirectoryButton->setSize(16, 16);
        createDirectoryButton->setImage(RkImage(createDirectoryButton->size(),
                                                RK_IMAGE_RC(add_button_16x16)),
                                        RkButton::State::Unpressed);
        createDirectoryButton->show();
        container->addWidget(createDirectoryButton);

        auto editDirectoryName = new RkLineEdit(this);
        editDirectoryName->setSize(80, 16);
        container->addWidget(editDirectoryName);
        editDirectoryName->hide();

        RK_ACT_BINDL(createDirectoryButton,
                     pressed,
                     RK_ACT_ARGS(),
                     [=, this](){
                             createDirectoryButton->hide();
                             editDirectoryName->show();
                             container->update();
                             editDirectoryName->setFocus();
                     });

        RK_ACT_BINDL(editDirectoryName,
                     escapePressed,
                     RK_ACT_ARGS(),
                     [=, this](){
                             editDirectoryName->hide();
                             createDirectoryButton->show();
                             editDirectoryName->setText("");
                             container->update();
                     });

        RK_ACT_BINDL(editDirectoryName,
                     enterPressed,
                     RK_ACT_ARGS(),
                     [=, this](){
                             editDirectoryName->hide();
                             createDirectoryButton->show();
                             createDirectory(editDirectoryName->text());
                             editDirectoryName->setText("");
                             container->update();
                     });

        RK_ACT_BINDL(editDirectoryName,
                     editingFinished,
                     RK_ACT_ARGS(),
                     [=, this](){
                             editDirectoryName->hide();
                             createDirectoryButton->show();
                             editDirectoryName->setText("");
                             container->update();
                             });*/
}

void FileBrowser::updateView()
{
        filesView->setHeight(parentWidget()->height() - 8 - breadcrumbBar->height() - 5);
        mainContainer->update();
}

void FileBrowser::goBack()
{
}

void FileBrowser::goForward()
{
}

void FileBrowser::onAccept()
{
        status = AcceptStatus::Accept;
        switch (dialogType) {
        case Type::Open:
                if (!filesView->selectedFile().empty()) {
                        pathSelected = filesView->selectedFile();
                        action selectedFile(pathSelected);
                        close();
                }
                break;
        case Type::Save:
                if (!fileNameEdit->text().empty()) {
                        pathSelected = (filesView->getCurrentPath() / std::filesystem::path(fileNameEdit->text())).string();
                        action selectedFile(pathSelected);
                        close();
                } else if (!filesView->selectedFile().empty()
                           && !std::filesystem::is_directory(filesView->selectedFile())) {
                        pathSelected = filesView->selectedFile();
                        action selectedFile(pathSelected);
                        close();
                }
                break;
        case Type::Browse:
                if (!filesView->selectedFile().empty()) {
                        pathSelected = filesView->selectedFile();
                        action selectedFile(pathSelected);
                }
                break;
        default:
                return;
        }
}

void FileBrowser::onCancel()
{
        status = AcceptStatus::Cancel;
        action rejected();
        close();
}

void FileBrowser::closeEvent(RkCloseEvent *event)
{
        status = AcceptStatus::Cancel;
        RkWidget::closeEvent(event);
}

std::string FileBrowser::currentDirectory() const
{
        return filesView->getCurrentPath();
}

void FileBrowser::setCurrentDirectoy(const std::string &path)
{
        filesView->setCurrentPath(path);
        breadcrumbBar->setPath(path);
}

std::string FileBrowser::filePath() const
{
        return pathSelected;
}

FileBrowser::AcceptStatus FileBrowser::acceptStatus() const
{
        return status;
}

void FileBrowser::setFilters(const std::vector<std::string> &filters)
{
        filesView->setFilters(filters);
}

void FileBrowser::setHomeDirectory(const std::string &path)
{
        shortcutDirectoriesModel->setHomeDirectory(path);
}

bool FileBrowser::createDirectory(const std::filesystem::path &dir)
{
        auto newPath = std::filesystem::path(currentDirectory()) / dir;
        try {
                if (!std::filesystem::create_directory(newPath)) {
                        GEONKICK_LOG_ERROR("Failed to create directory: " << newPath);
                        return false;
                }
        } catch (...) {
                GEONKICK_LOG_ERROR("Failed to create directory: " << newPath);
                return false;
        }

        setCurrentDirectoy(newPath.string());
        return true;
}

void FileBrowser::bookmarkDirectory(const std::filesystem::path &dir, bool bookmark)
{
        /*        if (bookmark) {
                if (!isPathBookmarked(dir)) {
                        GeonkickConfig cfg;
                        auto res = cfg.bookmarkPath(dir);
                        if (res)
                                res = cfg.save();
                        if (res)
                                shortcutDirectoriesModel->addPath(dir);
                }
        } else {
                if (isPathBookmarked(dir)) {
                        GeonkickConfig cfg;
                        auto res = cfg.removeBookmarkedPath(dir);
                        if (res)
                                res = cfg.save();
                        if (res)
                                shortcutDirectoriesModel->removePath(dir);
                }
        }
        updateBookmarkButton(dir);*/
}

bool FileBrowser::isPathBookmarked(const std::filesystem::path &path) const
{
        const auto &paths = shortcutDirectoriesModel->getPaths();
        auto it = std::find(paths.begin(), paths.end(), path);
        return it != paths.end();
}

void FileBrowser::updateBookmarkButton(const std::filesystem::path &path)
{
        //       bookmarkDirectoryButton->setPressed(isPathBookmarked(path));
}
