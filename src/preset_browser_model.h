/**
 * File name: preset_browser_model.h
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

#ifndef PRESET_BROWSER_MODEL_H
#define PRESET_BROWSER_MODEL_H

#include "globals.h"

#include <RkObject.h>

class GeonkickApi;
class Preset;
class PresetFolder;

class PresetBrowserModel : public RkObject {
 public:
        explicit PresetBrowserModel(RkObject *parent, GeonkickApi *api);
        std::string presetName(int row, int column) const;
        size_t folderPages() const;
        size_t folderPage() const;
        void folderNextPage();
        void folderPreviousPage();
        void setFolderPage(size_t index);
        size_t presetPages() const;
        size_t presetPage() const;
        void nextPresetPage();
        void previousPresetPage();
        void setPresetPage(size_t index);
        size_t columns() const;
        size_t rows() const;
        void select(size_t row, size_t column);
        bool isSelected(size_t row, size_t column) const;
        PresetFolder* currentSelectedFolder() const;
        Preset* currentSelectedPreset() const;
        GeonkickApi* getGeonkickApi() const;
        void selectPreviousFolder();
        void selectNextFolder();
        void selectPreviousPreset();
        void selectNextPreset();
        RK_DECL_ACT(folderSelected,
                    folderSelected(PresetFolder* folder),
                    RK_ARG_TYPE(PresetFolder*),
                    RK_ARG_VAL(folder));
        RK_DECL_ACT(presetSelected,
                    presetSelected(Preset* preset),
                    RK_ARG_TYPE(Preset*),
                    RK_ARG_VAL(preset));
        RK_DECL_ACT(folderPageChanged,
                    folderPageChanged(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(presetPageChanged,
                    presetPageChanged(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(presetFolderAdded,
                    presetFolderAdded(PresetFolder* folder),
                    RK_ARG_TYPE(PresetFolder*),
                    RK_ARG_VAL(folder));
        RK_DECL_ACT(presetFolderRemoved,
                    presetFolderRemoved(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

 protected:
        PresetFolder* getPresetFolder(int row) const;
        Preset* getPreset(int row, int column) const;
        bool setPreset(Preset* preset);


 private:
        GeonkickApi* geonkickApi;
        size_t folderPageIndex;
        size_t presetPageIndex;
        size_t numberOfPresetColumns;
        size_t rowsPerColumn;
        int folderSelectedRaw;
        int presetSelectedRaw;
        int presetSelectedColumn;
        PresetFolder* selectedFolder;
        Preset* selectedPreset;
};

#endif // PRESET_BROWSER_MODEL_H
