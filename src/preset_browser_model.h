/**
 * File name: preset_browser_model.h
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

#ifndef PRESET_BROWSER_MODEL_H
#define PRESET_BROWSER_MODEL_H

#include "globals.h"

#include <RkObject.h>

class GeonkickApi;
class PresetFolder;
class Preset;

class PresetBrowserModel : public RkObject {
 public:
        explicit PresetBrowserModel(RkObject *parent, GeonkickApi *api);
        std::string presetName(int row, int column) const;
        size_t page() const;
        void nextPage();
        void previousPage();
        void setPage(size_t index);
        size_t columns() const;
        size_t rows() const;
        void select(size_t row, size_t column);
        bool isSelected(size_t row, size_t column) const;
        RK_DECL_ACT(folderSelected,
                    folderSelected(PresetFolder* folder),
                    RK_ARG_TYPE(PresetFolder*),
                    RK_ARG_VAL(folder));
        RK_DECL_ACT(presetSelected,
                    presetSelected(Preset* preset),
                    RK_ARG_TYPE(Preset*),
                    RK_ARG_VAL(preset));

 protected:
        Preset* getPreset(int row, int column) const;
        bool setPreset(Preset* preset);


 private:
        GeonkickApi* geonkickApi;
        size_t pageIndex;
        size_t numberOfColumns;
        size_t presetPerColumn;
        PresetFolder* selectedFolder;
        Preset* selectedPreset;
};

#endif // PRESET_BROWSER_MODEL_H
