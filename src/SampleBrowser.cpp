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

#include <RkSpinBox.h>
#include <RkLabel.h>
#include <RkContainer.h>

RK_DECLARE_IMAGE_RC(osc1_load_sample);
RK_DECLARE_IMAGE_RC(osc1_load_sample_hover);
RK_DECLARE_IMAGE_RC(osc1_load_sample_pressed);
RK_DECLARE_IMAGE_RC(osc2_load_sample);
RK_DECLARE_IMAGE_RC(osc2_load_sample_hover);
RK_DECLARE_IMAGE_RC(osc2_load_sample_pressed);
RK_DECLARE_IMAGE_RC(osc1_load_sample);
RK_DECLARE_IMAGE_RC(osc1_load_sample_hover);
RK_DECLARE_IMAGE_RC(osc1_load_sample_pressed);
RK_DECLARE_IMAGE_RC(osc3_load_sample);
RK_DECLARE_IMAGE_RC(osc3_load_sample_hover);
RK_DECLARE_IMAGE_RC(osc3_load_sample_pressed);
RK_DECLARE_IMAGE_RC(export_mono_channel_botton);
RK_DECLARE_IMAGE_RC(export_mono_channel_botton_hover);
RK_DECLARE_IMAGE_RC(export_mono_channel_botton_pressed);
RK_DECLARE_IMAGE_RC(export_stereo_channel_botton);
RK_DECLARE_IMAGE_RC(export_stereo_channel_botton_hover);
RK_DECLARE_IMAGE_RC(export_stereo_channel_botton_pressed);

