/**
 * File name: percussion_model.cpp
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

#include "percussion_model.h"
#include "kit_model.h"

#include <RkAction.h>
#include <RkEventQueue.h>

PercussionModel::PercussionModel(KitModel* parent, int id)
        : RkObject(parent)
        , kitModel{parent}
        , percussionId{id}
{
        RK_ACT_BIND(kitModel,
                    percussionSelected,
                    RK_ACT_ARGS(),
                    this,
                    onPercussionSelected());
}

void PercussionModel::setId(int id)
{
        percussionId = id;
        action modelUpdated();
}

PercussionModel::PercussionIndex PercussionModel::index() const
{
        return kitModel->getIndex(percussionId);
}

void PercussionModel::select()
{
        kitModel->selectPercussion(index());
}

bool PercussionModel::isSelected() const
{
        return kitModel->isPercussionSelected(index());
}

void PercussionModel::increasePercussionChannel()
{
        auto channel = kitModel->percussionChannel(index());
        if (channel < 0)
                return;

        if (++channel > static_cast<decltype(channel)>(kitModel->numberOfChannels() - 1))
                channel = 0;
        if (kitModel->setPercussionChannel(index(), channel))
                action channelUpdated(channel);
}

void PercussionModel::decreasePercussionChannel()
{
        auto channel = kitModel->percussionChannel(index());
        if (channel < 0)
                return;

        if (channel - 1 < 0)
                channel = kitModel->numberOfChannels() - 1;
        else
                channel--;
        if (kitModel->setPercussionChannel(percussionId, channel))
                action channelUpdated(channel);
}

void PercussionModel::setChannel(int index)
{
        if (kitModel->setPercussionChannel(percussionId, index))
                action channelUpdated(index);
}

size_t PercussionModel::keysNumber() const
{
        return kitModel->keysNumber();
}

void PercussionModel::setKey(PercussionModel::KeyIndex keyIndex)
{
        if (kitModel->setPercussionKey(index(), keyIndex))
                action keyUpdated(keyIndex);
}

PercussionModel::KeyIndex PercussionModel::key() const
{
        return kitModel->percussionKey(index());
}

void PercussionModel::setName(const std::string &name)
{
        if (kitModel->setPercussionName(index(), name))
                action nameUpdated(name);
}

std::string PercussionModel::name() const
{
        return kitModel->percussionName(index());
}

int PercussionModel::channel() const
{
        return kitModel->percussionChannel(index());
}

size_t PercussionModel::numberOfChannels() const
{
        return kitModel->numberOfChannels();
}

bool PercussionModel::canCopy() const
{
        auto n = kitModel->percussionNumber();
        return n > 0 && n < kitModel->maxPercussionNumber();
}

bool PercussionModel::canRemove() const
{
        return kitModel->percussionNumber() > 1;
}

void PercussionModel::play()
{
        kitModel->playPercussion(index());
}

void PercussionModel::setLimiter(int value)
{
        if (kitModel->setPercussionLimiter(index(), value))
                action limiterUpdated(value);
}

int PercussionModel::limiter() const
{
        return kitModel->percussionLimiter(index());
}

int PercussionModel::leveler() const
{
        return kitModel->percussionLeveler(index());
}

void PercussionModel::mute(bool b)
{
        if (kitModel->mutePercussion(index(), b))
                action muteUpdated(b);
}

bool PercussionModel::isMuted() const
{
        return kitModel->isPercussionMuted(index());
}

void PercussionModel::solo(bool b)
{
        if (kitModel->soloPercussion(index(), b))
                action soloUpdated(b);
}

bool PercussionModel::isSolo() const
{
        return kitModel->isPercussionSolo(index());
}

void PercussionModel::onPercussionSelected()
{
        action selected();
}

void PercussionModel::remove()
{
        auto act = std::make_unique<RkAction>();
        auto model = kitModel;
        PercussionIndex i = index();
        act->setCallback([i, model](void){ model->removePercussion(i); });
        eventQueue()->postAction(std::move(act));
}

void PercussionModel::copy()
{
        kitModel->copyPercussion(index());
}

KitModel* PercussionModel::model() const
{
        return kitModel;
}

std::vector<float> PercussionModel::data() const
{
        return kitModel->instrumentData(index());
}
