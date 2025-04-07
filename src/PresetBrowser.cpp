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

#include <RkContainer.h>

PresetBrowser::PresetBrowser(GeonkickWidget *parent, KitModel* model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , geonkickConfig{std::make_unique<GeonkickConfig>(true)}
        , fileBrowser{nullptr}
{
        setSize(306, parent->height() - 30);

        fileBrowser = new FileBrowser(this, "Presets");
        fileBrowser->setSize({width(), height() - 25});
        fileBrowser->setFilters({".gkit"});
        fileBrowser->setCurrentDirectoy(geonkickConfig->getPresetCurrentPath());
        RK_ACT_BINDL(fileBrowser,
                     currentPathChanged,
                     RK_ACT_ARGS(const std::string &path),
                     [=,this](const std::string &path) {
                             geonkickConfig->setPresetCurrentPath(path);
                     });
        RK_ACT_BIND(fileBrowser,
                    fileActivated,
                    RK_ACT_ARGS(const fs::path &file),
                    kitModel,
                    loadPreset(file));
        RK_ACT_BIND(fileBrowser,
                    onCreateFile,
                    RK_ACT_ARGS(const fs::path &filePath),
                    kitModel,
                    save(filePath));

        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size());
        mainLayout->addWidget(fileBrowser);
        show();
}
