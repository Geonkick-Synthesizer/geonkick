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
#include "file_dialog.h"

#include <RkLabel.h>
#include <RkLineEdit.h>
#include <RkProgressBar.h>

#include <sndfile.h>

RK_DECLARE_IMAGE_RC(export_bk);
RK_DECLARE_IMAGE_RC(export_format_unpressed);
RK_DECLARE_IMAGE_RC(export_format_pressed);
RK_DECLARE_IMAGE_RC(export_browse);
RK_DECLARE_IMAGE_RC(export_button);
RK_DECLARE_IMAGE_RC(export_cancel);
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
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)
        , geonkickApi{api}
        , locationEdit{nullptr}
        , fileNameEdit{nullptr}
        , browseLocation{nullptr}
        , flac16Button{nullptr}
        , flac24Button{nullptr}
        , wav16Button{nullptr}
        , wav24Button{nullptr}
        , wav32Button{nullptr}
        , oggButton{nullptr}
        , monoButton{nullptr}
        , stereoButton{nullptr}
        , progressBar{nullptr}
        , exportButton{nullptr}
        , cancelButton{nullptr}
        , errorLabel{nullptr}
        , selectedFormat{ExportFormat::Wav16}
        , channelsType{ChannelsType::Mono}
{
        setPosition(30, 40);
        setBorderWidth(1);
        setBorderColor(40, 40, 40);

        std::string format = geonkickApi->getSettings("ExportDialog/Format");
        if (!format.empty())
                selectedFormat = static_cast<ExportFormat>(std::stoi(format));

        std::string channel = geonkickApi->getSettings("ExportDialog/Channel");
        if (!channel.empty())
                channelsType = static_cast<ChannelsType>(std::stoi(channel));

        setFixedSize(521, 184);
        setTitle("Export - " + std::string(GEONKICK_NAME));
        setBackgroundImage(RkImage(521, 184, RK_IMAGE_RC(export_bk)));

        errorLabel = new RkLabel(this);
        errorLabel->setFixedSize(150, 15);
        errorLabel->setBackgroundColor(background());
        errorLabel->setPosition(width() - 180, 105);
        errorLabel->setTextColor({200, 0, 0});
        errorLabel->hide();

        locationEdit = new RkLineEdit(this);
        locationEdit->setFont(font());
        locationEdit->setTitle("locationEdit");
        locationEdit->show();
        locationEdit->setText(geonkickApi->currentWorkingPath("ExportDialog/Location"));
        locationEdit->setSize(120, 25);
        locationEdit->setPosition(82, 54);
        RK_ACT_BIND(locationEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, resetProgressBar());

        browseLocation = new GeonkickButton(this);
        browseLocation->setCheckable(true);
        browseLocation->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(export_browse)));
        browseLocation->setPosition(205, 52);
        RK_ACT_BIND(browseLocation, toggled, RK_ACT_ARGS(bool b), this, browse());

        fileNameEdit = new RkLineEdit(this);
        fileNameEdit->setFont(font());
        fileNameEdit->show();
        fileNameEdit->setFixedSize(100, 25);
        fileNameEdit->setPosition(380, 54);
        fileNameEdit->setText(geonkickApi->getSettings("ExportDialog/FileName"));
        RK_ACT_BIND(fileNameEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, resetProgressBar());
        RK_ACT_BIND(fileNameEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, showError());

        createFormatButtons();
        createChannelsButtons();

        progressBar = new RkProgressBar(this);
        progressBar->setFixedSize(width() - 50, 4);
        progressBar->setPosition(25, 122);
        progressBar->setRange(0, 100);
        progressBar->setBorderWidth(1);
        progressBar->setBorderColor(40, 40, 40);
        progressBar->setBackgroundColor(background());

        exportButton = new GeonkickButton(this);
        exportButton->setCheckable(true);
        exportButton->setPosition((width() - 2 * 90 - 45) / 2, height() - 50);
        exportButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(export_button)));
        RK_ACT_BIND(exportButton, toggled, RK_ACT_ARGS(bool b), this, exportKick());

        cancelButton = new GeonkickButton(this);
        cancelButton->setCheckable(true);
        cancelButton->setPosition((width() - 2 * 90 - 45) / 2 + 90 + 45, height() - 50);
        cancelButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(export_cancel)));
        RK_ACT_BIND(cancelButton, toggled, RK_ACT_ARGS(bool b), this, close());
        show();
}

