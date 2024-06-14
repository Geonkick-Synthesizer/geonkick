/**
 * File name: PresetNavigator.cpp
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

#include "PresetNavigator.h"
#include "preset_browser_model.h"
#include "preset_folder.h"
#include "preset.h"

#include "RkContainer.h"
#include "RkSpinBox.h"
#include "RkLabel.h"
#include "RkButton.h"

PresetNavigator::PresetNavigator(GeonkickWidget *parent,
                                 PresetBrowserModel* model)
        : GeonkickWidget(parent)
        , presetsModel{model}
        , presetFolderName{new RkLabel(this)}
        , presetName{new RkLabel(this)}
{
        setFixedSize(160, 28);
        setBackgroundColor({0, 134, 156});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());
        mainLayout->setSpacing(0);

        // Preset folder name
        presetFolderName->setSize(width() / 2 - height()  / 2, height() - 2);
        presetFolderName->setBackgroundColor({255, 0,0}/*background()*/);
        presetFolderName->show();
        mainLayout->addWidget(presetFolderName);

        // Preset folder controls
        auto controlsLayout = new RkContainer(this, Rk::Orientation::Vertical);
        controlsLayout->setSize({height() / 2 - 2, height()});
        controlsLayout->addSpace(2);
        auto nextButton = new RkButton(this);
        nextButton->setType(RkButton::ButtonType::ButtonPush);
        nextButton->setSize(height() / 2 - 4, height() / 2 - 4);
        nextButton->setBackgroundColor({0, 255, 0});
        nextButton->show();
        controlsLayout->addWidget(nextButton);
        controlsLayout->addSpace(4);
        auto previousButton = new RkButton(this);
        previousButton->setType(RkButton::ButtonType::ButtonPush);
        previousButton->setSize(height() / 2 - 4, height() / 2 - 4);
        previousButton->setBackgroundColor({0, 255, 0});
        previousButton->show();
        controlsLayout->addWidget(previousButton);
        mainLayout->addContainer(controlsLayout);
        RK_ACT_BIND(nextButton,
                    pressed,
                    RK_ACT_ARGS(),
                    presetsModel,
                    selectNextFolder());
        RK_ACT_BIND(previousButton,
                    pressed,
                    RK_ACT_ARGS(),
                    presetsModel,
                    selectPreviousFolder());

        // Preset name
        presetName->setSize(width() / 2 - height() - 2, height() - 2);
        presetName->setBackgroundColor({255, 0,0}/*background()*/);
        presetName->show();
        mainLayout->addWidget(presetName);

        // Preset controls
        controlsLayout = new RkContainer(this, Rk::Orientation::Vertical);
        controlsLayout->setSize({height() / 2 - 2, height()});
        controlsLayout->addSpace(2);
        nextButton = new RkButton(this);
        nextButton->setType(RkButton::ButtonType::ButtonPush);
        nextButton->setSize(height() / 2 - 4, height() / 2 - 4);
        nextButton->setBackgroundColor({0, 255, 0});
        nextButton->show();
        controlsLayout->addWidget(nextButton);
        controlsLayout->addSpace(4);
        previousButton = new RkButton(this);
        previousButton->setType(RkButton::ButtonType::ButtonPush);
        previousButton->setSize(height() / 2 - 4, height() / 2 - 4);
        previousButton->setBackgroundColor({0, 255, 0});
        previousButton->show();
        controlsLayout->addWidget(previousButton);
        mainLayout->addContainer(controlsLayout);
        RK_ACT_BIND(nextButton,
                    pressed,
                    RK_ACT_ARGS(),
                    presetsModel,
                    selectNextPreset());
        RK_ACT_BIND(previousButton,
                    pressed,
                    RK_ACT_ARGS(),
                    presetsModel,
                    selectPreviousPreset());

        RK_ACT_BIND(presetsModel,
                    folderSelected,
                    RK_ACT_ARGS(PresetFolder*),
                    this,
                    updateView());
        RK_ACT_BIND(presetsModel,
                    presetSelected,
                    RK_ACT_ARGS(Preset*),
                    this,
                    updateView());
        updateView();
}

void PresetNavigator::updateView()
{
        auto folder = presetsModel->currentSelectedFolder();
        if (folder) {
                presetFolderName->setText(folder->name());
                auto preset = presetsModel->currentSelectedPreset();
                if (!preset)
                        preset = folder->preset(0);
                if (preset)
                        presetName->setText(preset->name());
        }
}


