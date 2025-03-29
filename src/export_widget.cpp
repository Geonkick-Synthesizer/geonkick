/**
 * File name: export_widget.cpp
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

#include "export_widget.h"
#include "geonkick_api.h"
#include "geonkick_button.h"
#include "FileBrowser.h"

#include <RkLabel.h>
#include <RkContainer.h>

#include <sndfile.h>

RK_DECLARE_IMAGE_RC(export_bk);
RK_DECLARE_IMAGE_RC(export_format_unpressed);
RK_DECLARE_IMAGE_RC(export_format_pressed);
RK_DECLARE_IMAGE_RC(flac16_radio_button);
RK_DECLARE_IMAGE_RC(flac16_radio_button_active);
RK_DECLARE_IMAGE_RC(flac24_radio_button);
RK_DECLARE_IMAGE_RC(flac24_radio_button_active);
RK_DECLARE_IMAGE_RC(wav16_radio_button);
RK_DECLARE_IMAGE_RC(wav16_radio_button_active);
RK_DECLARE_IMAGE_RC(wav24_radio_button);
RK_DECLARE_IMAGE_RC(wav24_radio_button_active);
RK_DECLARE_IMAGE_RC(wav32_radio_button);
RK_DECLARE_IMAGE_RC(wav32_radio_button_active);
RK_DECLARE_IMAGE_RC(ogg_radio_button);
RK_DECLARE_IMAGE_RC(ogg_radio_button_active);
RK_DECLARE_IMAGE_RC(mono_radio_button);
RK_DECLARE_IMAGE_RC(mono_radio_button_active);
RK_DECLARE_IMAGE_RC(stereo_radio_button);
RK_DECLARE_IMAGE_RC(stereo_radio_button_active);

ExportWidget::ExportWidget(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent, Rk::WidgetFlags::Popup)
        , geonkickApi{api}
        , flac16Button{nullptr}
        , flac24Button{nullptr}
        , wav16Button{nullptr}
        , wav24Button{nullptr}
        , wav32Button{nullptr}
        , oggButton{nullptr}
        , monoButton{nullptr}
        , stereoButton{nullptr}
        , selectedFormat{ExportFormat::Wav16}
        , channelsType{ChannelsType::Mono}
{
        setFixedSize(600, 410);
        setPosition(30, 40);
        setBorderWidth(2);
        setBorderColor(80, 80, 80);

        //auto fileDialog = new FileBrowser(this);//, FileBrowser::Type::Save,
        //Rk::WidgetFlags::Widget,
        //                                         "Select Path - " + std::string(GEONKICK_NAME));
        /*fileDialog->setFilters({".flac", ".wav", ".ogg", ".FLAC", ".WAV", ".OGG"});
        fileDialog->setPosition(0, 40);
        fileDialog->setHomeDirectory(geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH"));
        fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("ExportDialog/Location").string());
        RK_ACT_BIND(fileDialog, selectedFile,
                    RK_ACT_ARGS(const std::string &file), this,
                    exportInstrument(file));
                    RK_ACT_BIND(fileDialog, rejected, RK_ACT_ARGS(), this, close());*/

        std::string format = geonkickApi->getSettings("ExportDialog/Format");
        if (!format.empty())
                selectedFormat = static_cast<ExportFormat>(std::stoi(format));

        std::string channel = geonkickApi->getSettings("ExportDialog/Channel");
        if (!channel.empty())
                channelsType = static_cast<ChannelsType>(std::stoi(channel));

        setTitle("Export - " + std::string(GEONKICK_NAME));

        createFormatButtons();
        show();
}

ExportWidget::~ExportWidget()
{
}

