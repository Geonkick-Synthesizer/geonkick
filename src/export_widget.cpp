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

#include <sndfile.h>

extern unsigned char rk_export_bk_png[];
extern unsigned char rk_export_format_unpressed_png[];
extern unsigned char rk_export_format_pressed_png[];
/*extern unsigned char rk_location_label_png[];
extern unsigned char rk_file_name_label_png[];
extern unsigned char rk_export_browse_png[];
extern unsigned char rk_export_export_png[];
extern unsigned char rk_export_cancel_png[];
extern unsigned char rk_export_fromat_label_png[];*/

ExportWidget::ExportWidget(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)/*,
        , geonkickApi{nullptr}
        , locationEdit{nullptr}
        , fileNameEdit{nullptr}
        , browseLocation{nullptr}*/
        , flac16Button{nullptr}
        , flac24Button{nullptr}
        , wav16Button{nullptr}
        , wav24Button{nullptr}
        , wav32Button{nullptr}
        , oggButton{nullptr}
          /*, monoButton{nullptr}
        , stereoButton{nullptr}
        , exportButton{nullptr}
        , cancelButton{nullptr}
        , errorLabel{nullptr}
        , progressBar{nullptr}*/
        , selectedFormat{ExportFormat::Wav16}/*
        , channlesType{ChannlesType::Stereo}*/
{
        setFixedSize(521, 184);
        setTitle("Export - " + std::string(GEOKICK_APP_NAME));
        setBackgroundImage(RkImage(521, 184, rk_export_bk_png));

        /*        errorLabel = new RkLabel(this);
        errorLabel->setFixedSize(width() - 50, 25);
        errorLabel->setPosition(50, 5);
        errorLabel->hide();

        auto locationLabel = new RkLabel(this);
        locationLable->setImage(RkIamge(rk_location_label_png));
        locationLabel->setFixedSize();
        locationLabel->setPosition();
        locationEdit = new RkLineEdit(this);
        locationEdit->setText(std::experimental::filesystem::current_path());
        locationEdit->setSize(200, 25);
        locationEdit->setPosition();
        RK_ACT_BIND(locationEdit, textChanged, RK_ACT_ARGS(const std::string& text), this, resetProgressBar());

        browseLocation = new GeonkickButton(this);
        browseLocation->setCheckable(true);
        browseLocation->setUnpressedImage(RkImage(rk_export_browse_png));
        browseLocation->setFixedSize();
        browseLocation->setPosition();
        RK_ACT_BIND(browseLocation, toggled(bool)), this, SLOT(browse()));

        auto fileNameLabel = new RkLabel(this);
        fileNameLabel->setImage(RkImage(rk_file_name_label_png));
        fileNameLabel->setFixedSize();
        fileNameLabel->setPosition();
        fileNameEdit = new RkLineEdit(this);
        fileNameEdit->setFixedSize();
        fileNameEdit->setPosition();
        RK_ACT_BIND(fileNameEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, resetProgressBar());
        RK_ACT_BIND(fileNameEdit, textEdited, RK_ACT_ARGS(const std::string& text), this, showError());
        */
        createFormatButtons();
        /*createChannelsButtons();

        progressBar = new RkProgressBar(this);
        progressBar->setFixedSize(widht() - 50, 5);
        progressBar->setPosition();
        progressBar->setRange(0, 100);
        progressBar->setMarginWidth(1);
        progressBar->setMarginColor(40, 40, 40);
        progressBar->setBackgroundColor(background());
        progressBar->setProgressColor(0, 200, 0);
        progressBar->show();

        exportButton = new GeonkickButton(this);
        exportButton->setCheckable(true);
        exportButton->setFixedSize();
        exportButton->setPosition();
        exportButton->setUnpressedImage(RkImage(rk_export_export_png));
        RK_ACT_BIND(exportButton, toggled(bool)), this, SLOT(exportKick()));

        cancelButton = new GeonkickButton(this);
        cancelButton->setCheckable(true);
        cancelButton->setFixedSize();
        cancelButton->setPosition();
        cancelButton->setUnpressedImage(RkImage(rk_export_cancel_png));
        cancelButton->setPressedImage(RkImage(rk_export_stop_png));
        RK_ACT_BIND(cancelButton, toggled(bool)), this, closeDialog()));*/
        show();
}

