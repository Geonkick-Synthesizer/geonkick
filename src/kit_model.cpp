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

#include "kit.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "kit_state.h"

KitModel::KitModel(GeonkickApi *api)
        : geonkickApi{api}
{
}

bool KitModel::open(const std::string &file)
{
        = std::make_unique<KitState>();
        if (kit->open(file)) {
                GEONKICK_LOG_ERROR("can't open kit");
                return;
        }

        auto filePath = std::filesystem::path(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        geonkickApi->setKitState(std::move(kit));
        geonkickApi->setCurrentWorkingPath("OpenKit", path);
        editPercussion->setText("");
	updatePercussionsLines();
        geonkickApi->notifyUpdateGui();
}

// void KitWidget::createKeys()
// {
//         constexpr std::array<std::string_view, 17> keys {"A4", "A#4", "B4", "C5",
//                         "C#5", "D5", "D#5", "E5",
//                         "F5", "F#5", "G5", "G#5",
//                         "A5", "A#5", "B5", "C6", "Any"};

//         char n = 0;
//         for (const auto &key: keys) {
//                 KeyInfo midiKey;
//                 midiKey.name = key;
//                 midiKey.key = 69 + n;
//                 if (static_cast<decltype(keys.size())>(++n) > keys.size() - 1)
//                         midiKey.key = -1;
//                 midiKeys.push_back(midiKey);
//         }

//         percussionWidth = percussionNameWidth + midiKeys.size() * keyWidth + channelWidth;
// }


bool KitModel::save(const std::string &file)
{
        auto state = geonkickApi->getKitState();
        state->save(file);
}

bool KitModel::loadPercussion(const std::string &file)
{
}

bool KitModel::savePercussion(int index, const std::string &file)
{
}

void KitModel::selectPercussion(int index)
{
        auto id = getId(index);
        if (isIdValid(id))
                return geonkickApi->setCurrentPercussion(id);
}

int KitModel::selectedPercussion() const
{
}

void KitModel::addNewPercussion()
{
        editedLineId = -1;
        int id = geonkickApi->getUnusedPercussion();
        if (id < 0)
                return;

        auto state = geonkickApi->getDefaultPercussionState();
        state->setId(id);
        addPercussion(state);
}

void KitModel::addPercussion(const std::shared_ptr<PercussionState> &percussion)
{
        
}

// void KitModel::updatePercussion(int index, const std::shared_ptr<PercussionState> &percussion)
// {
//         auto id = getId(index);
//         if (!isIdValid(id))
//                 return;

//         auto state = std::make_shared<PercussionState>(*percussion);
//         state->setId(id);
//         state->setPlayingKey(geonkickApi->percussionPlayingKey(id));
//         state->setChannel(geonkickApi->percussionChannel(id));
//         state->setEnabled(true);
//         bool name = state->getName() != geonkickApi->getPercussionName(id);
//         geonkickApi->setPercussionState(id, state);
//         if (name)
//                 action percussioNameUpdated(state->getName());
//         action percussionUpdated(id);
// }

void KitModel::removePercussion(int index)
{
        auto id = getId(index);
        if (!isIdValid(id))
                return;

        for (auto it = percussionIds.begin(); it != percussionIds.end(); ++it) {
                if (*it == id) {
                        percussionIds.erase(it);
                        geonkickApi->enablePercussion(id, false);
                        action percussionRemoved(id);
                        break;
                }
        }
}

void KitModel::copyPercussion(int index)
{
                editedLineId = -1;
        auto newId = geonkickApi->getUnusedPercussion();
        if (newId > - 1) {
                auto state = geonkickApi->getPercussionState(id);
                state->setId(newId);
                geonkickApi->setPercussionState(state);
                if (state->getId() == geonkickApi->currentPercussion())
                        geonkickApi->notifyUpdateGui();
		percussionsLines.push_back(newId);
        }

}

void KitModel::movePercussionUp(int index)
{
}

void KitModel::movePercussionDown(int index)
{
}

int KitModel::percussionKey(int index) const
{
        auto id = getId(index);
        if (isIdValid(id))
                return geonkickApi->getPercussionKey(id);
        return -1;
}

void KitModel::setPercussionKey(int index, int key)
{
        auto id = getId(index);
        if (isIdValid(id)) {
                geonkickApi->setPercussionKey(key);
                action percussionKeyChanged(id, key);
        }
}

size_t KitModel::percussionsNumber() const
{
        return percussionIds.size()
}

int KitModel::getId(int index)
{
        if (index > -1 && index < static_cast<decltype(index)>(percussionIds.size()))
                return percussionIds[index];
        return -1;
}

bool KitModel::isIdValid(int id) const
{
        return (id > -1)
                && (id < geonkickApi->percussionNumber())
                && geonkickApi->isPercussionEnabled(id);
}

std::string KitModel::keyName(int index) const
{
        if (midiKeys.empty())
                return "";
        if (index > -1 && index < midiKeys.size())
                return midiKeys[index];
        return midiKeys.back();
}

size_t KitModel::keysNumber() const
{
        return midiKeys.size();
}

auto keyId = midiKeys.size() - 1;
auto key = geonkickApi->percussionKey(i);
if (key < 69 && key != -1)
        continue;
 else if (key - 69 >= 0 && static_cast<decltype(midiKeys.size())>(key - 69) < midiKeys.size() - 1)
         keyId = key - 69;
 else if ( key == -1 || static_cast<decltype(midiKeys.size())>(key - 69) > midiKeys.size() - 1)
         keyId = midiKeys.size() - 1;


/// encrease/ decrease
if (channel < 0)
        geonkickApi->setPercussionChannel(id, geonkickApi->numberOfChannels() - 1);
 else if (static_cast<size_t>(channel) > geonkickApi->numberOfChannels() - 1)
         geonkickApi->setPercussionChannel(id, 0);
 else
         geonkickApi->setPercussionChannel(id, static_cast<size_t>(channel));

bool KitModel::canCopy() const
{
        auto n = geonkickApi->enabledPercussions();
        if (n > 0 && n < geonkickApi->percussionNumber())
                return true;
}

bool KitModel::canRemove() const
{
        if (geonkickApi->enabledPercussions() > 1)
                return true;
}