void ExportWidget::createFormatButtons()
{
        auto topContainer = new RkContainer(this);
        topContainer->setSpacing(4);
        topContainer->setSize({width(), 16});
        topContainer->setPosition({10, 10});
        flac16Button = new GeonkickButton(this);
        flac16Button->setPressed(ExportFormat::Flac16 == selectedFormat);
        flac16Button->setFixedSize(63, 16);
        flac16Button->setUnpressedImage(RkImage(flac16Button->size(), RK_IMAGE_RC(flac16_radio_button)));
        flac16Button->setPressedImage(RkImage(flac16Button->size(), RK_IMAGE_RC(flac16_radio_button_active)));
        RK_ACT_BIND(flac16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac16));
        topContainer->addWidget(flac16Button);
        topContainer->addSpace(4);

        flac24Button = new GeonkickButton(this);
        flac24Button->setPressed(ExportFormat::Flac24 == selectedFormat);
        flac24Button->setFixedSize(65, 16);
        flac24Button->setUnpressedImage(RkImage(flac24Button->size(), RK_IMAGE_RC(flac24_radio_button)));
        flac24Button->setPressedImage(RkImage(flac24Button->size(), RK_IMAGE_RC(flac24_radio_button_active)));
        RK_ACT_BIND(flac24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac24));
        topContainer->addWidget(flac24Button);
        topContainer->addSpace(4);

        wav16Button = new GeonkickButton(this);
        wav16Button->setPressed(ExportFormat::Wav16 == selectedFormat);
        wav16Button->setFixedSize(62, 16);
        wav16Button->setUnpressedImage(RkImage(wav16Button->size(), RK_IMAGE_RC(wav16_radio_button)));
        wav16Button->setPressedImage(RkImage(wav16Button->size(), RK_IMAGE_RC(wav16_radio_button_active)));
        RK_ACT_BIND(wav16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav16));
        topContainer->addWidget(wav16Button);
        topContainer->addSpace(4);

        wav24Button = new GeonkickButton(this);
        wav24Button->setPressed(ExportFormat::Wav24 == selectedFormat);
        wav24Button->setFixedSize(62, 16);
        wav24Button->setUnpressedImage(RkImage(wav24Button->size(), RK_IMAGE_RC(wav24_radio_button)));
        wav24Button->setPressedImage(RkImage(wav24Button->size(), RK_IMAGE_RC(wav24_radio_button_active)));
        RK_ACT_BIND(wav24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav24));
        topContainer->addWidget(wav24Button);
        topContainer->addSpace(4);

        wav32Button = new GeonkickButton(this);
        wav32Button->setPressed(ExportFormat::Wav32 == selectedFormat);
        wav32Button->setFixedSize(62, 16);
        wav32Button->setUnpressedImage(RkImage(wav32Button->size(), RK_IMAGE_RC(wav32_radio_button)));
        wav32Button->setPressedImage(RkImage(wav32Button->size(), RK_IMAGE_RC(wav32_radio_button_active)));
        RK_ACT_BIND(wav32Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav32));
        topContainer->addWidget(wav32Button);
        topContainer->addSpace(4);

        oggButton = new GeonkickButton(this);
        oggButton->setPressed(ExportFormat::Ogg == selectedFormat);
        oggButton->setFixedSize(51, 16);
        oggButton->setUnpressedImage(RkImage(oggButton->size(), RK_IMAGE_RC(ogg_radio_button)));
        oggButton->setPressedImage(RkImage(oggButton->size(), RK_IMAGE_RC(ogg_radio_button_active)));
        RK_ACT_BIND(oggButton, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Ogg));
        topContainer->addWidget(oggButton);

        topContainer->addSpace(16);
        createChannelsButtons(topContainer);
}

void ExportWidget::setFormat(ExportFormat format)
{
        if (selectedFormat == format)
                return;

        flac16Button->setPressed(ExportFormat::Flac16 == format);
        flac24Button->setPressed(ExportFormat::Flac24 == format);
        wav16Button->setPressed(ExportFormat::Wav16 == format);
        wav24Button->setPressed(ExportFormat::Wav24 == format);
        wav32Button->setPressed(ExportFormat::Wav32 == format);
        oggButton->setPressed(ExportFormat::Ogg == format);
        selectedFormat = format;
}

void ExportWidget::createChannelsButtons(RkContainer* container)
{
        monoButton = new GeonkickButton(this);
        monoButton->setPressed(ChannelsType::Mono == channelsType);
        monoButton->setFixedSize(51, 16);
        monoButton->setUnpressedImage(RkImage(monoButton->size(), RK_IMAGE_RC(mono_radio_button)));
        monoButton->setPressedImage(RkImage(monoButton->size(), RK_IMAGE_RC(mono_radio_button_active)));
        RK_ACT_BIND(monoButton, toggled, RK_ACT_ARGS(bool b), this, setChannels(ChannelsType::Mono));
        container->addWidget(monoButton);
        container->addSpace(4);

        stereoButton = new GeonkickButton(this);
        stereoButton->setPressed(ChannelsType::Stereo == channelsType);
        stereoButton->setFixedSize(56, 16);
        stereoButton->setUnpressedImage(RkImage(stereoButton->size(), RK_IMAGE_RC(stereo_radio_button)));
        stereoButton->setPressedImage(RkImage(stereoButton->size(), RK_IMAGE_RC(stereo_radio_button_active)));
        RK_ACT_BIND(stereoButton, toggled, RK_ACT_ARGS(bool b), this, setChannels(ChannelsType::Stereo));
        container->addWidget(stereoButton);
}

