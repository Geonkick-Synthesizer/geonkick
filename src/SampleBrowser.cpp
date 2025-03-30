/**
 * File name: SampleBrowser.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#include "SampleBrowser.h"
#include "FileBrowser.h"
#include "geonkick_button.h"
#include "kit_model.h"
#include "percussion_model.h"
#include "GeonkickConfig.h"

#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(play_preview_sample);
RK_DECLARE_IMAGE_RC(play_preview_sample_hover);
RK_DECLARE_IMAGE_RC(play_preview_sample_pressed);
RK_DECLARE_IMAGE_RC(load_preview_sample);
RK_DECLARE_IMAGE_RC(load_preview_sample_hover);
RK_DECLARE_IMAGE_RC(load_preview_sample_pressed);
RK_DECLARE_IMAGE_RC(osc1_preview_sample);
RK_DECLARE_IMAGE_RC(osc1_preview_sample_hover);
RK_DECLARE_IMAGE_RC(osc1_preview_sample_pressed);
RK_DECLARE_IMAGE_RC(osc2_preview_sample);
RK_DECLARE_IMAGE_RC(osc2_preview_sample_hover);
RK_DECLARE_IMAGE_RC(osc2_preview_sample_pressed);
RK_DECLARE_IMAGE_RC(osc1_preview_sample);
RK_DECLARE_IMAGE_RC(osc1_preview_sample_hover);
RK_DECLARE_IMAGE_RC(osc1_preview_sample_pressed);
RK_DECLARE_IMAGE_RC(osc3_preview_sample);
RK_DECLARE_IMAGE_RC(osc3_preview_sample_hover);
RK_DECLARE_IMAGE_RC(osc3_preview_sample_pressed);

SampleBrowser::SampleBrowser(GeonkickWidget *parent, KitModel* model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , geonkickConfig{std::make_unique<GeonkickConfig>(true)}
        , fileBrowser{nullptr}
        , playButton{nullptr}
        , loadButton{nullptr}
        , osc1Button{nullptr}
        , osc2Button{nullptr}
        , osc3Button{nullptr}
{
        setSize(306, parent->height() - 30);

        fileBrowser = new FileBrowser(this, "Samples");
        fileBrowser->setSize({width(), height() - 25});
        fileBrowser->setFilters({".wav", ".flac", ".ogg"});
        fileBrowser->setCurrentDirectoy(geonkickConfig->getSampleCurrentPath());
         RK_ACT_BINDL(fileBrowser,
                      currentPathChanged,
                      RK_ACT_ARGS(const std::string &path),
                      [=,this](const std::string &path) {
                              geonkickConfig->setSampleCurrentPath(path);
                      });
        RK_ACT_BIND(fileBrowser,
                    fileSelected,
                    RK_ACT_ARGS(const fs::path &file),
                    this,
                    loadSample(file));
        RK_ACT_BIND(fileBrowser,
                    fileActivated,
                    RK_ACT_ARGS(const fs::path &file),
                    this,
                    loadSample(file));

        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size());
        mainLayout->addWidget(fileBrowser);
        auto previewMenu = createPreviewMenu();
        mainLayout->addSpace(3);
        mainLayout->addContainer(previewMenu);
        show();
}

RkContainer* SampleBrowser::createPreviewMenu()
{
        auto container = new RkContainer(this);
        container->setSize({width(), 20});
        playButton = new GeonkickButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(33, 18);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample)),
                             RkButton::State::Unpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), kitModel->api(), playSamplePreview());
        container->addSpace(5);
        container->addWidget(playButton);
        container->addSpace(3);

        osc1Button = new GeonkickButton(this);
        osc1Button->setPressed(true);
        osc1Button->setSize(33, 18);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample)),
                             RkButton::State::Unpressed);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc1Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator1));
        container->addWidget(osc1Button);
        container->addSpace(3);

        osc2Button = new GeonkickButton(this);
        osc2Button->setSize(33, 18);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample)),
                             RkButton::State::Unpressed);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc2Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator2));
        container->addWidget(osc2Button);
        container->addSpace(3);

        osc3Button = new GeonkickButton(this);
        osc3Button->setSize(33, 18);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_preview_sample)),
                             RkButton::State::Unpressed);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc3Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator3));
        container->addWidget(osc3Button);

        return container;
}

void SampleBrowser::setOscillator(GeonkickApi::OscillatorType osc)
{
        osc1Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator1);
        osc2Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator2);
        osc3Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator3);
}

void SampleBrowser::loadSample(const fs::path &file)
{
        try {
                if (!std::filesystem::exists(file) || std::filesystem::is_directory(file))
                        return;

                auto oscType = OscillatorModel::Type::Oscillator1;
                if (osc2Button->isPressed())
                        oscType = OscillatorModel::Type::Oscillator2;
                else if (osc3Button->isPressed())
                        oscType = OscillatorModel::Type::Oscillator3;

                auto currentInstrument = kitModel->currentPercussion();
                auto oscillator = currentInstrument->getCurrentLayerOscillator(oscType);
                oscillator->setSample(file);
                oscillator->setFunction(OscillatorModel::FunctionType::Sample);
        }  catch (...) {
        }
}
