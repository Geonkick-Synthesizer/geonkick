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
class PercussionModel;

class KitModel : public RkObject {
 public:
        using PercussionIndex = int;
        using KeyIndex = int;
        explicit KitModel(GeonkickApi *api);
        bool open(const std::string &file);
        bool save(const std::string &file);
        void addNewPercussion();
        void copyPercussion(PercussionIndex index);
        void removePercussion(PercussionIndex index);
        int percussionKeyIndex(PercussionIndex index) const;
        void moveSelectedPercussion(bool down = true);
        size_t keysNumber() const;
        std::string keyName(KeyIndex index) const;
        size_t percussionNumber() const;
        std::filesystem::path workingPath(const std::string &key) const;
        const std::vector<PercussionModel*>& percussionModels() const;

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

 protected:
        int getPercussionId(int index) const;

 private:
        GeonkickApi *geonkickApi;
        std::vector<PercussionModel*> percussionsList;
};

#endif // KIT_MODEL_H
