/**
 * File name: top_bar.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://iuriepage.wordpress.com)
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

#include "top_bar.h"
#include "geonkick_button.h"
#include "preset_browser_model.h"
#include "preset_browser_view.h"

#include <RkLabel.h>
#include <RkButton.h>
#include <RkTransition.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(logo);
RK_DECLARE_IMAGE_RC(open);
RK_DECLARE_IMAGE_RC(save);
RK_DECLARE_IMAGE_RC(export_active);
RK_DECLARE_IMAGE_RC(about);
RK_DECLARE_IMAGE_RC(reset_button);
RK_DECLARE_IMAGE_RC(reset_button_hover);
RK_DECLARE_IMAGE_RC(reset_button_on);
RK_DECLARE_IMAGE_RC(play);
RK_DECLARE_IMAGE_RC(play_pressed);
RK_DECLARE_IMAGE_RC(topbar_layer1);
RK_DECLARE_IMAGE_RC(topbar_layer2);
RK_DECLARE_IMAGE_RC(topbar_layer3);
RK_DECLARE_IMAGE_RC(topbar_layer1_disabled);
RK_DECLARE_IMAGE_RC(topbar_layer2_disabled);
RK_DECLARE_IMAGE_RC(topbar_layer3_disabled);
RK_DECLARE_IMAGE_RC(tune_checkbox_on);
RK_DECLARE_IMAGE_RC(tune_checkbox_off);

TopBar::TopBar(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent)
        , openFileButton{nullptr}
        , saveFileButton{nullptr}
        , exportFileButton{nullptr}
        , presetNameLabel{nullptr}
        , layer1Button{nullptr}
        , layer2Button{nullptr}
        , layer3Button{nullptr}
        , geonkickApi{api}
        , tuneCheckbox{nullptr}
{
        setFixedSize({parent->width(), 30});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());

        // auto logo = new RkLabel(this);
        // RkImage image(120, 20, RK_IMAGE_RC(logo));
        // logo->setSize(image.width(), image.height());
        // logo->setBackgroundColor(68, 68, 70);
        // logo->setImage(image);
        // logo->show();
        // mainLayout->addWidget(logo);
         mainLayout->addSpace(5);

        openFileButton = new GeonkickButton(this);
        openFileButton->show();
        openFileButton->setSize(43, 15);
        openFileButton->setUnpressedImage(RkImage(openFileButton->size(), RK_IMAGE_RC(open)));
        openFileButton->setCheckable(true);
        RK_ACT_BIND(openFileButton, pressed, RK_ACT_ARGS(), this, openFile());
        mainLayout->addWidget(openFileButton);

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setSize(43, 15);
        saveFileButton->setUnpressedImage(RkImage(saveFileButton->size(), RK_IMAGE_RC(save)));
        saveFileButton->setCheckable(true);
        RK_ACT_BIND(saveFileButton, pressed, RK_ACT_ARGS(), this, saveFile());
        mainLayout->addWidget(saveFileButton);

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setSize(43, 15);
        exportFileButton->setUnpressedImage(RkImage(exportFileButton->size(), RK_IMAGE_RC(export_active)));
        exportFileButton->setCheckable(true);
        RK_ACT_BIND(exportFileButton, pressed, RK_ACT_ARGS(), this, openExport());
        mainLayout->addWidget(exportFileButton);

        auto resetButton = new RkButton(this);
        resetButton->setSize(43, 15);
        resetButton->setType(RkButton::ButtonType::ButtonPush);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_button)),
                              RkButton::ButtonImage::ImageUnpressed);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_button_hover)),
                              RkButton::ButtonImage::ImageUnpressedHover);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_button_on)),
                              RkButton::ButtonImage::ImagePressed);
        resetButton->show();
        RK_ACT_BIND(resetButton, pressed, RK_ACT_ARGS(), this, resetToDefault());
        mainLayout->addSpace(20);
        mainLayout->addWidget(resetButton);

        presetNameLabel = new RkLabel(this);
        presetNameLabel->setBackgroundColor(background());
        presetNameLabel->setTextColor({210, 226, 226, 140});
        auto font = presetNameLabel->font();
        font.setSize(12);
        presetNameLabel->setFont(font);
        presetNameLabel->setSize(220, 30);
        presetNameLabel->show();
        mainLayout->addWidget(presetNameLabel);

	auto playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(37, 15);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play)),
                             RkButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), geonkickApi, playKick());
	playButton->show();
        mainLayout->addWidget(playButton);
        mainLayout->addSpace(10);

        createLyersButtons(mainLayout);
        mainLayout->addSpace(25);

        tuneCheckbox = new GeonkickButton(this);
        tuneCheckbox->setCheckable(true);
        tuneCheckbox->setFixedSize(46, 11);
        tuneCheckbox->setPressedImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_on)));
        tuneCheckbox->setUnpressedImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_off)));
        tuneCheckbox->show();
        RK_ACT_BIND(tuneCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi,
		    tuneAudioOutput(geonkickApi->currentPercussion(), b));
        mainLayout->addWidget(tuneCheckbox);

        updateGui();
}

void TopBar::createLyersButtons(RkContainer *mainLayout)
{
        layer1Button = new GeonkickButton(this);
        layer1Button->setBackgroundColor(background());
        layer1Button->setSize(24, 24);
        layer1Button->setUnpressedImage(RkImage(layer1Button->size(), RK_IMAGE_RC(topbar_layer1_disabled)));
        layer1Button->setPressedImage(RkImage(layer1Button->size(), RK_IMAGE_RC(topbar_layer1)));
        layer1Button->setCheckable(true);
        mainLayout->addWidget(layer1Button);
        mainLayout->addSpace(5);

        layer2Button = new GeonkickButton(this);
        layer2Button->setBackgroundColor(background());
        layer2Button->setSize(24, 24);
	layer2Button->setUnpressedImage(RkImage(layer2Button->size(), RK_IMAGE_RC(topbar_layer2_disabled)));
	layer2Button->setPressedImage(RkImage(layer2Button->size(), RK_IMAGE_RC(topbar_layer2)));
        layer2Button->setCheckable(true);
        mainLayout->addWidget(layer2Button);
        mainLayout->addSpace(5);

        layer3Button = new GeonkickButton(this);
        layer3Button->setBackgroundColor(background());
        layer3Button->setSize(24, 24);
        layer3Button->setUnpressedImage(RkImage(layer3Button->size(), RK_IMAGE_RC(topbar_layer3_disabled)));
        layer3Button->setPressedImage(RkImage(layer3Button->size(), RK_IMAGE_RC(topbar_layer3)));
        layer3Button->setCheckable(true);
        mainLayout->addWidget(layer3Button);

        RK_ACT_BIND(layer1Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer1, b));
        RK_ACT_BIND(layer3Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer3, b));
        RK_ACT_BIND(layer2Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer2, b));
}

void TopBar::setPresetName(const std::string &name)
{
        if (name.size() > 20) {
                std::string preset = name;
                preset.resize(15);
                preset += "...";
                presetNameLabel->setText(preset);
        } else {
                presetNameLabel->setText(name);
        }
}

void TopBar::updateGui()
{
        layer1Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer1));
        layer2Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer2));
        layer3Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer3));
        tuneCheckbox->setPressed(geonkickApi->isAudioOutputTuned(geonkickApi->currentPercussion()));
        setPresetName(geonkickApi->getPercussionName(geonkickApi->currentPercussion()));
}
