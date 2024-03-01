/**
 * File name: GeonkickConfig.cpp
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

#include "GeonkickConfig.h"
#include "DesktopPaths.h"

#include <rapidjson/document.h>
#include <iomanip>
#include <ranges>

GeonkickConfig::GeonkickConfig()
        : scaleFactor{1.0}
        , channelNumber{GeonkickTypes::geonkickAnyMidiChannel}
        , midiChannelForced{false}
	, configFile{DesktopPaths().getConfigPath() / "config.json"}
{
        open();
}

void GeonkickConfig::setMidiChannel(int channel)
{
        channelNumber = channel;
}

int GeonkickConfig::getMidiChannel() const
{
        return channelNumber;
}

bool GeonkickConfig::isMidiChannelForced() const
{
        return midiChannelForced;
}

void GeonkickConfig::setMidiChannelForced(bool b)
{
        midiChannelForced = b;
}

void GeonkickConfig::setScaleFactor(double factor)
{
        scaleFactor = factor;
}

double GeonkickConfig::getScaleFactor() const
{
        return scaleFactor;
}

bool GeonkickConfig::open()
{
        try {
                if (!std::filesystem::exists(configFile.parent_path()))
                        return true;
        } catch(...) {
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(configFile));
        if (!sfile.is_open()) {
                GEONKICK_LOG_ERROR("can't open file: " << configFile);
                return false;
        }

        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));
        loadConfig(fileData);
        return true;
}

void GeonkickConfig::loadConfig(const std::string &data)
{
        rapidjson::Document document;
        document.Parse(data.c_str());
        for (const auto &m: document.GetObject()) {
                if (m.name == "scaleFactor" && m.value.IsDouble())
                        scaleFactor = m.value.GetDouble();
                if (m.name == "midiChannel" && m.value.IsInt())
                        channelNumber = m.value.GetInt();
                if (m.name == "midiChannelForced" && m.value.IsInt())
                        midiChannelForced = m.value.GetInt();
                if (m.name == "bookmarkedPaths" && m.value.IsArray())
                        parseBookmarkedPaths(m.value);
                if (m.name == "customPresetFolders" && m.value.IsArray())
                        parseCustomPresetFolders(m.value);
        }
}

void GeonkickConfig::parseBookmarkedPaths(const auto &value)
{
        if (!value.IsArray())
                return;

        for (const auto &el: value.GetArray()) {
                if (el.IsString())
                        bookmarkedPaths.push_back(el.GetString());
        }

}

void GeonkickConfig::parseCustomPresetFolders(const auto &value)
{
        if (!value.IsArray())
                return;

        for (const auto &el: value.GetArray()) {
                if (el.IsString())
                        customPresetFolders.push_back(el.GetString());
        }

}

bool GeonkickConfig::save()
{
        try {
                auto configPath = configFile.parent_path();
                if (!std::filesystem::exists(configPath)) {
                        if (!std::filesystem::create_directories(configPath)) {
                                GEONKICK_LOG_ERROR("can't create path " << configPath);
                                return false;
                        }
                }
        } catch(const std::exception& e) {
                GEONKICK_LOG_ERROR("error on creating config path: " << e.what());
        }

        std::ofstream file;
        file.open(std::filesystem::absolute(configFile));
        if (!file.is_open()) {
                GEONKICK_LOG_ERROR("can't open file for saving: " << configFile);
                return false;
        }
        file << toJson();
        file.close();
        return true;
}

bool GeonkickConfig::bookmarkPath(const std::filesystem::path &path)
{
        if (!isPathBookmarked(path)) {
                bookmarkedPaths.push_back(path);
                return true;
        }
        return false;
}

bool GeonkickConfig::isPathBookmarked(const std::filesystem::path &path) const
{
        auto it = std::find(bookmarkedPaths.begin(), bookmarkedPaths.end(), path);
        return it != bookmarkedPaths.end();
}

bool GeonkickConfig::removeBookmarkedPath(const std::filesystem::path &path)
{
        if (isPathBookmarked(path)) {
                bookmarkedPaths.erase(std::remove_if(bookmarkedPaths.begin(),
                                                     bookmarkedPaths.end(),
                                                     [&path](const auto & p)
                                                     { return p == path; }),
                                      bookmarkedPaths.end());
                return true;
        }
        return false;
}

const std::vector<std::filesystem::path>& GeonkickConfig::getBookmarkedPaths() const
{
        return bookmarkedPaths;
}

bool GeonkickConfig::addCustomPresetFolder(const std::filesystem::path &folder)
{
        customPresetFolders.push_back(folder);
        return true;
}

bool GeonkickConfig::removeCustomPresetFolder(const std::filesystem::path &folder)
{
        auto it = customPresetFolders.erase(std::remove_if(customPresetFolders.begin(),
                                                           customPresetFolders.end(),
                                                           [&folder](const auto &p)
                                                           { return p == folder; }),
                                            customPresetFolders.end());
        return it != customPresetFolders.end();
}

const std::vector<std::filesystem::path>&
GeonkickConfig::getCustomPresetFolders() const
{
        return customPresetFolders;
}

std::string GeonkickConfig::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        jsonStream << "\"scaleFactor\": "
                   << std::fixed << std::setprecision(2) << scaleFactor
                   << "," << std::endl;
        jsonStream << "\"midiChannel\": " << channelNumber << "," << std::endl;
        jsonStream << "\"midiChannelForced\": " << midiChannelForced << ", " << std::endl;
        jsonStream << "\"bookmarkedPaths\": [";
        bool addComma = false;
        for (const auto &path: bookmarkedPaths) {
                if (addComma)
                        jsonStream << ", ";
                addComma = true;
                jsonStream << "\"" << path.string() << "\"";
        }
        jsonStream << "]," << std::endl;
        jsonStream << "\"customPresetFolders\": [";
        addComma = false;
        for (const auto &path: customPresetFolders) {
                if (addComma)
                        jsonStream << ", ";
                addComma = true;
                jsonStream << "\"" << path.string() << "\"";
        }
        jsonStream << "]" << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}
