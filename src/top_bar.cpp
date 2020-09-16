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
#include "ViewState.h"

#include <RkLabel.h>
#include <RkButton.h>
#include <RkTransition.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(logo);
RK_DECLARE_IMAGE_RC(open);
RK_DECLARE_IMAGE_RC(save);
RK_DECLARE_IMAGE_RC(export);
RK_DECLARE_IMAGE_RC(reset);
RK_DECLARE_IMAGE_RC(reset_hover);
RK_DECLARE_IMAGE_RC(reset_active);
RK_DECLARE_IMAGE_RC(play);
RK_DECLARE_IMAGE_RC(play_pressed);
RK_DECLARE_IMAGE_RC(play_hover);
RK_DECLARE_IMAGE_RC(layer1);
RK_DECLARE_IMAGE_RC(layer2);
RK_DECLARE_IMAGE_RC(layer3);
RK_DECLARE_IMAGE_RC(layer1_disabled);
RK_DECLARE_IMAGE_RC(layer2_disabled);
RK_DECLARE_IMAGE_RC(layer3_disabled);
RK_DECLARE_IMAGE_RC(layer1_hover);
RK_DECLARE_IMAGE_RC(layer2_hover);
RK_DECLARE_IMAGE_RC(layer3_hover);
RK_DECLARE_IMAGE_RC(tune_checkbox_on);
RK_DECLARE_IMAGE_RC(tune_checkbox_off);
RK_DECLARE_IMAGE_RC(tune_checkbox_hover);
RK_DECLARE_IMAGE_RC(topmenu_controls_active);
RK_DECLARE_IMAGE_RC(topmenu_controls_hover);
RK_DECLARE_IMAGE_RC(topmenu_controls_off);
RK_DECLARE_IMAGE_RC(topmenu_kit_active);
RK_DECLARE_IMAGE_RC(topmenu_kit_hover);
RK_DECLARE_IMAGE_RC(topmenu_kit_off);
RK_DECLARE_IMAGE_RC(topmenu_presets_active);
RK_DECLARE_IMAGE_RC(topmenu_presets_hover);
RK_DECLARE_IMAGE_RC(topmenu_presets_off);
RK_DECLARE_IMAGE_RC(topmenu_samples_active);
RK_DECLARE_IMAGE_RC(topmenu_samples_hover);
RK_DECLARE_IMAGE_RC(topmenu_samples_off);

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
        , viewState{dynamic_cast<ViewState*>(findObject("ViewState"))}
        , controlsButton{nullptr}
        , kitButton{nullptr}
        , presetsButton{nullptr}
        , samplesButton{nullptr}
{
        setFixedSize({parent->width(), 30});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());

        auto logo = new RkLabel(this);
        RkImage image(62, 10, RK_IMAGE_RC(logo));
        logo->setSize(image.width(), image.height());
        logo->setBackgroundColor(68, 68, 70);
        logo->setImage(image);
        logo->show();
        mainLayout->addWidget(logo);
        addSeparator(mainLayout, 10);

        openFileButton = new GeonkickButton(this);
        openFileButton->show();
        openFileButton->setSize(26, 10);
        openFileButton->setUnpressedImage(RkImage(openFileButton->size(), RK_IMAGE_RC(open)));
        openFileButton->setCheckable(true);
        RK_ACT_BIND(openFileButton, pressed, RK_ACT_ARGS(), this, openFile());
        mainLayout->addWidget(openFileButton);
        addSeparator(mainLayout, 10);

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setSize(23, 10);
        saveFileButton->setUnpressedImage(RkImage(saveFileButton->size(), RK_IMAGE_RC(save)));
        saveFileButton->setCheckable(true);
        RK_ACT_BIND(saveFileButton, pressed, RK_ACT_ARGS(), this, saveFile());
        mainLayout->addWidget(saveFileButton);
        addSeparator(mainLayout, 10);

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setSize(29, 10);
        exportFileButton->setUnpressedImage(RkImage(exportFileButton->size(), RK_IMAGE_RC(export)));
        exportFileButton->setCheckable(true);
        RK_ACT_BIND(exportFileButton, pressed, RK_ACT_ARGS(), this, openExport());
        mainLayout->addWidget(exportFileButton);
        addSeparator(mainLayout, 10);

	auto playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(43, 18);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play)),
                             RkButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), geonkickApi, playKick());
	playButton->show();
        mainLayout->addWidget(playButton);
        addSeparator(mainLayout, 10);

        createLyersButtons(mainLayout);
        addSeparator(mainLayout, 10);

        auto resetButton = new RkButton(this);
        resetButton->setSize(33, 18);
        resetButton->setType(RkButton::ButtonType::ButtonPush);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset)),
                              RkButton::ButtonImage::ImageUnpressed);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_hover)),
                              RkButton::ButtonImage::ImageUnpressedHover);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_active)),
                              RkButton::ButtonImage::ImagePressed);
        resetButton->show();
        RK_ACT_BIND(resetButton, pressed, RK_ACT_ARGS(), this, resetToDefault());
        mainLayout->addWidget(resetButton);

        addSeparator(mainLayout, 10);

        tuneCheckbox = new GeonkickButton(this);
        tuneCheckbox->setCheckable(true);
        tuneCheckbox->setFixedSize(33, 18);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_off)),
                               RkButton::ButtonImage::ImageUnpressed);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_on)),
                               RkButton::ButtonImage::ImagePressed);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_hover)),
                               RkButton::ButtonImage::ImagePressedHover);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        tuneCheckbox->show();
        RK_ACT_BIND(tuneCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi,
		    tuneAudioOutput(geonkickApi->currentPercussion(), b));
        mainLayout->addWidget(tuneCheckbox);
        addSeparator(mainLayout, 10);

        presetNameLabel = new RkLabel(this);
        presetNameLabel->setBackgroundColor(background());
        presetNameLabel->setTextColor({210, 226, 226, 140});
        auto font = presetNameLabel->font();
        font.setSize(10);
        presetNameLabel->setFont(font);
        presetNameLabel->setSize(150, 30);
        presetNameLabel->show();
        mainLayout->addWidget(presetNameLabel);

        // Controls button
        addSeparator(mainLayout);
        controlsButton = new GeonkickButton(this);
        controlsButton->setPressed(viewState->getMainView() == ViewState::View::Controls);
        controlsButton->setFixedSize(54, 20);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_off)),
                               RkButton::ButtonImage::ImageUnpressed);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_active)),
                               RkButton::ButtonImage::ImagePressed);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        controlsButton->show();
        mainLayout->addWidget(controlsButton);
        RK_ACT_BIND(controlsButton, pressed, RK_ACT_ARGS(),
                    viewState, setMainView(ViewState::View::Controls));
        RK_ACT_BIND(viewState, mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    controlsButton, setPressed(view == ViewState::View::Controls));

        // Kit button
        addSeparator(mainLayout);
        kitButton = new GeonkickButton(this);
        kitButton->setPressed(viewState->getMainView() == ViewState::View::Kit);
        kitButton->setFixedSize(54, 20);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_off)),
                               RkButton::ButtonImage::ImageUnpressed);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_active)),
                               RkButton::ButtonImage::ImagePressed);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        kitButton->show();
        RK_ACT_BIND(kitButton, pressed, RK_ACT_ARGS(),
                    viewState, setMainView(ViewState::View::Kit));
        RK_ACT_BIND(viewState, mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    kitButton, setPressed(view == ViewState::View::Kit));
        mainLayout->addWidget(kitButton);

        // Presets button
        addSeparator(mainLayout);
        presetsButton = new GeonkickButton(this);
        presetsButton->setPressed(viewState->getMainView() == ViewState::View::Presets);
        presetsButton->setFixedSize(54, 20);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_off)),
                               RkButton::ButtonImage::ImageUnpressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_active)),
                               RkButton::ButtonImage::ImagePressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        presetsButton->show();
        RK_ACT_BIND(presetsButton, pressed, RK_ACT_ARGS(),
                    viewState, setMainView(ViewState::View::Presets));
        RK_ACT_BIND(viewState, mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    presetsButton, setPressed(view == ViewState::View::Presets));
        mainLayout->addWidget(presetsButton);

        // Samples button
        addSeparator(mainLayout);
        samplesButton = new GeonkickButton(this);
        samplesButton->setPressed(viewState->getMainView() == ViewState::View::Samples);
        samplesButton->setFixedSize(54, 20);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_off)),
                               RkButton::ButtonImage::ImageUnpressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_active)),
                               RkButton::ButtonImage::ImagePressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        samplesButton->show();
        RK_ACT_BIND(samplesButton, pressed, RK_ACT_ARGS(),
                    viewState, setMainView(ViewState::View::Samples));
        RK_ACT_BIND(viewState, mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    samplesButton, setPressed(view == ViewState::View::Samples));
        mainLayout->addWidget(samplesButton);
        updateGui();
}

