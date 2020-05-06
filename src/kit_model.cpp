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
#include "percussion_model.h"
#include "kit_state.h"

KitModel::KitModel(GeonkickApi *api)
        : geonkickApi{api}
        , midiKeys {"A4", "A#4", "B4", "C5",
                   "C#5", "D5", "D#5", "E5",
                   "F5", "F#5", "G5", "G#5",
                   "A5", "A#5", "B5", "C6", "Any"}
{
        loadModelData();
}

bool KitModel::isValidIndex(KitModel::PercussionIndex index)
{
        return index > -1 && static_cast<size_t>(index) < percussionsList.size();
}

void KitModel::selectPercussion(PercussionIndex index)
{
        if (isValidIndex(index) && geonkickApi->setCurrentPercussion(percussionId(index))) {
                geonkickApi->notifyUpdateGui();
                action percussionSelected();
        }
}

bool KitModel::isPercussionSelected(PercussionIndex index) const
{
        return static_cast<size_t>(percussionId(index)) == geonkickApi->currentPercussion();
}

size_t KitModel::numberOfChannels() const
{
        return geonkickApi->numberOfChannels();
}

int KitModel::percussionChannel(PercussionIndex index) const
{
        return geonkickApi->getPercussionChannel(percussionId(index));
}

bool KitModel::setPercussionChannel(PercussionIndex index, int channel)
{
        return geonkickApi->setPercussionChannel(percussionId(index), channel);
}

bool KitModel::setPercussionName(PercussionIndex index, const std::string &name)
{
        return geonkickApi->setPercussionName(percussionId(index), name);
}

std::string KitModel::percussionName(PercussionIndex index) const
{
        return geonkickApi->getPercussionName(percussionId(index));
}

size_t KitModel::keysNumber() const
{
        return midiKeys.size();
}

bool KitModel::setPercussionKey(PercussionIndex index, KeyIndex keyIndex)
{
        int key = -1;
        if (keyIndex < static_cast<decltype(keyIndex)>(keysNumber()) - 1) {
                auto refKey = geonkickApi->percussionsReferenceKey();
                key = refKey + keyIndex;
        }

        if (geonkickApi->setPercussionPlayingKey(percussionId(index), key))
                return true;
        return false;
}

KitModel::KeyIndex KitModel::percussionKey(PercussionIndex index) const
{
        KeyIndex keyIndex = geonkickApi->getPercussionPlayingKey(percussionId(index));
        if (keyIndex < 0)
                return keysNumber() - 1;
        keyIndex -= geonkickApi->percussionsReferenceKey();
        if (keyIndex < 0 || keyIndex > static_cast<decltype(keyIndex)>(keysNumber() - 1))
                return keysNumber() - 1;
        else
                return keyIndex;
}

void KitModel::playPercussion(PercussionIndex index)
{
        geonkickApi->playKick(percussionId(index));
}

bool KitModel::setPercussionLimiter(PercussionIndex index, int value)
{
        auto realVal = pow(10, static_cast<double>(value - 80) / 20);
        return geonkickApi->setPercussionLimiter(percussionId(index), realVal);
}

int KitModel::percussionLimiter(PercussionIndex index) const
{
        auto realVal = geonkickApi->percussionLimiter(percussionId(index));
        return 20 * log10(realVal) + 80;
}

bool KitModel::mutePercussion(PercussionIndex index, bool b)
{
        return false;
}

bool KitModel::isPercussionMuted(PercussionIndex index) const
{
        return geonkickApi->isPercussionMuted(percussionId(index));
}

bool KitModel::soloPercussion(PercussionIndex index, bool b)
{
        return geonkickApi->soloPercussion(percussionId(index), b);
}

bool KitModel::isPercussionSolo(PercussionIndex index) const
{
        return geonkickApi->isPercussionSolo(percussionId(index));
}

void KitModel::loadModelData()
{
        for (auto &per: percussionsList)
                delete per;
        percussionsList.clear();
        for (const auto &id : geonkickApi->ordredPercussionIds()) {
                auto model = new PercussionModel(this, id);
                percussionsList.push_back(model);
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
                loadModelData();
                geonkickApi->notifyUpdateGui();
                action modelUpdated();
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
        auto model = new PercussionModel(this, newId);
        percussionsList.push_back(model);
        action percussionAdded(model);
}

void KitModel::copyPercussion(PercussionIndex index)
{
        auto newId = geonkickApi->getUnusedPercussion();
        if (newId < 0)
                return;

        auto state = geonkickApi->getPercussionState(percussionId(index));
        if (state) {
                state->setId(newId);
                state->enable(true);
                geonkickApi->setPercussionState(state);
                geonkickApi->addOrderedPercussionId(newId);
                auto model = new PercussionModel(this, newId);
                percussionsList.push_back(model);
                action percussionAdded(model);
        }
}

void KitModel::removePercussion(PercussionIndex index)
{
        if (static_cast<decltype(percussionsList.size())>(index) < percussionsList.size()) {
                delete percussionsList[index];
                percussionsList.erase(percussionsList.begin() + index);
                action percussionRemoved(index);
        }
}

size_t KitModel::percussionNumber() const
{
        return geonkickApi->ordredPercussionIds().size();
}

size_t KitModel::maxPercussionNumber() const
{
        return geonkickApi->getPercussionsNumber();
}

int KitModel::percussionId(int index) const
{
        const auto &ids = geonkickApi->ordredPercussionIds();
        if (index < -1 || index > static_cast<decltype(index)>(ids.size() - 1))
                return -1;
        return ids[index];
}

KitModel::PercussionIndex KitModel::getIndex(int id) const
{
        const auto &ids = geonkickApi->ordredPercussionIds();
        auto it = std::find(ids.begin(), ids.end(), id);
        if (it != ids.end())
                return it - ids.begin();
        return -1;
}

std::filesystem::path
KitModel::workingPath(const std::string &key) const
{
        return geonkickApi->currentWorkingPath(key);
}

const std::vector<PercussionModel*>& KitModel::percussionModels() const
{
        return percussionsList;
}
