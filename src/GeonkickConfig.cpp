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
#include <rapidjson/prettywriter.h>
#include "rapidjson/stringbuffer.h"

#include <iomanip>
#include <ranges>

GeonkickConfig::GeonkickConfig(bool autosave)
        : autoSave{autosave}
        , scaleFactor{1.0}
        , channelNumber{GeonkickTypes::geonkickAnyMidiChannel}
        , midiChannelForced{false}
	, configFile{DesktopPaths().getConfigPath() / "config.json"}
        , presetCurrentPath{DesktopPaths().getUserPresetsPath()}
        , sampleCurrentPath{DesktopPaths().getDataPath()}
        , showSidebar{false}
        , exportFormat{Geonkick::defaultExportFormat}
        , exportNumberOfChannels{1}
{
        open();
}

GeonkickConfig::~GeonkickConfig()
{
        if (autoSave)
                save();
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
        if (!document.IsObject()) {
                GEONKICK_LOG_ERROR("can't parse JSON document");
                return;
        }

        for (const auto &m: document.GetObject()) {
                if (m.name == "scaleFactor" && m.value.IsDouble())
                        scaleFactor = m.value.GetDouble();
                if (m.name == "midiChannel" && m.value.IsInt())
                        channelNumber = m.value.GetInt();
                if (m.name == "midiChannelForced" && m.value.IsBool())
                        midiChannelForced = m.value.GetBool();
                if (m.name == "showSidebar" && m.value.IsBool())
                        showSidebar = m.value.GetBool();
                if (m.name == "bookmarkedPaths" && m.value.IsArray())
                        parseBookmarkedPaths(m.value);
                if (m.name == "presetCurrentPath" && m.value.IsString())
                        presetCurrentPath = m.value.GetString();
                if (m.name == "sampleCurrentPath" && m.value.IsString())
                        sampleCurrentPath = m.value.GetString();
                if (m.name == "exportFormat" && m.value.IsString())
                        exportFormat = m.value.GetString();
                if (m.name == "exportNumberOfChannels" && m.value.IsInt())
                        exportNumberOfChannels = m.value.GetInt();
        }
}

std::vector<std::filesystem::path> GeonkickConfig::parsePathsArray(const auto &value) const
{
    std::vector<std::filesystem::path> paths;
    for (const auto &el : value.GetArray()) {
            if (el.IsString())
                paths.emplace_back(el.GetString());
    }

    return paths;
}

void GeonkickConfig::parseBookmarkedPaths(const auto &value)
{
        if (!value.IsArray())
                return;

        for (const auto &el: value.GetArray()) {
                if (el.IsObject()) {
                        std::pair<std::string, std::vector<std::filesystem::path>> entry;
                        for (const auto &m: el.GetObject()) {
                                if (m.name == "name" && m.value.IsString())
                                        entry.first = std::string(m.value.GetString());
                                if (m.name == "paths" && m.value.IsArray())
                                        entry.second = parsePathsArray(m.value);
                        }
                        if (!entry.first.empty() && !entry.second.empty())
                                bookmarkedPaths.emplace(std::move(entry));
                }
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

bool GeonkickConfig::bookmarkPath(const std::filesystem::path &path,
                                  const std::string& name)
{
        if (!isPathBookmarked(path, name)) {
                auto res = bookmarkedPaths.find(name);
                if (res == bookmarkedPaths.end())
                        bookmarkedPaths.insert({name, {path}});
                else
                        res->second.push_back(path);
                return true;
        }
        return false;
}

bool GeonkickConfig::isPathBookmarked(const std::filesystem::path &path,
                                      const std::string& name) const
{
        auto it = bookmarkedPaths.find(name);
        if (it != bookmarkedPaths.end()) {
                const auto& paths = it->second;
                return std::find(paths.begin(), paths.end(), path) != paths.end();
        }
        return false;
}

bool GeonkickConfig::removeBookmarkedPath(const std::filesystem::path &path,
                                          const std::string& name)
{
        auto res = bookmarkedPaths.find(name);
        if (res != bookmarkedPaths.end()) {
                auto& paths = res->second;
                auto it = std::find(paths.begin(), paths.end(), path);
                if (it != paths.end()) {
                        paths.erase(it);
                        if (paths.empty())
                                bookmarkedPaths.erase(res);
                        return true;
                }
        }
        return false;
}

std::vector<std::filesystem::path> GeonkickConfig::getBookmarkedPaths(const std::string& name) const
{
        if (auto res = bookmarkedPaths.find(name); res != bookmarkedPaths.end())
                return res->second;
        return {};
}

void GeonkickConfig::setShowSidebar(bool b)
{
        showSidebar = b;
}

bool GeonkickConfig::isShowSidebar() const
{
        return showSidebar;
}

bool GeonkickConfig::setSampleCurrentPath(const fs::path &path)
{
        sampleCurrentPath = path;
        return true;
}

const fs::path& GeonkickConfig::getSampleCurrentPath() const
{
        return sampleCurrentPath;
}

bool GeonkickConfig::setPresetCurrentPath(const fs::path &path)
{
        presetCurrentPath = path;
        return true;
}

const fs::path& GeonkickConfig::getPresetCurrentPath() const
{
        return presetCurrentPath;
}

void GeonkickConfig::setExportFormat(const std::string_view &format)
{
        exportFormat = format;
}

const std::string& GeonkickConfig::getExportFormat() const
{
        return exportFormat;
}

void GeonkickConfig::setExportNumberOfChannels(unsigned int channels)
{
        exportNumberOfChannels = channels;
}

unsigned int GeonkickConfig::getExportNumberOfChannels() const
{
        return exportNumberOfChannels;
}

void GeonkickConfig::writeBookmarkedPathsToJson(auto& writer) const
{
        writer.Key("bookmarkedPaths");
        writer.StartArray();
        for (const auto& el : bookmarkedPaths) {
                writer.StartObject();
                writer.Key("name");
                writer.String(el.first.c_str());
                writer.Key("paths");
                writer.StartArray();
                for (const auto& path : el.second)
                        writer.String(path.string().c_str());
                writer.EndArray();
                writer.EndObject();
        }
        writer.EndArray();
}

std::string GeonkickConfig::toJson() const
{
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<decltype(s)> writer(s);
        writer.StartObject();
        writer.Key("scaleFactor");
        writer.Double(scaleFactor);
        writer.Key("midiChannel");
        writer.Int(channelNumber);
        writer.Key("midiChannelForced");
        writer.Bool(midiChannelForced);
        writer.Key("showSidebar");
        writer.Bool(showSidebar);

        writeBookmarkedPathsToJson(writer);

        writer.Key("presetCurrentPath");
        writer.String(presetCurrentPath.string().c_str());
        writer.Key("sampleCurrentPath");
        writer.String(sampleCurrentPath.string().c_str());

        if (!exportFormat.empty()) {
                writer.Key("exportFormat");
                writer.String(exportFormat.c_str());
                writer.Key("exportNumberOfChannels");
                writer.Int(exportNumberOfChannels);
        }
        writer.EndObject();
        return s.GetString();
}
