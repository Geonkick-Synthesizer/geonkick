/**
 * File name: preset_folder.h
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

#ifndef PRESET_FOLDER_H
#define PRESET_FOLDER_H

#include "globals.h"

class Preset;

class PresetFolder {
 public:
        explicit PresetFolder(const std::filesystem::path &path);
        std::string name() const;
        const std::filesystem::path& path() const;
        void setPath(const std::filesystem::path &path);
        bool loadPresets();
        Preset* preset(size_t index) const;
        size_t numberOfPresets() const;

 private:
        std::filesystem::path folderPath;
        std::vector<std::unique_ptr<Preset>> presetList;
};

#endif // PRESET_FOLDER_H
