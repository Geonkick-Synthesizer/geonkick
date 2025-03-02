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
#include "ViewState.h"

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
        , fileBrowser{nullptr}
{
        setSize(306, parent->height() - 30);

        fileBrowser = new FileBrowser(this);
        fileBrowser->setSize(size());
        fileBrowser->setFilters({".wav", ".WAV", ".flac", ".FLAC", ".ogg", ".OGG"});
        fileBrowser->setCurrentDirectoy(viewState()->samplesBrowserPath());
        RK_ACT_BIND(fileBrowser,
                    currentPathChanged,
                    RK_ACT_ARGS(const std::string &path),
                    viewState(),
                    setSamplesBrowserPath(path));
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
        show();
}

void SampleBrowser::loadSample(const fs::path &file)
{
        try {
                if (!std::filesystem::exists(file) || std::filesystem::is_directory(file))
                        return;

                auto data = geonkickApi->setPreviewSample(file);
                if (data.empty())
                        return;

                auto oscIndex = static_cast<int>(GeonkickApi::OscillatorType::Oscillator1);
                geonkickApi->setOscillatorSample(data, oscIndex);
        }  catch (...) {
                return;
        }
}
