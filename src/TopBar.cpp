/**
 * File name: TopBar.cpp
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

#include "TopBar.h"
#include "geonkick_button.h"
#include "preset_browser_model.h"
#include "preset_browser_view.h"
#include "ViewState.h"
#include "GeonkickModel.h"
#include "kit_model.h"
#include "percussion_model.h"
#include "MidiKeyWidget.h"
#include "PresetNavigator.h"
#include "SettingsWidget.h"

#include <RkLabel.h>
#include <RkButton.h>
#include <RkContainer.h>
#include <RkSpinBox.h>
#include <RkLineEdit.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(logo);
RK_DECLARE_IMAGE_RC(export);
RK_DECLARE_IMAGE_RC(export_hover);
RK_DECLARE_IMAGE_RC(reset);
RK_DECLARE_IMAGE_RC(reset_hover);
RK_DECLARE_IMAGE_RC(reset_active);
RK_DECLARE_IMAGE_RC(play);
RK_DECLARE_IMAGE_RC(play_pressed);
RK_DECLARE_IMAGE_RC(play_hover);
#ifndef GEONKICK_LIMITED_VERSION
RK_DECLARE_IMAGE_RC(layer1);
RK_DECLARE_IMAGE_RC(layer2);
RK_DECLARE_IMAGE_RC(layer3);
RK_DECLARE_IMAGE_RC(layer1_disabled);
RK_DECLARE_IMAGE_RC(layer2_disabled);
RK_DECLARE_IMAGE_RC(layer3_disabled);
RK_DECLARE_IMAGE_RC(layer1_hover);
RK_DECLARE_IMAGE_RC(layer2_hover);
RK_DECLARE_IMAGE_RC(layer3_hover);
#endif // GEONKICK_LIMITED_VERSION
RK_DECLARE_IMAGE_RC(tune_checkbox_on);
RK_DECLARE_IMAGE_RC(tune_checkbox_off);
RK_DECLARE_IMAGE_RC(tune_checkbox_hover);
RK_DECLARE_IMAGE_RC(topmenu_controls_active);
RK_DECLARE_IMAGE_RC(topmenu_controls_hover);
RK_DECLARE_IMAGE_RC(topmenu_controls_off);
RK_DECLARE_IMAGE_RC(topmenu_midi_off);
RK_DECLARE_IMAGE_RC(topmenu_midi_active);
RK_DECLARE_IMAGE_RC(topmenu_midi_hover);
RK_DECLARE_IMAGE_RC(note_off_unpressed);
RK_DECLARE_IMAGE_RC(note_off_hover);
RK_DECLARE_IMAGE_RC(note_off_pressed);
#ifndef GEONKICK_SINGLE
RK_DECLARE_IMAGE_RC(topmenu_kit_active);
RK_DECLARE_IMAGE_RC(topmenu_kit_hover);
RK_DECLARE_IMAGE_RC(topmenu_kit_off);
#endif // GEONKICK_SINGLE
RK_DECLARE_IMAGE_RC(topmenu_settings_active);
RK_DECLARE_IMAGE_RC(topmenu_settings_hover);
RK_DECLARE_IMAGE_RC(topmenu_settings_off);

TopBar::TopBar(GeonkickWidget *parent, GeonkickModel *model)
        : GeonkickWidget(parent)
        , geonkickModel{model}
        , exportFileButton{nullptr}
        , presetNavigator{nullptr}
#ifndef GEONKICK_LIMITED_VERSION
        , layer1Button{nullptr}
        , layer2Button{nullptr}
        , layer3Button{nullptr}
#endif // GEONKICK_LIMITED_VERSION
        , instrumentName {nullptr}
        , controlsButton{nullptr}
        , midiKeyButton{nullptr}
        , midiChannelSpinBox{nullptr}
        , noteOffButton{nullptr}
#ifndef GEONKICK_SINGLE
        , kitButton{nullptr}
#endif // GEONKICK_SINGLE
        , presetsButton{nullptr}
        , samplesButton{nullptr}
        , settingsButton{nullptr}
{
        setName("TopBar");
        setFixedSize({940 - 10, 30});
        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(size());

        auto logo = new RkLabel(this);
        logo->setBackgroundColor(background());
        RkImage image(22, 22, RK_IMAGE_RC(logo));
        image.grayscaleImage();
        logo->setSize(image.width(), image.height());
        logo->setImage(image);
        logo->show();
        mainLayout->addWidget(logo);

        // Setting button
        addSeparator(mainLayout);
        settingsButton = new GeonkickButton(this);
        settingsButton->setType(RkButton::ButtonType::ButtonUncheckable);
        settingsButton->setFixedSize(16, 16);
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
        RK_ACT_BIND(settingsButton, pressed, RK_ACT_ARGS(), this, showSettings());
        mainLayout->addWidget(settingsButton);

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

#ifndef GEONKICK_LIMITED_VERSION
        createLyersButtons(mainLayout);
        addSeparator(mainLayout, 5);
#endif // GEONKICK_LIMITED_VERSION

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

        // Instrument name
        addSeparator(mainLayout);
        mainLayout->addWidget(createInstrumentNameLabel());
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

        // Midi channel
        addSeparator(mainLayout);
        midiChannelSpinBox = new RkSpinBox(this);
        midiChannelSpinBox->setTextColor({250, 250, 250});
        midiChannelSpinBox->setBackgroundColor({60, 57, 57});
        midiChannelSpinBox->upControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->upControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->downControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->downControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->setSize(50, 20);
        midiChannelSpinBox->show();
        mainLayout->addWidget(midiChannelSpinBox);
        RK_ACT_BINDL(midiChannelSpinBox,
                    currentIndexChanged,
                    RK_ACT_ARGS(int index),
                     [=, this](int index) {
                             geonkickModel->getKitModel()->currentPercussion()->setMidiChannel(index - 1);
                     });

        // Note off button
        addSeparator(mainLayout);
        noteOffButton = new GeonkickButton(this);
        noteOffButton->setType(RkButton::ButtonType::ButtonCheckable);
        noteOffButton->setSize(23, 16);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_unpressed)),
                                RkButton::State::Unpressed);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_hover)),
                                RkButton::State::UnpressedHover);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_pressed)),
                                RkButton::State::Pressed);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_hover)),
                                RkButton::State::PressedHover);
        mainLayout->addWidget(noteOffButton);
        RK_ACT_BINDL(noteOffButton,
                     toggled,
                     RK_ACT_ARGS(bool toggled),
                     [=, this](bool toggled) {
                             geonkickModel->getKitModel()->currentPercussion()->enableNoteOff(toggled);
                     } );

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

#ifndef GEONKICK_LIMITED_VERSION
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
#endif // GEONKICK_LIMITED_VERSION

RkWidget* TopBar::createInstrumentNameLabel()
{
        instrumentName = new RkLineEdit(this);
        instrumentName->setBackgroundColor({44, 44, 44});
        instrumentName->setTextColor({180, 180, 180});
        instrumentName->setCursorColor({180, 180, 180});
        instrumentName->setSize(100, 20);
        instrumentName->show();
        RK_ACT_BINDL(instrumentName, editingFinished, RK_ACT_ARGS(),
                     [=, this]() {
                             auto currentInstrument = geonkickModel->getKitModel()->currentPercussion();
                             if (!currentInstrument->setName(instrumentName->text()))
                                     instrumentName->setText(currentInstrument->name());
                     });
        RK_ACT_BINDL(instrumentName, escapePressed, RK_ACT_ARGS(),
                     [=, this]() {
                             auto currentInstrument = geonkickModel->getKitModel()->currentPercussion();
                             instrumentName->setText(currentInstrument->name());
                     });

        return instrumentName;
}

void TopBar::setPresetName(const std::string &name)
{
        if (name.size() > 20) {
                std::string preset = name;
                preset.resize(15);
                preset += "...";
                instrumentName->setText(preset);
        } else {
                instrumentName->setText(name);
        }
        instrumentName->moveCursorToEnd();
}

void TopBar::updateGui()
{
        auto api = geonkickModel->api();
#ifndef GEONKICK_LIMITED_VERSION
        layer1Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer1));
        layer2Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer2));
        layer3Button->setPressed(api->isLayerEnabled(GeonkickApi::Layer::Layer3));
#endif // GEONKICK_LIMITED_VERSION
        tuneCheckbox->setPressed(api->isAudioOutputTuned(api->currentPercussion()));
        setPresetName(geonkickModel->getKitModel()->currentPercussion()->name());
        auto kitModel = geonkickModel->getKitModel();
        midiKeyButton->setText(MidiKeyWidget::midiKeyToNote(kitModel->currentPercussion()->key()));
        auto percussionModel = geonkickModel->getKitModel()->currentPercussion();
        auto nMidiChannels = percussionModel->numberOfMidiChannels();
        midiChannelSpinBox->clear();
        midiChannelSpinBox->addItem("Any");
        for (size_t i = 0; i < nMidiChannels; i++)
                midiChannelSpinBox->addItem(std::to_string(i + 1));
        midiChannelSpinBox->setCurrentIndex(percussionModel->midiChannel() + 1);
        noteOffButton->setPressed(percussionModel->isNoteOffEnabled());
}

void TopBar::showMidiPopup()
{
        auto kitModel = geonkickModel->getKitModel();
        auto midiPopup = new MidiKeyWidget(dynamic_cast<GeonkickWidget*>(getTopWidget()),
                                           kitModel->currentPercussion());
        midiPopup->setPosition(150, y() + 35);
        RK_ACT_BIND(midiPopup,
                    isAboutToClose,
                    RK_ACT_ARGS(),
                    midiKeyButton,
                    setPressed(false));
        midiPopup->show();
}

void TopBar::showSettings()
{
        settingsButton->setPressed(false);
        auto settingsPopup = new SettingsWidget(dynamic_cast<GeonkickWidget*>(getTopWidget()),
                                                geonkickModel->api());
        settingsPopup->setPosition((getTopWidget()->width()
                                    - settingsPopup->width()) / 2 - 120,
                                   50);
        settingsPopup->show();
}
