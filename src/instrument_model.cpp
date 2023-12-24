/**
 * File name: instrument_model.cpp
 * Project: Geonkick (A percussive synthesizer)
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

#include "instrument_model.h"
#include "kit_model.h"

#include <RkAction.h>
#include <RkEventQueue.h>

InstrumentModel::InstrumentModel(KitModel* parent, int id)
        : RkObject(parent)
        , kitModel{parent}
        , instrumentId{id}
{
        RK_ACT_BIND(kitModel,
                    instrumentSelected,
                    RK_ACT_ARGS(),
                    this,
                    onInstrumentSelected());
}

void InstrumentModel::setId(int id)
{
        instrumentId = id;
        action modelUpdated();
}

InstrumentModel::InstrumentIndex InstrumentModel::index() const
{
        return kitModel->getIndex(instrumentId);
}

void InstrumentModel::select()
{
        kitModel->selectInstrument(index());
}

bool InstrumentModel::isSelected() const
{
        return kitModel->isInstrumentSelected(index());
}

void InstrumentModel::increaseInstrumentChannel()
{
        auto channel = kitModel->instrumentChannel(index());
        if (channel < 0)
                return;

        if (++channel > static_cast<decltype(channel)>(kitModel->numberOfChannels() - 1))
                channel = 0;
        if (kitModel->setInstrumentChannel(index(), channel))
                action channelUpdated(channel);
}

void InstrumentModel::decreaseInstrumentChannel()
{
        auto channel = kitModel->instrumentChannel(index());
        if (channel < 0)
                return;

        if (channel - 1 < 0)
                channel = kitModel->numberOfChannels() - 1;
        else
                channel--;
        if (kitModel->setInstrumentChannel(instrumentId, channel))
                action channelUpdated(channel);
}

void InstrumentModel::setChannel(int index)
{
        if (kitModel->setInstrumentChannel(instrumentId, index))
                action channelUpdated(index);
}

size_t InstrumentModel::keysNumber() const
{
        return kitModel->keysNumber();
}

void InstrumentModel::setKey(InstrumentModel::KeyIndex keyIndex)
{
        if (kitModel->setInstrumentKey(index(), keyIndex))
                action keyUpdated(keyIndex);
}

InstrumentModel::KeyIndex InstrumentModel::key() const
{
        return kitModel->instrumentKey(index());
}

void InstrumentModel::setName(const std::string &name)
{
        if (kitModel->setInstrumentName(index(), name))
                action nameUpdated(name);
}

std::string InstrumentModel::name() const
{
        return kitModel->instrumentName(index());
}

int InstrumentModel::channel() const
{
        return kitModel->instrumentChannel(index());
}

size_t InstrumentModel::numberOfChannels() const
{
        return kitModel->numberOfChannels();
}

bool InstrumentModel::canCopy() const
{
        auto n = kitModel->instrumentNumber();
        return n > 0 && n < kitModel->maxInstrumentNumber();
}

bool InstrumentModel::canRemove() const
{
        return kitModel->instrumentNumber() > 1;
}

void InstrumentModel::play()
{
        kitModel->playInstrument(index());
}

void InstrumentModel::setLimiter(int value)
{
        if (kitModel->setInstrumentLimiter(index(), value))
                action limiterUpdated(value);
}

int InstrumentModel::limiter() const
{
        return kitModel->instrumentLimiter(index());
}

int InstrumentModel::leveler() const
{
        return kitModel->instrumentLeveler(index());
}

void InstrumentModel::mute(bool b)
{
        if (kitModel->muteInstrument(index(), b))
                action muteUpdated(b);
}

bool InstrumentModel::isMuted() const
{
        return kitModel->isInstrumentMuted(index());
}

void InstrumentModel::solo(bool b)
{
        if (kitModel->soloInstrument(index(), b))
                action soloUpdated(b);
}

bool InstrumentModel::isSolo() const
{
        return kitModel->isInstrumentSolo(index());
}

void InstrumentModel::onInstrumentSelected()
{
        action selected();
}

void InstrumentModel::remove()
{
        auto act = std::make_unique<RkAction>();
        auto model = kitModel;
        InstrumentIndex i = index();
        act->setCallback([i, model](void){ model->removeInstrument(i); });
        eventQueue()->postAction(std::move(act));
}

void InstrumentModel::copy()
{
        kitModel->copyInstrument(index());
}

KitModel* InstrumentModel::model() const
{
        return kitModel;
}

std::vector<float> InstrumentModel::data() const
{
        return kitModel->instrumentData(index());
}
