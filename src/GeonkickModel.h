/**
 * File name: GeonkickModel.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef GEONKICK_MODEL_H
#define GEONKICK_MODEL_H

#include "globals.h"
#include "geonkick_api.h"

#include <RkObject.h>

class KitModel;
class PresetBrowserModel;
class OscillatorModel;

class GeonkickModel : public RkObject {
 public:
        enum class KitType: int {
                SingleInstrument,
                MultiInstrument,
        };
        using InstanceType = GeonkickApi::InstanceType;
        explicit GeonkickModel(RkObject* parent, GeonkickApi *api);
        GeonkickApi* api() const;
        KitModel* getKitModel() const;
        PresetBrowserModel* getPresetsModel() const;
        const std::vector<OscillatorModel*>& getOscillatorModels() const;
        InstanceType instanceType() const;
        KitType kitType() const;
        bool isBasic() const;

 private:
        GeonkickApi *geonkickApi;
        std::vector<OscillatorModel*> oscillatorModels;
        KitModel *kitModel;
        PresetBrowserModel* presetModel;
};

#endif // GEONKICK_MODEL_H
