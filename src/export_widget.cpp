/**
 * File name: export_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

extern const unsigned char rk_export_bk_png[];
extern const unsigned char rk_export_format_unpressed_png[];
const auto rk_mono_unpressed_png = rk_export_format_unpressed_png;
const auto rk_stereo_unpressed_png = rk_export_format_unpressed_png;
extern const unsigned char rk_export_format_pressed_png[];
const auto rk_mono_pressed_png = rk_export_format_pressed_png;
const auto rk_stereo_pressed_png = rk_export_format_pressed_png;
extern const unsigned char rk_export_browse_png[];
extern const unsigned char rk_export_active_png[];
extern const unsigned char rk_export_cancel_png[];

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
        , exportButton{nullptr}
        , cancelButton{nullptr}
        , errorLabel{nullptr}
        , progressBar{nullptr}
        , selectedFormat{ExportFormat::Wav16}
        , channelsType{ChannelsType::Mono}
{
        setFixedSize(521, 184);
        setTitle("Export - " + std::string(GEOKICK_APP_NAME));
        setBackgroundImage(RkImage(521, 184, rk_export_bk_png));

        errorLabel = new RkLabel(this);
        errorLabel->setFixedSize(150, 15);
        errorLabel->setBackgroundColor(background());
        errorLabel->setPosition(width() - 180, 105);
        errorLabel->setTextColor({200, 0, 0});
        errorLabel->hide();

        locationEdit = new RkLineEdit(this);
        locationEdit->setTitle("locationEdit");
        locationEdit->show();
        locationEdit->setText(std::filesystem::current_path());
        locationEdit->setSize(120, 25);
        locationEdit->setPosition(82, 54);
        RK_ACT_BIND(locationEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, resetProgressBar());

        browseLocation = new GeonkickButton(this);
        browseLocation->setCheckable(true);
        browseLocation->setUnpressedImage(RkImage(90, 30, rk_export_browse_png));
        browseLocation->setPosition(205, 52);
        RK_ACT_BIND(browseLocation, toggled, RK_ACT_ARGS(bool b), this, browse());

        fileNameEdit = new RkLineEdit(this);
        fileNameEdit->setTitle("fileNameEdit");
        fileNameEdit->show();
        fileNameEdit->setFixedSize(100, 25);
        fileNameEdit->setPosition(380, 54);
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
        exportButton->setUnpressedImage(RkImage(90, 30, rk_export_active_png));
        RK_ACT_BIND(exportButton, toggled, RK_ACT_ARGS(bool b), this, exportKick());

        cancelButton = new GeonkickButton(this);
        cancelButton->setCheckable(true);
        cancelButton->setPosition((width() - 2 * 90 - 45) / 2 + 90 + 45, height() - 50);
        cancelButton->setUnpressedImage(RkImage(90, 30, rk_export_cancel_png));
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
        flac16Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        flac16Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        flac16Button->setFixedSize(14, 12);
        flac16Button->setPosition(34, y);
        RK_ACT_BIND(flac16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac16));

        flac24Button = new GeonkickButton(this);
        flac24Button->setPressed(ExportFormat::Flac24 == selectedFormat);
        flac24Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        flac24Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        flac24Button->setFixedSize(14, 12);
        flac24Button->setPosition(34 + 79, y);
        RK_ACT_BIND(flac24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac24));

        wav16Button = new GeonkickButton(this);
        wav16Button->setPressed(ExportFormat::Wav16 == selectedFormat);
        wav16Button->setPressed(true);
        wav16Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav16Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav16Button->setFixedSize(14, 12);
        wav16Button->setPosition(34 + 2 * 79, y);
        RK_ACT_BIND(wav16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav16));

        wav24Button = new GeonkickButton(this);
        wav24Button->setPressed(ExportFormat::Wav24 == selectedFormat);
        wav24Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav24Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav24Button->setFixedSize(14, 12);
        wav24Button->setPosition(34 + 3 * 79, y);
        RK_ACT_BIND(wav24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav24));

        wav32Button = new GeonkickButton(this);
        wav32Button->setPressed(ExportFormat::Wav32 == selectedFormat);
        wav32Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav32Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav32Button->setFixedSize(14, 12);
        wav32Button->setPosition(34 + 4 * 79, y);
        RK_ACT_BIND(wav32Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav32));

        oggButton = new GeonkickButton(this);
        oggButton->setPressed(ExportFormat::Ogg == selectedFormat);
        oggButton->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        oggButton->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        oggButton->setFixedSize(14, 12);
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
        monoButton->setFixedSize(14, 12);
        monoButton->setPosition(x , y);
        monoButton->setUnpressedImage(RkImage(12, 12, rk_mono_unpressed_png));
        monoButton->setPressedImage(RkImage(12, 12, rk_mono_pressed_png));
        RK_ACT_BIND(monoButton, toggled, RK_ACT_ARGS(bool b), this, setChannels(ChannelsType::Mono));

        stereoButton = new GeonkickButton(this);
        stereoButton->setPressed(ChannelsType::Stereo == channelsType);
        stereoButton->setFixedSize(12, 12);
        stereoButton->setPosition(x + 66, y);
        stereoButton->setUnpressedImage(RkImage(12, 12, rk_stereo_unpressed_png));
        stereoButton->setPressedImage(RkImage(12, 12, rk_stereo_pressed_png));
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
        auto fileDialog = new FileDialog(this, FileDialog::Type::Open, "Select Path - " + std::string(GEOKICK_APP_NAME));
        fileDialog->exec();
        locationEdit->setText(fileDialog->currentDirectory());
}

void ExportWidget::setLocation(const std::string &location)
{

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
        sndinfo.samplerate = geonkickApi->getSampleRate();
        if (sndinfo.samplerate == 0) {
                showError("Error on exporting kick3");
                return;
        }

        sndinfo.channels   = channelsType == ChannelsType::Mono ? 1 : 2;
        sndinfo.format     = exportFormat();

        auto tempBuffer = geonkickApi->getKickBuffer();
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
                GEONKICK_LOG_ERROR("size: " << kickBuffer.size());
                showError("Error: error on exporting kick33");
                return;
        }

        SNDFILE *sndFile = sf_open(getFilePath().c_str(), SFM_WRITE, &sndinfo);
        if (!sndFile) {
                showError("Error on exporting kick2");
                return;
        }

        size_t chunk = kickBuffer.size() / 100;
        size_t i = 0;
        size_t n;
        while (i < kickBuffer.size()) {
                if (i + chunk >= kickBuffer.size()) {
                        chunk = kickBuffer.size() - i;
                }
#ifdef GEONKICK_DOUBLE_PRECISION
                n = sf_write_double(sndFile, kickBuffer.data() + i, chunk);
#else
                n = sf_write_float(sndFile, kickBuffer.data() + i, chunk);
#endif
                if (n != chunk) {
                        showError("Error on exporting kick1");
                        break;
                }
                i += chunk;
                progressBar->setValue(100 * (static_cast<float>(i) / kickBuffer.size()));
                eventQueue()->processQueue();
        }

        sf_close(sndFile);
        close();
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
        return std::filesystem::path(locationEdit->text()) / std::filesystem::path(fileNameEdit->text() + "." + fileSuffix());
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
