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
#include "PathBookmarksModel.h"
#include "GeonkickConfig.h"
#include "PathHistory.h"
#include "PathBookmarksView.h"

#include <RkLabel.h>
#include <RkEvent.h>
#include <RkPainter.h>
#include <RkList.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(file_history_button_back);
RK_DECLARE_IMAGE_RC(file_history_button_back_pressed);
RK_DECLARE_IMAGE_RC(file_history_button_back_hover);
RK_DECLARE_IMAGE_RC(file_history_button_back_disabled);
RK_DECLARE_IMAGE_RC(file_history_button_forward);
RK_DECLARE_IMAGE_RC(file_history_button_forward_pressed);
RK_DECLARE_IMAGE_RC(file_history_button_forward_hover);
RK_DECLARE_IMAGE_RC(file_history_button_forward_disabled);
RK_DECLARE_IMAGE_RC(save_button_16x16);
RK_DECLARE_IMAGE_RC(save_button_hover_16x16);
RK_DECLARE_IMAGE_RC(save_button_active_16x16);
RK_DECLARE_IMAGE_RC(add_button_16x16);
RK_DECLARE_IMAGE_RC(bookmark_16x16_unpressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_pressed);
RK_DECLARE_IMAGE_RC(bookmark_16x16_hover);

FileBrowser::FileBrowser(GeonkickWidget *parent,
                         const std::string &name,
                         bool saveAction)
        : GeonkickWidget(parent)
        , mainContainer{nullptr}
        , bookmarkDirectoryButton{nullptr}
        , pathHistory{new PathHistory(this)}
        , pathBookmarksModel{new PathBookmarksModel(this, name)}
        , bookmarksView{nullptr}
        , breadcrumbBar{nullptr}
        , filesView{nullptr}
        , saveButtonAction{saveAction}
{
        setSize(parent->size());
        createUi();
        show();
}

void FileBrowser::setSize(const RkSize &size)
{
        GeonkickWidget::setSize(size);
        if (filesView)
                filesView->setSize(size);
        if (mainContainer)
                mainContainer->setSize(size);
}

void FileBrowser::createUi()
{
        mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->setSize(size());
        mainContainer->addSpace(8);

        auto topContainer = new RkContainer(this);
        topContainer->setSize({mainContainer->width(), 16});
        mainContainer->addContainer(topContainer);
        mainContainer->addSpace(5);

        // Create top menu.
        createTopMenu(topContainer);

        // Create bookmarsk bar.
        bookmarksView = new PathBookmarksView(this, pathBookmarksModel);
        mainContainer->addWidget(bookmarksView);
        RK_ACT_BIND(bookmarksView,
                    sizeUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());

        // Create breadcrumb bar.
        mainContainer->addSpace(5);
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
        filesView->setBookmarksModel(pathBookmarksModel);
        RK_ACT_BIND(pathHistory,
                    pathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    pathHistory,
                    goTo(path));
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const fs::path& path),
                    this,
                    currentPathChanged(path));
        RK_ACT_BIND(filesView,
                    fileActivated,
                    RK_ACT_ARGS(const fs::path& path),
                    this,
                    fileActivated(path));
        RK_ACT_BIND(filesView,
                    folderSelected,
                    RK_ACT_ARGS(const fs::path& path),
                    this,
                    folderSelected(path));
        RK_ACT_BIND(filesView,
                    fileSelected,
                    RK_ACT_ARGS(const fs::path& path),
                    this,
                    fileSelected(path));
        RK_ACT_BIND(bookmarksView,
                    pathSelected,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(breadcrumbBar,
                    pathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    breadcrumbBar,
                    setPath(path));
        mainContainer->addWidget(filesView);

        pathHistory->goTo(filesView->getCurrentPath());

        createNewDirectoryControls(topContainer);
        createBookmarkDirectoryControls(topContainer);
        createSaveControls(topContainer);

        updateBookmarkButton(filesView->getCurrentPath());
        updateView();
}

