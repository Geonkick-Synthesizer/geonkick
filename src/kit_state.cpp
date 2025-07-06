/**
 * File name: kit_state.cpp
 * Project: Geonkick (A kick synthesizer)
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

#include "percussion_state.h"
#include "kit_state.h"


KitState::KitState()
        : kitAppVersion{GEONKICK_VERSION}
        , kitName{"Default"}
        , kitAuthor{"Unknown"}
{
        addPercussion(std::move(GeonkickApi::getDefaultPercussionState()));
}

bool KitState::open(const std::string &fileName)
{
        if (fileName.size() < 6) {
                GEONKICK_LOG_ERROR("can't open preset. File name empty or wrong format.");
                return false;
        }

        std::filesystem::path filePath(fileName);
        auto fileExt = Geonkick::toLower(filePath.extension().string());
        if (filePath.extension().empty() || (fileExt != ".gkit" && fileExt != ".gkick")) {
                GEONKICK_LOG_ERROR("can't open kit. Wrong file format.");
                return false;
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(filePath));
        if (!sfile.is_open()) {
                GEONKICK_LOG_ERROR("can't open kit.");
                return false;
        }

        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));

        sfile.close();
        return fromJson(fileData, fileExt == ".gkick");
}

bool KitState::save(const std::string &fileName)
{
        if (fileName.size() < 6) {
                GEONKICK_LOG_ERROR("can't save kit. Wrong file name");
                return false;
        }

        std::filesystem::path filePath(fileName);
        if (filePath.extension().empty()
            || Geonkick::toLower(filePath.extension().string()) != ".gkit") {
                filePath.replace_extension(".gkit");
        }

        std::ofstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                GEONKICK_LOG_ERROR("can't open file for saving: " << filePath);
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

const std::vector<std::unique_ptr<PercussionState>>& KitState::percussions() const
{
        return percussionsList;
}

 bool KitState::fromJson(const std::string &jsonData, bool oldPreset)
{
        percussionsList.clear();
        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        if (!document.IsObject())
                return false;
        return fromJsonObject(document, oldPreset);
}

 bool KitState::fromJsonObject(const rapidjson::Value &obj, bool oldPreset)
{
        // For backward compatibility
        if (oldPreset) {
                auto state = std::make_unique<PercussionState>();
                if (!state->loadObject(obj))
                        return false;
#ifdef GEONKICK_SINGLE
                state->setId(0);
#endif // GEONKICK_SINGLE
                addPercussion(std::move(state));
                return true;
        }

        bool isOk = false;
        for (const auto &m: obj.GetObject()) {
                if (m.name == "KitAppVersion" && m.value.IsInt())
                        kitAppVersion = m.value.GetInt();
                if (m.name == "name" && m.value.IsString())
                        setName(m.value.GetString());
                if (m.name == "author" && m.value.IsString())
                        setAuthor(m.value.GetString());
                if (m.name == "url" && m.value.IsString())
                        setUrl(m.value.GetString());
                if ((m.name == "percussions" // For backward compatibility
                     || m.name == "instruments") && m.value.IsArray()) {
                        isOk = parsePercussions(m.value);
                }
        }

        return isOk;
}

bool KitState::parsePercussions(const rapidjson::Value &percussionsArray)
{
        if (percussionsArray.Empty())
                return false;

        size_t i = 0;
        for (const auto &per: percussionsArray.GetArray()) {
                auto state = std::make_unique<PercussionState>();
                state->setId(i++);
                if (!state->loadObject(per))
                        return false;
                addPercussion(std::move(state));
        }
        return true;
}

std::string KitState::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        jsonStream << "\"KitAppVersion\": " << GEONKICK_VERSION << "," << std::endl;
        jsonStream << "\"name\": \"" << getName() << "\"," << std::endl;
        jsonStream << "\"author\": \"" << getAuthor() << "\"," << std::endl;
        jsonStream << "\"url\": \"" << getUrl() << "\"," << std::endl;
        jsonStream <<  "\"instruments\": [" << std::endl;

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

void KitState::addPercussion(std::unique_ptr<PercussionState> percussion)
{
        percussionsList.push_back(std::move(percussion));
}

const PercussionState* KitState::getPercussion(size_t id) const
{
        if (id < percussionsList.size())
                return percussionsList[id].get();
        return nullptr;
}