void TopBar::addSeparator(RkContainer *mainLayout, int width)
{
        mainLayout->addSpace(width);
        auto separator = new RkLabel(this);
        separator->setSize(2, 21);
        separator->setBackgroundColor(68, 68, 70);
        separator->setImage(RkImage(separator->size(), RK_IMAGE_RC(separator)));
        separator->show();
        mainLayout->addWidget(separator);
        mainLayout->addSpace(width);
}

void TopBar::createLyersButtons(RkContainer *mainLayout)
{
        layer1Button = new GeonkickButton(this);
        layer1Button->setBackgroundColor(background());
        layer1Button->setSize(24, 18);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_disabled)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1)),
                               RkButton::ButtonImage::ImagePressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_hover)),
                               RkButton::ButtonImage::ImagePressedHover);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);

        layer1Button->setCheckable(true);
        mainLayout->addWidget(layer1Button);
        mainLayout->addSpace(2);

        layer2Button = new GeonkickButton(this);
        layer2Button->setBackgroundColor(background());
        layer2Button->setSize(24, 18);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_disabled)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2)),
                               RkButton::ButtonImage::ImagePressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_hover)),
                               RkButton::ButtonImage::ImagePressedHover);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);

        layer2Button->setCheckable(true);
        mainLayout->addWidget(layer2Button);
        mainLayout->addSpace(2);

        layer3Button = new GeonkickButton(this);
        layer3Button->setBackgroundColor(background());
        layer3Button->setSize(24, 18);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_disabled)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3)),
                               RkButton::ButtonImage::ImagePressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_hover)),
                               RkButton::ButtonImage::ImagePressedHover);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_hover)),
                               RkButton::ButtonImage::ImageUnpressedHover);

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