ExportWidget::~ExportWidget()
{
}

void ExportWidget::createFormatButtons()
{
        int y = 19;
        flac16Button = new GeonkickButton(this);
        flac16Button->setPressed(ExportFormat::Flac16 == selectedFormat);
        flac16Button->setFixedSize(63, 16);
        flac16Button->setUnpressedImage(RkImage(flac16Button->size(), RK_IMAGE_RC(flac16_radio_button)));
        flac16Button->setPressedImage(RkImage(flac16Button->size(), RK_IMAGE_RC(flac16_radio_button_active)));
        flac16Button->setPosition(34, y);
        RK_ACT_BIND(flac16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac16));

        flac24Button = new GeonkickButton(this);
        flac24Button->setPressed(ExportFormat::Flac24 == selectedFormat);
        flac24Button->setFixedSize(65, 16);
        flac24Button->setUnpressedImage(RkImage(flac24Button->size(), RK_IMAGE_RC(flac24_radio_button)));
        flac24Button->setPressedImage(RkImage(flac24Button->size(), RK_IMAGE_RC(flac24_radio_button_active)));
        flac24Button->setPosition(34 + 79, y);
        RK_ACT_BIND(flac24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac24));

        wav16Button = new GeonkickButton(this);
        wav16Button->setPressed(ExportFormat::Wav16 == selectedFormat);
        wav16Button->setFixedSize(62, 16);
        wav16Button->setUnpressedImage(RkImage(wav16Button->size(), RK_IMAGE_RC(wav16_radio_button)));
        wav16Button->setPressedImage(RkImage(wav16Button->size(), RK_IMAGE_RC(wav16_radio_button_active)));
        wav16Button->setPosition(34 + 2 * 79, y);
        RK_ACT_BIND(wav16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav16));

        wav24Button = new GeonkickButton(this);
        wav24Button->setPressed(ExportFormat::Wav24 == selectedFormat);
        wav24Button->setFixedSize(62, 16);
        wav24Button->setUnpressedImage(RkImage(wav24Button->size(), RK_IMAGE_RC(wav24_radio_button)));
        wav24Button->setPressedImage(RkImage(wav24Button->size(), RK_IMAGE_RC(wav24_radio_button_active)));
        wav24Button->setPosition(34 + 3 * 79, y);
        RK_ACT_BIND(wav24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav24));

        wav32Button = new GeonkickButton(this);
        wav32Button->setPressed(ExportFormat::Wav32 == selectedFormat);
        wav32Button->setFixedSize(62, 16);
        wav32Button->setUnpressedImage(RkImage(wav32Button->size(), RK_IMAGE_RC(wav32_radio_button)));
        wav32Button->setPressedImage(RkImage(wav32Button->size(), RK_IMAGE_RC(wav32_radio_button_active)));
        wav32Button->setPosition(34 + 4 * 79, y);
        RK_ACT_BIND(wav32Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav32));

        oggButton = new GeonkickButton(this);
        oggButton->setPressed(ExportFormat::Ogg == selectedFormat);
        oggButton->setFixedSize(51, 16);
        oggButton->setUnpressedImage(RkImage(oggButton->size(), RK_IMAGE_RC(ogg_radio_button)));
        oggButton->setPressedImage(RkImage(oggButton->size(), RK_IMAGE_RC(ogg_radio_button_active)));
        oggButton->setPosition(34 + 5 * 79, y);
        RK_ACT_BIND(oggButton, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Ogg));
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
        resetProgressBar();
        showError();
}

void ExportWidget::createChannelsButtons()
{
        int y = 95;
        int x = 192;
        monoButton = new GeonkickButton(this);
        monoButton->setPressed(ChannelsType::Mono == channelsType);
        monoButton->setFixedSize(51, 16);
        monoButton->setPosition(x , y);
        monoButton->setUnpressedImage(RkImage(monoButton->size(), RK_IMAGE_RC(mono_radio_button)));
        monoButton->setPressedImage(RkImage(monoButton->size(), RK_IMAGE_RC(mono_radio_button_active)));
        RK_ACT_BIND(monoButton, toggled, RK_ACT_ARGS(bool b), this, setChannels(ChannelsType::Mono));

        stereoButton = new GeonkickButton(this);
        stereoButton->setPressed(ChannelsType::Stereo == channelsType);
        stereoButton->setFixedSize(56, 16);
        stereoButton->setPosition(x + 66, y);
        stereoButton->setUnpressedImage(RkImage(stereoButton->size(), RK_IMAGE_RC(stereo_radio_button)));
        stereoButton->setPressedImage(RkImage(stereoButton->size(), RK_IMAGE_RC(stereo_radio_button_active)));
        RK_ACT_BIND(stereoButton, toggled, RK_ACT_ARGS(bool b), this, setChannels(ChannelsType::Stereo));
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
        resetProgressBar();
        showError();
}

