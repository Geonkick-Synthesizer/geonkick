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

#include <QEventLoop>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QProgressBar>
#include <QFileDialog>
#include <QFont>

#include <sndfile.h>

ExportWidget::ExportWidget(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent),
          geonkickApi(api),
          exportResult(ExportResult::Rejected),
          locationEdit(nullptr),
          browseLocation(nullptr),
          formatComboBox(nullptr),
          exportProgress(nullptr),
          monoRadioButton(nullptr),
          stereoRadioButton(nullptr),
          exportButton(nullptr),
          cancelButton(nullptr)
{
        setWindowFlags(Qt::Dialog);
        setWindowModality(Qt::ApplicationModal);
        setWindowTitle(tr("Export Kick"));

        auto mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(20);
        exportProgress = new QProgressBar(this);
        exportProgress->setMaximumHeight(5);
        exportProgress->setRange(0, 100);
        exportProgress->setTextVisible(false);

        QFont font("./themes/geontime/urw_gothic_l_book.ttf");
        setFont(font);
        auto locationLayout = new QHBoxLayout;
        locationLayout->addWidget(new QLabel(tr("Location"), this));
        locationEdit = new QLineEdit(this);
        connect(locationEdit, SIGNAL(textChanged(const QString&)), this, SLOT(resetProgressBar()));
        locationLayout->addWidget(locationEdit);
        browseLocation = new GeonkickButton(this);
        browseLocation->setCheckable(true);
        browseLocation->setUnpressedImage(QPixmap("./themes/geontime/export_browse.png"));
        connect(browseLocation, SIGNAL(toggled(bool)), this, SLOT(browse()));
        locationLayout->addWidget(browseLocation);
        locationLayout->addWidget(new QLabel(tr("File name"), this));
        fileNameEdit = new QLineEdit(this);
        fileNameEdit->setFont(font);
        connect(fileNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(resetProgressBar()));
        locationLayout->addWidget(fileNameEdit);
        locationLayout->setSpacing(0);
        locationLayout->addStretch();

        auto formatLayout = new QHBoxLayout;
        formatLayout->addWidget(new QLabel(tr("Export format"), this));
        formatComboBox = new QComboBox(this);
        connect(formatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(resetProgressBar()));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Flac16), tr("FLAC, 16 bit"));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Flac24), tr("FLAC, 24 bit"));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Wav16), tr("WAV, 16 bit"));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Wav24), tr("WAV, 24 bit"));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Wav32), tr("WAV, 32 bit"));
        formatComboBox->insertItem(static_cast<int>(ExportFormat::Ogg), tr("Ogg Vorbis"));
        formatComboBox->setCurrentIndex(static_cast<int>(ExportFormat::Wav16));
        formatLayout->addWidget(formatComboBox);
        monoRadioButton = new QRadioButton(tr("Mono"), this);
        connect(monoRadioButton, SIGNAL(toggled(bool)), this, SLOT(resetProgressBar()));
        monoRadioButton->setChecked(true);
        stereoRadioButton = new QRadioButton(tr("Stereo"), this);
        connect(stereoRadioButton, SIGNAL(toggled(bool)), this, SLOT(resetProgressBar()));
        formatLayout->addWidget(monoRadioButton);
        formatLayout->addWidget(stereoRadioButton);

        formatLayout->setSpacing(0);
        formatLayout->insertSpacing(3, 16);
        formatLayout->insertSpacing(4, 10);
        formatLayout->addStretch();

        auto buttonsLayout = new QHBoxLayout;
        exportButton = new GeonkickButton(this);
        exportButton->setCheckable(true);
        exportButton->setUnpressedImage(QPixmap("./themes/geontime/export_export.png"));
        connect(exportButton, SIGNAL(toggled(bool)), this, SLOT(exportKick()));
        cancelButton = new GeonkickButton(this);
        cancelButton->setUnpressedImage(QPixmap("./themes/geontime/export_cancel.png"));
        cancelButton->setCheckable(true);
        connect(cancelButton, SIGNAL(toggled(bool)), this, SIGNAL(closeDialog()));
        buttonsLayout->addWidget(cancelButton);
        buttonsLayout->addWidget(exportButton);

        //mainLayout->addWidget();
        mainLayout->addLayout(formatLayout);
        mainLayout->addLayout(locationLayout);
        mainLayout->addWidget(exportProgress);
        mainLayout->addLayout(buttonsLayout);
        setLayout(mainLayout);
        show();
}

