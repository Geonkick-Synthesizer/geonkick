/**
 * File name: kit.cpp
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

#include "kit_model.h"
#include "geonkick_api.h"
#include "instrument_state.h"
#include "instrument_model.h"
#include "kit_state.h"
#include "ExportToSfz.h"

#include <RkAction.h>
#include <RkEventQueue.h>

KitModel::KitModel(RkObject *parent, GeonkickApi *api)
        : RkObject(parent)
        , geonkickApi{api}
        , midiKeys {"A4", "A#4", "B4", "C5",
                   "C#5", "D5", "D#5", "E5",
                   "F5", "F#5", "G5", "G#5",
                   "A5", "A#5", "B5", "C6", "Any"}
{
        loadModelData();
        RK_ACT_BIND(geonkickApi, kitUpdated, RK_ACT_ARGS(), this, loadModelData());
        RK_ACT_BIND(geonkickApi, instrumentUpdated, RK_ACT_ARGS(size_t id), this, updateInstrument(getIndex(id)));
}

void KitModel::updateInstrument(InstrumentIndex index)
{
        if (isValidIndex(index))
                action instrumentsList[index]->modelUpdated();
}

bool KitModel::isValidIndex(InstrumentIndex index)
{
        return index > -1 && static_cast<size_t>(index) < instrumentsList.size();
}

void KitModel::selectInstrument(InstrumentIndex index)
{
        if (isValidIndex(index) && geonkickApi->setCurrentInstrument(instrumentId(index))) {
                geonkickApi->notifyUpdateGui();
                action instrumentSelected();
        }
}

bool KitModel::isInstrumentSelected(InstrumentIndex index) const
{
        return static_cast<size_t>(instrumentId(index)) == geonkickApi->currentInstrument();
}

KitModel::InstrumentIndex KitModel::selectedInstrument() const
{
        return getIndex(geonkickApi->currentInstrument());
}

InstrumentModel* KitModel::currentInstrument() const
{
        return instrumentsList[getIndex(geonkickApi->currentInstrument())];
}

size_t KitModel::numberOfChannels() const
{
        return geonkickApi->numberOfChannels();
}

int KitModel::instrumentChannel(InstrumentIndex index) const
{
        return geonkickApi->getInstrumentChannel(instrumentId(index));
}

bool KitModel::setInstrumentChannel(InstrumentIndex index, int channel)
{
        return geonkickApi->setInstrumentChannel(instrumentId(index), channel);
}

bool KitModel::setInstrumentName(InstrumentIndex index, const std::string &name)
{
        if (geonkickApi->setInstrumentName(instrumentId(index), name)) {
                geonkickApi->notifyUpdateGui();
                return true;
        }
        return false;
}

std::string KitModel::instrumentName(InstrumentIndex index) const
{
        return geonkickApi->getInstrumentName(instrumentId(index));
}

size_t KitModel::keysNumber() const
{
        return midiKeys.size();
}

std::string KitModel::keyName(KeyIndex index) const
{
        if (index >= 0 && static_cast<size_t>(index) < midiKeys.size())
                return midiKeys[index];
        return std::string();
}

bool KitModel::setInstrumentKey(InstrumentIndex index, KeyIndex keyIndex)
{
        if (!isValidIndex(index))
                return false;
        
        if (geonkickApi->setInstrumentPlayingKey(instrumentId(index), keyIndex)) {
                action instrumentUpdated(instrumentsList[index]);
                return true;
        }
        return false;
}

KitModel::KeyIndex KitModel::instrumentKey(InstrumentIndex index) const
{
        return geonkickApi->getInstrumentPlayingKey(instrumentId(index));
}

void KitModel::playInstrument(InstrumentIndex index)
{
        geonkickApi->playKick(instrumentId(index));
}

bool KitModel::setInstrumentLimiter(InstrumentIndex index, int value)
{
        double logVal = (75.0 / 100) * value - 55;
        auto realVal = pow(10, logVal / 20);
        if (geonkickApi->setInstrumentLimiter(instrumentId(index), realVal)) {
                action limiterUpdated(index);
                return true;
        }
        return false;
}

int KitModel::instrumentLimiter(InstrumentIndex index) const
{
        auto realVal = geonkickApi->instrumentLimiter(instrumentId(index));
        double logVal = 20 * log10(realVal);
        int val = (logVal + 55.0) * 100.0 / 75;
        return  val;
}

int KitModel::instrumentLeveler(InstrumentIndex index) const
{
        auto realVal = geonkickApi->getLimiterLevelerValue(instrumentId(index));
        double logVal = 20 * log10(realVal);
        int val = (logVal + 55.0) * 100.0 / 75;
        if (val < 0)
                val = 0;
        return val;
}

bool KitModel::muteInstrument(InstrumentIndex index, bool b)
{
        return geonkickApi->muteInstrument(instrumentId(index), b);
}

bool KitModel::isInstrumentMuted(InstrumentIndex index) const
{
        return geonkickApi->isInstrumentMuted(instrumentId(index));
}

bool KitModel::soloInstrument(InstrumentIndex index, bool b)
{
        return geonkickApi->soloInstrument(instrumentId(index), b);
}

bool KitModel::isInstrumentSolo(InstrumentIndex index) const
{
        return geonkickApi->isInstrumentSolo(instrumentId(index));
}

void KitModel::loadModelData()
{
        for (auto &per: instrumentsList)
                delete per;
        instrumentsList.clear();
        for (const auto &id : geonkickApi->ordredInstrumentIds()) {
                auto model = new InstrumentModel(this, id);
                instrumentsList.push_back(model);
        }
        action modelUpdated();
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

void KitModel::addNewInstrument()
{
        int newId = geonkickApi->getUnusedInstrument();
        if (newId < 0)
                return;

        auto state = geonkickApi->getDefaultInstrumentState();
        state->setId(newId);
        state->enable(true);
        geonkickApi->setInstrumentState(state);
        geonkickApi->addOrderedInstrumentId(newId);
        auto model = new InstrumentModel(this, newId);
        instrumentsList.push_back(model);
        action instrumentAdded(model);
}

void KitModel::copyInstrument(InstrumentIndex index)
{
        if (!isValidIndex(index))
                return;

        auto newId = geonkickApi->getUnusedInstrument();
        if (newId < 0)
                return;

        auto state = geonkickApi->getInstrumentState(instrumentId(index));
        if (state) {
                state->setId(newId);
                state->enable(true);
                geonkickApi->setInstrumentState(state);
                geonkickApi->addOrderedInstrumentId(newId);
                auto model = new InstrumentModel(this, newId);
                instrumentsList.push_back(model);
                action instrumentAdded(model);
        }
}

void KitModel::removeInstrument(InstrumentIndex index)
{
        if (!isValidIndex(index) || instrumentsList.size() < 2)
                return;

        for (auto it = instrumentsList.begin(); it != instrumentsList.end(); ++it) {
                if ((*it)->index() == index && geonkickApi->enableInstrument(instrumentId(index), false)) {
                        action instrumentRemoved(index);
                        bool notify = (*it)->isSelected();
                        delete *it;
                        instrumentsList.erase(it);
                        geonkickApi->removeOrderedInstrumentId(instrumentId(index));
                        if (notify) {
                                geonkickApi->setCurrentInstrument(instrumentId(0));
                                action selectInstrument(0);
                        }
                        break;
                }
        }

        for (const auto & per: instrumentsList)
                action per->modelUpdated();
}

void KitModel::moveSelectedInstrument(bool down)
{
        auto currentIndex = getIndex(geonkickApi->currentInstrument());
        auto nextIndex = currentIndex + (down ? 1 : -1);
        if (isValidIndex(currentIndex) && isValidIndex(nextIndex)) {
                bool res = geonkickApi->moveOrdrepedInstrumentId(geonkickApi->currentInstrument(), down ? 1 : -1);
                if (res) {
                        instrumentsList[currentIndex]->setId(instrumentId(currentIndex));
                        instrumentsList[nextIndex]->setId(instrumentId(nextIndex));
                        selectInstrument(nextIndex);
                }
        }
}

size_t KitModel::instrumentNumber() const
{
        return geonkickApi->ordredInstrumentIds().size();
}

size_t KitModel::maxInstrumentNumber() const
{
        return geonkickApi->getInstrumentsNumber();
}

int KitModel::instrumentId(int index) const
{
        const auto &ids = geonkickApi->ordredInstrumentIds();
        if (index < -1 || index > static_cast<decltype(index)>(ids.size() - 1))
                return -1;
        return ids[index];
}

KitModel::InstrumentIndex KitModel::getIndex(int id) const
{
        const auto &ids = geonkickApi->ordredInstrumentIds();
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

std::filesystem::path
KitModel::getHomePath() const
{
        return geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH");
}

const std::vector<InstrumentModel*>& KitModel::instrumentModels() const
{
        return instrumentsList;
}

GeonkickApi* KitModel::api() const
{
        return geonkickApi;
}

bool KitModel::doExport(const std::string &file, KitModel::ExportFormat format)
{
        switch (format) {
        case ExportFormat::Sfz:
         {
                 ExportToSfz toSfz(this, file);
                 return toSfz.doExport();
         }
        default:
                return false;
        }
}

RkString KitModel::name() const
{
        return RkString("Unknown");
}

RkString KitModel::author() const
{
        return RkString("Unknown");
}

RkString KitModel::license() const
{
        return RkString("Unknown");
}

std::vector<float> KitModel::instrumentData(InstrumentIndex index) const
{
        return geonkickApi->getInstrumentBuffer(instrumentId(index));
}