void ExportWidget::browse()
{
        locationEdit->setFocus(false);
        fileNameEdit->setFocus(false);
        auto fileDialog = new FileDialog(this, FileDialog::Type::Open,
                                         "Select Path - " + std::string(GEONKICK_NAME));
        fileDialog->setPosition(20, 20);
        fileDialog->setHomeDirectory(geonkickApi->getSettings("GEONKICK_CONFIG/HOME_PATH"));
        fileDialog->setCurrentDirectoy(geonkickApi->currentWorkingPath("ExportDialog/Location"));
        RK_ACT_BIND(fileDialog, selectedFile,
                    RK_ACT_ARGS(const std::string &file), this,
                    setLocation(fileDialog->currentDirectory()));
}

void ExportWidget::setLocation(const std::string &location)
{
        locationEdit->setText(location);
        geonkickApi->setCurrentWorkingPath("ExportDialog/Location", std::filesystem::path(location));
}

bool ExportWidget::validateInput()
{
        if (locationEdit->text().empty()) {
                showError("File location is empty");
                return false;
        }

        if (fileNameEdit->text().empty()) {
                showError("File name is empty");
                return false;
        }

        return true;
}

void ExportWidget::exportKick()
{
        resetProgressBar();
        if (!validateInput())
                return;

        SF_INFO sndinfo;
        memset(&sndinfo, 0, sizeof(sndinfo));
        sndinfo.samplerate = geonkickApi->getSampleRate();
        if (sndinfo.samplerate == 0) {
                showError("Error on exporting kick");
                return;
        }

        sndinfo.channels   = channelsType == ChannelsType::Mono ? 1 : 2;
        sndinfo.format     = exportFormat();

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

        if (kickBuffer.empty() || !sf_format_check(&sndinfo)) {
                showError("Error: error on exporting kick");
                return;
        }

        auto filePath = getFilePath();
        if (filePath.empty()) {
                showError("Wrong file name format");
                return;
        }

        SNDFILE *sndFile = sf_open(filePath.c_str(), SFM_WRITE, &sndinfo);
        if (!sndFile) {
                showError("Error on exporting kick1");
                return;
        }

        size_t n;
#ifdef GEONKICK_DOUBLE_PRECISION
        n = sf_write_double(sndFile, kickBuffer.data(), kickBuffer.size());
#else
        n = sf_write_float(sndFile, kickBuffer.data(), kickBuffer.size());
#endif
        if (n != kickBuffer.size())
                showError("Error on exporting");
        else
                progressBar->setValue(100);

        sf_close(sndFile);
        close();

        if (!fileNameEdit->text().empty())
                geonkickApi->setSettings("ExportDialog/FileName", fileNameEdit->text());
        geonkickApi->setSettings("ExportDialog/Format", std::to_string(static_cast<int>(selectedFormat)));
        geonkickApi->setSettings("ExportDialog/Channel", std::to_string(static_cast<int>(channelsType)));
}

void ExportWidget::resetProgressBar()
{
        progressBar->reset();
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

std::string ExportWidget::getFilePath()
{
        auto path = std::filesystem::path(fileNameEdit->text());
        std::string ext = path.extension();
        if (ext == ".wav" || ext == ".WAV"
            || ext == ".flac" || ext == ".FLAC"
            || ext == ".ogg" || ext == ".OGG") {
                path.replace_extension("." + fileSuffix());
        } else {
                path = std::filesystem::path(path.string() + "." + fileSuffix());
        }

        return std::filesystem::path(locationEdit->text()) / path;
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

void ExportWidget::showError(const std::string &error)
{
        if (error.empty()) {
                errorLabel->hide();
                progressBar->setProgressColor({52, 116, 209});
        } else {
                errorLabel->setText(error);
                errorLabel->show();
                progressBar->setProgressColor({200, 0, 0});
                progressBar->setValue(100);
        }
}
