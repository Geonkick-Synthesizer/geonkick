/**
 * File name: preset_browser_model.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "preset_browser_model.h"
#include "preset_folder.h"
#include "preset.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "kit_state.h"

PresetBrowserModel::PresetBrowserModel(RkObject *parent, GeonkickApi *api)
        : RkObject(parent)
        , geonkickApi{api}
        , pageIndex{0}
        , numberOfColumns{4}
        , presetPerColumn{12}
        , selectedFolder{geonkickApi->getPresetFolder(0)}
        , selectedPreset{nullptr}
{
}

std::string PresetBrowserModel::presetName(int row, int column) const
{
        if (column == 0) {
                auto presetFolder = geonkickApi->getPresetFolder(row);
                if (presetFolder)
                        return presetFolder->name();
        } else if (column) {
                auto preset = getPreset(row, column);
                if (preset)
                        return preset->name();
        }
        return "";
}

Preset* PresetBrowserModel::getPreset(int row, int column) const
{
        if (column > 0 && selectedFolder) {
                return selectedFolder->preset(page() * (numberOfColumns - 1) + (column - 1) * presetPerColumn + row);
        } else {
                return nullptr;
        }
}


size_t PresetBrowserModel::page() const
{
        return pageIndex;
}

void PresetBrowserModel::nextPage()
{
        pageIndex++;
}

void PresetBrowserModel::previousPage()
{
        if (pageIndex > 0)
                pageIndex--;
}

void PresetBrowserModel::setPage(size_t index)
{
        if (index >=0)
                pageIndex = index;
}

size_t PresetBrowserModel::columns() const
{
        return numberOfColumns;
}

size_t PresetBrowserModel::rows() const
{
        return presetPerColumn;
}

void PresetBrowserModel::select(size_t row, size_t column)
{
        GEONKICK_LOG_DEBUG("row = " << row << ", column = " << column);
        if (column == 0 && geonkickApi->getPresetFolder(row)) {
                selectedFolder = geonkickApi->getPresetFolder(row);
                selectedPreset = nullptr;
                if (selectedFolder)
                        action folderSelected(selectedFolder);
        } if (column > 0 && selectedFolder) {
                selectedPreset = getPreset(row, column);
                if (selectedPreset && setPreset(selectedPreset))
                        action presetSelected(selectedPreset);
        }
}

bool PresetBrowserModel::isSelected(size_t row, size_t column) const
{
        if (column == 0)
                return geonkickApi->getPresetFolder(row) == selectedFolder;
        else if (column > 0)
                return getPreset(row, column) == selectedPreset;
        else
                return false;
}

bool PresetBrowserModel::setPreset(Preset* preset)
{
        if (preset->type() == Preset::PresetType::Percussion) {
                GEONKICK_LOG_DEBUG("path:" << preset->path());
                auto state = geonkickApi->getDefaultPercussionState();
                if (!state->loadFile(preset->path())) {
                        GEONKICK_LOG_ERROR("can't open preset");
                        return false;
                } else {
                        state->setId(geonkickApi->currentPercussion());
                        geonkickApi->setPercussionState(state);
                        geonkickApi->notifyUpdateGui();
                        GEONKICK_LOG_DEBUG("update percussion :" << state->getId());
                        geonkickApi->notifyPercussionUpdated(state->getId());
                        return true;
                }
        } else if (preset->type() == Preset::PresetType::PercussionKit) {
                auto kit = std::make_unique<KitState>();
                if (kit->open(preset->path())) {
                        GEONKICK_LOG_ERROR("can't open kit");
                        return false;
                } else if (geonkickApi->setKitState(kit)) {
                        geonkickApi->notifyKitUpdated();
                        geonkickApi->notifyUpdateGui();
                        return true;
                }
        }
        return false;
}
