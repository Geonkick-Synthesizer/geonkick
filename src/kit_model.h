/**
 * File name: kit_model.h
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

#ifndef KIT_MODEL_H
#define KIT_MODEL_H

#include "globals.h"
#include "ExportAbstract.h"

class GeonkickApi;
class GeonkickState;
class InstrumentModel;

class KitModel : public RkObject {
 public:
        using ExportFormat = ExportAbstract::ExportFormat;
        using InstrumentIndex = int;
        using KeyIndex = int;
        explicit KitModel(RkObject* parent, GeonkickApi *api);
        bool isValidIndex(InstrumentIndex index);
        bool open(const std::string &file);
        bool save(const std::string &file);
        void selectInstrument(InstrumentIndex index);
        bool isInstrumentSelected(InstrumentIndex index) const;
        InstrumentIndex selectedInstrument() const;
        InstrumentModel* currentInstrument() const;
        size_t numberOfChannels() const;
        int instrumentChannel(InstrumentIndex index) const;
        bool setInstrumentChannel(InstrumentIndex index, int channel);
        bool setInstrumentKey(InstrumentIndex index, KeyIndex key);
        KeyIndex instrumentKey(InstrumentIndex index) const;
        bool setInstrumentName(InstrumentIndex index, const std::string &name);
        std::string instrumentName(InstrumentIndex index) const;
        void addNewInstrument();
        void copyInstrument(InstrumentIndex index);
        void removeInstrument(InstrumentIndex index);
        void moveSelectedInstrument(bool down = true);
        int instrumentKeyIndex(InstrumentIndex index) const;
        size_t keysNumber() const;
        std::string keyName(KeyIndex index) const;
        size_t instrumentNumber() const;
        size_t maxInstrumentNumber() const;
        void playInstrument(InstrumentIndex index);
        std::filesystem::path workingPath(const std::string &key) const;
        std::filesystem::path getHomePath() const;
        const std::vector<InstrumentModel*>& instrumentModels() const;
        InstrumentIndex getIndex(int id) const;
        bool setInstrumentLimiter(InstrumentIndex index, int value);
        int instrumentLimiter(InstrumentIndex index) const;
        int instrumentLeveler(InstrumentIndex index) const;
        bool muteInstrument(InstrumentIndex index, bool b);
        bool isInstrumentMuted(InstrumentIndex index) const;
        bool soloInstrument(InstrumentIndex index, bool b);
        bool isInstrumentSolo(InstrumentIndex index) const;
        void updateInstrument(InstrumentIndex index);
        GeonkickApi* api() const;
        bool doExport(const std::string &file, ExportFormat format);

        RK_DECL_ACT(modelUpdated,
                    modelUpdated(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(instrumentAdded,
                    instrumentAdded(InstrumentModel* model),
                    RK_ARG_TYPE(InstrumentModel*),
                    RK_ARG_VAL(model));
        RK_DECL_ACT(instrumentRemoved,
                    instrumentRemoved(InstrumentIndex index),
                    RK_ARG_TYPE(InstrumentIndex),
                    RK_ARG_VAL(index));
        RK_DECL_ACT(instrumentSelected,
                    instrumentSelected(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(limiterUpdated,
                    limiterUpdated(InstrumentIndex index),
                    RK_ARG_TYPE(InstrumentIndex),
                    RK_ARG_VAL(index));
        RK_DECL_ACT(instrumentUpdated,
                    instrumentUpdated(InstrumentModel* model),
                    RK_ARG_TYPE(InstrumentModel*),
                    RK_ARG_VAL(model));
        RkString name() const;
        RkString author() const;
        RkString license() const;
        std::vector<float> instrumentData(InstrumentIndex index) const;

 protected:
        int instrumentId(int index) const;
        void loadModelData();

 private:
        GeonkickApi *geonkickApi;
        std::vector<InstrumentModel*> instrumentsList;
        std::vector<std::string> midiKeys;
};

#endif // KIT_MODEL_H