ExportWidget::~ExportWidget()
{
}

void ExportWidget::createFormatButtons()
{
        int y = 19;
        flac16Button = new GeonkickButton(this);
        flac16Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        flac16Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        flac16Button->setFixedSize(14, 12);
        flac16Button->setPosition(34, y);
        RK_ACT_BIND(flac16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac16));

        flac24Button = new GeonkickButton(this);
        flac24Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        flac24Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        flac24Button->setFixedSize(14, 12);
        flac24Button->setPosition(34 + 79, y);
        RK_ACT_BIND(flac24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Flac24));

        wav16Button = new GeonkickButton(this);
        wav16Button->setPressed(true);
        wav16Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav16Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav16Button->setFixedSize(14, 12);
        wav16Button->setPosition(34 + 2 * 79, y);
        RK_ACT_BIND(wav16Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav16));

        wav24Button = new GeonkickButton(this);
        wav24Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav24Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav24Button->setFixedSize(14, 12);
        wav24Button->setPosition(34 + 3 * 79, y);
        RK_ACT_BIND(wav24Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav24));

        wav32Button = new GeonkickButton(this);
        wav32Button->setUnpressedImage(RkImage(12, 12, rk_export_format_unpressed_png));
        wav32Button->setPressedImage(RkImage(12, 12, rk_export_format_pressed_png));
        wav32Button->setFixedSize(14, 12);
        wav32Button->setPosition(34 + 4 * 79, y);
        RK_ACT_BIND(wav32Button, toggled, RK_ACT_ARGS(bool b), this, setFormat(ExportFormat::Wav32));

        oggButton = new GeonkickButton(this);
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

        flac16Button->setPressed(false);
        flac24Button->setPressed(false);
        wav16Button->setPressed(false);
        wav24Button->setPressed(false);
        wav32Button->setPressed(false);
        oggButton->setPressed(false);

        switch (format)
        {
        case ExportFormat::Flac16:
                flac16Button->setPressed(true);
                break;
        case ExportFormat::Flac24:
                flac24Button->setPressed(true);
                break;
        case ExportFormat::Wav16:
                wav16Button->setPressed(true);
                break;
        case ExportFormat::Wav24:
                wav24Button->setPressed(true);
                break;
        case ExportFormat::Wav32:
                wav32Button->setPressed(true);
                break;
        case ExportFormat::Ogg:
                oggButton->setPressed(true);
                break;
        default:
                wav16Button->setPressed(true);
        }

        selectedFormat = format;
        /*        resetProgressBar();
        showError();*/
}

void ExportWidget::createChannelsButtons()
{
        /*        auto channlesLabel = new RkLabel(this);
        formatLabel->setImage(RkImage(www, hhh, rk_export_fromat_label_png));
        formatLabel->setFixedSize(www, hhh);
        formatLabel->setPosition(www, hhh);
        formatLabel->show();

        monoButton = new GkickButton(this);
        monoButton->setFixedSize(,);
        monoButton->setPostion(,);
        monoButton->setUnpressedImage(RkImage(, rk_mono_unpressed_png));
        monoButton->setPressedImage(RkImage(, rk_mono_pressed_png));
        RK_ACT_BIND(monoButton, toggled(bool b), RK_ACT_ARGS(), setChannels(ChannlesType::Mono));

        stereoButton = new GkickButton(this);
        stereoButton->setFixedSize(,);
        stereoButton->setPostion(,);
        stereoButton->setUnpressedImage(RkImage(, rk_stereo_unpressed_png));
        stereoButton->setPressedImage(RkImage(, rk_stereo_pressed_png));
        RK_ACT_BIND(stereoButton, toggled(bool b), RK_ACT_ARGS(), setChannels(ExportFormat::Stereo));*/
}

