/**
 * File name: kit_state.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#include "percussion_state.h"
#include "kit_state.h"


KitState::KitState()
        : kitAppVersion{GEONKICK_VERSION}
        , kitName{"Default"}
        , kitAuthor{"Unknown"}
{
}

bool KitState::open(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("can't open preset. File name empty or wrong format.");
                return false;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkit"
            && filePath.extension() != ".GKIT")) {
                RK_LOG_ERROR("can't open kit. Wrong file format.");
                return false;
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(filePath));
        if (!sfile.is_open()) {
                RK_LOG_ERROR("can't open kit.");
                return false;
        }

        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));

        sfile.close();
        fromJson(fileData);
        return false;
}

bool KitState::save(const std::string &fileName)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("can't save kit. Wrong file name");
                return false;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkit"
            && filePath.extension() != ".GKIT"))
                filePath.replace_extension(".gkit");

        std::ofstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("can't open file for saving: " << filePath);
                return false;
        }
        file << toJson();
        file.close();
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        return true;
}

void KitState::setName(const std::string &name)
{
        kitName = name;
}

std::string KitState::getName() const
{
        return kitName;
}

void KitState::setAuthor(const std::string &author)
{
        kitAuthor = author;
}

std::string KitState::getAuthor() const
{
        return kitAuthor;
}

void KitState::setUrl(const std::string &url)
{
        kitUrl = url;
}

std::string KitState::getUrl() const
{
        return kitUrl;
}

std::vector<std::shared_ptr<PercussionState>>& KitState::percussions()
{
        return percussionsList;
}

void KitState::fromJson(const std::string &jsonData)
{
        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        if (!document.IsObject())
                return;

        bool backward = true;
        for (const auto &m: document.GetObject()) {
                if (m.name == "KitAppVersion" && m.value.IsInt()) {
                        kitAppVersion = m.value.GetInt();
                        backward = false;
                        break;
                }
        }

        if (backward) {
                GEONKICK_LOG_INFO("KitAppVersion missing, old preset");
                kitAppVersion = 0;
        }

        if (kitAppVersion < 0x011000) {
                // Compatibility with older versions.
                for (int i = 0; i < 2; i++) {
                        auto state = std::make_shared<PercussionState>();
                        state->loadObject(document);
                        state->setId(i);
                        state->setChannel(i);
                        addPercussion(state);
                }
        } else {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                setName(m.value.GetString());
                        if (m.name == "author" && m.value.IsString())
                                setAuthor(m.value.GetString());
                        if (m.name == "url" && m.value.IsString())
                                setUrl(m.value.GetString());
                        if (m.name == "percussions" && m.value.IsArray())
                                parsePercussions(m.value);
                }
        }
}

void KitState::parsePercussions(const rapidjson::Value &percussionsArray)
{
        size_t i = 0;
        for (const auto &per: percussionsArray.GetArray()) {
                auto state = std::make_shared<PercussionState>();
                state->setId(i++);
                state->loadObject(per);
                addPercussion(state);
        }
}

std::string KitState::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        jsonStream << "\"KitAppVersion\": " << GEONKICK_VERSION << "," << std::endl;
        jsonStream << "\"name\": \"" << getName() << "\"," << std::endl;
        jsonStream << "\"author\": \"" << getAuthor() << "\"," << std::endl;
        jsonStream << "\"url\": \"" << getUrl() << "\"," << std::endl;
        jsonStream <<  "\"percussions\": [" << std::endl;

        size_t i = 0;
        for (const auto &per: percussionsList) {
                if (i < percussionsList.size() - 1)
                        jsonStream << per->toJson() << "," << std::endl;
                else
                        jsonStream << per->toJson();
                i++;
        }
        jsonStream <<  "]" << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

void KitState::addPercussion(const std::shared_ptr<PercussionState> &percussion)
{
        percussionsList.push_back(percussion);
}

std::shared_ptr<PercussionState> KitState::getPercussion(size_t id)
{
        if (id < percussionsList.size())
                return percussionsList[id];
        return nullptr;
}
