/**
 * File name: preset_browser_model.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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
#include "GeonkickConfig.h"

PresetBrowserModel::PresetBrowserModel(RkObject *parent, GeonkickApi *api)
        : RkObject(parent)
        , geonkickApi{api}
        , folderPageIndex{0}
        , presetPageIndex{0}
        , numberOfPresetColumns{3}
        , rowsPerColumn{12}
        , folderSelectedRaw{0}
        , presetSelectedRaw{0}
        , presetSelectedColumn{0}
        , selectedFolder{geonkickApi->getPresetFolder(0)}
        , selectedPreset{nullptr}
{
}

std::string PresetBrowserModel::presetName(int row, int column) const
{
        if (column == 0) {
                auto presetFolder = getPresetFolder(row);
                if (presetFolder)
                        return presetFolder->name();
        } else if (column) {
                auto preset = getPreset(row, column);
                if (preset)
                        return preset->name();
        }
        return std::string();
}

PresetFolder* PresetBrowserModel::getPresetFolder(int row) const
{
        return geonkickApi->getPresetFolder(folderPage() * rowsPerColumn + row);
}

Preset* PresetBrowserModel::getPreset(int row, int column) const
{
        if (column > 0 && row >=0 && row < static_cast<int>(rowsPerColumn)
            && selectedFolder) {
                const auto index = presetPage() * numberOfPresetColumns * rowsPerColumn
                        + (column - 1) * rowsPerColumn + row;
                return selectedFolder->preset(index);
        } else {
                return nullptr;
        }
}

size_t PresetBrowserModel::folderPages() const
{
        return geonkickApi->numberOfPresetFolders() / rowsPerColumn
                + ((geonkickApi->numberOfPresetFolders() % rowsPerColumn) ? 1 : 0);
}

size_t PresetBrowserModel::folderPage() const
{
        return folderPageIndex;
}

void PresetBrowserModel::folderNextPage()
{
        if (folderPageIndex < folderPages() - 1) {
                folderPageIndex++;
                action folderPageChanged();
        }
}

void PresetBrowserModel::folderPreviousPage()
{
        if (folderPageIndex > 0) {
                folderPageIndex--;
                action folderPageChanged();
        }
}

void PresetBrowserModel::setFolderPage(size_t index)
{
        folderPageIndex = index;
}

size_t PresetBrowserModel::presetPages() const
{
        if (selectedFolder) {
                auto presetsPerPage = numberOfPresetColumns * rowsPerColumn;
                return selectedFolder->numberOfPresets() / presetsPerPage
                        + ((selectedFolder->numberOfPresets() % presetsPerPage) ? 1 : 0);
        }
        return 0;
}

size_t PresetBrowserModel::presetPage() const
{
        return presetPageIndex;
}

void PresetBrowserModel::nextPresetPage()
{
        if (presetPageIndex < presetPages() - 1) {
                presetPageIndex++;
                action presetPageChanged();
        }
}

void PresetBrowserModel::previousPresetPage()
{
        if (presetPageIndex > 0) {
                presetPageIndex--;
                action presetPageChanged();
        }
}

void PresetBrowserModel::setPresetPage(size_t index)
{
	presetPageIndex = index;
}

size_t PresetBrowserModel::columns() const
{
        return numberOfPresetColumns + 1;
}

size_t PresetBrowserModel::rows() const
{
        return rowsPerColumn;
}

void PresetBrowserModel::select(size_t row, size_t column)
{
        if (row < rows() && column < columns()) {
                if (column == 0 && getPresetFolder(row)) {
                        selectedFolder = getPresetFolder(row);
                        selectedPreset = nullptr;
                        presetPageIndex = 0;
                        if (selectedFolder) {
                                folderSelectedRaw    = row;
                                presetSelectedRaw    = 0;
                                presetSelectedColumn = 0;
                                action folderSelected(selectedFolder);
                        }
                } if (column > 0 && selectedFolder) {
                        selectedPreset = getPreset(row, column);
                        if (selectedPreset && setPreset(selectedPreset)) {
                                presetSelectedRaw     = row;
                                presetSelectedColumn  = column;
                                action presetSelected(selectedPreset);
                        }
                }
        }
}

bool PresetBrowserModel::isSelected(size_t row, size_t column) const
{
        if (column == 0)
                return getPresetFolder(row) == selectedFolder;
        else if (column > 0)
                return getPreset(row, column) == selectedPreset;
        else
                return false;
}

bool PresetBrowserModel::setPreset(Preset* preset)
{
        auto kit = std::make_unique<KitState>();
        if (!kit->open(preset->path().string())) {
                GEONKICK_LOG_ERROR("can't open kit: " << preset->path().string());
                return false;
        }

        if (geonkickApi->setKitState(kit)) {
                geonkickApi->notifyKitUpdated();
                geonkickApi->notifyUpdateGui();
                return true;
        }
        return false;
}

PresetFolder* PresetBrowserModel::currentSelectedFolder() const
{
        return selectedFolder;
}

Preset* PresetBrowserModel::currentSelectedPreset() const
{
        return selectedPreset;
}

GeonkickApi* PresetBrowserModel::getGeonkickApi() const
{
        return geonkickApi;
}

void PresetBrowserModel::selectPreviousFolder()
{
        if (getPresetFolder(folderSelectedRaw - 1)) {
                select(folderSelectedRaw - 1, 0);
        } else {
                folderPreviousPage();
                select(0, 0);
        }
        select(0, 1);
}

void PresetBrowserModel::selectNextFolder()
{
        if (getPresetFolder(folderSelectedRaw + 1)) {
                select(folderSelectedRaw + 1, 0);
        } else {
                folderNextPage();
                select(0, 0);
        }
        select(0, 1);
}

void PresetBrowserModel::selectPreviousPreset()
{
        if (getPreset(presetSelectedRaw - 1, presetSelectedColumn))
                select(presetSelectedRaw - 1, presetSelectedColumn);
        else if (getPreset(rows() - 1, presetSelectedColumn - 1))
                select(rows() - 1, presetSelectedColumn - 1);
        else {
                previousPresetPage();
                select(rows() - 1, columns() - 1);
        }
}

void PresetBrowserModel::selectNextPreset()
{
        if (getPreset(presetSelectedRaw + 1, presetSelectedColumn)) {
                select(presetSelectedRaw + 1, presetSelectedColumn);
        } else if (getPreset(0, presetSelectedColumn + 1)) {
                select(0, presetSelectedColumn + 1);
        } else {
                nextPresetPage();
                select(0, 1);
        }
}
