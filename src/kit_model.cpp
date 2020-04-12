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
        if (state || !state->save(file)) {
                GEONKICK_LOG_ERROR("can't save kit state");
                return false;
        }
        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setCurrentWorkingPath("SaveKit", path);
        return true;
}

void KitModel::selectPercussion(int index)
{
        auto id = getPercussionId(index);
        if (id > -1 && id != static_cast<decltype(id)>(geonkickApi->currentPercussion())) {
                if (geonkickApi->setCurrentPercussion(id))
                        geonkickApi->notifyUpdateGui();
        }
}

bool KitModel::percussionSelected(int index) const
{
        auto id = getPercussionId(index);
        return static_cast<decltype(id)>(geonkickApi->currentPercussion()) == id;
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
        action modelUpdated();
}

void KitModel::copyPercussion(int index)
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
                if (newId == static_cast<decltype(newId)>(geonkickApi->currentPercussion()))
                        geonkickApi->notifyUpdateGui();
                else
                        action modelUpdated();
        }
}

void KitModel::removePercussion(int index)
{
        auto id = getPercussionId(index);
        if (geonkickApi->enablePercussion(id, false)) {
                geonkickApi->removeOrderedPercussionId(id);
                if (id == static_cast<decltype(id)>(geonkickApi->currentPercussion())) {
                        geonkickApi->setCurrentPercussion(getPercussionId(0));
                        geonkickApi->notifyUpdateGui();
                } else {
                        action modelUpdated();
                }
        }
}

void KitModel::increasePercussionChannel(int index)
{
        auto id = getPercussionId(index);
        auto channel = geonkickApi->getPercussionChannel(id);
        if (channel < 0)
                return;

        if (++channel > static_cast<decltype(channel)>(geonkickApi->numberOfChannels() - 1))
                channel = 0;
        if (geonkickApi->setPercussionChannel(id, channel))
                action modelUpdated();
}

void KitModel::decreasePercussionChannel(int index)
{
        auto id = getPercussionId(index);
        auto channel = geonkickApi->getPercussionChannel(id);
        if (channel < 0)
                return;

        if (channel - 1 < 0)
                channel = geonkickApi->numberOfChannels() - 1;
        else
                channel--;
        if (geonkickApi->setPercussionChannel(id, channel))
                action modelUpdated();
}

void KitModel::moveSelectedPercussion(bool down)
{
        if (geonkickApi->moveOrdrepedPercussionId(geonkickApi->currentPercussion(), down ? 1 : -1))
                action modelUpdated();
}

void KitModel::setPercussionKey(int index, int keyIndex)
{
        auto id = getPercussionId(index);
        if (id > -1) {
                auto refKey = geonkickApi->percussionsReferenceKey();
                auto key = refKey + keyIndex;
                auto res = geonkickApi->setPercussionPlayingKey(id, key);
                if (res)
                        action modelUpdated();
        }
}

size_t KitModel::keysNumber() const
{
        return midiKeys.size();
}

std::string KitModel::keyName(int index) const
{
        if (midiKeys.empty() || index < 0 || index > static_cast<decltype(index)>(midiKeys.size() - 1))
                return "";
        return midiKeys[index];
}

int KitModel::percussionKeyIndex(int index) const
{
        auto id = getPercussionId(index);
        if (id < 0)
                return keysNumber() - 1;

        int keyIndex = geonkickApi->getPercussionPlayingKey(id);
        if (keyIndex < 0)
                return keysNumber() - 1;
        keyIndex -= geonkickApi->percussionsReferenceKey();
        if (keyIndex < 0 || keyIndex > static_cast<decltype(keyIndex)>(keysNumber() - 1))
                return keysNumber() - 1;
        else
                return keyIndex;
}

size_t KitModel::percussionNumber() const
{
        return geonkickApi->ordredPercussionIds().size();
}

void KitModel::setPercussionName(int index, const std::string &name)
{
        auto res = geonkickApi->setPercussionName(getPercussionId(index), name);
        if (res)
                geonkickApi->notifyUpdateGui();
}

std::string KitModel::percussionName(int index) const
{
        return geonkickApi->getPercussionName(getPercussionId(index));
}

int KitModel::percussionChannel(int index) const
{
        return geonkickApi->getPercussionChannel(getPercussionId(index));
}

bool KitModel::canCopy() const
{
        auto n = geonkickApi->ordredPercussionIds().size();
        if (n > 0 && n < geonkickApi->getPercussionsNumber())
                return true;
        return false;
}

bool KitModel::canRemove() const
{
        if (geonkickApi->ordredPercussionIds().size() > 1)
                return true;
        return false;
}

int KitModel::getPercussionId(int index) const
{
        const auto &ids = geonkickApi->ordredPercussionIds();
        if (index < -1 || index > static_cast<decltype(index)>(ids.size() - 1))
                return -1;
        return ids[index];
}

std::filesystem::path
KitModel::workingPath(const std::string &key) const
{
        return geonkickApi->currentWorkingPath(key);
}
