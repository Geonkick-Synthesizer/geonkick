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

#include <QEventLoop>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QProgressBar>

ExportWidget::ExportWidget(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent),
          geonkickApi(api),
          exportResult(ExportResult::Rejected),
          locationEdit(nullptr),
          browseLocation(nullptr),
          formatComboBox(nullptr),
          monoRadioButton(nullptr),
          stereoRadioButton(nullptr)
{
        setFixedSize(600, 500);
        setWindowFlags(Qt::Dialog);
        setWindowModality(Qt::ApplicationModal);
        setWindowTitle(tr("Export Kick"));

        auto mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(new QLabel(tr("Location"), this));

        auto locationLayout = new QHBoxLayout;
        locationEdit = new QLineEdit(this);
        locationLayout->addWidget(locationEdit);
        browseLocation = new QPushButton(tr("Browse"), this);
        connect(browseLocation, SIGNAL(clicked(bool)), this, SLOT(browse()));
        locationLayout->addWidget(browseLocation);
        mainLayout->addLayout(locationLayout);

        mainLayout->addWidget(new QLabel(tr("Export format"), this));
        auto formatLayout = new QHBoxLayout;

        formatComboBox = new QComboBox(this);
        formatComboBox->addItem(tr("FLAC, 16 bit"));
        formatComboBox->addItem(tr("FLAC, 24 bit"));
        formatComboBox->addItem(tr("WAV, 16 bit"));
        formatComboBox->addItem(tr("WAV, 32 bit"));
        formatComboBox->addItem(tr("Ogg Vorbis"));
        formatLayout->addWidget(formatComboBox);
        monoRadioButton = new QRadioButton(tr("Mono"), this);
        stereoRadioButton = new QRadioButton(tr("Stereo"), this);
        formatLayout->addWidget(monoRadioButton);
        formatLayout->addWidget(stereoRadioButton);
        mainLayout->addLayout(formatLayout);

        auto exportProgress = new QProgressBar(this);
        mainLayout->addWidget(exportProgress);

        auto buttonsLayout = new QHBoxLayout;
        auto exportButton = new QPushButton(tr("Export"), this);
        connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(exportKick()));
        auto cancelButton = new QPushButton(tr("Cancel"), this);
        connect(cancelButton, SIGNAL(clicked(bool)), this, SIGNAL(closeDialog()));
        buttonsLayout->addWidget(cancelButton);
        buttonsLayout->addWidget(exportButton);
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
        GEONKICK_LOG_INFO("browse...");
}

void ExportWidget::exportKick()
{
        GEONKICK_LOG_INFO("export...");
}

void ExportWidget::cancel()
{
        emit closeDialog();
}

