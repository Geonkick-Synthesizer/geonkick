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
                if (m.name == "midiChannelForced" && m.value.IsBool())
                        midiChannelForced = m.value.GetBool();
        }
}

bool GeonkickConfig::save()
{
        try {
                auto configPath = configFile.parent_path();
                GEONKICK_LOG_INFO("configPath" << configPath);
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

std::string GeonkickConfig::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        jsonStream << "\"scaleFactor\": " << std::fixed << std::setprecision(2) << scaleFactor << std::endl;
        jsonStream << "," << std::endl;
        jsonStream << "\"midiChannel\": " << channelNumber << std::endl;
        jsonStream << "," << std::endl;
        jsonStream << "\"midiChannelForced\": " << midiChannelForced << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}