void ExportWidget::setChannels(ChannelsType channels)
{
        /*        if (channles == channlesType)
                return;

        if (channels == ExportFormat::Stereo) {
                stereoButton->setPressed(true);
                stereoButton->setPressed(false);
        } else {
                stereoButton->setPressed(false);
                stereoButton->setPressed(true);
        }

        channlesType = channles;
        resetProgressBar();
        showError();*/
}

void ExportWidget::browse()
{
        /*        FileDialog fileDialog(this, tr("Select Path") + std::string(" - ") + std::string(GEOKICK_APP_NAME));
        fileDialog.setFileMode(QFileDialog::Directory);
        fileDialog.setOption(QFileDialog::ShowDirsOnly);
        fileDialog.setFilter(QDir::Dirs);
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.exec();
        std::stringList paths = fileDialog.selectedFiles();
        if (!paths.isEmpty())
        locationEdit->setText(paths.first());*/
}

bool ExportWidget::validateInput()
{
        /*        if (locationEdit->text().empty()) {
                showError("File location is empty");
                return false;
        }

        if (fileNameEdit->text().empty()) {
                showError("File name is empty");
                return false;
                }*/

        return true;
}

void ExportWidget::exportKick()
{
        /*        resetProgressBar();
        enableButtons(false);

        if (!validateInput()) {
                cancel();
                return;
        }

        SF_INFO sndinfo;
        sndinfo.samplerate = geonkickApi->getSampleRate();
        if (sndinfo.samplerate == 0) {
                showError("Error on exporting kick");
                cancel();
                return;
        }

        sndinfo.channels   = channlesType == ChannelsType::Mono ? 1 : 2;
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
                showError("Error: error on exporting kick");
                cancel();
                return;
        }

        SNDFILE *sndFile = sf_open(getFilePath().toLatin1().data(), SFM_WRITE, &sndinfo);
        if (!sndFile) {
                showError("Error on exporting kick");
                cancel();
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
                        showError("Error on exporting kick");
                        break;
                }
                i += chunk;
                exportProgress->setValue(100 * (static_cast<float>(i) / kickBuffer.size()));
        }

        sf_close(sndFile);
        enableButtons(true);*/
}

void ExportWidget::cancel()
{
        /*        if (!exportButton->isEnabled())
                enableButtons(true);
        else
        closeDialog();*/
}

void ExportWidget::resetProgressBar()
{
        //        progressBar->reset();
}

int ExportWidget::exportFormat()
{
        /*        switch (selectedFormat)
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
                }*/
}

void ExportWidget::enableButtons(bool enable)
{
        /*        exportButton->setEnabled(enable);
        cancelButton->setPressed(enable);
        monoButton->setEnabled(enable);
        stereoButton->setEnabled(enable);
        locationEdit->setEnabled(enable);
        browseLocation->setEnabled(enable);
        flac16Button->setPressed(enable);
        flac242Button->setPressed(enable);
        wav16Button->setPressed(enable);
        wav242Button->setPressed(enable);
        wav32Button->setPressed(enable);
        oggButton->setPressed(enable);*/
}

std::string ExportWidget::getFilePath()
{
        return "";//        return locationEdit->text() + QDir::separator()
        //                + fileNameEdit->text() + "." + fileSuffix();
}

std::string ExportWidget::fileSuffix()
{
        /*        switch (selectedFormat)
        {
        case ExportFormat::Flac16:
        case ExportFormat::Flac24:
                return std::string(".flac");
        case ExportFormat::Wav16:
        case ExportFormat::Wav24:
        case ExportFormat::Wav32:
                return std::string(".wav");
        case ExportFormat::Ogg:
                return std::string(".ogg");
        default:
                return std::string();
                }*/
        return "";
}

void ExportWidget::showError(const std::string &error)
{
        /*        if (error.mepty()) {
                errorLabel->hide();
        } else {
                errorLabel->setText(error);
                errorLabel->show();
                }*/
}

