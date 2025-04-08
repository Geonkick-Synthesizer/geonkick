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
#include "RkButton.h"
#include "RkEvent.h"

RK_DECLARE_IMAGE_RC(presetnav_next);
RK_DECLARE_IMAGE_RC(presetnav_next_hover);
RK_DECLARE_IMAGE_RC(presetnav_next_pressed);
RK_DECLARE_IMAGE_RC(presetnav_previous);
RK_DECLARE_IMAGE_RC(presetnav_previous_hover);
RK_DECLARE_IMAGE_RC(presetnav_previous_pressed);

PresetNameLabel::PresetNameLabel(RkWidget* parent)
        : RkLabel(parent)
{
}

void PresetNameLabel::wheelEvent(RkWheelEvent *event)
{
        if (event->direction() == RkWheelEvent::WheelDirection::DirectionDown)
                action nextPreset();
        else
                action previousPreset();
}

PresetNavigator::PresetNavigator(GeonkickWidget *parent,
                                 PresetBrowserModel* model)
        : GeonkickWidget(parent)
        , presetsModel{model}
        , presetFolderName{new PresetNameLabel(this)}
        , presetName{new PresetNameLabel(this)}
{
        setFixedSize(232, 23);
        setBackgroundColor({30, 30, 30});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());

        // Preset folder name
        mainLayout->addSpace(1);
        presetFolderName->setSize(width() / 2 - height()  / 2, height() - 2);
        presetFolderName->setBackgroundColor({44, 44, 44});
        presetFolderName->setTextColor({140, 140, 140});
        presetFolderName->show();
        mainLayout->addWidget(presetFolderName);

        // Preset folder controls
        auto controlsLayout = new RkContainer(this, Rk::Orientation::Vertical);
        controlsLayout->setSize({height() / 2, height()});
        controlsLayout->addSpace(2);
        auto previousButton = new RkButton(this);
        previousButton->setType(RkButton::ButtonType::ButtonPush);
        previousButton->setSize(height() / 2 - 2, height() / 2 - 2);
        previousButton->setBackgroundColor({44, 44, 44});
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous)),
                                 RkButton::State::Unpressed);
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous_hover)),
                                 RkButton::State::UnpressedHover);
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous_pressed)),
                                 RkButton::State::Pressed);
        previousButton->show();
        controlsLayout->addWidget(previousButton);
        controlsLayout->addSpace(1);
        auto nextButton = new RkButton(this);
        nextButton->setType(RkButton::ButtonType::ButtonPush);
        nextButton->setSize(height() / 2 - 2, height() / 2 - 2);
        nextButton->setBackgroundColor({44, 44, 44});
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next)),
                             RkButton::State::Unpressed);
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next_hover)),
                             RkButton::State::UnpressedHover);
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next_pressed)),
                             RkButton::State::Pressed);
        nextButton->show();
        controlsLayout->addWidget(nextButton);
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

        RK_ACT_BIND(presetName, previousPreset,
                    RK_ACT_ARGS(), presetsModel, selectPreviousPreset());
        RK_ACT_BIND(presetName, nextPreset,
                    RK_ACT_ARGS(), presetsModel, selectNextPreset());
        RK_ACT_BIND(presetFolderName, previousPreset,
                    RK_ACT_ARGS(), presetsModel, selectPreviousFolder());
        RK_ACT_BIND(presetFolderName, nextPreset,
                    RK_ACT_ARGS(), presetsModel, selectNextFolder());

        // Preset name
        presetName->setSize(width() / 2 - 13, height() - 2);
        presetName->setTextColor({140, 140, 140});
        presetName->setBackgroundColor({44, 44, 44});
        presetName->show();
        mainLayout->addWidget(presetName);

        // Preset controls
        controlsLayout = new RkContainer(this, Rk::Orientation::Vertical);
        controlsLayout->setSize({height() / 2, height()});
        controlsLayout->addSpace(2);
        previousButton = new RkButton(this);
        previousButton->setType(RkButton::ButtonType::ButtonPush);
        previousButton->setSize(height() / 2 - 2, height() / 2 - 2);
        previousButton->setBackgroundColor({44, 44, 44});
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous)),
                                 RkButton::State::Unpressed);
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous_hover)),
                                 RkButton::State::UnpressedHover);
        previousButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_previous_pressed)),
                                 RkButton::State::Pressed);
        previousButton->show();
        controlsLayout->addWidget(previousButton);
        controlsLayout->addSpace(1);
        nextButton = new RkButton(this);
        nextButton->setType(RkButton::ButtonType::ButtonPush);
        nextButton->setSize(height() / 2 - 2, height() / 2 - 2);
        nextButton->setBackgroundColor({44, 44, 44});
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next)),
                             RkButton::State::Unpressed);
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next_hover)),
                             RkButton::State::UnpressedHover);
        nextButton->setImage(RkImage({9, 9}, RK_IMAGE_RC(presetnav_next_pressed)),
                             RkButton::State::Pressed);
        nextButton->show();
        controlsLayout->addWidget(nextButton);
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
        if (!folder)
                return;

        presetFolderName->setText(folder->name());
        auto preset = presetsModel->currentSelectedPreset();
        if (!preset)
                preset = folder->preset(0);
        if (!preset)
                return;
        presetName->setText(preset->name());
}


