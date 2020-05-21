/**
 * File name: presets_browser_model.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "presets_browser_model.h"

PresetsBrowserModel::PresetsBrowserModel(RkObject *parent, GeonkickApi *api)
        : RkObject(parent)
          numberOfColumns{4}
          presetsPerColumn{10}
          presetsPerPage{presetsPerColumn * 3}
{
}

std::string PresetsBrowserModel::presetName(int row, int column) const
{
        if (0 <= column < numberOfColumns && row < presetsPerColumn) {
                auto presetFolder = geonkickApi->getPresetFolder(row);
                if (presetFolder) {
                        if (column == 0)
                                return presetFolder->name();
                        else if (column > 0)
                                return presetFolder->preset(page() * presetsPerPage * (column - 1) * presetsPerColumn + row);
                }
        }
}


        // RK_DECL_ACT(modelUpdated,
        //             modelUpdated(),
        //             RK_ARG_TYPE(),
        //             RK_ARG_VAL());
        // RK_DECL_ACT(selected,
        //             selected(),
        //             RK_ARG_TYPE(),
        //             RK_ARG_VAL());
        // RK_DECL_ACT(nameUpdated,
        //             nameUpdated(std::string name),
        //             RK_ARG_TYPE(std::string),
        //             RK_ARG_VAL(name));
        // RK_DECL_ACT(keyUpdated,
        //             keyUpdated(KeyIndex key),
        //             RK_ARG_TYPE(KeyIndex),
        //             RK_ARG_VAL(key));
        // RK_DECL_ACT(channelUpdated,
        //             channelUpdated(int val),
        //             RK_ARG_TYPE(int),
        //             RK_ARG_VAL(val));
        // RK_DECL_ACT(limiterUpdated,
        //             limiterUpdated(int val),
        //             RK_ARG_TYPE(int),
        //             RK_ARG_VAL(val));
        // RK_DECL_ACT(levelerUpdated,
        //             levelerUpdated(int val),
        //             RK_ARG_TYPE(int),
        //             RK_ARG_VAL(val));
        // RK_DECL_ACT(muteUpdated,
        //             muteUpdated(bool b),
        //             RK_ARG_TYPE(bool),
        //             RK_ARG_VAL(b));
        // RK_DECL_ACT(soloUpdated,
        //             soloUpdated(bool b),
        //             RK_ARG_TYPE(bool),
        //             RK_ARG_VAL(b));

 protected:

 private:
        GeonkickApi* geonkickApi;
};

#endif // BROWSER_MODEL_H
