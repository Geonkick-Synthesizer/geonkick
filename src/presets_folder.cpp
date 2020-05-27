/**
 * File name: preset_folder.cpp
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

#include "preset_folder.h"
#include "preset.h"

PresetFolder::PresetFolder(std::filesystem::path &path)
        : folderPath{path}
{
}

std::string PresetFolder::name() const
{
        return folderPath.filename();
}

bool PresetFolder::loadPresets()
{
        std::vector<std::filesystem::path> presetsDirs;
        try {
                for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
                        if (!entry.path().empty() && std::filesystem::is_file(entry.path())
                            && (entry.path().stem() == "*.gkick" || entry.path().stem() == "*.gkit")) {
                                GEONKICK_LOG_DEBUG("load preset: " << entry.path());
                                presetList.push_back(std::move(std::make_unique<Preset>(entry.path())));
                        }
                }
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading path: " << path);
        }
}

Preset* PresetFolder::preset(size_t index) const
{
        if (index < presetsList.size())
                return presetsList[index].get();
}
