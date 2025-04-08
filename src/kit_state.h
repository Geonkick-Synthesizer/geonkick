/**
 * File name: kit_state.h
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

#include "globals.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class PercussionState;

class KitState {
 public:
        KitState();
        bool open(const std::string &fileName);
        bool save(const std::string &fileName);
        bool fromJson(const std::string &jsonData, bool oldPreset = false);
        bool fromJsonObject(const rapidjson::Value &obj, bool oldPreset = false);
        void setName(const std::string &name);
        std::string getName() const;
        void setAuthor(const std::string &author);
        std::string getAuthor() const;
        void setUrl(const std::string &url);
        std::string getUrl() const;
        std::string toJson() const;
        void addPercussion(std::unique_ptr<PercussionState> percussion);
        const PercussionState* getPercussion(size_t id) const;
        const std::vector<std::unique_ptr<PercussionState>>& percussions() const;

 protected:
        bool parsePercussions(const rapidjson::Value &percussionsArray);

 private:
        std::vector<std::unique_ptr<PercussionState>> percussionsList;
        int kitAppVersion;
        std::string kitName;
        std::string kitAuthor;
        std::string kitUrl;
};
