/**
 * File name: top_bar.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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
#include "GeonkickModel.h"
#include "kit_model.h"
#include "percussion_model.h"
#include "MidiKeyWidget.h"
#include "PresetNavigator.h"

#include <RkLabel.h>
#include <RkButton.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(logo);
RK_DECLARE_IMAGE_RC(open);
RK_DECLARE_IMAGE_RC(open_hover);
RK_DECLARE_IMAGE_RC(save);
RK_DECLARE_IMAGE_RC(save_hover);
RK_DECLARE_IMAGE_RC(export);
RK_DECLARE_IMAGE_RC(export_hover);
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
RK_DECLARE_IMAGE_RC(topmenu_midi_off);
RK_DECLARE_IMAGE_RC(topmenu_midi_active);
RK_DECLARE_IMAGE_RC(topmenu_midi_hover);
#ifndef GEONKICK_SINGLE
RK_DECLARE_IMAGE_RC(topmenu_kit_active);
RK_DECLARE_IMAGE_RC(topmenu_kit_hover);
RK_DECLARE_IMAGE_RC(topmenu_kit_off);
#endif // GEONKICK_SINGLE
RK_DECLARE_IMAGE_RC(topmenu_presets_active);
RK_DECLARE_IMAGE_RC(topmenu_presets_hover);
RK_DECLARE_IMAGE_RC(topmenu_presets_off);
RK_DECLARE_IMAGE_RC(topmenu_samples_active);
RK_DECLARE_IMAGE_RC(topmenu_samples_hover);
RK_DECLARE_IMAGE_RC(topmenu_samples_off);
RK_DECLARE_IMAGE_RC(topmenu_settings_active);
RK_DECLARE_IMAGE_RC(topmenu_settings_hover);
RK_DECLARE_IMAGE_RC(topmenu_settings_off);

TopBar::TopBar(GeonkickWidget *parent, GeonkickModel *model)
        : GeonkickWidget(parent)
        , geonkickModel{model}
        , openFileButton{nullptr}
        , saveFileButton{nullptr}
        , exportFileButton{nullptr}
        , presetNavigator{nullptr}
        , layer1Button{nullptr}
        , layer2Button{nullptr}
        , layer3Button{nullptr}
        , controlsButton{nullptr}
        , midiKeyButton{nullptr}
#ifndef GEONKICK_SINGLE
        , kitButton{nullptr}
#endif // GEONKICK_SINGLE
        , presetsButton{nullptr}
        , samplesButton{nullptr}
{
        setName("TopBar");
        setFixedSize({parent->width(), 30});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());

        auto logo = new RkLabel(this);
        logo->setBackgroundColor(background());
        RkImage image(22, 22, RK_IMAGE_RC(logo));
        logo->setSize(image.width(), image.height());
        logo->setImage(image);
        logo->show();
        mainLayout->addWidget(logo);
        addSeparator(mainLayout, 5);

        openFileButton = new GeonkickButton(this);
        openFileButton->show();
        openFileButton->setSize(26, 10);
        openFileButton->setType(RkButton::ButtonType::ButtonCheckable);
        openFileButton->setImage(RkImage(openFileButton->size(), RK_IMAGE_RC(open)),
                                 RkButton::State::Unpressed);
        openFileButton->setImage(RkImage(openFileButton->size(), RK_IMAGE_RC(open_hover)),
                                 RkButton::State::UnpressedHover);
        openFileButton->setImage(RkImage(openFileButton->size(), RK_IMAGE_RC(open_hover)),
                                 RkButton::State::Pressed);
        RK_ACT_BINDL(openFileButton, pressed, RK_ACT_ARGS(), [=,this](){openFileButton->setPressed(false);
                                                              action openFile();});
        mainLayout->addWidget(openFileButton);
        addSeparator(mainLayout, 5);

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setSize(23, 10);
        saveFileButton->setType(RkButton::ButtonType::ButtonCheckable);
        saveFileButton->setImage(RkImage(saveFileButton->size(), RK_IMAGE_RC(save)),
                                 RkButton::State::Unpressed);
        saveFileButton->setImage(RkImage(saveFileButton->size(), RK_IMAGE_RC(save_hover)),
                                 RkButton::State::UnpressedHover);
        saveFileButton->setImage(RkImage(saveFileButton->size(), RK_IMAGE_RC(save_hover)),
                                 RkButton::State::Pressed);
        RK_ACT_BINDL(saveFileButton, pressed, RK_ACT_ARGS(), [=,this](){saveFileButton->setPressed(false);
                                                             action saveFile();});
        mainLayout->addWidget(saveFileButton);
        addSeparator(mainLayout, 5);
        exportFileButton = new GeonkickButton(this);
        exportFileButton->setSize(29, 10);
        saveFileButton->setType(RkButton::ButtonType::ButtonCheckable);
        exportFileButton->setImage(RkImage(exportFileButton->size(), RK_IMAGE_RC(export)),
                                 RkButton::State::Unpressed);
        exportFileButton->setImage(RkImage(exportFileButton->size(), RK_IMAGE_RC(export_hover)),
                                 RkButton::State::UnpressedHover);
        exportFileButton->setImage(RkImage(exportFileButton->size(), RK_IMAGE_RC(export_hover)),
                                RkButton::State::Pressed);
        RK_ACT_BINDL(exportFileButton, pressed, RK_ACT_ARGS(), [=,this](){exportFileButton->setPressed(false);
                                                               action openExport();});
        mainLayout->addWidget(exportFileButton);
        addSeparator(mainLayout, 5);

	auto playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(21, 18);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play)),
                             RkButton::State::Unpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_hover)),
                             RkButton::State::UnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), geonkickModel->api(), playKick());
	playButton->show();
        mainLayout->addWidget(playButton);
        addSeparator(mainLayout, 5);

        createLyersButtons(mainLayout);
        addSeparator(mainLayout, 5);

        auto resetButton = new RkButton(this);
        resetButton->setSize(33, 18);
        resetButton->setType(RkButton::ButtonType::ButtonPush);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset)),
                              RkButton::State::Unpressed);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_hover)),
                              RkButton::State::UnpressedHover);
        resetButton->setImage(RkImage(resetButton->size(), RK_IMAGE_RC(reset_active)),
                              RkButton::State::Pressed);
        resetButton->show();
        RK_ACT_BIND(resetButton, pressed, RK_ACT_ARGS(), this, resetToDefault());
        mainLayout->addWidget(resetButton);

        addSeparator(mainLayout, 5);

        tuneCheckbox = new GeonkickButton(this);
        tuneCheckbox->setCheckable(true);
        tuneCheckbox->setFixedSize(33, 18);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_off)),
                               RkButton::State::Unpressed);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_on)),
                               RkButton::State::Pressed);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_hover)),
                               RkButton::State::PressedHover);
        tuneCheckbox->setImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_hover)),
                               RkButton::State::UnpressedHover);
        tuneCheckbox->show();
        RK_ACT_BIND(tuneCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickModel->api(),
		    tuneAudioOutput(geonkickModel->api()->currentPercussion(), b));
        mainLayout->addWidget(tuneCheckbox);
        addSeparator(mainLayout);

        presetNavigator = new PresetNavigator(this, geonkickModel->getPresetsModel());
        mainLayout->addWidget(presetNavigator);

        addSeparator(mainLayout);
        midiKeyButton = new GeonkickButton(this);
        midiKeyButton->setTextColor({200, 200, 200});
        midiKeyButton->setType(RkButton::ButtonType::ButtonUncheckable);
        midiKeyButton->setSize(36, 20);
        midiKeyButton->setImage(RkImage(midiKeyButton->size(), RK_IMAGE_RC(topmenu_midi_off)),
                                RkButton::State::Unpressed);
        midiKeyButton->setImage(RkImage(midiKeyButton->size(), RK_IMAGE_RC(topmenu_midi_active)),
                                RkButton::State::Pressed);
        midiKeyButton->setImage(RkImage(midiKeyButton->size(), RK_IMAGE_RC(topmenu_midi_hover)),
                                RkButton::State::UnpressedHover);
        RK_ACT_BIND(midiKeyButton, toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showMidiPopup());
        mainLayout->addWidget(midiKeyButton);

        // Controls button
        addSeparator(mainLayout);
        controlsButton = new GeonkickButton(this);
        controlsButton->setPressed(viewState()->getMainView() == ViewState::View::Controls);
        controlsButton->setFixedSize(50, 20);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_off)),
                               RkButton::State::Unpressed);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_active)),
                               RkButton::State::Pressed);
        controlsButton->setImage(RkImage(controlsButton->size(), RK_IMAGE_RC(topmenu_controls_hover)),
                               RkButton::State::UnpressedHover);
        controlsButton->show();
        mainLayout->addWidget(controlsButton);
        RK_ACT_BIND(controlsButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Controls));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    controlsButton, setPressed(view == ViewState::View::Controls));

#ifndef GEONKICK_SINGLE
        // Kit button
        addSeparator(mainLayout);
        kitButton = new GeonkickButton(this);
        kitButton->setPressed(viewState()->getMainView() == ViewState::View::Kit);
        kitButton->setFixedSize(25, 20);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_off)),
                               RkButton::State::Unpressed);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_active)),
                               RkButton::State::Pressed);
        kitButton->setImage(RkImage(kitButton->size(), RK_IMAGE_RC(topmenu_kit_hover)),
                               RkButton::State::UnpressedHover);
        kitButton->show();
        RK_ACT_BIND(kitButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Kit));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    kitButton, setPressed(view == ViewState::View::Kit));
                    mainLayout->addWidget(kitButton);
#endif // GEONKICK_SINGLE

        // Presets button
        addSeparator(mainLayout);
        presetsButton = new GeonkickButton(this);
        presetsButton->setPressed(viewState()->getMainView() == ViewState::View::Presets);
        presetsButton->setFixedSize(45, 20);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_off)),
                               RkButton::State::Unpressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_active)),
                               RkButton::State::Pressed);
        presetsButton->setImage(RkImage(presetsButton->size(), RK_IMAGE_RC(topmenu_presets_hover)),
                               RkButton::State::UnpressedHover);
        presetsButton->show();
        RK_ACT_BIND(presetsButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Presets));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    presetsButton, setPressed(view == ViewState::View::Presets));
        mainLayout->addWidget(presetsButton);

        // Samples button
        addSeparator(mainLayout);
        samplesButton = new GeonkickButton(this);
        samplesButton->setPressed(viewState()->getMainView() == ViewState::View::Samples);
        samplesButton->setFixedSize(54, 20);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_off)),
                                RkButton::State::Unpressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_active)),
                                RkButton::State::Pressed);
        samplesButton->setImage(RkImage(samplesButton->size(), RK_IMAGE_RC(topmenu_samples_hover)),
                                RkButton::State::UnpressedHover);
        samplesButton->show();
        RK_ACT_BIND(samplesButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Samples));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    samplesButton, setPressed(view == ViewState::View::Samples));
        mainLayout->addWidget(samplesButton);

        // Setting button
        addSeparator(mainLayout);
        auto settingsButton = new GeonkickButton(this);
        settingsButton->setPressed(viewState()->getMainView() == ViewState::View::Settings);
        settingsButton->setFixedSize(48, 20);
        settingsButton->setImage(RkImage(settingsButton->size(),
                                         RK_IMAGE_RC(topmenu_settings_off)),
                                 RkButton::State::Unpressed);
        settingsButton->setImage(RkImage(settingsButton->size(),
                                         RK_IMAGE_RC(topmenu_settings_active)),
                                 RkButton::State::Pressed);
        settingsButton->setImage(RkImage(settingsButton->size(),
                                         RK_IMAGE_RC(topmenu_settings_hover)),
                                 RkButton::State::UnpressedHover);
        settingsButton->show();
        RK_ACT_BIND(settingsButton, pressed, RK_ACT_ARGS(),
                    viewState(), setMainView(ViewState::View::Settings));
        RK_ACT_BIND(viewState(), mainViewChanged, RK_ACT_ARGS(ViewState::View view),
                    settingsButton, setPressed(view == ViewState::View::Settings));
        mainLayout->addWidget(settingsButton);

        RK_ACT_BIND(geonkickModel->getKitModel(),
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this, updateGui());
        RK_ACT_BINDL(geonkickModel->getKitModel(),
                     percussionUpdated,
                     RK_ACT_ARGS(PercussionModel* model),
                     [=, this](PercussionModel* model) {
                             if (model->isSelected())
                                     updateGui();
                     } );
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
                               RkButton::State::Unpressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1)),
                               RkButton::State::Pressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_hover)),
                               RkButton::State::PressedHover);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_hover)),
                               RkButton::State::UnpressedHover);

        layer1Button->setCheckable(true);
        mainLayout->addWidget(layer1Button);
        mainLayout->addSpace(2);

        layer2Button = new GeonkickButton(this);
        layer2Button->setBackgroundColor(background());
        layer2Button->setSize(24, 18);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_disabled)),
                               RkButton::State::Unpressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2)),
                               RkButton::State::Pressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_hover)),
                               RkButton::State::PressedHover);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_hover)),
                               RkButton::State::UnpressedHover);

        layer2Button->setCheckable(true);
        mainLayout->addWidget(layer2Button);
        mainLayout->addSpace(2);

        layer3Button = new GeonkickButton(this);
        layer3Button->setBackgroundColor(background());
        layer3Button->setSize(24, 18);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_disabled)),
                               RkButton::State::Unpressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3)),
                               RkButton::State::Pressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_hover)),
                               RkButton::State::PressedHover);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_hover)),
                               RkButton::State::UnpressedHover);

        layer3Button->setCheckable(true);
        mainLayout->addWidget(layer3Button);

        RK_ACT_BIND(layer1Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickModel->api(), enbaleLayer(GeonkickApi::Layer::Layer1, b));
        RK_ACT_BIND(layer3Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickModel->api(), enbaleLayer(GeonkickApi::Layer::Layer3, b));
        RK_ACT_BIND(layer2Button, toggled, RK_ACT_ARGS(bool b),
        geonkickModel->api(), enbaleLayer(GeonkickApi::Layer::Layer2, b));
}

void TopBar::setPresetName(const std::string &name)
{
        if (name.size() > 20) {
                std::string preset = name;
                preset.resize(15);
                preset += "...";
                //                presetNameLabel->setText(preset);
        } else {
                //                presetNameLabel->setText(name);
        }
}

void TopBar::updateGui()
{
        auto api = geonkickModel->api();
        layer1Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer1));
        layer2Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer2));
        layer3Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer3));
        tuneCheckbox->setPressed(api->isAudioOutputTuned(api->currentPercussion()));
        setPresetName(geonkickModel->getKitModel()->currentPercussion()->name());
        auto kitModel = geonkickModel->getKitModel();
        midiKeyButton->setText(MidiKeyWidget::midiKeyToNote(kitModel->currentPercussion()->key()));
}

void TopBar::showMidiPopup()
{
        auto kitModel = geonkickModel->getKitModel();
        auto midiPopup = new MidiKeyWidget(dynamic_cast<GeonkickWidget*>(getTopWidget()),
                                           kitModel->currentPercussion());
        midiPopup->setPosition(midiKeyButton->x() - 170, y() + 35);
        RK_ACT_BIND(midiPopup,
                    isAboutToClose,
                    RK_ACT_ARGS(),
                    midiKeyButton,
                    setPressed(false));
        midiPopup->show();
}