void ExportWidget::setChannels(ChannelsType channels)
{
        if (channels == channelsType)
                return;

        if (channels == ChannelsType::Stereo) {
                stereoButton->setPressed(true);
                monoButton->setPressed(false);
        } else {
                stereoButton->setPressed(false);
                monoButton->setPressed(true);
        }

        channelsType = channels;
}

void ExportWidget::exportInstrument(const std::string &filePath)
{
        SF_INFO sndinfo;
        memset(&sndinfo, 0, sizeof(sndinfo));
        sndinfo.samplerate = geonkickApi->getSampleRate();
        if (sndinfo.samplerate == 0)
                return;

        sndinfo.channels = channelsType == ChannelsType::Mono ? 1 : 2;
        sndinfo.format   = exportFormat();

        auto tempBuffer = geonkickApi->getKickBuffer();
        sndinfo.frames = tempBuffer.size();
        std::vector<gkick_real> kickBuffer;
        if (sndinfo.channels == 2) {
                kickBuffer.resize(2 * tempBuffer.size());
                size_t k = 0;
                while (k < tempBuffer.size()) {
                        kickBuffer[2 * k] = kickBuffer[2 * k + 1] = tempBuffer[k];
                        k++;
                }
        } else {
                kickBuffer = std::move(tempBuffer);
        }

        auto exportedFilePath = getExportedFilePath(filePath);
        if (exportedFilePath.empty())
                return;

        auto sndFile = sf_open(exportedFilePath.string().c_str(), SFM_WRITE, &sndinfo);
        if (!sndFile)
                return;

#ifdef GEONKICK_DOUBLE_PRECISION
        sf_write_double(sndFile, kickBuffer.data(), kickBuffer.size());
#else
        sf_write_float(sndFile, kickBuffer.data(), kickBuffer.size());
#endif

        sf_close(sndFile);

        if (!exportedFilePath.empty()
            && exportedFilePath.has_parent_path()) {
                geonkickApi->setCurrentWorkingPath("ExportDialog/Location",
                                                   exportedFilePath.parent_path().string());
        }
        geonkickApi->setSettings("ExportDialog/Format", std::to_string(static_cast<int>(selectedFormat)));
        geonkickApi->setSettings("ExportDialog/Channel", std::to_string(static_cast<int>(channelsType)));
        close();
}

int ExportWidget::exportFormat()
{
        switch (selectedFormat)
        {
        case ExportFormat::Flac16:
                return SF_FORMAT_FLAC | SF_FORMAT_PCM_16;
        case ExportFormat::Flac24:
                return SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
        case ExportFormat::Wav16:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        case ExportFormat::Wav24:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        case ExportFormat::Wav32:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_32;
        case ExportFormat::Ogg:
                return SF_FORMAT_OGG | SF_FORMAT_VORBIS;
        default:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        }
}

std::filesystem::path
ExportWidget::getExportedFilePath(const std::filesystem::path &file)
{
        auto filePath = file;
        std::string ext = file.extension().string();
        if (ext == ".wav" || ext == ".WAV"
            || ext == ".flac" || ext == ".FLAC"
            || ext == ".ogg" || ext == ".OGG") {
                filePath.replace_extension("." + fileSuffix());
        } else {
                filePath = std::filesystem::path(filePath.string() + "." + fileSuffix());
        }
        return filePath;
}

std::string ExportWidget::fileSuffix()
{
        switch (selectedFormat)
        {
        case ExportFormat::Flac16:
        case ExportFormat::Flac24:
                return std::string("flac");
        case ExportFormat::Wav16:
        case ExportFormat::Wav24:
        case ExportFormat::Wav32:
                return std::string("wav");
        case ExportFormat::Ogg:
                return std::string("ogg");
        default:
                return std::string();
        }
}
