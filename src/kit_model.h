/**
 * File name: kit_model.h
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

#ifndef KIT_MODEL_H
#define KIT_MODEL_H

#include "globals.h"
#include "ExportAbstract.h"
#include "OscillatorModel.h"

class GeonkickApi;
class GeonkickState;
class PercussionModel;
class GeonkickModel;
class Preset;

class KitModel : public RkObject {
 public:
        struct ExportInfo {
                ExportAbstract::ExportFormat format;
                int bitDepth;
                int channels;
        };

        using PercussionIndex = int;
        using KeyIndex = int;
        explicit KitModel(GeonkickModel* parent);
        bool enableInstrument(PercussionIndex index, bool b = true);
        bool isInstrumentEnabled(PercussionIndex index) const;
        bool isValidIndex(PercussionIndex index);
        bool open(const std::string &file);
        bool save(const std::string &file);
        void selectPercussion(PercussionIndex index);
        bool isPercussionSelected(PercussionIndex index) const;
        PercussionIndex selectedPercussion() const;
        PercussionModel* currentPercussion() const;
        size_t numberOfChannels() const;
        int percussionChannel(PercussionIndex index) const;
        bool setPercussionChannel(PercussionIndex index, int channel);
        size_t numberOfMidiChannels() const;
        int percussionMidiChannel(PercussionIndex index) const;
        bool setPercussionMidiChannel(PercussionIndex index, int channel);
        bool setPercussionKey(PercussionIndex index, KeyIndex key);
        KeyIndex percussionKey(PercussionIndex index) const;
        bool setPercussionName(PercussionIndex index, const std::string &name);
        std::string percussionName(PercussionIndex index) const;
        void addNewPercussion();
        void copyPercussion(PercussionIndex index);
        void removePercussion(PercussionIndex index);
        void moveSelectedPercussion(bool down = true);
        int percussionKeyIndex(PercussionIndex index) const;
        size_t keysNumber() const;
        std::string keyName(KeyIndex index) const;
        size_t percussionNumber() const;
        size_t maxPercussionNumber() const;
        double getInstrumentMaxLength(PercussionIndex index) const;
        bool setInstrumentLength(PercussionIndex index, double val);
        double getInstrumentLength(PercussionIndex index) const;
        bool setInstrumentAmplitude(PercussionIndex index, double val);
        double getInstrumentAmplitude(PercussionIndex index) const;
        void playPercussion(PercussionIndex index);
        std::filesystem::path workingPath(const std::string &key) const;
        std::filesystem::path getHomePath() const;
        const std::vector<PercussionModel*>& percussionModels() const;
        PercussionIndex getIndex(int id) const;
        bool setPercussionLimiter(PercussionIndex index, int value);
        int percussionLimiter(PercussionIndex index) const;
        int percussionLeveler(PercussionIndex index) const;
        bool mutePercussion(PercussionIndex index, bool b);
        bool isPercussionMuted(PercussionIndex index) const;
        bool soloPercussion(PercussionIndex index, bool b);
        bool isPercussionSolo(PercussionIndex index) const;
        void updatePercussion(PercussionIndex index);
        GeonkickApi* api() const;
        bool doExport(const std::string &file, const ExportInfo &info) const;
        bool enableNoteOff(PercussionIndex index, bool b);
        bool isNoteOffEnabled(PercussionIndex index) const;
        OscillatorModel* getCurrentLayerOscillator(OscillatorModel::Type type) const;
        bool loadPreset(const Preset &preset, PercussionIndex index);
        bool loadPreset(const Preset &preset);

        RK_DECL_ACT(modelUpdated,
                    modelUpdated(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(percussionAdded,
                    percussionAdded(PercussionModel* model),
                    RK_ARG_TYPE(PercussionModel*),
                    RK_ARG_VAL(model));
        RK_DECL_ACT(percussionRemoved,
                    percussionRemoved(PercussionIndex index),
                    RK_ARG_TYPE(PercussionIndex),
                    RK_ARG_VAL(index));
        RK_DECL_ACT(percussionSelected,
                    percussionSelected(PercussionModel* model),
                    RK_ARG_TYPE(PercussionModel*),
                    RK_ARG_VAL(model));
        RK_DECL_ACT(percussionUpdated,
                    percussionUpdated(PercussionModel* model),
                    RK_ARG_TYPE(PercussionModel*),
                    RK_ARG_VAL(model));
        RK_DECL_ACT(limiterUpdated,
                    limiterUpdated(PercussionIndex index),
                    RK_ARG_TYPE(PercussionIndex),
                    RK_ARG_VAL(index));
        RK_DECL_ACT(instrumentEnabled,
                    instrumentEnabled(PercussionModel* model),
                    RK_ARG_TYPE(PercussionModel*),
                    RK_ARG_VAL(model));
        RkString name() const;
        RkString author() const;
        RkString license() const;
        std::vector<float> instrumentData(PercussionIndex index) const;
        int percussionId(int index) const;

 protected:
        void loadModelData();

 private:
        GeonkickModel *geonkickModel;
        GeonkickApi *geonkickApi;
        std::vector<PercussionModel*> percussionsList;
        std::vector<std::string> midiKeys;
};

#endif // KIT_MODEL_H
