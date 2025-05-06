/**
 * File name: PresetBrowser.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#include "PresetBrowser.h"
#include "GeonkickConfig.h"
#include "FileBrowser.h"
#include "kit_model.h"
#include "percussion_model.h"
#include "preset.h"
#include "DesktopPaths.h"
#include "PathBookmarksModel.h"
#include "FilesView.h"

#include <RkContainer.h>

PresetBrowser::PresetBrowser(GeonkickWidget *parent, KitModel* model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , geonkickConfig{std::make_unique<GeonkickConfig>(true)}
        , fileBrowser{nullptr}
{
        setSize(306, parent->height() - 30);

        // Preset Folders
        fileBrowser = new FileBrowser(this, "Presets Folders", true);
        fileBrowser->setSize({width(), height() / 2});
        fileBrowser->setCurrentDirectoy(geonkickConfig->getPresetCurrentPath());
        fileBrowser->getBookmarks()->addPath(DesktopPaths().getPresetsPath());
        RK_ACT_BINDL(fileBrowser,
                     currentPathChanged,
                     RK_ACT_ARGS(const std::string &path),
                     [=,this](const std::string &path) {
                             geonkickConfig->setPresetCurrentPath(path);
                     });

        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size());
        mainLayout->addWidget(fileBrowser);

        // Presets
        mainLayout->addSpace(5);
        auto filesView = new FilesView(this);
        filesView->showFolders(false);
        filesView->setSize({width(), height() / 2});
        filesView->setFilters({".gkit", /* for backward compatibility */ ".gkick"});
        filesView->setCurrentPath(geonkickConfig->getPresetCurrentPath());
        RK_ACT_BIND(fileBrowser,
                    currentPathChanged,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(fileBrowser,
                    folderSelected,
                    RK_ACT_ARGS(const fs::path &path),
                    filesView,
                    setCurrentPath(path));
        RK_ACT_BIND(filesView,
                    fileActivated,
                    RK_ACT_ARGS(const fs::path &file),
                    kitModel,
                    loadPreset(file));
        RK_ACT_BIND(filesView,
                    onCreateFile,
                    RK_ACT_ARGS(const fs::path &filePath),
                    kitModel,
                    save(filePath));
        RK_ACT_BIND(fileBrowser,
                    createFile,
                    RK_ACT_ARGS(),
                    filesView,
                    createFile());
        filesView->show();
        mainLayout->addWidget(filesView);
        show();
}
