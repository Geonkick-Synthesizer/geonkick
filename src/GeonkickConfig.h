/**
 * File name: GeonkickConfig.h
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

#ifndef GEONGKICK_CONFIG_H
#define GEONGKICK_CONFIG_H

#include "globals.h"

class GeonkickConfig {
 public:
        explicit GeonkickConfig(bool autosave = true);
        ~GeonkickConfig();
        void setScaleFactor(double factor);
        double getScaleFactor() const;
        void setMidiChannel(int channel);
        int getMidiChannel() const;
        bool isMidiChannelForced() const;
        void setMidiChannelForced(bool b);
        bool open();
        bool save();
        bool bookmarkPath(const std::filesystem::path &path,
                          const std::string& name);
        bool isPathBookmarked(const std::filesystem::path &path,
                              const std::string& name) const;
        bool removeBookmarkedPath(const std::filesystem::path &path,
                                  const std::string& name);
        std::vector<std::filesystem::path> getBookmarkedPaths(const std::string& name) const;
        void setShowSidebar(bool b = true);
        bool isShowSidebar() const;
        bool setSampleCurrentPath(const fs::path &path);
        const fs::path& getSampleCurrentPath() const;
        bool setPresetCurrentPath(const fs::path &path);
        const fs::path& getPresetCurrentPath() const;
        void setExportFormat(const std::string_view &format);
        const std::string& getExportFormat() const;
        void setExportNumberOfChannels(unsigned int channels);
        unsigned int getExportNumberOfChannels() const;

 protected:
        void loadConfig(const std::string &data);
        std::vector<std::filesystem::path> parsePathsArray(const auto &value) const;
        void parseBookmarkedPaths(const auto &value);
        void writeBookmarkedPathsToJson(auto& writer) const;
        std::string toJson() const;

 private:
        bool autoSave;
        double scaleFactor;
        int channelNumber;
        bool midiChannelForced;
        std::filesystem::path configFile;
        std::unordered_map<std::string, std::vector<std::filesystem::path>> bookmarkedPaths;
        fs::path presetCurrentPath;
        fs::path sampleCurrentPath;
        bool showSidebar;
        std::string exportFormat;
        unsigned int exportNumberOfChannels;
};

#endif // GEONGKICK_CONFIG_H
