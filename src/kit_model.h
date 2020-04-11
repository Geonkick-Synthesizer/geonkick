/**
 * File name: kit_model.h
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

#ifndef KIT_MODEL_H
#define KIT_MODEL_H

#include "globals.h"

class GeonkickApi;
class GeonkickState;

class KitModel {
 public:
        explicit KitModel(GeonkickApi *api);
        bool open(const std::string &file);
        bool save(const std::string &file);
        void selectPercussion(int index);
        bool percussionSelected(int index) const;
        void addNewPercussion();
        void copyPercussion(int index);
        void removePercussion(int index);
        void increasePercussionChannel(int index);
        void decreasePercussionChannel(int index);
        int percussionKeyIndex(int index) const;
        void moveSelectedPercussion(bool down = true);
        void setPercussionKey(int index, int keyIndex);
        size_t keysNumber() const;
        std::string keyName(int index) const;
        size_t percussionNumber() const;
        void setPercussionName(int index, const std::string &name);
        std::string percussionName(int index) const;
        int percussionChannel(int index) const;
        bool canCopy() const;
        bool canRemove() const;
        std::filesystem::path workingPath(const std::string &key) const;

        RK_DECL_ACT(modelUpdated, modelUpdated(), RK_ARG_TYPE(), RK_ARG_VAL());

 protected:
        int getPercussionId(int index) const;

 private:
        GeonkickApi *geonkickApi;
        std::vector<std::string> midiKeys;
};

#endif // KIT_MODEL_H
