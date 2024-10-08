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
#include "file_dialog.h"
#include "ViewState.h"
#include "BufferView.h"
#include "geonkick_button.h"
#include "geonkick_slider.h"

#include "RkContainer.h"

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

SampleBrowser::SampleBrowser(GeonkickWidget *parent, GeonkickApi* api)
        : GeonkickWidget(parent)
        , geonkickApi{api}
        , fileBrowser{new FileDialog(this, FileDialog::Type::Browse, std::string())}
        , samplePreviewWidget{new BufferView(this)}
        , playButton{nullptr}
        , loadButton{nullptr}
        , osc1Button{nullptr}
        , osc2Button{nullptr}
        , osc3Button{nullptr}
        , previewLimiter{nullptr}

{
        setFixedSize(parent->size());
        fileBrowser->setFilters({".wav", ".WAV", ".flac", ".FLAC", ".ogg", ".OGG"});
        fileBrowser->setHomeDirectory(geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH"));
        fileBrowser->setCurrentDirectoy(viewState()->samplesBrowserPath());
        setPreviewSample(viewState()->samplesBrowserPreviewFile());
        RK_ACT_BIND(fileBrowser,
                    directoryChanged,
                    RK_ACT_ARGS(const std::string &path),
                    viewState(),
                    setSamplesBrowserPath(path));
        RK_ACT_BIND(fileBrowser,
                    currentFileChanged,
                    RK_ACT_ARGS(const std::string &file),
                    this,
                    setPreviewSample(file));
        RK_ACT_BIND(fileBrowser,
                    currentFileChanged,
                    RK_ACT_ARGS(const std::string &file),
                    viewState(),
                    setSamplesBrowserPreviewFile(file));
        RK_ACT_BINDL(fileBrowser,
                    selectedFile,
                    RK_ACT_ARGS(const std::string &file),
                     [&](const std::string &file) {
                             viewState()->setSamplesBrowserPreviewFile(file);
                             loadSample();
                    });

        samplePreviewWidget->setSize(250, 260);
        samplePreviewWidget->show();
        RK_ACT_BIND(samplePreviewWidget, graphPressed, RK_ACT_ARGS(), geonkickApi, playSamplePreview());

        previewLimiter = new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical);
        previewLimiter->onSetValue(Geonkick::toDecibel(geonkickApi->samplePreviewLimiter()) + 80);
        RK_ACT_BIND(previewLimiter,
                    valueUpdated, RK_ACT_ARGS(int val),
                    geonkickApi,
                    setSamplePreviewLimiter(Geonkick::fromDecibel(val - 80)));

        previewLimiter->setSize(16, samplePreviewWidget->height());

        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(parent->size());
        mainLayout->addWidget(fileBrowser);

        auto previewWidgetContainer = new RkContainer(this, Rk::Orientation::Horizontal);
        previewWidgetContainer->setSize({samplePreviewWidget->width() + 10 + previewLimiter->width(),
                                         samplePreviewWidget->height()});
        previewWidgetContainer->addWidget(samplePreviewWidget);
        previewWidgetContainer->addSpace(8);
        previewWidgetContainer->addWidget(previewLimiter);

        auto previewContainer = new RkContainer(this, Rk::Orientation::Vertical);
        previewContainer->setSize({previewWidgetContainer->width() + 10, mainLayout->height()});
        previewContainer->addSpace(35);
        previewContainer->addContainer(previewWidgetContainer);

        auto previewMenuContainer = new RkContainer(this);
        previewMenuContainer->setSize({previewContainer->width(), 25});
        createPreviewMenu(previewMenuContainer);
        previewContainer->addSpace(5);
        previewContainer->addContainer(previewMenuContainer);
        mainLayout->addSpace(5);
        mainLayout->addContainer(previewContainer);
        setOscillator(viewState()->samplesBrowserOscillator());
        show();
}

void SampleBrowser::createPreviewMenu(RkContainer* container)
{
        playButton = new GeonkickButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(33, 18);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample)),
                             RkButton::State::Unpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), geonkickApi, playSamplePreview());
        container->addSpace(5);
        container->addWidget(playButton);
        container->addSpace(3);

        loadButton = new GeonkickButton(this);
        loadButton->setType(RkButton::ButtonType::ButtonPush);
        loadButton->setSize(33, 18);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample)),
                             RkButton::State::Unpressed);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample_hover)),
                             RkButton::State::UnpressedHover);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(loadButton, pressed, RK_ACT_ARGS(), this, loadSample());
        container->addWidget(loadButton);
        container->addSpace(15);

        osc1Button = new GeonkickButton(this);
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
}

void SampleBrowser::setOscillator(GeonkickApi::OscillatorType osc)
{
        osc1Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator1);
        osc2Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator2);
        osc3Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator3);
}

void SampleBrowser::loadSample()
{
        if (osc1Button->isPressed()) {
                geonkickApi->setOscillatorSample(samplePreviewWidget->getData(),
                                                 static_cast<int>(GeonkickApi::OscillatorType::Oscillator1));
                geonkickApi->setOscillatorFunction(static_cast<int>(GeonkickApi::OscillatorType::Oscillator1),
                                                   GeonkickApi::FunctionType::Sample);
        }

        if (osc2Button->isPressed()) {
                geonkickApi->setOscillatorSample(samplePreviewWidget->getData(),
                                                 static_cast<int>(GeonkickApi::OscillatorType::Oscillator2));
                geonkickApi->setOscillatorFunction(static_cast<int>(GeonkickApi::OscillatorType::Oscillator2),
                                                   GeonkickApi::FunctionType::Sample);
        }

        if (osc3Button->isPressed()) {
                geonkickApi->setOscillatorSample(samplePreviewWidget->getData(),
                                                 static_cast<int>(GeonkickApi::OscillatorType::Oscillator3));
                geonkickApi->setOscillatorFunction(static_cast<int>(GeonkickApi::OscillatorType::Oscillator3),
                                                   GeonkickApi::FunctionType::Sample);
        }
}

void SampleBrowser::setPreviewSample(const std::string &file)
{
        try {
                if (std::filesystem::exists(file) && !std::filesystem::is_directory(file)) {
                        std::vector<float> data = geonkickApi->setPreviewSample(file);
                        if (!data.empty()) {
                                samplePreviewWidget->setData(data);
                                geonkickApi->playSamplePreview();
                        }
                }
        }  catch (...) {
        }
}
