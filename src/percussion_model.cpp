/**
 * File name: percussion_model.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "percussion_model.h"
#include "geonkick_api.h"

PercussionModel::PercussionModel(RkObject* parent, GeonkickApi* api, int id)
        : RkObject(parent)
        , geonkickApi{api}
        , midiKeys {"A4", "A#4", "B4", "C5",
                    "C#5", "D5", "D#5", "E5",
                    "F5", "F#5", "G5", "G#5",
                    "A5", "A#5", "B5", "C6", "Any"}
        , percussionId{id}
{
}

void PercussionModel::setId(int id)
{
        percussionId = id;
        action modelUpdated();
}

int PercussionModel::id() const
{
        return percussionId;
}

void PercussionModel::select()
{
        if (geonkickApi->setCurrentPercussion(id()))
                action selected();
}

bool PercussionModel::isSelected() const
{
        return static_cast<decltype(id())>(geonkickApi->currentPercussion()) == id();
}

void PercussionModel::increasePercussionChannel()
{
        auto channel = geonkickApi->getPercussionChannel(id());
        if (channel < 0)
                return;

        if (++channel > static_cast<decltype(channel)>(geonkickApi->numberOfChannels() - 1))
                channel = 0;
        if (geonkickApi->setPercussionChannel(id(), channel))
                action channelUpdated(channel);
}

void PercussionModel::decreasePercussionChannel()
{
        auto channel = geonkickApi->getPercussionChannel(id());
        if (channel < 0)
                return;

        if (channel - 1 < 0)
                channel = geonkickApi->numberOfChannels() - 1;
        else
                channel--;
        if (geonkickApi->setPercussionChannel(id(), channel))
                action channelUpdated(channel);
}

size_t PercussionModel::keysNumber() const
{
        return midiKeys.size();
}

void PercussionModel::setKey(PercussionModel::KeyIndex keyIndex)
{
        int key = -1;
        if (keyIndex < static_cast<decltype(keyIndex)>(keysNumber()) - 1) {
                auto refKey = geonkickApi->percussionsReferenceKey();
                key = refKey + keyIndex;
        }

        if (geonkickApi->setPercussionPlayingKey(id(), key))
                action keyUpdated(keyIndex);
}

PercussionModel::KeyIndex PercussionModel::key() const
{
        KeyIndex keyIndex = geonkickApi->getPercussionPlayingKey(id());
        if (keyIndex < 0)
                return keysNumber() - 1;
        keyIndex -= geonkickApi->percussionsReferenceKey();
        if (keyIndex < 0 || keyIndex > static_cast<decltype(keyIndex)>(keysNumber() - 1))
                return keysNumber() - 1;
        else
                return keyIndex;
}

void PercussionModel::setName(const std::string &name)
{
        if (geonkickApi->setPercussionName(id(), name))
                action nameUpdated(name);
}

std::string PercussionModel::name() const
{
        return geonkickApi->getPercussionName(id());
}

int PercussionModel::channel() const
{
        return geonkickApi->getPercussionChannel(id());
}

bool PercussionModel::canCopy() const
{
        auto n = geonkickApi->ordredPercussionIds().size();
        if (n > 0 && n < geonkickApi->getPercussionsNumber())
                return true;
        return false;
}

bool PercussionModel::canRemove() const
{
        return geonkickApi->ordredPercussionIds().size() > 1;
}

void PercussionModel::play()
{
        geonkickApi->playKick(id());
}

void PercussionModel::setLimiter(int value)
{
        auto realVal = pow(10, static_cast<double>(value - 80) / 20);
        if (geonkickApi->setPercussionLimiter(id(), realVal))
                action limiterUpdated(value);
}

int PercussionModel::limiter() const
{
        auto realVal = geonkickApi->percussionLimiter(id());
        return 20 * log10(realVal) + 80;
}

void PercussionModel::mute(bool b)
{
        if (geonkickApi->mutePercussion(id(), b))
                action muteUpdated(b);
}

bool PercussionModel::isMuted() const
{
        return geonkickApi->isPercussionMuted(id());
}

void PercussionModel::solo(bool b)
{
        if (geonkickApi->soloPercussion(id(), b))
                action soloUpdated(b);
}

bool PercussionModel::isSolo() const
{
        return geonkickApi->isPercussionSolo(id());
}
