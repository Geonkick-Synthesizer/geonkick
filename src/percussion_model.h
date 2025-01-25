/**
 * File name: percussion_model.h
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

#ifndef PERCUSSION_MODEL_H
#define PERCUSSION_MODEL_H

#include "globals.h"

class GeonkickApi;
class GeonkickState;
class KitModel;

class PercussionModel : public RkObject {
 public:
        using PercussionIndex = int;
        using KeyIndex = GeonkickTypes::MidiKey;
        explicit PercussionModel(KitModel* parent, int id = -1);
        virtual ~PercussionModel() = default;
        void setId(int id);
        PercussionIndex index() const;
        void select();
        bool isSelected() const;
        void increasePercussionChannel();
        void decreasePercussionChannel();
        size_t keysNumber() const;
        void setKey(KeyIndex keyIndex);
        KeyIndex key() const;
        void setName(const std::string &name);
        std::string name() const;
        void setChannel(int index);
        int channel() const;
        size_t numberOfChannels() const;
        bool canCopy() const;
        bool canRemove() const;
        void play();
        void setLimiter(int value);
        int limiter() const;
        int leveler() const;
        void mute(bool b);
        bool isMuted() const;
        void solo(bool b);
        bool isSolo() const;
        void remove();
        void copy();
        KitModel* model() const;
        std::vector<float> data() const;
        size_t numberOfMidiChannels() const;
        int midiChannel() const;
        void setMidiChannel(int chIndex);
        void enableNoteOff(bool b);
        bool isNoteOffEnabled() const;

        RK_DECL_ACT(modelUpdated,
                    modelUpdated(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(selected,
                    selected(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(nameUpdated,
                    nameUpdated(std::string name),
                    RK_ARG_TYPE(std::string),
                    RK_ARG_VAL(name));
        RK_DECL_ACT(keyUpdated,
                    keyUpdated(KeyIndex key),
                    RK_ARG_TYPE(KeyIndex),
                    RK_ARG_VAL(key));
        RK_DECL_ACT(channelUpdated,
                    channelUpdated(int val),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(val));
        RK_DECL_ACT(limiterUpdated,
                    limiterUpdated(int val),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(val));
        RK_DECL_ACT(levelerUpdated,
                    levelerUpdated(int val),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(val));
        RK_DECL_ACT(muteUpdated,
                    muteUpdated(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(soloUpdated,
                    soloUpdated(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(midiChannelUpdated,
                    midiChannelUpdated(int index),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(index));
        RK_DECL_ACT(noteOffUpdated,
                    noteOffUpdated(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));

 protected:
        void onPercussionSelected();

 private:
        KitModel* kitModel;
        int percussionId;
};

#endif // PERCUSSION_MODEL_H
