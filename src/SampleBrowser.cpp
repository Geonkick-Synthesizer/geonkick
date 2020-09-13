/**
 * File name: SampleBrowser.cpp
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

#include "SampleBrowser.h"
#include "file_dialog.h"
#include "ViewState.h"
#include "geonkick_api.h"
#include "BufferView.h"
#include "geonkick_button.h"

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

SampleBrowser::SampleBrowser(GeonkickWidget *parent, GeonkickApi* api)
        : GeonkickWidget(parent)
        , geonkickApi{api}
        , fileBrowser{new FileDialog(this)}
        , samplePreviewWidget{new BufferView(this)}
        , playButton{nullptr}
        , loadButton{nullptr}
        , osc1Button{nullptr}
        , osc2Button{nullptr}

{
        fileBrowser->setFilters({".wav", ".WAV", ".flac", ".FLAC", ".ogg", ".OGG"});
        fileBrowser->setHomeDirectory(geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH"));
        auto viewState = static_cast<ViewState*>(findObject("ViewState"));
        fileBrowser->setCurrentDirectoy(viewState->samplesBrowserPath());
        RK_ACT_BIND(fileBrowser,
                    directoryChanged,
                    RK_ACT_ARGS(const std::string &path),
                    viewState,
                    setSamplesBrowserPath(path));
        RK_ACT_BIND(fileBrowser,
                    currentFileChanged,
                    RK_ACT_ARGS(const std::string &file),
                    this,
                    setPreviewSample(file));
        setFixedSize(parent->size());
        samplePreviewWidget->setPosition(fileBrowser->x() + fileBrowser->width() + 10, 50);
        samplePreviewWidget->setSize(width() - fileBrowser->width()  -  10, fileBrowser->height() - 100);
        samplePreviewWidget->show();
        RK_ACT_BIND(samplePreviewWidget, graphPressed, RK_ACT_ARGS(), geonkickApi, playSamplePreview());

        auto mainLayout = new RkContainer(this);
        mainLayout->setSize(parent->size());
        mainLayout->addWidget(fileBrowser);

        auto previewContianer = new RkContainer(this, Rk::Orientaton::Vertical);
        previewContianer->setSize(200, mainLayout->height());
        previewContianer->addSpace(100);
        previewContianer->addWidget(samplePreviewWidget);
        previewContianer->addSpace(5);

        auto previewMenuContianer = new RkContianer(this);
        previewMenuContianer->setSize({previewContianer->width(), 25});
        createPreviewMenu(previewMenuContianer);
        previewContianer->addContinaer(previewMenuContianer);
        mainLayout->addContianer(previewContianer);
        setOscillator(viewState->samplesBrowserOscillator());

        show();
}

void SampleBrowser::createPreviewMenu(RkContinaer* contaier)
{
        playButton = new GeonkickButton(this);
        playButton->>setSize(24, 24);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample)),
                             RkButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_preview_sample_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(playButton, pressed(), RK_ACT_ARGS(), geonkickApi, playSamplePreview());
        container->addWidget(playButton);
        container->addSpace(5);

        loadButton = new GeonkickButton(this);
        loadButton->>setSize(24, 24);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample)),
                             RkButton::ButtonImage::ImageUnpressed);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        loadButton->setImage(RkImage(loadButton->size(), RK_IMAGE_RC(load_preview_sample_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(loadButton, pressed(), RK_ACT_ARGS(), this, loadSample());
        container->addWidget(loadButton);
        container->addSpace(5);

        osc1Button = new GeonkickButton(this);
        osc1Button->>setSize(24, 24);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample)),
                             RkButton::ButtonImage::ImageUnpressed);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_preview_sample_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(osc1Button, pressed(),
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator1));
        container->addWidget(osc1Button);
        container->addSpace(5);

        osc2Button = new GeonkickButton(this);
        osc2Button->>setSize(24, 24);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample)),
                             RkButton::ButtonImage::ImageUnpressed);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_preview_sample_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(osc2Button, pressed(),
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator2));
        container->addWidget(osc2Button);
        // container->addSpace(5);
        //        sampleOscButton = new GeonkickButton(this);
        //        container->addWidget(sampleOscButton);
}

void SampleBrowser::setOscillator(GeonkickApi::OscillatorType osc)
{
        osc1Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator1);
        osc2Button->setPressed(osc == GeonkickApi::OscillatorType::Oscillator2);
}

void SampleBrowser::loadSample()
{
        if (osc1Button->isPressed())
                geonkickApi->setOscillatorSample(samplePreviewWidget->getData(),
                                                 GeonkickApi::OscillatorType::Oscillator1);
        else if (osc2Button->isPressed())
                geonkickApi->setOscillatorSample(samplePreviewWidget->getData(),
                                                 GeonkickApi::OscillatorType::Oscillator2);
}

void SampleBrowser::setPreviewSample(const std::string &file)
{
        std::vector<float> data = geonkickApi->setPreviewSample(file);
        samplePreviewWidget->setData(data);
}