ExportWidget::~ExportWidget()
{
}

ExportWidget::ExportResult ExportWidget::exec()
{
        QEventLoop eventLoop(this);
        connect(this, SIGNAL(closeDialog()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        return exportResult;
}

void ExportWidget::browse()
{
        QFileDialog fileDialog(this, tr("Select Path"));
        fileDialog.setFileMode(QFileDialog::Directory);
        fileDialog.setOption(QFileDialog::ShowDirsOnly);
        fileDialog.setFilter(QDir::Dirs);
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.exec();
        QStringList paths = fileDialog.selectedFiles();
        if (!paths.isEmpty()) {
                locationEdit->setText(paths.first());
        }
}

void ExportWidget::exportKick()
{
        resetProgressBar();
        enableButtons(false);

        SF_INFO sndinfo;
        sndinfo.samplerate = geonkickApi->getSampleRate();
        if (sndinfo.samplerate == 0) {
                cancel();
                showError();
                GEONKICK_LOG_ERROR("can't get sample rate");
                return;
        }

        sndinfo.channels   = monoRadioButton->isChecked() ? 1 : 2;
        sndinfo.format     = exportFormat();

        std::vector<gkick_real> tempBuffer = geonkickApi->getKickBuffer();
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

        if (kickBuffer.empty()) {
                cancel();
                showError();
                GEONKICK_LOG_ERROR("kick buffer is empty");
                return;
        }

        if (!sf_format_check(&sndinfo)) {
                cancel();
                showError();
                GEONKICK_LOG_ERROR("wrong sndinfo");
                return;
        }

        QString filePath = getFilePath();
        SNDFILE *sndFile = sf_open(filePath.toLatin1().data(), SFM_WRITE, &sndinfo);
        if (!sndFile) {
                cancel();
                showError();
                GEONKICK_LOG_ERROR("can't open file");
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
                        cancel();
                        showError();
                        GEONKICK_LOG_ERROR("error on writing to file");
                        break;
                }
                i += chunk;
                exportProgress->setValue(100 * (static_cast<float>(i) / kickBuffer.size()));
        }

        sf_close(sndFile);
        enableButtons(true);
}

void ExportWidget::cancel()
{
        if (!exportButton->isEnabled()) {
                enableButtons(true);
        } else {
                emit closeDialog();
        }
}

void ExportWidget::resetProgressBar()
{
        exportProgress->setValue(0);
}

int ExportWidget::exportFormat()
{
        switch (static_cast<ExportFormat>(formatComboBox->currentIndex()))
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

void ExportWidget::showError()
{
        GEONKICK_LOG_ERROR("error");
        cancel();
}

void ExportWidget::enableButtons(bool enable)
{
        exportButton->setEnabled(enable);
        if (enable) {
                cancelButton->setUnpressedImage(QPixmap("./themes/geontime/export_cancel.png"));
        } else {
                cancelButton->setUnpressedImage(QPixmap("./themes/geontime/export_stop.png"));
        }
        monoRadioButton->setEnabled(enable);
        stereoRadioButton->setEnabled(enable);
        locationEdit->setEnabled(enable);
        browseLocation->setEnabled(enable);
        formatComboBox->setEnabled(enable);
}

QString ExportWidget::getFilePath()
{
        return locationEdit->text() + QDir::separator()
                + fileNameEdit->text() + "." + fileSuffix();
}

QString ExportWidget::fileSuffix()
{
        switch (static_cast<ExportFormat>(formatComboBox->currentIndex()))
        {
        case ExportFormat::Flac16:
        case ExportFormat::Flac24:
                return QString("flac");
        case ExportFormat::Wav16:
        case ExportFormat::Wav24:
        case ExportFormat::Wav32:
                return QString("wav");
        case ExportFormat::Ogg:
                return QString("ogg");
        default:
                return QString();
        }
}

/*void ExportWidget::paintWidget(QPaintEvent *event)
{
        QPainter painter(this);
        QPen pen(QColor(20, 20, 20, 255));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(0, 0, width() - 1, height() - 1);
        }*/
