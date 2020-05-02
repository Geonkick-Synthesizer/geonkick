/**
 * File name: kit.cpp
 * Project: Geonkick (A percussion synthesizer)
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

#include "kit_model.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "kit_state.h"

KitModel::KitModel(GeonkickApi *api)
        : geonkickApi{api}
        , midiKeys {"A4", "A#4", "B4", "C5",
                    "C#5", "D5", "D#5", "E5",
                    "F5", "F#5", "G5", "G#5",
                    "A5", "A#5", "B5", "C6", "Any"}
{
        size_t n = percussionNumber();
        if (PercussionIndex i = 0; i < n; i++) {
                auto mode = new PercussionModel(this, geonkickApi, getPercussionId(i));
                percussionModelsList.push_back(model);
        }
}

bool KitModel::open(const std::string &file)
{
        auto kit = std::make_unique<KitState>();
        if (kit->open(file)) {
                GEONKICK_LOG_ERROR("can't open kit");
                return false;
        }

        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        if (!geonkickApi->setKitState(kit)) {
                GEONKICK_LOG_ERROR("can't set kit state");
                return false;
        } else {
                geonkickApi->setCurrentWorkingPath("OpenKit", path);
                geonkickApi->notifyUpdateGui();
        }
        return true;
}

bool KitModel::save(const std::string &file)
{
        auto state = geonkickApi->getKitState();
        if (!state || !state->save(file)) {
                GEONKICK_LOG_ERROR("can't save kit state");
                return false;
        }
        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setCurrentWorkingPath("SaveKit", path);
        return true;
}

void KitModel::addNewPercussion()
{
        int newId = geonkickApi->getUnusedPercussion();
        if (newId < 0)
                return;

        auto state = geonkickApi->getDefaultPercussionState();
        state->setId(newId);
        state->enable(true);
        geonkickApi->setPercussionState(state);
        geonkickApi->addOrderedPercussionId(newId);
        auto mode = new PercussionModel(this, geonkickApi, newId);
        percussionModelsList.push_back(model);
        action percussionAdded(model);
}

void KitModel::copyPercussion(PercussionIndex index)
{
        auto newId = geonkickApi->getUnusedPercussion();
        if (newId < 0)
                return;

        auto state = geonkickApi->getPercussionState(getPercussionId(index));
        if (state) {
                state->setId(newId);
                state->enable(true);
                geonkickApi->setPercussionState(state);
                geonkickApi->addOrderedPercussionId(newId);
                auto mode = new PercussionModel(this, geonkickApi, newId);
                percussionModelsList.push_back(model);
                action percussionAdded(model);
        }
}

void KitModel::removePercussion(PercussionIndex index)
{
        if (index < percussionModelsList.size()) {
                delete percussionModelsList[index];
                percussionModelsList.erase(percussionModelsList.begin() + index);
                action percussionRemoved(index);
        }
}

size_t KitModel::keysNumber() const
{
        return midiKeys.size();
}

std::string KitModel::keyName(KeyIndex index) const
{
        if (midiKeys.empty() || index < 0 || index > static_cast<decltype(index)>(midiKeys.size() - 1))
                return "";
        return midiKeys[index];
}


size_t KitModel::percussionNumber() const
{
        return geonkickApi->ordredPercussionIds().size();
}

int KitModel::getPercussionId(int index) const
{
        const auto &ids = geonkickApi->ordredPercussionIds();
        if (index < -1 || index > static_cast<decltype(index)>(ids.size() - 1))
                return -1;
        return ids[index];
}

// void KitModel::moveSelectedPercussion(bool down)
// {
//         if (geonkickApi->moveOrdrepedPercussionId(geonkickApi->currentPercussion(), down ? 1 : -1))
//                 action modelUpdated();
// }

std::filesystem::path
KitModel::workingPath(const std::string &key) const
{
        return geonkickApi->currentWorkingPath(key);
}