SampleBrowser::SampleBrowser(GeonkickWidget *parent, KitModel* model)
        : GeonkickWidget(parent)
        , kitModel{model}
        , fileBrowser{nullptr}
        , osc1Button{nullptr}
        , osc2Button{nullptr}
        , osc3Button{nullptr}
        , exportFormatSpinBox{nullptr}
{
        setSize(306, parent->height() - 30);

        fileBrowser = new FileBrowser(this, "Samples");
        fileBrowser->setSize({width(), height() - 25});
        fileBrowser->setFilters({".wav", ".flac", ".ogg"});
        fileBrowser->setCurrentDirectoy(GeonkickConfig().getSampleCurrentPath());
         RK_ACT_BINDL(fileBrowser,
                      currentPathChanged,
                      RK_ACT_ARGS(const fs::string &path),
                      [=,this](const fs::path &path) {
                              GeonkickConfig().setSampleCurrentPath(path);
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
        RK_ACT_BIND(fileBrowser,
                    onCreateFile,
                    RK_ACT_ARGS(const fs::path &filePath),
                    this,
                    doExport(filePath));

        auto mainLayout = new RkContainer(this, Rk::Orientation::Vertical);
        mainLayout->setSize(size());
        mainLayout->addWidget(fileBrowser);

        // Bottom menu
        auto bottomMenu = new RkContainer(this);
        bottomMenu->setSize({width(), 20});
        createBottomMenu(bottomMenu);
        mainLayout->addSpace(3);
        mainLayout->addContainer(bottomMenu);

        show();
}

void SampleBrowser::createBottomMenu(RkContainer* container)
{
        // Load to oscillator label
        auto loadLabel = new RkLabel(this, "Load to:");
        loadLabel->setBackgroundColor(background());
        loadLabel->setTextColor({170, 170, 170});
        loadLabel->setSize(45, 18);
        container->addWidget(loadLabel);

        // Load to osc1
        osc1Button = new GeonkickButton(this);
        osc1Button->setPressed(true);
        osc1Button->setSize(26, 18);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_load_sample)),
                             RkButton::State::Unpressed);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_load_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc1Button->setImage(RkImage(osc1Button->size(), RK_IMAGE_RC(osc1_load_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc1Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator1));
        container->addWidget(osc1Button);

        // Load to osc2
        container->addSpace(3);
        osc2Button = new GeonkickButton(this);
        osc2Button->setSize(26, 18);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_load_sample)),
                             RkButton::State::Unpressed);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_load_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc2Button->setImage(RkImage(osc2Button->size(), RK_IMAGE_RC(osc2_load_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc2Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator2));
        container->addWidget(osc2Button);
        container->addSpace(3);

        // Load to osc3
        osc3Button = new GeonkickButton(this);
        osc3Button->setSize(26, 18);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_load_sample)),
                             RkButton::State::Unpressed);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_load_sample_hover)),
                             RkButton::State::UnpressedHover);
        osc3Button->setImage(RkImage(osc3Button->size(), RK_IMAGE_RC(osc3_load_sample_pressed)),
                             RkButton::State::Pressed);
        RK_ACT_BIND(osc3Button, pressed,
                    RK_ACT_ARGS(), this,
                    setOscillator(GeonkickApi::OscillatorType::Oscillator3));
        container->addWidget(osc3Button);

        // Export label
        auto exportLabel = new RkLabel(this, "Export as:");
        exportLabel->setBackgroundColor(background());
        exportLabel->setTextColor({170, 170, 170});
        exportLabel->setSize(52, 18);
        container->addSpace(8);
        container->addWidget(exportLabel);

        // Export format spin box.
        exportFormatSpinBox = new RkSpinBox(this);
        exportFormatSpinBox->setSize(52, 18);
        exportFormatSpinBox->setTextColor({170, 170, 170});
        exportFormatSpinBox->setBackgroundColor({60, 57, 57});
        exportFormatSpinBox->upControl()->setBackgroundColor({50, 47, 47});
        exportFormatSpinBox->upControl()->setTextColor({100, 100, 100});
        exportFormatSpinBox->downControl()->setBackgroundColor({50, 47, 47});
        exportFormatSpinBox->downControl()->setTextColor({100, 100, 100});
        exportFormatSpinBox->addItem("flac16");
        exportFormatSpinBox->addItem("flac24");
        exportFormatSpinBox->addItem("wav16");
        exportFormatSpinBox->addItem("wav24");
        exportFormatSpinBox->addItem("wav32");
        exportFormatSpinBox->addItem("ogg");
        exportFormatSpinBox->addItem("sfz");
        exportFormatSpinBox->setCurrentItem(GeonkickConfig().getExportFormat());
        exportFormatSpinBox->show();
        container->addWidget(exportFormatSpinBox);
        RK_ACT_BINDL(exportFormatSpinBox,
                     currentIndexChanged,
                     RK_ACT_ARGS(int index),
                     [=, this]([[maybe_unused]] int index){
                             auto item = exportFormatSpinBox->currentItem();
                             GeonkickConfig().setExportFormat(std::get<std::string>(item));
                     });

        container->addSpace(8);

        GeonkickConfig config;
        auto monoChannelButton = new GeonkickButton(this);
        monoChannelButton->setPressed(config.getExportNumberOfChannels() == 1);
        monoChannelButton->setSize(18, 18);
        monoChannelButton->setImage(RK_RC_IMAGE(export_mono_channel_botton),
                                    RkButton::State::Unpressed);
        monoChannelButton->setImage(RK_RC_IMAGE(export_mono_channel_botton_hover),
                                    RkButton::State::UnpressedHover);
        monoChannelButton->setImage(RK_RC_IMAGE(export_mono_channel_botton_pressed),
                                    RkButton::State::Pressed);
        container->addWidget(monoChannelButton);

        container->addSpace(3);
        auto stereoChannelButton = new GeonkickButton(this);
        stereoChannelButton->setPressed(config.getExportNumberOfChannels() == 2);
        stereoChannelButton->setSize(18, 18);
        stereoChannelButton->setImage(RK_RC_IMAGE(export_stereo_channel_botton),
                                      RkButton::State::Unpressed);
        stereoChannelButton->setImage(RK_RC_IMAGE(export_stereo_channel_botton_hover),
                                      RkButton::State::UnpressedHover);
        stereoChannelButton->setImage(RK_RC_IMAGE(export_stereo_channel_botton_pressed),
                                      RkButton::State::Pressed);

        RK_ACT_BINDL(monoChannelButton,
                     pressed,
                     RK_ACT_ARGS(),
                     [=,this]() {
                     GeonkickConfig().setExportNumberOfChannels(1);
                     stereoChannelButton->setPressed(false); });

        RK_ACT_BINDL(stereoChannelButton,
                     pressed,
                     RK_ACT_ARGS(),
                     [=,this]() {
                     GeonkickConfig().setExportNumberOfChannels(2);
                     monoChannelButton->setPressed(false); });
        container->addWidget(stereoChannelButton);
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
                oscillator->setSample(file.string());
                oscillator->setFunction(OscillatorModel::FunctionType::Sample);
        }  catch (...) {
        }
}

void SampleBrowser::doExport(const fs::path &path) const
{
    KitModel::ExportInfo exportInfo{};
    auto formatStr = std::get<std::string>(exportFormatSpinBox->currentItem());

    fs::path adjustedPath = path;
    if (formatStr == "flac16" || formatStr == "flac24") {
        exportInfo.format = ExportAbstract::ExportFormat::Flac;
        exportInfo.bitDepth = formatStr == "flac16" ? 16 : 24;
        adjustedPath.replace_extension(".flac");
    } else if (formatStr == "wav16" || formatStr == "wav24" || formatStr == "wav32") {
        exportInfo.format = ExportAbstract::ExportFormat::Wav;
        exportInfo.bitDepth = formatStr == "wav16" ? 16 : (formatStr == "wav24" ? 24 : 32);
        adjustedPath.replace_extension(".wav");
    } else if (formatStr == "ogg") {
        exportInfo.format = ExportAbstract::ExportFormat::Ogg;
        adjustedPath.replace_extension(".ogg");
    } else if (formatStr == "sfz") {
        exportInfo.format = ExportAbstract::ExportFormat::Sfz;
        adjustedPath.replace_extension(".sfz");
    }

    exportInfo.channels = GeonkickConfig().getExportNumberOfChannels();
    kitModel->doExport(adjustedPath.string(), exportInfo);
}
