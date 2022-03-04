/**
 * File name: preset_folder.h
 * Project: Geonkick (A kick synthesizer)
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

#include "globals.h"

clss Preset {
 public:
        class enum PresetType : int {
            PercussionPreset,
            KitPreset
        };

        explicit Preset(const std::filesystem::path path);
        virtual ~Preset() = default;
        PresetType type() const;
        std::string name() const;
        std::string license() const;
        std::string author() const;
        std::string authorUrl() const;
        std::filesystem::path path() const;
        void load();

 private:
        PresetType presetType;
        std::string presetName;
        std::filesystem::path filePath;
        std::string presetLicense;
        std::string presetAuthor;
        std::string presetAuthorUrl;
};

class PresetFolder {
 public:
        explicit PresetsFolder(std::filesystem::path &dir);
        virtual ~PresetsFolder() = default;
        bool load(std::filesystem::path &dir);
        std::string name() const;
        const std::vector<Preset>& presets() const;
        void addPreset(const Preset &preset);

 private:
        std::vector<Preset> presetList;
};

void GeonkickApi::loadPresetsFromDir(const std::filesystem::path &dir)
{
        if (presetFolder.)
        try {
                for (const auto &file : std::filesystem::directory_iterator(dir)) {
                        if (!file.path().empty() && std::filesystem::s::is_regular_file(file.path())
                            && (file.path().extention() == ".gkick" || file.path().extention() == ".gkit"
                                || file.path().extention() == ".GKICK" || file.path().extention() == ".GKIT")) {
                                loadPresetsFromDir(entry.path());
                        }
                }
        } catch(...) {
                GEONKICK_LOG_ERROR("error on reading path: " << path);
        }
}
