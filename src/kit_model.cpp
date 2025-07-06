/**
 * File name: kit.cpp
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

#include "kit_model.h"
#include "geonkick_api.h"
#include "percussion_state.h"
#include "percussion_model.h"
#include "kit_state.h"
#include "ExportToSfz.h"
#include "ExportSoundData.h"
#include "GeonkickModel.h"
#include "preset.h"

#include <RkAction.h>
#include <RkEventQueue.h>

KitModel::KitModel(GeonkickModel *parent)
        : RkObject(parent)
        , geonkickModel{parent}
        , geonkickApi{geonkickModel->api()}
        , midiKeys {"A4", "A#4", "B4", "C5",
                   "C#5", "D5", "D#5", "E5",
                   "F5", "F#5", "G5", "G#5",
                   "A5", "A#5", "B5", "C6", "Any"}
{
        loadModelData();
        RK_ACT_BIND(geonkickApi, kitUpdated, RK_ACT_ARGS(), this, loadModelData());
        RK_ACT_BIND(geonkickApi, percussionUpdated, RK_ACT_ARGS(size_t id), this, updatePercussion(getIndex(id)));
}

void KitModel::updatePercussion(PercussionIndex index)
{
        if (isValidIndex(index)) {
                action percussionUpdated(percussionsList[index]);
                action percussionsList[index]->modelUpdated();
        }
}

bool KitModel::isValidIndex(PercussionIndex index)
{
        return index > -1 && static_cast<size_t>(index) < percussionsList.size();
}

bool KitModel::enableInstrument(PercussionIndex index, bool b)
{
        if (isValidIndex(index) && geonkickApi->enablePercussion(percussionId(index))) {
                action instrumentEnabled(percussionsList[getIndex(index)]);
                return true;
        }
        return false;
}

bool KitModel::isInstrumentEnabled(PercussionIndex index) const
{
        return geonkickApi->isPercussionEnabled(percussionId(index));
}

void KitModel::selectPercussion(PercussionIndex index)
{
        if (isValidIndex(index) && geonkickApi->setCurrentPercussion(percussionId(index))) {
                geonkickApi->notifyUpdateGui();
                action percussionSelected(percussionsList[index]);
        }
}

bool KitModel::isPercussionSelected(PercussionIndex index) const
{
        return static_cast<size_t>(percussionId(index)) == geonkickApi->currentPercussion();
}

KitModel::PercussionIndex KitModel::selectedPercussion() const
{
        return getIndex(geonkickApi->currentPercussion());
}

PercussionModel* KitModel::currentPercussion() const
{
        return percussionsList[getIndex(geonkickApi->currentPercussion())];
}

size_t KitModel::numberOfChannels() const
{
        return geonkickApi->numberOfInstruments();
}

int KitModel::percussionChannel(PercussionIndex index) const
{
        return geonkickApi->getPercussionChannel(percussionId(index));
}

bool KitModel::setPercussionChannel(PercussionIndex index, int channel)
{
        return geonkickApi->setPercussionChannel(percussionId(index), channel);
}

size_t KitModel::numberOfMidiChannels() const
{
        return geonkickApi->numberOfMidiChannels();
}

int KitModel::percussionMidiChannel(PercussionIndex index) const
{
        return geonkickApi->getPercussionMidiChannel(percussionId(index));
}

bool KitModel::setPercussionMidiChannel(PercussionIndex index, int channel)
{
        return geonkickApi->setPercussionMidiChannel(percussionId(index), channel);
}

bool KitModel::setPercussionName(PercussionIndex index, const std::string &name)
{
        if (geonkickApi->setPercussionName(percussionId(index), name)) {
                geonkickApi->notifyUpdateGui();
                return true;
        }
        return false;
}

std::string KitModel::percussionName(PercussionIndex index) const
{
        return geonkickApi->getPercussionName(percussionId(index));
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

bool KitModel::setPercussionKey(PercussionIndex index, KeyIndex keyIndex)
{
        if (!isValidIndex(index))
                return false;
        if (geonkickApi->setPercussionPlayingKey(percussionId(index), keyIndex)) {
                action percussionUpdated(percussionsList[index]);
                return true;
        }
        return false;
}

KitModel::KeyIndex KitModel::percussionKey(PercussionIndex index) const
{
        return geonkickApi->getPercussionPlayingKey(percussionId(index));
}

double KitModel::getInstrumentMaxLength([[maybe_unused]] PercussionIndex index) const
{
        return geonkickApi->kickMaxLength();
}

bool KitModel::setInstrumentLength([[maybe_unused]] PercussionIndex index, double val)
{
        return geonkickApi->setKickLength(val);
}

double KitModel::getInstrumentLength([[maybe_unused]] PercussionIndex index) const
{
        return geonkickApi->kickLength();
}

bool KitModel::setInstrumentAmplitude([[maybe_unused]] PercussionIndex index, double val)
{
        return geonkickApi->setKickAmplitude(val);
}

double KitModel::getInstrumentAmplitude([[maybe_unused]] PercussionIndex index) const
{
        return geonkickApi->kickAmplitude();
}

void KitModel::playPercussion(PercussionIndex index)
{
        geonkickApi->playKick(percussionId(index));
}

bool KitModel::setPercussionLimiter(PercussionIndex index, int value)
{
        double logVal = (75.0 / 100) * value - 55;
        auto realVal = pow(10, logVal / 20);
        if (geonkickApi->setPercussionLimiter(percussionId(index), realVal)) {
                action limiterUpdated(index);
                return true;
        }
        return false;
}

int KitModel::percussionLimiter(PercussionIndex index) const
{
        auto realVal = geonkickApi->percussionLimiter(percussionId(index));
        double logVal = 20 * log10(realVal);
        int val = (logVal + 55.0) * 100.0 / 75;
        return  val;
}

int KitModel::percussionLeveler(PercussionIndex index) const
{
        auto realVal = geonkickApi->getLimiterLevelerValue(percussionId(index));
        // add small delta to avoid -inf for zero value
        double logVal = 20 * log10(realVal + 0.000000001);
        int val = (logVal + 55.0) * 100.0 / 75;
        if (val < 0)
                val = 0;
        return val;
}

bool KitModel::mutePercussion(PercussionIndex index, bool b)
{
        return geonkickApi->mutePercussion(percussionId(index), b);
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
        action modelUpdated();
}

bool KitModel::open(const std::string &file)
{
        auto kit = std::make_unique<KitState>();
        if (!kit->open(file)) {
                GEONKICK_LOG_ERROR("can't open kit, the preset might be wrong or corrupted");
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
        if (!isValidIndex(index))
                return;

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
        if (!isValidIndex(index) || percussionsList.size() < 2)
                return;

        for (auto it = percussionsList.begin(); it != percussionsList.end(); ++it) {
                if ((*it)->index() == index && geonkickApi->enablePercussion(percussionId(index), false)) {
                        action percussionRemoved(index);
                        bool notify = (*it)->isSelected();
                        delete *it;
                        percussionsList.erase(it);
                        geonkickApi->removeOrderedPercussionId(percussionId(index));
                        if (notify) {
                                geonkickApi->setCurrentPercussion(percussionId(0));
                                action selectPercussion(0);
                        }
                        break;
                }
        }

        for (const auto & per: percussionsList)
                action per->modelUpdated();
}

void KitModel::moveSelectedPercussion(bool down)
{
        auto currentIndex = getIndex(geonkickApi->currentPercussion());
        auto nextIndex = currentIndex + (down ? 1 : -1);
        if (isValidIndex(currentIndex) && isValidIndex(nextIndex)) {
                bool res = geonkickApi->moveOrdrepedPercussionId(geonkickApi->currentPercussion(), down ? 1 : -1);
                if (res) {
                        percussionsList[currentIndex]->setId(percussionId(currentIndex));
                        percussionsList[nextIndex]->setId(percussionId(nextIndex));
                        selectPercussion(nextIndex);
                }
        }
}

size_t KitModel::percussionNumber() const
{
        return geonkickApi->ordredPercussionIds().size();
}

size_t KitModel::maxPercussionNumber() const
{
        return geonkickApi->numberOfInstruments();
}

int KitModel::percussionId(int index) const
{
        const auto &ids = geonkickApi->ordredPercussionIds();
        if (index < 0 || index >= static_cast<decltype(index)>(ids.size()))
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

std::filesystem::path
KitModel::getHomePath() const
{
        return geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH");
}

const std::vector<PercussionModel*>& KitModel::percussionModels() const
{
        return percussionsList;
}

GeonkickApi* KitModel::api() const
{
        return geonkickApi;
}

bool KitModel::doExport(const std::string &file, const ExportInfo &info) const
{
        using ExportFormat = ExportAbstract::ExportFormat;
        switch (info.format) {
        case ExportFormat::Sfz:
        {
                 ExportToSfz toSfz(this, file);
                 return toSfz.doExport();
        }
        case ExportFormat::Flac:
        case ExportFormat::Wav:
        case ExportFormat::Ogg:
        {
                auto currentIndex = getIndex(geonkickApi->currentPercussion());
                ExportSoundData exportToAudioFile(file,
                                                  instrumentData(currentIndex),
                                                  info.format);
                exportToAudioFile.setBitDepth(info.bitDepth);
                exportToAudioFile.setNumberOfChannels(info.channels);
                exportToAudioFile.setSampleRate(api()->getSampleRate());
                return exportToAudioFile.doExport();
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

std::vector<float> KitModel::instrumentData(PercussionIndex index) const
{
        return geonkickApi->getInstrumentBuffer(percussionId(index));
}

bool KitModel::enableNoteOff(PercussionIndex index, bool b)
{
        return geonkickApi->enableNoteOff(percussionId(index), b);
}

bool KitModel::isNoteOffEnabled(PercussionIndex index) const
{
        return geonkickApi->isNoteOffEnabled(percussionId(index));
}

OscillatorModel* KitModel::getCurrentLayerOscillator(OscillatorModel::Type type) const
{
        return geonkickModel->getOscillatorModels()[static_cast<int>(type)];
}

bool KitModel::loadPreset(const Preset &preset, PercussionIndex index)
{
        if (!isValidIndex(index))
                return false;

        auto state = geonkickApi->getDefaultPercussionState();
        if (!state->loadFile(preset.path().string())) {
                GEONKICK_LOG_ERROR("can't open preset");
                return false;
        } else {
                state->setId(geonkickApi->currentPercussion());
                geonkickApi->setPercussionState(state);
                geonkickApi->notifyUpdateGui();
                geonkickApi->notifyPercussionUpdated(state->getId());
                return true;
        }
}

bool KitModel::loadPreset(const Preset &preset)
{
        return open(preset.path().string());
}