void FileBrowser::createTopMenu(RkContainer *container)
{
        // Backward button.
        container->addSpace(4);
        auto backwardButton = new GeonkickButton(this);
        backwardButton->setType(GeonkickButton::ButtonType::ButtonPush);
        backwardButton->setSize(RK_IMAGE_SIZE_RC(file_history_button_back));
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_history_button_back)),
                         RkButton::State::Unpressed);
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_history_button_back_pressed)),
                         RkButton::State::Pressed);
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_history_button_back_hover)),
                         RkButton::State::PressedHover);
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_history_button_back_hover)),
                         RkButton::State::UnpressedHover);
        backwardButton->setImage(RkImage(backwardButton->size(),
                         RK_IMAGE_RC(file_history_button_back_disabled)),
                         RkButton::State::Disabled);
        RK_ACT_BIND(backwardButton, pressed, RK_ACT_ARGS(), pathHistory, goBack());
        RK_ACT_BIND(pathHistory,
                    backwardHistoryUpdated,
                    RK_ACT_ARGS(bool hasHistory),
                    backwardButton,
                    setEnabled(hasHistory));
        container->addWidget(backwardButton);

        // Forward button.
        container->addSpace(2);
        auto forwardButton = new GeonkickButton(this);
        forwardButton->setType(GeonkickButton::ButtonType::ButtonPush);
        forwardButton->setSize(RK_IMAGE_SIZE_RC(file_history_button_forward));
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_history_button_forward)),
                         RkButton::State::Unpressed);
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_history_button_forward_pressed)),
                         RkButton::State::Pressed);
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_history_button_forward_hover)),
                         RkButton::State::PressedHover);
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_history_button_forward_hover)),
                         RkButton::State::UnpressedHover);
        forwardButton->setImage(RkImage(forwardButton->size(),
                         RK_IMAGE_RC(file_history_button_forward_disabled)),
                         RkButton::State::Disabled);
        RK_ACT_BIND(forwardButton, pressed, RK_ACT_ARGS(), pathHistory, goForward());
        RK_ACT_BIND(pathHistory,
                    forwardHistoryUpdated,
                    RK_ACT_ARGS(bool hasHistory),
                    forwardButton, setEnabled(hasHistory));
        container->addWidget(forwardButton);
}

void FileBrowser::createBookmarkDirectoryControls(RkContainer *container)
{
        container->addSpace(5);
        bookmarkDirectoryButton = new GeonkickButton(this);
        bookmarkDirectoryButton->setCheckable(true);
        bookmarkDirectoryButton->setSize(14, 13);
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
        RK_ACT_BINDL(bookmarkDirectoryButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    [=,this](bool b){
                            if (b)
                                    pathBookmarksModel->addPath(currentDirectory());
                            else
                                    pathBookmarksModel->removePath(currentDirectory());
                     } );
        RK_ACT_BIND(filesView,
                    currentPathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    this,
                    updateBookmarkButton(path));
}

void FileBrowser::createNewDirectoryControls(RkContainer *container)
{
        container->addSpace(5);
        auto createDirectoryButton = new GeonkickButton(this);
        createDirectoryButton->setSize(16, 16);
        createDirectoryButton->setImage(RkImage(createDirectoryButton->size(),
                                                RK_IMAGE_RC(add_button_16x16)),
                                        RkButton::State::Unpressed);
        createDirectoryButton->show();
        container->addWidget(createDirectoryButton);
        RK_ACT_BIND(createDirectoryButton, pressed, RK_ACT_ARGS(), filesView, addNewPath());
}

void FileBrowser::createSaveControls(RkContainer *container)
{
        container->addSpace(5);
        auto saveButton = new GeonkickButton(this);
        saveButton->setType(RkButton::ButtonType::ButtonPush);
        saveButton->setSize(16, 16);
        saveButton->setImage(RK_RC_IMAGE(save_button_16x16),
                             RkButton::State::Unpressed);
        saveButton->setImage(RK_RC_IMAGE(save_button_hover_16x16),
                             RkButton::State::UnpressedHover);
        saveButton->setImage(RK_RC_IMAGE(save_button_hover_16x16),
                             RkButton::State::PressedHover);
        saveButton->setImage(RK_RC_IMAGE(save_button_active_16x16),
                             RkButton::State::Pressed);
        saveButton->show();
        container->addWidget(saveButton);
        if (saveButtonAction) {
                RK_ACT_BIND(saveButton, pressed, RK_ACT_ARGS(), this, createFile());
        } else {
                RK_ACT_BIND(saveButton, pressed, RK_ACT_ARGS(), filesView, createFile());
                RK_ACT_BIND(filesView,
                            onCreateFile,
                            RK_ACT_ARGS(const fs::path &filePath),
                            this,
                            onCreateFile(filePath));
        }
}

void FileBrowser::updateView()
{
        if (breadcrumbBar->size().isEmpty() || bookmarksView->size().isEmpty())
                return;

        filesView->setSize({filesView->size().width(),
                        height() - 8
                        - breadcrumbBar->height() - 5
                        - bookmarksView->height() - 5});
        mainContainer->update();
}

fs::path FileBrowser::currentDirectory() const
{
        return filesView->getCurrentPath();
}

fs::path FileBrowser::currentFile() const
{
        return filesView->selectedFile();
}

void FileBrowser::setCurrentDirectoy(const fs::path &path)
{
        filesView->setCurrentPath(path);
}

void FileBrowser::setFilters(const std::vector<std::string> &filters)
{
        filesView->setFilters(filters);
}

void FileBrowser::setHomeDirectory(const fs::path &path)
{
        filesView->setCurrentPath(path);
}

bool FileBrowser::createDirectory(const fs::path &dir)
{
        if (filesView)
                return filesView->createPath(dir);
        return false;
}

PathBookmarksModel* FileBrowser::getBookmarks() const
{
        return pathBookmarksModel;
}

void FileBrowser::updateBookmarkButton(const fs::path &path)
{
        bookmarkDirectoryButton->setPressed(pathBookmarksModel->containsPath(path));
}
