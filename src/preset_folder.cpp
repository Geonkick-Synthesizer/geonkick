/**
 * File name: preset_folder.cpp
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

#include "preset_folder.h"
#include "preset.h"

PresetFolder::PresetFolder(const std::filesystem::path &path)
        : folderPath{path}
{
        loadPresets();
}

std::string PresetFolder::name() const
{
        return folderPath.stem().string();
}

const std::filesystem::path& PresetFolder::path() const
{
        return folderPath;
}

void PresetFolder::setPath(const std::filesystem::path &path)
{
        folderPath = path;
}

bool PresetFolder::loadPresets()
{
        try {
                for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
                        if (!entry.path().empty() && std::filesystem::is_regular_file(entry.path())
                            && (Geonkick::toLower(entry.path().extension().string()) == ".gkit"
                                // For backward compatibity
                                || Geonkick::toLower(entry.path().extension().string()) == ".gkick")) {
                                presetList.emplace_back(std::make_unique<Preset>(entry.path()));
                        }
                }
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading path: " << folderPath);
                return false;
        }
        return true;
}

Preset* PresetFolder::preset(size_t index) const
{
        if (index < presetList.size())
                return presetList[index].get();
        return nullptr;
}

size_t PresetFolder::numberOfPresets() const
{
        return presetList.size();
}
