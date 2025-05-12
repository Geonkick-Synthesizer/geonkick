/**
 * File name: GeonkickModel.cpp
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

#include "GeonkickModel.h"
#include "geonkick_api.h"
#include "kit_model.h"
#include "preset_browser_model.h"
#include "OscillatorModel.h"

GeonkickModel::GeonkickModel(RkObject* parent, GeonkickApi *api)
        : RkObject(parent)
        , geonkickApi{api}
        , kitModel{new KitModel(this)}
        , presetModel{new PresetBrowserModel(this, geonkickApi)}
{
        auto n = geonkickApi->oscillatorsPerLayer();
        for (decltype(n) i = 0; i < n; i++)
                oscillatorModels.emplace_back(new OscillatorModel(this, static_cast<OscillatorModel::Type>(i)));
}

GeonkickApi* GeonkickModel::api() const
{
        return geonkickApi;
}

const std::vector<OscillatorModel*>& GeonkickModel::getOscillatorModels() const
{
        return oscillatorModels;
}


KitModel* GeonkickModel::getKitModel() const
{
        return kitModel;
}

PresetBrowserModel* GeonkickModel::getPresetsModel() const
{
        return presetModel;
}

GeonkickModel::InstanceType GeonkickModel::instanceType() const
{
        return geonkickApi->getInstanceType();
}

GeonkickModel::KitType GeonkickModel::kitType() const
{
#if defined(GEONKICK_SINGLE)
        return KitType::SingleInstrument;
#else
        return KitType::MultiInstrument;
#endif // GEONKICK_SINGLE
}

bool GeonkickModel::isBasic() const
{
#if defined(GEONKICK_LIMITED_VERSION)
        return true;
#else
        return false;
#endif // GEONKICK_LIMITED_VERSION
}
